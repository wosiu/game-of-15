#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    plansza = new Plansza( scene );
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void Pomoc()
{
    QMessageBox* box = new QMessageBox();
    box->setWindowTitle(QString("Hello"));
    box->setText(QString("You Pressed: ")+ e->text());
    box->show();
}*/

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if ( e == QKeySequence::Undo )
    {
        qDebug() <<"MainWindow: Wcisnieto kombinacje \"undo\"";
        //plansza->undo();
    }
    else
        qDebug() << "MainWindow: Wcisnieto klawisz:" << e->text();
};
