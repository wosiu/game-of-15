#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "plansza.h"

#include<QMainWindow>
#include<QMessageBox>
#include<QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene *scene;
    void keyPressEvent(QKeyEvent* e);

private:
    Ui::MainWindow *ui;
    Plansza *plansza;
};

#endif // MAINWINDOW_H
