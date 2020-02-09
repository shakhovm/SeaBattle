#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <algorithm>
#include <QInputDialog>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setStyleSheet("background-color: white;");
    ui->OponField->hide();
    bool ok;
    playerName = QInputDialog::getText(this, tr("Sea Battle"),
                                 tr("Enter Your Name"), QLineEdit::Normal,
                                 "Player 1", &ok);
    ui->nameLabel->setText(playerName);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    ui->FindOpon->hide();
    game.set_alPlayer(new Player(playerName.toStdString()));
    diff = images["white cell"].width();
    create_field(images["white cell"]);
}



MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
}


void MainWindow::set_cell(QPoint point)
{
    QPoint newPoint(point.x()/diff, point.y()/diff);
    if(std::find(added_position.begin(), added_position.end(), newPoint) != added_position.end())
        return;
    QImage ima = ui->myField->pixmap()->toImage();
    QPainter painter(&ima);
    painter.drawImage(newPoint*diff, images["orange cell"]);
    added_position.push_back(newPoint);
    ui->myField->setPixmap(QPixmap::fromImage(ima));
}

void MainWindow::remove_cell(QPoint point)
{
    QPoint newPoint(point.x()/diff, point.y()/diff);
    QImage ima = ui->myField->pixmap()->toImage();
    QPainter painter(&ima);
    painter.drawImage(newPoint*diff, images["white cell"]);

    for (int i = 0; i < added_position.size(); ++i) {
        if (added_position[i] == newPoint) {
            added_position.erase(added_position.begin() + i);
        }
    }
    ui->myField->setPixmap(QPixmap::fromImage(ima));
}




void MainWindow::create_field(Player* player, vector<QPoint>& added_position)
{

    vector<int> Length;
    Field f;
    ExtraHandler::sortPointsArray(added_position);
    for(auto& x: added_position)
    {
        if(f.is_ship(P(x.x(), x.y())))
        {
            continue;
        }

        QVector<QPoint> ship_position = ship_positions(x, added_position);
        Ship new_ship(P(ship_position.front().x(), ship_position.front().y()),
                      P(ship_position.back().x(), ship_position.back().y()));


        if(f.is_valid(new_ship))
        {
            f.set_ship(new_ship);
            Length.push_back(new_ship.size());
        }
        else
        {
            qDebug() << x;
            f.clear();
            player->set_field(f);
            return;

        }

    }

    std::sort(Length.begin(), Length.end(),
              [](int s1, int s2){return s1 > s2;});
    if (Length.size() != 10) {
        f.clear();
    }
    else {
        for (int i = 0; i < 10; ++i) {
            if (SHIP_LENGTHS[i] != Length[i])
            {
                f.clear();
                break;
            }
        }
    }
    player->set_field(f);
}

QVector<QPoint> MainWindow::ship_positions(const QPoint& point1, const vector<QPoint>& added_position)
{

    QVector<QPoint> ship_position;
    ship_position.push_back(point1);
    QPoint pot1(point1 + QPoint(0, 1));
    QPoint pot2(point1 + QPoint(1, 0));
    if(std::find(added_position.begin(), added_position.end(), pot1) != added_position.end())
    {
        ship_position.append(ship_positions(pot1, added_position));
    }

    else if(std::find(added_position.begin(), added_position.end(), pot2) != added_position.end())
    {
        ship_position.append(ship_positions(pot2, added_position));
    }
    return ship_position;



}

void MainWindow::show_field(Field& field, QLabel* label, QImage ship,
                                                         QImage shot_ship,
                                                         QImage cell,
                                                         QImage shot_cell)
{
    QPainter painter(&images["field"]);

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            P position(i, j);
            int x = i*diff;
            int y = j*diff;
            if(!field.is_shot(position))
            {


                if(field.is_ship(position))
                {
                    painter.drawImage(x, y, ship);
                }
                else {
                    painter.drawImage(x, y, cell);
                }
            }
            else
            {
                if(field.is_ship(position))
                {
                    painter.drawImage(x, y, shot_ship);
                }
                else {
                    painter.drawImage(x, y, shot_cell);
                }
            }
         }
     }



    QPixmap m_NewPixMap = QPixmap::fromImage(images["field"]);
    label->setPixmap(QPixmap(m_NewPixMap));
}

vector<QPoint> MainWindow::toPoint(QString stringPoints)
{
    vector<QPoint> v;
    for (int i = 0; i < stringPoints.size(); i += 2) {
        v.emplace_back(stringPoints[i].digitValue(), stringPoints[i + 1].digitValue());
    }
    return v;
}

