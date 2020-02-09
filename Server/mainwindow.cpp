#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>

#include <random>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    startServer();
    model = new QStringListModel(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(auto& x: players) {
        x.second->socket->disconnectFromHost();
    }
}

void MainWindow::listViewHandler(const QString &value)
{
    stringList.push_back(value);
    model->setStringList(stringList);
    ui->listView->setModel(model);
}

void MainWindow::incomingConnenction()
{
    socket = server->nextPendingConnection();

    int id = socket->socketDescriptor();
    connect(socket,SIGNAL(readyRead()),this, SLOT(sockReady()));
    connect(socket,SIGNAL(disconnected()),this, SLOT(sockDisc()));
    qDebug() << "Client Connected!";

    Player* player = new Player;
    player->socket = socket;
    players.emplace(id, player);
    sockets.push_back(id);

}

void MainWindow::sockReady()
{

    socket = static_cast<QTcpSocket*>(sender());
    int socket_id = socket->socketDescriptor();
    socket->waitForReadyRead(50);


    QString data = socket->readAll();
    if (data == "PLAYER LIST")
    {
        QString string = "";
        for (int i = 0; i < sockets.size(); ++i)
        {
            string = string + QString::number(i) + '.' +
                    players[sockets[i]]->name + '\n';
        }
        socket->write(string.toUtf8());
        return;
    }

    if (data == "DISCONNECTED")
    {
        qDebug() << players[socket_id]->status;
        players[socket_id]->status = Player::ROOM;
        disconnectHandler(players[socket_id]);
        return;
    }


    switch (players[socket_id] -> status)
    {
    case Player::NAME:
        players[socket_id]->name = data;
        players[socket_id]->status = Player::ROOM;
        players[socket_id]->id = socket_id;
        listViewHandler(data + '-' + QString::number(socket_id));
        break;

    case Player::ROOM:

    {
        int oponId = sockets[data.toInt()];
        players[socket_id]->oponId = oponId;
        players[oponId]->oponId = players[socket_id]->id;
        players[socket_id]->status = Player::FIELD;
        players[oponId]->status = Player::FIELD;
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(0, 1);
        int turn = distribution(generator);
        players[socket_id]->socket->write(players[oponId]->name.toUtf8());
        players[oponId]->socket->write(players[socket_id]->name.toUtf8());

        players[socket_id]->socket->waitForReadyRead(50);
        players[oponId]->socket->waitForReadyRead(50);
        players[socket_id]->socket->write(QString::number(turn).toUtf8());
        players[oponId]->socket->write(QString::number(!turn).toUtf8());
        break;
    }

    case Player::FIELD:
        players[players[socket_id]->oponId]->socket->write(data.toUtf8());
        break;
    }

}

void MainWindow::startServer()
{
    bool ok;
    QString address = QInputDialog::getText(this, tr("Sea Battle"),
                                         tr("Enter Server Address"), QLineEdit::Normal,
                                         "127.0.0.1", &ok);

    if(!ok)
        return;

    int port = QInputDialog::getInt(this, tr("Sea Battle"),
                                         tr("Enter Port"), 8000);
    if (server->listen(QHostAddress(address), port))
    {
        connect(server, SIGNAL(newConnection()), this, SLOT(incomingConnenction()));
        qDebug() << "Listening!";
    }

    else {
        qDebug() << "Not listen!";
    }
}


void MainWindow::sockDisc()
{
    QTcpSocket* currentSocket = static_cast<QTcpSocket*>(sender());
    int current_ds;
    stringList.clear();
    for(auto& values: players)
    {

        if (values.second->socket->socketDescriptor() == -1)
        {

            current_ds = values.first;
            disconnectHandler(values.second);
            players.erase(values.first);
            continue;
        }
        stringList.push_back(values.second->name + '-' + QString::number(values.first));

    }

    model->setStringList(stringList);
    ui->listView->setModel(model);

    for(int i = 0; i < sockets.size(); ++i)
    {
        if (sockets[i] == current_ds)
        {
            sockets.erase(sockets.begin() + i);
            break;
        }
    }
    currentSocket->deleteLater();

}

void MainWindow::disconnectHandler(Player* player)
{
    if (player->oponId == -1)
        return;
    players[player->oponId]->socket->write("DISCONNECTED");
    players[player->oponId]->oponId = -1;
}
