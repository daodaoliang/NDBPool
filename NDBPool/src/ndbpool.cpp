#include "ndbpool.h"

QMap<QString, NDBPool_p *> NDBPool::poolMap;
bool NDBPool::isDebug;
QMap<QString, connectActive> NDBPool::connectionLastActiveTimeMap;
QMutex NDBPool::mLock;

void NDBPool::release()
{
    QMutexLocker locker(&(NDBPool::mLock));
    foreach (QString hostName, poolMap.keys()) {
        NDBPool_p * tempDBPool = poolMap.value(hostName);
        if(tempDBPool){
            delete tempDBPool;
            tempDBPool = NULL;
        }
    }
}

QSqlDatabase NDBPool::getNewConnection(QString paramHostName, QString paramDatabaseName, QString paramUserName, QString paramPassword, int paramPort, bool longConnect, QString longConnnectName)
{
    connectActive tempItem;
    tempItem.poolName = paramHostName;
    // 选择连接池
    if(poolMap.contains(paramHostName)){
        NDBPool_p * dbConnectPtr = NDBPool::instance().poolMap.value(paramHostName);
        // 尝试获取一个连接
        if(dbConnectPtr->semaphore.tryAcquire(1, dbConnectPtr->getMaxWaitTime())){
            NDBPool::mLock.lock();
            // 依据长短连接从连接池中获取
            if(longConnect){
                // 长连接
                QSqlDatabase innerDB = dbConnectPtr->getNewConnection(longConnnectName);
                tempItem.lastActiveTime = QDateTime::currentMSecsSinceEpoch();
                tempItem.longConnect = true;
                connectionLastActiveTimeMap.insert(longConnnectName, tempItem);
                NDBPool::mLock.unlock();
                if(!innerDB.isOpen()){
                    NDBPool::mLock.lock();
                    if(isDebug){
                        qDebug()<<"long connection:"<<longConnnectName<<"is dead link!";
                    }
                    dbConnectPtr->semaphore.release();
                    NDBPool::mLock.unlock();
                    return QSqlDatabase();
                } else {
                    NDBPool::mLock.lock();
                    if(!dbConnectPtr->usedConnectionName.contains(longConnnectName))
                        dbConnectPtr->usedConnectionName.push(longConnnectName);
                    dbConnectPtr->semaphore.release();
                    NDBPool::mLock.unlock();
                    return innerDB;
                }
            } else {
                // 短连接
                QString tempConnectionName = dbConnectPtr->unusedConnectionName.length()>0 ? dbConnectPtr->unusedConnectionName.pop() : QUuid::createUuid().toString();
                QSqlDatabase innerDB = dbConnectPtr->getNewConnection(tempConnectionName);
                tempItem.lastActiveTime = QDateTime::currentMSecsSinceEpoch();
                tempItem.longConnect = false;
                connectionLastActiveTimeMap.insert(tempConnectionName, tempItem);
                NDBPool::mLock.unlock();
                if(!innerDB.isOpen()){
                    NDBPool::mLock.lock();
                    dbConnectPtr->semaphore.release();
                    if(isDebug){
                        qDebug()<<"short connection:"<<tempConnectionName<<"is dead link!";
                    }
                    NDBPool::mLock.unlock();
                    return QSqlDatabase();
                } else {
                    NDBPool::mLock.lock();
                    dbConnectPtr->usedConnectionName.push(tempConnectionName);
                    NDBPool::mLock.unlock();
                    return innerDB;
                }
            }

        } else {
            if(isDebug){
                qDebug() << "Time out to create connection.";
            }
            return QSqlDatabase();
        }
    } else {
        // 链接池不存在则创建并使用连接池
        NDBPool::mLock.lock();
        NDBPool_p * tempPool = new NDBPool_p();
        poolMap.insert(paramHostName, tempPool);
        tempPool->setHostName(paramHostName);
        tempPool->setDatabaseName(paramDatabaseName);
        tempPool->setUsername(paramUserName);
        tempPool->setPassword(paramPassword);
        tempPool->setPort(paramPort);
        NDBPool::mLock.unlock();
        return getNewConnection(paramHostName,paramDatabaseName,paramUserName,paramPassword,paramPort,longConnect,longConnnectName);
    }
}