void MainWindow::show_myField(Field& field)
{
    show_field(field, ui->myField, images["orange cell"],
                                   images["red cell"],
                                   images["white cell"],
                                   images["blue cell"]);
}

void MainWindow::show_enemyField(Field& field)
{
    show_field(field, ui->OponField, images["white cell"],
                                     images["red cell"],
                                     images["white cell"],
                                     images["blue cell"]);
}


void MainWindow::create_field(QImage& image)
{

    QPainter painter(&images["field"]);

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            painter.drawImage(i*diff, j*diff, image);
        }
    }



    QPixmap m_NewPixMap = QPixmap::fromImage(images["field"]);
    ui->OponField->setPixmap(QPixmap(m_NewPixMap));
    ui->myField->setPixmap(QPixmap(m_NewPixMap));

}



void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(game_end)
        return;

    if(!game_begin)
    {

        setShipsHandler(ev);
        return;
    }

    if(!turn)
    {
        return;
    }

    int x = ev->x() - ui->OponField->x();
    int y = ev->y() - ui->OponField->y();


    if(((0 <= x && x < 10*diff) && (0 <= y && y < diff*10)) == 0)
    {

        return;
    }

    y /= diff;
    x /= diff;
    if(game.enemy_field().is_shot(P(x, y)))
    {
        return;
    }
    QImage ima = ui->OponField->pixmap()->toImage();

    game.enemy_field().shoot(P(x, y));
    show_enemyField(game.enemy_field());
    if(!game.get_enemyPlayer()->is_computer)
        socket->write((QString::number(x) + QString::number(y)).toUtf8());
    if(game.enemy_field().is_ship(P(x, y)))
    {
        endGame("You won!", game.enemy_field());
        return;
    }
    turn = false;
    ui->turnLabel->setText("Enemy's turn!");
    if(game.get_enemyPlayer()->is_computer)
    {
        playAi();
        turn = true;
    }

}


bool MainWindow::endGame(const QString& value, Field& field)
{
    if(game.game_over(field))
    {
        game_begin = false;
        QMessageBox quitMsg;
        quitMsg.setWindowTitle("Sea Battle");
        quitMsg.setText(value);
        quitMsg.setStandardButtons(QMessageBox::Ok);
        quitMsg.setDefaultButton(QMessageBox::Ok);
        if (quitMsg.exec() == QMessageBox::Ok)
        {
            return true;
        }
        return true;

    }
}

void MainWindow::messageWrite(const QString &value)
{
    QMessageBox msg;
    msg.setWindowTitle("Sea Battle");
    msg.setText(value);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    if (msg.exec() == QMessageBox::Ok)
    {
        return;
    }
    return;
}


void MainWindow::playAi()
{
    while(game.get_enemyPlayer()->random_shoot(game.al_field()))
    {
          show_myField(game.al_field());
          if(endGame("You lost", game.al_field()))
          {
            show_myField(game.al_field());
            return;
          }
    }
    show_myField(game.al_field());
}

void MainWindow::playerShootHandler(P point)
{
    if(game.al_field().is_shot(point))
    {

        return;
    }
    QImage ima = ui->OponField->pixmap()->toImage();

    game.al_field().shoot(point);
    show_myField(game.al_field());
    this->update();
    if(game.al_field().is_ship(point))
    {
        endGame("You lost!", game.al_field());
        return;
    }
    turn = true;
}



void MainWindow::on_pushButton_clicked()
{
    Field f;
    f.generate_field();
    added_position.clear();
    for (auto& x: f.POSITIONS) {
        added_position.emplace_back(x.first, x.second);
    }
    show_myField(f);
}


void MainWindow::on_Button_Go_clicked()
{
    if(!game.get_enemyPlayer())
    {
        messageWrite("There is no enemy!");
        return;
    }
    create_field(game.get_alPlayer(), added_position);
    qDebug() << "OK";
    if(game.al_field().is_empty())
    {
        messageWrite("Unappropriate field!");
        return;
    }

    ui->PlayAIButton->hide();
    ui->ServerButton->hide();
    ui->FindOpon->hide();
    show_myField(game.al_field());
    ui->Button_Go->hide();
    ui->pushButton->hide();
    if(game.get_enemyPlayer()->is_computer)
    {
        ui->turnLabel->setText("Your turn!");
        game_begin = true;
        return;
    }
    QByteArray positionsData;
    for (auto& x: added_position) {
        positionsData.push_back((QString::number(x.x()) + QString::number(x.y())).toUtf8());
    }
    socket->write(positionsData);
}

void MainWindow::on_Button_Restart_clicked()
{
    restart();
}


