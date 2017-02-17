#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <ndbpool.h>

class TestConnection : public QThread
{
    Q_OBJECT
public:
    explicit TestConnection(int param_num,QThread *parent = 0);
    void run();
    int threadNum;
};

#endif // TESTCONNECTION_H