void NDBPool::closeConnection(const QSqlDatabase &connection)
{
    QMutexLocker locker(&(NDBPool::mLock));
    bool tempFlag = false;
    bool longConnenct = false;
    QString tempConnectionName = connection.connectionName();
    foreach (QString hostName, poolMap.keys()) {
        NDBPool_p * tempPool = poolMap.value(hostName);
        if(tempPool->usedConnectionName.contains(tempConnectionName)){
            // 长连接关闭连接
            if(connectionLastActiveTimeMap[tempConnectionName].longConnect){
                tempPool->closeConnection(tempConnectionName);
                connectionLastActiveTimeMap.remove(tempConnectionName);
                longConnenct = true;
            } else {                
                tempPool->unusedConnectionName.push(tempConnectionName);
            }
            tempPool->usedConnectionName.removeOne(tempConnectionName);
            tempFlag = true;
            tempPool->semaphore.release();
        }
    }
    if(isDebug && tempFlag){
        if(longConnenct){
            qDebug()<<QString("long connect %1 is exist and closed").arg(tempConnectionName);
        } else {
            qDebug()<<QString("short connect %1 is exist and closed").arg(tempConnectionName);
        }
    }
}

NDBPool::NDBPool(bool paramIsDebug, QObject *parent):QObject(parent)
{
    // 调试模式
    isDebug = paramIsDebug;
    // 强制断开连接检查频率
    checkForceBreakInterval = 1000;
    // 强制断开连接的时间差
    forceBreakTime = 120000;
    // 强制断开连接
    connect(&forceBreakTimer, SIGNAL(timeout()),this,SLOT(slots_forceBreak()));
    forceBreakTimer.setInterval(checkForceBreakInterval);
    forceBreakTimer.start();
}

NDBPool::~NDBPool()
{
    QMutexLocker locker(&(NDBPool::mLock));
    foreach (QString hostName, poolMap.keys()) {
        NDBPool_p * tempDBPool = poolMap.value(hostName);
        if(tempDBPool){
            delete tempDBPool;
            tempDBPool = NULL;
        }
    }
}

void NDBPool::slots_forceBreak()
{
    QMutexLocker tempLocker(&NDBPool::mLock);
    int use_count = 0;
    int unuse_count = 0;
    int long_connection = 0;
    foreach (QString connectionName, connectionLastActiveTimeMap.keys()) {
        if(connectionLastActiveTimeMap[connectionName].longConnect){
            long_connection++;
            continue;
        }
        qint64 timeDiff = QDateTime::currentMSecsSinceEpoch() - connectionLastActiveTimeMap.value(connectionName).lastActiveTime;
        NDBPool_p * tempConnection = poolMap.value(connectionLastActiveTimeMap.value(connectionName).poolName);
        if(timeDiff >= forceBreakTime){
            if(tempConnection->usedConnectionName.contains(connectionName)){
                tempConnection->usedConnectionName.removeOne(connectionName);
                tempConnection->unusedConnectionName.push(connectionName);
            }
            if(tempConnection->unusedConnectionName.contains(connectionName)){
                tempConnection->unusedConnectionName.removeOne(connectionName);
                tempConnection->closeConnection(connectionName);
            }
            connectionLastActiveTimeMap.remove(connectionName);
        }
        use_count = tempConnection->usedConnectionName.count();
        unuse_count = tempConnection->unusedConnectionName.count();
    }
    if(isDebug){
        qDebug()<<"使用中连接总数:"<<use_count;
        qDebug()<<"长连接使用数:"<<long_connection;
        qDebug()<<"未使用连接总数:"<<unuse_count;
    }
}


NDBPool &NDBPool::instance(bool paramIsDebug)
{
    static NDBPool innerInstance(paramIsDebug);
    return innerInstance;
}
