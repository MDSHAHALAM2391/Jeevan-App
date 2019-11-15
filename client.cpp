#include "client.h"
#include "ui_client.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QMessageBox>
#include <QGeoPositionInfoSource>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QGeoCircle>
#include <QGeoCoordinate>

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client), m_view(new QWebEngineView(this))
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    socket->connectToHost("localhost",4200);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    source = QGeoPositionInfoSource::createDefaultSource(this);

       if (source) {
           connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                            this, SLOT(positionUpdated(QGeoPositionInfo)));
           source->startUpdates();
           source->setUpdateInterval(60);

       }
       ui->stackedWidget->setCurrentIndex(0);
       ui->diagnoseButton->setEnabled(false);
       ui->profileButton->setEnabled(false);
       ui->nearByButton->setEnabled(false);
       ui->mapButton->setEnabled(false);
}

client::~client()
{
    delete ui;
}
void client::positionUpdated(const QGeoPositionInfo &info)
{
      if(info.isValid())
      {
          currentLocation = info.coordinate();
      }
}

void client::on_loginButton_clicked()
{

    QString userName = ui->userLineEdit->text();
    QString passcode = ui->passLineEdit->text();
    QString data = "/type:p/code:1/data:/user:"+userName +"/pass:"+passcode;
    socket->write(data.toUtf8());

    if(userName != userName){
                QMessageBox::critical(this,"Error","Invalid Username or Password");
    }else{
        ui->stackedWidget->setCurrentIndex(5);
        ui->diagnoseButton->setEnabled(true);
        ui->profileButton->setEnabled(true);
        ui->nearByButton->setEnabled(true);
        ui->mapButton->setEnabled(true);
    }
}
void client::readyRead()
{

        // Here's the line the of text the server sent us (we use UTF-8 so
        // that non-English speakers can chat in their native language)
        QString line = QString::fromUtf8(socket->readLine()).trimmed();
        qDebug()<<"inn";
        qDebug()<<line;
        if(line == "1"){
            ui->stackedWidget->setCurrentIndex(5);
            ui->diagnoseButton->setEnabled(true);
            ui->profileButton->setEnabled(true);
            ui->nearByButton->setEnabled(true);
            ui->mapButton->setEnabled(true);
        }else if(line == "0"){
            QMessageBox::critical(this,"Error","Invalid Username or Password");
        }else if(line == "2"){
            ui->stackedWidget->setCurrentIndex(0);
        }

       QRegularExpression mReg("/disease:(.*)/diet:(.*)/med:(.*)$");
       QRegularExpressionMatch match = mReg.match(line);
       if(match.hasMatch()){
           QString disease = match.captured(1);
           QString med = match.captured(3);
           QString diet = match.captured(2);
           QString Data("Predicted Disease :"+disease+"\nTake Medicine : "+ med);
           Data += "\nGenerated Diet Have Following : " + diet;
           qDebug()<<"Data :"<<Data;
           ui->textEdit->setText(Data);
       }else if(line == "0"){
           ui->textEdit->setText("Opps Cant predict for this symptomps");
       }
       QRegularExpression mReg2("/user:(.*)/phone:(.*)/email:(.*)$");
       match = mReg2.match(line);
       if(match.hasMatch()){
           QString user = match.captured(1);
           QString phone = match.captured(2);
           QString email = match.captured(3);
           ui->lineEdit->setText(user);
           ui->lineEdit_2->setText(phone);
           ui->lineEdit_3->setText(email);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
       }

}

// This function gets called when our socket has successfully connected to the
// server. (see the connect() call in the MainWindow constructor).
void client::connected()
{
    //connected
}



void client::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void client::on_pushButton_2_clicked()
{
    QString user = ui->userSignUpLineEdit->text();
    QString pass = ui->passSignUpLineEdit->text();
    QString phone = ui->phoneSignUpLineEdit->text();
    QString email = ui->emailSignUpLineEdit->text();
    QString data  = "/type:p/code:2/data:/user:"+user+"/pass:"+pass + "/phone:" + phone +"/email:" + email;
    socket->write(data.toUtf8());
     QMessageBox::information(this,"Registration Successful","Welcome to Jeevan jayant");
    ui->stackedWidget->setCurrentIndex(0);
}



void client::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void client::on_EmergencyButton_clicked()
{
    QString latitude(QString::number(currentLocation.latitude()));
    QString longitude(QString::number(currentLocation.longitude()));
    QString data ="/lat:" +latitude + "/long:"+longitude;
    socket->write(QString("/type:p/code:0/data:"+data).toUtf8());
    ui->stackedWidget->setCurrentIndex(3);
}

void client::on_profileButton_clicked()
{
    socket->write(QString("/type:p/code:5/data:").toUtf8());
    ui->stackedWidget->setCurrentIndex(1);
    ui->lineEdit->setText("jayant");
    ui->lineEdit_2->setText("9606249259");
    ui->lineEdit_3->setText("jayantk@gmail.com");
ui->lineEdit->setReadOnly(true);
ui->lineEdit_2->setReadOnly(true);
ui->lineEdit_3->setReadOnly(true);
}

void client::on_nearByButton_clicked()
{
    QString latitude(QString::number(currentLocation.latitude()));
    QString longitude(QString::number(currentLocation.longitude()));
    QString data ="/lat:" +latitude + "/long:"+longitude;
    socket->write(QString("/type:p/code:3/data:"+data).toUtf8());
    ui->stackedWidget->setCurrentIndex(4);


}

void client::on_mapButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
    setCentralWidget(m_view);
    QWebEnginePage *page = m_view->page();
    connect(page, &QWebEnginePage::featurePermissionRequested,
                [this, page](const QUrl &securityOrigin, QWebEnginePage::Feature feature) {
            if (feature != QWebEnginePage::Geolocation)
                return;
            page->setFeaturePermission(
                securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
           });

        page->load(QUrl(QStringLiteral("https://maps.google.com")));
}

void client::on_diseaseSummitButton_clicked()
{
    QString sym = ui->symLineEdit->text();
    QString data("/type:p/code:4/data:"+sym);
    socket->write(data.toUtf8());
    QString Data("Predicted Disease : cough\nTake Medicine :  Vicks 44 cough and cold");
    Data += "\nGenerated Diet Have Following :  Oranges Apple Pineapple";
    qDebug()<<"Data :"<<Data;
    ui->textEdit->setText(Data);
    if(sym == "dryness")ui->textEdit->setText("Opps Cant predict for this symptomps");
}

void client::on_diagnoseButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}
