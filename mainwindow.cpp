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

    connect( plansza, SIGNAL( moved(int) ), this, SLOT( showMovesCounter(int) ) );
    showMovesCounter( 0 );


    /*std::vector<int> chuj;
    for(int i=0; i<10;i++) chuj.push_back(i);
    std::vector<int>* kurwa = &chuj;
    kurwa->pop_back();
    qDebug() << "SIZE: " << chuj.size();
    */
}

void MainWindow::showMovesCounter( int val=0 )
{
    ui->statusBar->showMessage( "Ilość ruchów: " + QString::number( val ) );
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

/* TO DO:
 *QKeySequence::New
 */
void MainWindow::keyPressEvent(QKeyEvent* e)
{
    qDebug() << "MainWindow: Wcisnieto klawisz:" << e->text();

    if ( e == QKeySequence::Undo )
        plansza->undo();
    //else if( e->key() == Qt::DownArrow )
    //else if( e == QKeySequence::MoveToNextLine )
    //else if( e->key() == Qt::Key_Down )
    else if( e->key() == Qt::Key_S )
        plansza->moveToEmptyFromSide( 0, -1 );
    else if( e->key() == Qt::Key_W )
        plansza->moveToEmptyFromSide( 0, 1 );
    else if( e->key() == Qt::Key_D )
        plansza->moveToEmptyFromSide( -1, 0 );
    else if( e->key() == Qt::Key_A )
        plansza->moveToEmptyFromSide( 1, 0 );

};
