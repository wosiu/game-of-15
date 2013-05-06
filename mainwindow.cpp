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
    ui->actionStart->setEnabled(true);
    ui->actionForward->setEnabled(false);
    connect( plansza, SIGNAL(solutionBack(bool)), this, SLOT(backVisibility(bool)) );
    connect( plansza, SIGNAL(solutionForward(bool)), this, SLOT(forwardVisibility(bool)) );
}

//void MyQGraphicsView::resizeEvent(QResizeEvent *event)
void MainWindow::resizeEvent(QResizeEvent *event)
{
    const qreal defaultHeight = 482;
    const qreal defaultWidth = 422;

    const qreal heighScale = event->size().height() / defaultHeight;
    const qreal widthScale = event->size().width() / defaultWidth;

    ui->graphicsView->resetTransform();
    ui->graphicsView->scale( widthScale, heighScale );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene( scene );

    licznik = new QLabel();
    licznik->setIndent(10);
    ui->statusBar->addWidget( licznik );

    play = false;
}

void MainWindow::showMovesCounter( int val=0 )
{
    licznik->setText( "Ilość ruchów: " + QString::number( val ) );
    //ui->statusBar->showMessage( "Ilość ruchów: " + QString::number( val ) );
}


MainWindow::~MainWindow()
{
    delete licznik;
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent* e)
{
    qDebug() << "MainWindow: Wcisnieto klawisz:" << e->text();

    if( e->key() == Qt::Key_S )
        plansza->moveToEmptyFromSide( 0, -1 );
    else if( e->key() == Qt::Key_W )
        plansza->moveToEmptyFromSide( 0, 1 );
    else if( e->key() == Qt::Key_D )
        plansza->moveToEmptyFromSide( -1, 0 );
    else if( e->key() == Qt::Key_A )
        plansza->moveToEmptyFromSide( 1, 0 );

};


/* O B S Ł U G A    M E N U : */

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

void MainWindow::on_actionZamknij_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionAutor_triggered()
{
    QMessageBox* box = new QMessageBox(this);
    box->setWindowTitle(QString("Autor"));
    box->setText(QString(
                     "Michał Woś\nmw336071@students.mimuw.edu.pl"));
    box->setButtonText(1,"Zamknij");
    box->show();
}

void MainWindow::on_actionBack_triggered()
{
    plansza->showSolutionBack();
}

void MainWindow::on_actionForward_triggered()
{
    plansza->showSolutionForward();
}

void MainWindow::on_actionStart_triggered()
{
    ui->actionStart->setEnabled( false );
    ui->actionStop->setEnabled( true );
    plansza->setSolutionRunning( true );
}


void MainWindow::on_actionStop_triggered()
{
    ui->actionStart->setEnabled( true );
    ui->actionStop->setEnabled( false );
    plansza->setSolutionRunning( false );
}

void MainWindow::backVisibility( bool b)
{
    ui->actionBack->setEnabled( b );
    ui->actionStart->setEnabled( b );

    if( b == false ) ui->actionStop->setEnabled( b );
}

void MainWindow::forwardVisibility( bool b )
{
    ui->actionForward->setEnabled( b );
}

void MainWindow::on_actionSkroty_klawiszowe_triggered()
{
    QMessageBox* box = new QMessageBox(this);
    box->setWindowTitle(QString("Dodatowe skróty klawiszowe"));
    QString tekst = "<h3>Poruszanie klockami sąsiednimi do wolnego pola:</h3>";
    tekst += "W - dolnym, ";
    tekst += "S - górnym, ";
    tekst += "A - lewym, ";
    tekst += "D - prawym.";

    box->setText( tekst );
    box->setButtonText(1,"Zamknij");
    box->show();
}
