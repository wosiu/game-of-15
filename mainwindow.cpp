#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::newGame( int level )
{
    if( play )
    {
        plansza->disconnect();
        delete plansza;
        scene->clear();
    }

    plansza = new Plansza( scene, level );

    connect( plansza, SIGNAL( moved(int) ), this, SLOT( showMovesCounter(int) ) );
    showMovesCounter( 0 );

    play = true;
    ui->actionCofnij_ruch->setEnabled(true);
    ui->actionBack->setEnabled(true);
    ui->actionForward->setEnabled(true);
    ui->actionStart->setEnabled(true);
    //ui->actionStop->setEnabled(true);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene( scene );
    //ui->statusBar->showMessage("tekst");

    /*plansza = new Plansza( scene, 3 );
    connect( plansza, SIGNAL( moved(int) ), this, SLOT( showMovesCounter(int) ) );
    showMovesCounter( 0 );*/

    play = false;
}

void MainWindow::showMovesCounter( int val=0 )
{
    ui->statusBar->showMessage( "Ilość ruchów: " + QString::number( val ) );
}

//TO DO: show level

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

    //else if( e->key() == Qt::DownArrow )
    //else if( e == QKeySequence::MoveToNextLine )
    //else if( e->key() == Qt::Key_Down )
    if( e->key() == Qt::Key_S )
        plansza->moveToEmptyFromSide( 0, -1 );
    else if( e->key() == Qt::Key_W )
        plansza->moveToEmptyFromSide( 0, 1 );
    else if( e->key() == Qt::Key_D )
        plansza->moveToEmptyFromSide( -1, 0 );
    else if( e->key() == Qt::Key_A )
        plansza->moveToEmptyFromSide( 1, 0 );

};


//OBSŁUGA MENU:
void MainWindow::on_actionLatwa_triggered()
{
    newGame(1);
}

void MainWindow::on_actionNormalna_triggered()
{
    newGame(2);
}

void MainWindow::on_actionTrudna_triggered()
{
    newGame(3);
}

void MainWindow::on_actionHardcore_triggered()
{
    newGame(4);
}

void MainWindow::on_actionCofnij_ruch_triggered()
{
    if ( play )
      plansza->undo();
}
