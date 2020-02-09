#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "room.h"
#include <vector>
#include <QStringListModel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::map<int, Player*> players;
    std::vector<int> sockets;
    QTcpSocket* socket;
    QTcpServer* server;
    QStringList stringList;
    QStringListModel* model;
    QByteArray data;

    void listViewHandler(const QString& value);

public slots:
    void incomingConnenction();
    void sockReady();
    void startServer();
    void sockDisc();
    void disconnectHandler(Player* player);
};

#endif // MAINWINDOW_H
