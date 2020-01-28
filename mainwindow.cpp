#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setStyleSheet("background-color: white;");
    ui->OponField->hide();



    Player p("Player 1");
    Player comp("Computer", true);
    Field field;
    field.generate_field();
    comp.set_field(field);
    game.add_player(p);
    game.add_player(comp);
    QImage image(":/Images/white_cell.png");
    diff = image.width();
    create_field(image);
    this->update();
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::set_cell(QPoint point)
{
    QPoint newPoint(point.x()/diff, point.y()/diff);
    if(added_position.contains(newPoint))
        return;
    QImage ima = ui->myField->pixmap()->toImage();
    QPainter painter(&ima);
    QImage orange_cell(":/Images/orange_cell.png");
    painter.drawImage(newPoint*diff, orange_cell);
    added_position.push_back(newPoint);
    ui->myField->setPixmap(QPixmap::fromImage(ima));
}

void MainWindow::remove_cell(QPoint point)
{
    QPoint newPoint(point.x()/diff, point.y()/diff);
    QImage ima = ui->myField->pixmap()->toImage();
    QPainter painter(&ima);
    QImage orange_cell(":/Images/white_cell.png");
    painter.drawImage(newPoint*diff, orange_cell);
    added_position.removeOne(newPoint);
    std::sort(added_position.begin(), added_position.end(),
              [](QPoint p1, QPoint p2){ return p1.x() < p2.x() || p1.y() < p2.y(); });
    ui->myField->setPixmap(QPixmap::fromImage(ima));
}

void MainWindow::create_field()
{

    QVector<int> Length;
    Field f;


    std::sort(added_position.begin(), added_position.end(),
              [](QPoint p1, QPoint p2){return p1.y() < p2.y() || p1.x() < p2.x();});
    std::vector<QPoint> a(added_position.begin(), added_position.end());
    for(auto& x: a)
    {
        if(f.is_ship(P(x.x(), x.y())))
        {
            continue;
        }

        QVector<QPoint> ship_position = ship_positions(x);
        Ship new_ship(P(ship_position.front().x(), ship_position.front().y()),
                      P(ship_position.back().x(), ship_position.back().y()));


        if(f.is_valid(new_ship))
        {
            f.set_ship(new_ship);
            Length.push_back(new_ship.size());
        }
        else
        {
            f.clear();
            game.get_current_player().set_field(f);
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

    game.get_current_player().set_field(f);
}

QVector<QPoint> MainWindow::ship_positions(const QPoint& point1)
{

    QVector<QPoint> ship_position;
    ship_position.push_back(point1);
    QPoint pot1(point1 + QPoint(0, 1));
    QPoint pot2(point1 + QPoint(1, 0));
    if(added_position.contains(pot1))
    {


        ship_position.append(ship_positions(pot1));
    }

    else if(added_position.contains(pot2))
    {

        ship_position.append(ship_positions(pot2));
    }
    return ship_position;



}

void MainWindow::show_field(Field& field, QLabel* label, QImage ship,
                                                         QImage shot_ship,
                                                         QImage cell,
                                                         QImage shot_cell)
{
    QImage field_image(":/Images/real_field");
    QPainter painter(&field_image);

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



    QPixmap m_NewPixMap = QPixmap::fromImage(field_image);
    label->setPixmap(QPixmap(m_NewPixMap));
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
    QImage field(":/Images/real_field.png");

    QPainter painter(&field);

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            painter.drawImage(i*diff, j*diff, image);
        }
    }



    QPixmap m_NewPixMap = QPixmap::fromImage(field);
    ui->OponField->setPixmap(QPixmap(m_NewPixMap));
    ui->myField->setPixmap(QPixmap(m_NewPixMap));

}



void MainWindow::mousePressEvent(QMouseEvent *ev)
{

    if(!game_begin)
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
        return;
    }
    int x = ev->x() - ui->OponField->x();
    int y = ev->y() - ui->OponField->y();


    if(((0 <= x && x < 10*diff) && (0 <= y && y < diff*10)) == 0)
    {

        return;
    }

    y = (y / diff)*diff;
    x = (x / diff)*diff;
    if(game.enemy_field().is_shot(P(x/diff, y/diff)))
    {

        return;
    }
    QImage ima= ui->OponField->pixmap()->toImage();

    game.enemy_field().shoot(P(x/diff, y/diff));
    show_enemyField(game.enemy_field());
    this->update();
    if(game.enemy_field().is_ship(P(x/diff, y/diff)))
    {
        if(game.game_over(game.enemy_field()))
            {
                QMessageBox quitMsg;
                quitMsg.setWindowTitle("Sea Battle");
                quitMsg.setText("You won!");
                quitMsg.setStandardButtons(QMessageBox::Ok);
                quitMsg.setDefaultButton(QMessageBox::Ok);
                if (quitMsg.exec() == QMessageBox::Ok)
                {
                    return;
                }

            }

        return;
    }
    game.change_player();
    if(game.get_current_player().is_computer)
    {
        while(game.get_current_player().random_shoot(game.enemy_field()))
        {
              show_myField(game.enemy_field());
              if(game.game_over(game.enemy_field()))
              {
                  QMessageBox quitMsg;
                  quitMsg.setWindowTitle("Sea Battle");
                  quitMsg.setText("You lost!");
                  quitMsg.setStandardButtons(QMessageBox::Ok);
                  quitMsg.setDefaultButton(QMessageBox::Ok);
                  if (quitMsg.exec() == QMessageBox::Ok)
                  {
                      return;
                  }
                  return;
              }

              this->update();
        }
        show_myField(game.enemy_field());
        this->update();
        game.change_player();


    }
}


void MainWindow::on_pushButton_clicked()
{
    Field f;
    f.generate_field();
    game.get_current_player().set_field(f);
    show_myField(game.current_field());
    generated = true;
}


void MainWindow::on_Button_Go_clicked()
{

    if(!generated)
        create_field();
    if(game.current_field().is_empty())
    {

        QMessageBox msg;
        msg.setWindowTitle("Sea Battle");
        msg.setText("Unappropriate field!");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        if (msg.exec() == QMessageBox::Ok)
        {
            return;
        }
        return;
    }
    show_myField(game.current_field());
    ui->Button_Go->hide();
    ui->pushButton->hide();
    ui->OponField->show();
    game_begin = true;

}

void MainWindow::on_Button_Restart_clicked()
{
    ui->Button_Go->show();
    ui->pushButton->show();
    ui->OponField->hide();
    game_begin = false;
    generated = false;
    added_position.clear();
    game.clear();
    Player p("Player 1");
    Player comp("Computer", true);
    Field field;
    show_myField(field);
    show_enemyField(field);
    field.generate_field();
    comp.set_field(field);
    game.add_player(p);
    game.add_player(comp);

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
