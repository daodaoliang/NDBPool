#ifndef THREADTEST_H
#define THREADTEST_H

#include <QObject>
#include "testconnection.h"
#include <QTimer>
class threadTest : public QObject
{
    Q_OBJECT
public:
    explicit threadTest(QObject *parent = 0);
    ~threadTest();
void startTest();
public slots:
    void start_test_short_connect();
private:
    QTimer *mPrivate;
};

#endif // THREADTEST_H