void MainWindow::restart()
{
    ui->Button_Go->show();
    ui->pushButton->show();
    ui->OponField->hide();
    ui->ServerButton->show();

    game_begin = false;
    added_position.clear();
    create_field(images["white cell"]);
    game_end = false;

    if (game.get_enemyPlayer()->is_computer)
    {
        ui->PlayAIButton->show();
        Field f;
        f.generate_field();

        game.get_enemyPlayer()->set_field(f);
        return;
    }

    SOCKET_STATUS = NAME;
    ui->FindOpon->show();
    if(socket->state() == QTcpSocket::ConnectedState)
        socket->write("DISCONNECTED");
}



void MainWindow::quit()
{
    QMessageBox quitMsg;
    quitMsg.setWindowTitle("Sea Battle");
    quitMsg.setText("Do you really want to quit?");
    quitMsg.setStandardButtons(QMessageBox::Yes |
                               QMessageBox::Cancel);
    quitMsg.setDefaultButton(QMessageBox::Cancel);
    if (quitMsg.exec() == QMessageBox::Yes)
        QApplication::instance()->quit();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    quit();
}

void MainWindow::setShipsHandler(QMouseEvent *ev)
{
    if(((0 <= ev->pos().x()/diff && ev->pos().x()/diff < 10) &&
        (0 <= ev->pos().y()/diff && ev->pos().y()/diff < 10)) == 0)
    {
        return;
    }

    if(ev->button() == Qt::RightButton)
    {
        remove_cell(ev->pos());
    }
    else if (ev->button() == Qt::LeftButton) {
        set_cell(ev->pos());
    }


    last_position = ev->pos();
}


void MainWindow::on_ServerButton_released()
{
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        socket->disconnectFromHost();
        ui->ServerButton->setText("Connect");
        ui->PlayAIButton->show();
        ui->FindOpon->hide();
        return;
    }

    bool ok;
    QString address = QInputDialog::getText(this, tr("Sea Battle"),
                                         tr("Enter Server Address"), QLineEdit::Normal,
                                         "127.0.0.1", &ok);

    if(!ok)
        return;
    int port = QInputDialog::getInt(this, tr("Sea Battle"),
                                         tr("Enter Port"), 8000, 0, 9999, 1, &ok);
    if(!ok)
        return;


    socket->connectToHost(address, port);
    if(socket->waitForConnected(500))
    {
        ui->PlayAIButton->hide();
        ui->FindOpon->show();
        SOCKET_STATUS = NAME;

        socket->write(playerName.toUtf8());
        ui->ServerButton->setText("Disconnect");
    }
    else {
        messageWrite("Failed to connect!");
    }

}

void MainWindow::sockDisc()
{
    restart();
}


void MainWindow::sockReady()
{
    QByteArray data;
    if (socket->waitForConnected(50))
    {
        socket->waitForReadyRead(50);
        data = socket->readAll();
        if (data == "DISCONNECTED")
        {
            messageWrite("Your opponent was disconnected!");
            restart();
            return;
        }
        switch (SOCKET_STATUS) {

        case PLAYER_CHOOSE:
        {
            QStringList stringList = QString(data).split('\n');
            bool x;

            QString value = QInputDialog::getItem(this, "Sea Battle",
                                                  "Choose Oponent", stringList, 0, true, &x);
            if(!x)
                return;
            socket->write(QString(value[0]).toUtf8());
            SOCKET_STATUS = NAME;
            break;
        }

        case NAME:
            game.set_enemyPlayer(new Player(data.toStdString()));

            SOCKET_STATUS = TURN_CHOOSE;
            ui->enemy_nameLabel->setText(data);
            break;

        case TURN_CHOOSE:
            turn = data.toInt();
            SOCKET_STATUS = FIELD;
            if(turn)
                ui->turnLabel->setText("Your turn!");
            else
                ui->turnLabel->setText("Enemy's turn!");
            break;

        case FIELD:
        {
            vector<QPoint> enemyPositions = toPoint(data);
            create_field(game.get_enemyPlayer(), enemyPositions);
            ui->OponField->show();
            game_begin = true;
            SOCKET_STATUS = TURN;
            break;
        }

        case TURN:
            playerShootHandler(P(data.toInt()/10, data.toInt()%10));
            break;
        }
    }
}

void MainWindow::on_FindOpon_clicked()
{
    SOCKET_STATUS = PLAYER_CHOOSE;
    socket->write("PLAYER LIST");
}

void MainWindow::on_PlayAIButton_clicked()
{
    game.set_enemyPlayer(new Player("AI", true));
    Field f;
    f.generate_field();
    game.get_enemyPlayer()->set_field(f);
    ui->OponField->show();
    turn = true;
}
