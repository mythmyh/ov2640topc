// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SERVER_H
#define SERVER_H
#include <vector>
#include <QDialog>
#include <QString>
#include <QList>
#include <QTcpSocket>
#include <QSqlTableModel>
#include <QTcpServer>
#include <QImage>
#include <map>
#include <vector>
#include <QComboBox>
#include <QVBoxLayout>
QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QDialogButtonBox;
class QPushButton;
class QSqlTableModel;
class Server;
class ThreadBmp;
QT_END_NAMESPACE
class QThreadServer:public QTcpServer{

protected:
    void incomingConnection (qintptr socketDescripter);
public:
    Server *server;

};

//! [0]
class Server : public QWidget
{
public:
    static int syn;

    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    std::vector<unsigned char> decode_array(int num);
    void receive_period(int start,int end);
public slots:
    void updateBmp(qintptr index);
    void updateCombo(QString item);
    void changeCombo(const QString &name);

protected:
     void paintEvent(QPaintEvent *event) override;
signals:
    void stopOrStart();
public:
    void initServer();
    QSqlTableModel *model;
    QLabel *statusLabel = nullptr;
    QThreadServer *tcpServer = nullptr;

    QThreadServer *tcpServer2 = nullptr;
    QTcpSocket *receiver = nullptr;
    QList<QString> fortunes;
    QDataStream in;
    int circle_times;
    int last_bytes;
    int tcp_wnd;
    int step;
    int current_location;
    int total_length=0;
    QByteArray bmp;
    QHBoxLayout * bmpLayout;
    QVBoxLayout * comboLayout;

    int counts=0;
    std::vector<QLabel*>threadList;
    QLabel *drawLabel;
    QImage draw;
    QLabel *drawLabel2;
    QImage draw2;
    char *bmp_raw_arr=new char [614400];
    char *abc = new char [614400];
    int next_thread=0;
    ThreadBmp *current;
    QPushButton *stopButton;
    QPushButton *disconnectButton;
    QComboBox *clientCombo = nullptr;

    QString current_chose;
    int bls;
};


//! [0]

#endif
