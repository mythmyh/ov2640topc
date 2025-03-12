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
#include <QImage>
QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
class QDialogButtonBox;
class QPushButton;
class QSqlTableModel;

QT_END_NAMESPACE

//! [0]
class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    std::vector<unsigned char> decode_array(int num);
    void receive_period(int start,int end);
    void RGBtoBMP(QByteArray array,int width,int height);

private slots:
    void sendFortune();
    void readData();
protected:
     void paintEvent(QPaintEvent *event) override;
private:
    void initServer();
    QSqlTableModel *model;
    QLabel *statusLabel = nullptr;
    QTcpServer *tcpServer = nullptr;
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
    QLabel *bmpLabel;
    QImage *img;
    char bmp_raw[614400];

    QLabel *drawLabel;
    QImage draw;

};
//! [0]

#endif
