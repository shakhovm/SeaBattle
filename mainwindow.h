#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "image.h"
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include <SeaBattle/Field.h>
#include <SeaBattle/game.h>

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
    Image images;
    int SHIP_LENGTHS[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1 ,1};
    QVector<QPoint> added_position;
    QPoint last_position;
    bool game_begin = false;
    void show_field(Field& field, QLabel* label, QImage ship,
                                                 QImage shot_ship,
                                                 QImage cell,
                                                 QImage shot_cell);

    void show_myField(Field& field);
    void show_enemyField(Field& field);
    void create_field(QImage& image);
    void set_cell(QPoint point);
    void remove_cell(QPoint point);
    void create_field();
    QVector<QPoint> ship_positions(const QPoint& point);
    bool generated;
    void quit();
    void closeEvent(QCloseEvent *event);
protected:

    void mousePressEvent(QMouseEvent *ev);

private slots:

    void on_pushButton_clicked();


    void on_Button_Go_clicked();
    void on_Button_Restart_clicked();
};


#endif // MAINWINDOW_H
