/*-------------------------------------------------
#
# Project created by QtCreator
# Author: 沙振宇
# CreateTime: 2016-04-22
# UpdateTime: 2019-12-27
# Info: QTcpSocket客户端、服务端互发字符串Demo
# Url:https://shazhenyu.blog.csdn.net/article/details/53283686
# Github:https://github.com/ShaShiDiZhuanLan/Demo_TcpSocket_Qt
#
#-------------------------------------------------*/
#include "tcpsever.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <iostream>
#include "hex.h"
#include "cryptlib.h"
#include "modes.h"
#include "aes.h"
#include "cryptopp/files.h"
using namespace std;
using namespace CryptoPP;
std::string str = "11111111111111111111111111111111";
std::string str2 ="0000000000000000";
SecByteBlock key(reinterpret_cast<const CryptoPP::byte*>(&str[0]),str.size());
SecByteBlock iv(reinterpret_cast<const CryptoPP::byte*>(&str2[0]),str.size());
QString newCipher;
HexEncoder encoder(new FileSink(std::cout));
TcpSever::TcpSever(QObject *parent):
    QTcpServer(parent),
    socket(new QTcpSocket),
    strTMP("pre")
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(readSomething()));
    connect(this,SIGNAL(alreadyRead()),this,SLOT(returnReadSomething()));
}
TcpSever::~TcpSever()
{
    delete socket;
}
QString TcpSever::getIPAddress()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    return ipAddress.isEmpty() ? QHostAddress(QHostAddress::LocalHost).toString() : ipAddress;
}
quint16 TcpSever::returnPeer()
{
    return socket->peerPort();
}

QString TcpSever::returnIP()
{
    return socket->peerAddress().toString();
}

QString TcpSever::returnOutIP()
{
    QString tmp = GetHtml("https://ip.cn/");
    qDebug() << "returnOutIP" << tmp;
    return GetNetIP(tmp);
}

string TcpSever::Decrypt(std::string a)
{
    std::cout<<"deneme:"<<a;
  a="deneme";
  return a;
}

QString TcpSever::returnReadSomething()
{
    std::string  recovered,resimString;
    //qDebug()<<strTMP; Şifrelenmiş veri
    //std::cout << strTMP.toStdString() << std::endl;
    resimString=strTMP.toStdString();
    std::byte bytes[resimString.length()];
       std::memcpy(bytes, resimString.data(), resimString.length());
   /* string output;
        CryptoPP::StringSource(resimString,true,
                               new CryptoPP::HexEncoder(
                                   new CryptoPP::StringSink(output)));
       /* std::cout << "son text:           "<<endl<<endl;
        encoder.Put((const CryptoPP::byte*)&output[0], output.size());
        encoder.MessageEnd();*/


     //std::string resimString = strTMP.toUtf8().constData();
     //std::cout<<"resim:"<<resimString;
         try
         {
            qDebug()<<"deneme:";
             CBC_Mode< AES >::Decryption d;
             d.SetKeyWithIV(key, 32, iv);

             StringSource s(bytes, true,
                 new StreamTransformationFilter(d,
                     new StringSink(recovered)
                 ) // StreamTransformationFilter
             ); // StringSource

             //std::cout << "recovered text: " << recovered << std::endl;
         }
         catch(const Exception& e)
         {
             std::cerr << e.what() << std::endl;
             exit(1);
         }


     /*std::string stringJson = strTMP.toUtf8().constData();
     QByteArray byteArray(stringJson.c_str(), stringJson.length());
     //qDebug()<<"array:"<<byteArray;
      QJsonDocument jsonDocument = QJsonDocument::fromJson(byteArray);
     qDebug()<<"document:"<<byteArray;*/


    return strTMP;
}
void TcpSever::setSendMessage(QString str)
{
    socket->write(QByteArray(str.toLocal8Bit()));
}
void TcpSever::incomingConnection(qintptr handle)
{
    socket->setSocketDescriptor(handle);
}
void TcpSever::readSomething()
{
    strTMP = QString::fromLocal8Bit(socket->readAll());
    emit alreadyRead();
}

QString TcpSever::GetHtml(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    return QString(responseData);
}

QString TcpSever::GetNetIP(QString webCode)
{
    QString tmp = webCode.split("IP: ").last();
    return tmp.split("\n<br").first();
}
