#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image.h"
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include <SeaBattle/Field.h>
#include <SeaBattle/game.h>
#include <QTcpSocket>
#include "extrahandler.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int diff;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Game game;

    enum {
        PLAYER_CHOOSE,
        NAME,
        FIELD,
        TURN_CHOOSE,
        TURN

    } SOCKET_STATUS = NAME;

    Image images;
    QTcpSocket* socket;
    bool generated;
    bool turn = false;
    int SHIP_LENGTHS[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1 ,1};
    QString playerName;
    vector<QPoint> added_position;
    QPoint last_position;
    bool game_begin = false;
    bool game_end = false;
    void show_field(Field& field, QLabel* label, QImage ship,
                                                 QImage shot_ship,
                                                 QImage cell,
                                                 QImage shot_cell);

    static vector<QPoint> toPoint(QString stringPoints);

    void show_myField(Field& field);
    void show_enemyField(Field& field);
    void create_field(QImage& image);
    void set_cell(QPoint point);
    void remove_cell(QPoint point);
    void create_field(Player* player, vector<QPoint>& added_position);

    QVector<QPoint> ship_positions(const QPoint& point, const vector<QPoint>& added_position);

    void quit();
    void closeEvent(QCloseEvent *event);

    void setShipsHandler(QMouseEvent *ev);
    void playAi();
    void playerShootHandler(P point);
    bool endGame(const QString& value, Field& field);

    void messageWrite(const QString& value);

    void restart();
protected:

    void mousePressEvent(QMouseEvent *ev);

private slots:

    void on_pushButton_clicked();


    void on_Button_Go_clicked();
    void on_Button_Restart_clicked();
    void on_ServerButton_released();


    void on_FindOpon_clicked();

    void on_PlayAIButton_clicked();

public slots:
    void sockReady();
    void sockDisc();
};


#endif // MAINWINDOW_H
