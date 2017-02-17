#include "threadtest.h"

threadTest::threadTest(QObject *parent) : QObject(parent)
{
    mPrivate= NULL;
}

threadTest::~threadTest()
{
    if(mPrivate){
        mPrivate->deleteLater();
    }
}

void threadTest::startTest()
{
    mPrivate = new QTimer;
    mPrivate->setInterval(1000);
    connect(mPrivate,SIGNAL(timeout()), this, SLOT(start_test_short_connect()));
    mPrivate->start();
}

void threadTest::start_test_short_connect()
{
    QStack<TestConnection *> connectlist;
    for(int tempIndex =0; tempIndex != 10; ++tempIndex){
        TestConnection * temp = new TestConnection(tempIndex);
        connectlist.push(temp);
        temp->start();
        connect(temp,SIGNAL(finished()),temp,SLOT(deleteLater()));
    }
}
