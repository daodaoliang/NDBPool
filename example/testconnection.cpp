#include "testconnection.h"

TestConnection::TestConnection(int param_num, QThread *parent) : QThread(parent),threadNum(param_num)
{
}

void TestConnection::run()
{
    QSqlDatabase innerDB = NDBPool::getNewConnection("127.0.0.1","test","root","",3306);
    qDebug() << "short thread no:" << threadNum << "connection name:" << innerDB.connectionName() << "db is vaild:" << innerDB.isOpen() << "\n";
    QThread::msleep(2000);
    NDBPool::closeConnection(innerDB);
    QSqlDatabase innerDB_002 = NDBPool::getNewConnection("127.0.0.1","test","root","",3306,true,"daodaoliang");
    qDebug() << "long thread no:" << threadNum << "connection name:" << innerDB_002.connectionName() << "db is vaild:" << innerDB_002.isOpen() << "\n";
    this->quit();
}

