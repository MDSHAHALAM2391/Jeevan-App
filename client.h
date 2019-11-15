#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QGeoCoordinate>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QWebEngineView>
namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

private slots:
     void on_loginButton_clicked();
     // This is a function we'll connect to a socket's readyRead()
     // signal, which tells us there's text to be read from the chat
     // server.
     void readyRead();

     // This function gets called when the socket tells us it's connected.
     void connected();

     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

     void positionUpdated(const QGeoPositionInfo &info);

     void on_pushButton_4_clicked();

     void on_EmergencyButton_clicked();

     void on_profileButton_clicked();

     void on_nearByButton_clicked();

     void on_mapButton_clicked();

     void on_diseaseSummitButton_clicked();

     void on_diagnoseButton_clicked();

private:
    Ui::client *ui;
    QTcpSocket *socket;
    QGeoCoordinate currentLocation;
    QWebEngineView *m_view;
   QGeoPositionInfoSource *source;
};

#endif // CLIENT_H
