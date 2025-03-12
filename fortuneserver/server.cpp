





// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "server.h"
#include <QDataStream>
#include <QPixmap>
#include <QThread>
#include <QThreadPool>
#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <threadbmp.h>
using namespace std;
void Server::paintEvent(QPaintEvent *event) {}
Server::Server(QWidget *parent) : QWidget(parent), statusLabel(new QLabel) {
  statusLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  model = new QSqlTableModel(this);
  std::cout << this << std::endl;
  model->setTable("words");
  model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  model->select();
  model->setHeaderData(0, Qt::Horizontal, tr("ID"));
  model->setHeaderData(1, Qt::Horizontal, tr("First name"));
  model->setHeaderData(2, Qt::Horizontal, tr("Last name"));
  QTableView *view = new QTableView;
  view->setModel(model);
  view->resizeColumnsToContents();
  initServer();
  //! [2]
  fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
           << tr("You've got to think about tomorrow.")
           << tr("You will be surprised by a loud noise.")
           << tr("You will feel hungry again in another hour.")
           << tr("You might have mail.")
           << tr("You cannot kill time without injuring eternity.")
           << tr("Computers are not intelligent. They only think they are.");
  //! [2]
  circle_times = 640 * 480 * 2 / 1400;
  last_bytes = 640 * 480 * 2 % 1400;
  if (last_bytes != 0)
    circle_times += 1;
  tcp_wnd = 0;
  step = 3;
  current_location = 0;
  //  drawLabel = new QLabel;
  //  drawLabel->setAlignment(Qt::AlignCenter);
  //  draw = QImage(640, 480, QImage::Format_RGB16);
  //  QColor backColor = qRgb(240, 255, 255);

  //  draw.fill(backColor);
  //  drawLabel->setPixmap(QPixmap::fromImage(draw));

  auto quitButton = new QPushButton(tr("Quit"));

  quitButton->setAutoDefault(false);
  stopButton = new QPushButton(tr("Stop"));

  disconnectButton= new QPushButton(tr("disconnect"));
  //stopButton->setDisabled(true);
  connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
  //! [3]
  // connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendFortune);
  //! [3]
  //!
  //  for(int i=0;i<2;i++){
  //    for(int j=0;j<614400;j++){

  //        bmp_raw_arr[i][j]=0;
  //    }

  //  }
  clientCombo=new QComboBox;
  clientCombo->setEditable(true);
  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addStretch(1);
  buttonLayout->addWidget(stopButton);
  buttonLayout->addWidget(disconnectButton);

  buttonLayout->addWidget(quitButton);
  comboLayout= new QVBoxLayout;
  comboLayout->addWidget(clientCombo);

  buttonLayout->addStretch(1);
  bmpLayout = new QHBoxLayout;
  drawLabel = new QLabel;
  drawLabel2 = new QLabel;
  bmpLayout->addWidget(drawLabel);
  bmpLayout->addWidget(drawLabel2);

  QVBoxLayout *mainLayout = nullptr;
  if (QGuiApplication::styleHints()->showIsFullScreen() ||
      QGuiApplication::styleHints()->showIsMaximized()) {
    auto outerVerticalLayout = new QVBoxLayout(this);
    outerVerticalLayout->addItem(new QSpacerItem(
        0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    auto outerHorizontalLayout = new QHBoxLayout;
    outerHorizontalLayout->addItem(new QSpacerItem(
        0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
    auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
    mainLayout = new QVBoxLayout(groupBox);
    outerHorizontalLayout->addWidget(groupBox);
    outerHorizontalLayout->addItem(new QSpacerItem(
        0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
    outerVerticalLayout->addLayout(outerHorizontalLayout);
    outerVerticalLayout->addItem(new QSpacerItem(
        0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
  } else {
    mainLayout = new QVBoxLayout(this);
  }
  mainLayout->addLayout(comboLayout);
  mainLayout->addWidget(statusLabel);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addLayout(bmpLayout);
  //  mainLayout->addWidget(view);
  tcpServer->server = this;
  setWindowTitle(QGuiApplication::applicationDisplayName());
  // MultipleServer::getinstance();

  // this->setFixedSize(1000, 700);
}
int Server::syn = 0;
void Server::receive_period(int start, int end) {

  // cout << circle_times << " circle time " << last_bytes << endl;
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_6_5);
  int number = start + circle_times + 1;
  vector<unsigned char> deco = decode_array(number);
  char o1[4];
  char *abs = (char *)o1;
  for (int i = 0; i < deco.size(); i++) {
    o1[i] = deco[i];
  }
  block = QByteArray::fromRawData(abs, sizeof(o1));
  out << *abs;
  receiver->write(block);
  if (end == circle_times) {
    current_location = -1;
  }
}

std::vector<unsigned char> Server::decode_array(int number) {
  int length = ceil(double(number) / 255);

  vector<unsigned char> deco_arr(4);

  for (int i = 0; i < 4; i++) {
    if (i < length - 1) {
      deco_arr[i] = 255;
    } else if (i == length - 1) {
      deco_arr[i] = number % 255;
    } else {
      deco_arr[i] = 0;
    }
    //   cout << int(deco_arr[i]) << " ";
  }
  // cout << endl;

  return deco_arr;
}
void Server::changeCombo(const QString &name){
  qDebug()<<name;
 // void * a=bmpList[0];
  //ThreadBmp *tmp =static_cast<ThreadBmp>();
}
void Server::initServer() {
  //! [0] //! [1]
  tcpServer = new QThreadServer;

  if (!tcpServer->listen(QHostAddress("192.168.1.7"), 12346)) {
    QMessageBox::critical(
        this, tr("Fortune Server"),
        tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
    close();
    return;
  }

  tcpServer2 = new QThreadServer;

  if (!tcpServer2->listen(QHostAddress("192.168.1.7"), 12347)) {
    QMessageBox::critical(
        this, tr("Fortune Server"),
        tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
    close();
    return;
  }
  //! [0]
  QString ipAddress;
  const QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  // use the first non-localhost IPv4 address
  for (const QHostAddress &entry : ipAddressesList) {
    //        if (entry != QHostAddress::LocalHost && entry.toIPv4Address()) {
    //            ipAddress = entry.toString();
    //            break;
    //        }

    if (entry.toString().toStdString() == "192.168.1.7") {
      ipAddress = entry.toString();
      break;
    }
  }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty())
    ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
  statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n")
                           .arg(ipAddress)
                           .arg(tcpServer->serverPort()));
  //! [1]
}

void Server::updateBmp(qintptr index) {
 // cout<<"bmp "<<QThread::currentThreadId()<<"descriptor "<<index;
  // "<<bls<<endl;
  draw = QImage((unsigned char *)bmp_raw_arr, 640, 480, QImage::Format_RGB16);
  drawLabel->setPixmap(QPixmap::fromImage(draw));
//  draw2 = QImage((unsigned char *)abc, 640, 480, QImage::Format_RGB16);
//  drawLabel2->setPixmap(QPixmap::fromImage(draw2));
  bls++;
}
void Server::updateCombo(QString item){

  this->clientCombo->addItem(item);
}
void QThreadServer::incomingConnection(qintptr socketDescriptor) {

  QThread *th = new QThread();
  ThreadBmp *bmp;
 // if (this->server->counts == 0) {
    bmp = new ThreadBmp(socketDescriptor, this->server->bmp_raw_arr);
//  } else {
//    bmp = new ThreadBmp(socketDescriptor, this->server->abc);
//  }
  bmp->moveToThread(th);
  //  QLabel *label=new QLabel();
  //  this->server->bmpLayout->addWidget(label);
  //  this->server->threadList.push_back(label);
  bmp->label_count = this->server->counts;
  bmp->server = this->server;
  this->server->counts++;
  //ThreadBmp::bmpList.append(*bmp);
 // cout<<ThreadBmp::bmpList.size()<<endl;
  connect(bmp->server->disconnectButton,&QAbstractButton::clicked,bmp,&ThreadBmp::disconnect);
  connect(th, &QThread::started, bmp, &ThreadBmp::test);
  connect(bmp, &ThreadBmp::tps, this->server, &Server::updateBmp);
  connect(bmp->server->stopButton, &QAbstractButton::clicked, bmp,
          &ThreadBmp::changeRunStatus);
  connect(th,&QThread::finished,bmp,&ThreadBmp::deleteLater);
  connect(th,&QThread::finished,th,&QThread::deleteLater);
  //update combobox if disconnected
  connect(bmp, &ThreadBmp::updateServerCombo, this->server, &Server::updateCombo);
  connect(bmp->server->clientCombo,&QComboBox::currentTextChanged,bmp,&ThreadBmp::changeRunComboSelect);
  th->start();
}
//! [8]
