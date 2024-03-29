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
    void resizeEvent( QResizeEvent *event );

protected:
    void keyPressEvent(QKeyEvent* e);

private:
    Ui::MainWindow *ui;
    Plansza *plansza;
    QLabel *licznik;
    void newGame( int level );
    bool play; //czy trwa rozgrywka

public slots:
    void showMovesCounter( int );

private slots:
    void on_actionLatwa_triggered();
    void on_actionNormalna_triggered();
    void on_actionTrudna_triggered();
    void on_actionHardcore_triggered();
    void on_actionCofnij_ruch_triggered();
    void on_actionZamknij_triggered();
    void on_actionAutor_triggered();
    void on_actionBack_triggered();
    void on_actionForward_triggered();
    void backVisibility( bool b );
    void forwardVisibility( bool b );
    void on_actionSkroty_klawiszowe_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
};

#endif // MAINWINDOW_H
