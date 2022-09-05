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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <iostream>
#include "hex.h"
#include "cryptlib.h"
#include "modes.h"
#include "cryptopp/files.h"
#include "aes.h"

#include <iostream>
#include <string>
using namespace std;
using namespace CryptoPP;
std::string str = "11111111111111111111111111111111";
std::string str2 ="0000000000000000";
SecByteBlock key(reinterpret_cast<const CryptoPP::byte*>(&str[0]),str.size());
SecByteBlock iv(reinterpret_cast<const CryptoPP::byte*>(&str2[0]),str.size());
std::string cipher;
QString newCipher;

QJsonObject mainObj;
QByteArray j_data;
std::string output;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new TcpClient;
    connect(tcpClient,SIGNAL(alreadyRead()),this,SLOT(readSSMessage()));

    ui->lineEdit_ip->setText(tcpClient->returnIP());
    ui->lineEdit_port->setText(tcpClient->returnPort());
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string MainWindow::Encrypt(std::string a)
{
    try
    {
        CBC_Mode< AES >::Encryption e;
        e.SetKeyWithIV(key, 32, iv);

        StringSource s(a , true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch(const Exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    //std::cout << " new plain text: " << NewPlain << std::endl;

    CryptoPP::StringSource(cipher,true,
                           new CryptoPP::HexEncoder (
                               new CryptoPP::StringSink(output)));

   /* std::cout << "key: ";
    encoder.Put(key, key.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "iv: ";
    encoder.Put(iv, iv.size());
    encoder.MessageEnd();
    std::cout << std::endl;

    std::cout << "cipher text: ";
    encoder.Put((const byte*)&cipher[0], cipher.size());
    encoder.MessageEnd();
    std::cout << std::endl;*/

    //QByteArray qba = QString::fromStdString(cipher).toStdString();

    //pArray->append(cipher);
    //qDebug() << "pArray : " <<qba;
    //QByteArray::QByteArray(cipher,cipher.size());

    //qDebug() << "son cipher : " << newCipher ;
    return output;

}

void MainWindow::on_pushButton_clicked()
{
    QString filename= QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images(*.png *.jpg *.jpeg *.bmp *.gif)"));
    if(QString::compare(filename,QString()) !=0)
    {
        QImage image;
        bool valid=image.load(filename);
        if(valid)
        {
            image=image.scaledToWidth(ui->lbl_image->width(),Qt::SmoothTransformation);
            ui->lbl_image->setPixmap(QPixmap::fromImage(image));//Resmi ekleme
            //Resmi Base64 çevirme
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            image.save(&buffer, "PNG");
            QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
            std::string resim = iconBase64.toUtf8().constData();
            //sifreleme
            std::string plaintext = resim;
            std::string resimSifre=Encrypt(plaintext);
            //std::cout<<"Resim Cipher:"<<resimSifre;

           /* //text
            QString a=ui->lineEdit->text();
            std::string textString = a.toUtf8().constData();
            std::string textSifre=dene.sifre(textString);
            //std::cout<<"Text Cipher:"<<textSifre;*/
            //JSON
            QString resimSif = QString::fromUtf8(resimSifre.c_str());
            //QString textSif = QString::fromUtf8(textSifre.c_str());
            /*QJsonObject mainObject;
            mainObject.insert("Resim",resimSif);
            //mainObject.insert("Text",textSif);
            QJsonDocument jsonDoc;
            jsonDoc.setObject(mainObject);
            QByteArray data = jsonDoc.toJson();
            qDebug()<<"client:"<<data;*/


            tcpClient->sendMessage(resimSif);
        }
    }


}

void MainWindow::readSSMessage()
{
    //ui->label_3->setText(tcpClient->returnMessage());
}

void MainWindow::on_pushButton_update_clicked()
{
    tcpClient->setIP(ui->lineEdit_ip->text());
    tcpClient->setPort(ui->lineEdit_port->text().toInt());
    tcpClient->newConnect();
}

QString MainWindow::on_pushButton_2_clicked()
{

}

