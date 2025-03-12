#ifndef THREADBMP_H
#define THREADBMP_H
#include <QObject>
#include <QTcpSocket>
#include <QRunnable>
#include "server.h"
#include <vector>
#include <time.h>
//QObject要放在第一个
class ThreadBmp:public QObject
{
    Q_OBJECT
public:

    explicit ThreadBmp(qintptr descriptor,char * target);
    void receive_period(int start, int end);
    std::vector<unsigned char> decode_array(int number);
    void RGBtoBMP(QByteArray array, int width, int height,int index);
signals:
    void tps(int index);
    void updateServerCombo(QString item);
public:
    int circle_times;
    int last_bytes;
    int tcp_wnd;
    int step;
    int current_location;
    int total_length=0;
    QByteArray bmp;
    QTcpSocket *receiver = nullptr;
    Server *server;
    qintptr  m_socketDescriptor;
    QLabel *drawLabel;
    QImage draw;
    char *dest;
    char bmp_raw[614400];
    int index=0;
    bool if_run;
    clock_t start;
    clock_t end;
    float duration;

    ThreadBmp *bmpThread;
    int label_count=0;

public slots:
    void readData();
    void test();

    void disconnect();
    void changeRunStatus();
    void changeRunComboSelect(const QString &name);
    void exitThread();
};

#endif // THREADBMP_H
