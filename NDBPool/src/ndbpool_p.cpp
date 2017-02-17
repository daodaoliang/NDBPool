#include "ndbpool_p.h"

QMutex NDBPool_p::mutex;

NDBPool_p::NDBPool_p(QObject *parent, bool paramDebug):QObject(parent)
{
    // 读取配置文件获取并设置数据库的基本信息
    setMaxConnectionCount(50);
    setMaxWaitTime(10);
    // 获取连接数量的互斥量
    semaphore.release(getMaxConnectionCount());
    // 设置程序的运行级别
    isDebug = paramDebug;
    // 测试数据连接通畅性的SQL语句
    testSql = "select 0;";
}

NDBPool_p::~NDBPool_p()
{
    // 销毁所有正在连接的数据库连接
    foreach (QString connectionName, usedConnectionName) {
        getNewConnection(connectionName).close();
        if(isDebug){
            qDebug()<<"close connected connectionName:" << connectionName;
        }
        if(QSqlDatabase::contains(connectionName)){
            closeConnection(connectionName);
        }
    }
    // 销毁所有处于等待状态的短数据库连接
    foreach (QString connectionName, unusedConnectionName) {
        if(isDebug){
            qDebug()<<"close unconnected short connectionName:" << connectionName;
        }
        if(QSqlDatabase::contains(connectionName)){
            closeConnection(connectionName);
        }
    }
}

QSqlDatabase NDBPool_p::getNewConnection(const QString &paramConnectionName)
{
    QMutexLocker tempLock(&mutex);
    if(QSqlDatabase::contains(paramConnectionName)){
        QSqlDatabase tempDB = QSqlDatabase::database(paramConnectionName);
        if(testSql.length()!=0){
            if(!tempDB.isOpen()){
                tempDB.open();
            }
            if(!tempDB.isOpenError()){
                QSqlQuery tempQuery = tempDB.exec(testSql);
                if((tempQuery.lastError().type() == QSqlError::NoError)){
                    return tempDB;
                }
            } else {
                if(isDebug){
                    qDebug() << "Open old datatabase error-创建一个新的连接:" << tempDB.lastError().text();
                }
                return QSqlDatabase();
            }
        }
    }
    QSqlDatabase tempDBNew = QSqlDatabase::addDatabase("QMYSQL", paramConnectionName);
    tempDBNew.setHostName(hostName);
    tempDBNew.setDatabaseName(databaseName);
    tempDBNew.setUserName(username);
    tempDBNew.setPassword(password);
    tempDBNew.setPort(port);
    if(!tempDBNew.open()){
        if(isDebug){
            qDebug() << "Open new datatabase error:" << tempDBNew.lastError().text();
        }
        return QSqlDatabase();
    }
    return tempDBNew;
}

bool NDBPool_p::closeConnection(const QString &paramConnectionName)
{
    QMutexLocker tempLock(&mutex);
    if(QSqlDatabase::contains(paramConnectionName)){
        QSqlDatabase tempDBNew = QSqlDatabase::database(paramConnectionName);
        if(tempDBNew.isOpen()){
            tempDBNew.close();
        }
        QSqlDatabase::removeDatabase(paramConnectionName);
        return true;
    }
    if(isDebug){
        qDebug()<<"数据库连接并不存在:"<< paramConnectionName;
    }
    return false;
}

QString NDBPool_p::getHostName() const
{
    return hostName;
}

void NDBPool_p::setHostName(const QString &value)
{
    hostName = value;
}

QString NDBPool_p::getDatabaseName() const
{
    return databaseName;
}

void NDBPool_p::setDatabaseName(const QString &value)
{
    databaseName = value;
}

QString NDBPool_p::getUsername() const
{
    return username;
}

void NDBPool_p::setUsername(const QString &value)
{
    username = value;
}

QString NDBPool_p::getPassword() const
{
    return password;
}

void NDBPool_p::setPassword(const QString &value)
{
    password = value;
}

int NDBPool_p::getPort() const
{
    return port;
}

void NDBPool_p::setPort(int value)
{
    port = value;
}

int NDBPool_p::getMaxConnectionCount() const
{
    return maxConnectionCount;
}

void NDBPool_p::setMaxConnectionCount(int value)
{
    maxConnectionCount = value;
}

int NDBPool_p::getMaxWaitTime() const
{
    return maxWaitTime;
}

void NDBPool_p::setMaxWaitTime(int value)
{
    maxWaitTime = value;
}
