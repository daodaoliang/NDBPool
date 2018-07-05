#include <QCoreApplication>
#include "ndbpool.h"
#include <QDebug>
#include "threadtest.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "/**************************开始测试用例**************************/";
    {
        // get new short connection
        QSqlDatabase tempDB = NDBPool::getNewConnection("127.0.0.1","test","root","",3306);
        qDebug() << " connection name:" << tempDB.connectionName() << "is vaild:" << tempDB.isOpen() << "\n";
        QSqlDatabase tempDB1 = NDBPool::getNewConnection("127.0.0.1","test","root","",3306);
        qDebug() << " connection name:" << tempDB1.connectionName() << "is vaild:" << tempDB1.isOpen() <<"\n";
        QSqlDatabase tempDB2 = NDBPool::getNewConnection("127.0.0.1","test","root","",3306);
        qDebug() << " connection name:" << tempDB2.connectionName() << "is vaild:" << tempDB2.isOpen() <<"\n";
        QSqlDatabase tempDB3 = NDBPool::getNewConnection("127.0.0.1","test","root","",3306);
        qDebug() << " connection name:" << tempDB3.connectionName() << "is vaild:" << tempDB3.isOpen() <<"\n";
        QSqlDatabase tempDB4 = NDBPool::getNewConnection("127.0.0.1","test","root","",3306);
        qDebug() << " connection name:" << tempDB4.connectionName() << "is vaild:" << tempDB4.isOpen() <<"\n";

        // release new connection
        NDBPool::closeConnection(tempDB);
        NDBPool::closeConnection(tempDB1);
        NDBPool::closeConnection(tempDB2);
        NDBPool::closeConnection(tempDB3);
        NDBPool::closeConnection(tempDB4);
    }

    // thread get new connnection
    threadTest tempTest;
    tempTest.startTest();
    qDebug() << "/**************************结束测试用例**************************/";
    return a.exec();
}
