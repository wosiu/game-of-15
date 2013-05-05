#include "plansza.h"
#include "mysquare.h"
#include <QDebug>
#include <algorithm>    // std::random_shuffle
#include <QTime>

//sprawdza parzystosc inwersji wygenerowanej w generate()
//Algorytm tego Pana: http://students.mimuw.edu.pl/~potato/otwarty_problem.jpg :
bool Plansza::checkInversions()
{
    int res = 0;
    int iter = squareNumber;

    int L[squareNumber];
    int tb[squareNumber]; //kopia naszej permutacji

    for ( int i = 0; i < squareNumber; i++ )
    {
        L[i] = 0;
        tb[i] = idToPosition[i];
    }

    do {
        for ( int i = 0; i < squareNumber; i++ )
            if ( tb[i] % 2 )
                L[ tb[i] ]++;
            else
                res += L[ tb[i] + 1 ];

        for ( int i = 0; i < squareNumber; i++)
        {
            L[i] = 0;
            tb[i] /= 2;
        }
    } while ( iter /= 2 ); //wykonuje log2 n + 1

    return (res + 1) % 2;
}


//generuje plansze. Uzycie w konstruktorze.
void Plansza::generate( int mode )
{

    for ( int _id = 0; _id < squareNumber; _id++ )
        positionToId[_id] = idToPosition[_id] = _id;

    //poziom trudnosci:
    if ( mode == 4 ) //kompletny rand
    {
        do {
            qDebug() << "Plansza: generate(): generuje parzysta permutacje";
            std::random_shuffle( idToPosition, idToPosition + squareNumber );
        } while ( !checkInversions() );

        for ( int _id = 0; _id < squareNumber; _id++ )
            positionToId[ idToPosition[_id] ] = _id;

        emptyPosition = idToPosition[ 0 ];
    }
    else
    {
        int  _id, dx, dy, pos, lastEmptyPos = 0, n = 20 * mode;
        emptyPosition = 0;

        while ( n-- )
        {
            //losuję tylko prawidłowe ruchy
            if( qrand() % 2 ) //wybieram ruch - pion / poziom
            {
                if ( posX( emptyPosition ) == 0 ) dx = 1;
                else if ( posX( emptyPosition ) == wymiar - 1 ) dx = -1;
                else dx = ( qrand() % 2 ) ? (-1) : 1;
                dy = 0;
            }
            else
            {
                if ( posY( emptyPosition ) == 0 ) dy = 1;
                else if ( posY( emptyPosition ) == wymiar - 1 ) dy = -1;
                else dy = ( qrand() % 2 ) ? (-1) : 1;
                dx = 0;
            }


            //pozycja i id klocka, z ktorym zamienie puste miejsce:
            pos = emptyPosition + dx + dy * wymiar;

            _id = positionToId[ pos ];

            if( pos == lastEmptyPos ) n++; //jesli nic nie wnąszący ruch
            else
            {
                lastEmptyPos = emptyPosition;

                positionToId[ pos ] = positionToId[ emptyPosition ];
                positionToId[ emptyPosition ] = _id;

                solution.push_back( _id );
                idToPosition[ _id ] = emptyPosition;
                // !!! id emptyPosition = 0

                idToPosition[ 0 ] = emptyPosition = pos;

                //qDebug() << "Plansza: Generuje plansze, puste na:" << pos;
            }
        }

        isSolution = true;
        solutionIter = solution.size() - 1;
    }
}


//konstruktor
Plansza::Plansza( QGraphicsScene *scene, int generateMode ) :
    QObject( scene )
{
    scena = scene;
    movesCounter = 0;
    isSolution = false;

    generate( generateMode );

    //wypelnianie square'ami:
    for ( int id = 1; id < squareNumber; id++ )
    {
        squares[id] = new MySquare(
                    posX( idToPosition[id] ) * MySquare().rozmiar, //x na scenie
                    posY( idToPosition[id] ) * MySquare().rozmiar, //y na scenie
                    id, this );

        scena->addItem( squares[id] );

        connect( squares[id], SIGNAL( clicked(int) ),
                 this, SLOT( clickDetector(int) ) );
    }
}


//daje wspolrzedne danej pozycji:
int Plansza::posX( int pos )
{
    return pos % wymiar;
}
int Plansza::posY( int pos )
{
    return pos / wymiar;
}


//Sprawdza (czy wszystkie operacje na planszy zostały skończone
//oraz czy puzel o [id] ma w BEZPOŚREDNIM sasiedztwie empty)
//jak tak - rozkazuje puzlowi fizyczny ruch.
//uzuwana w clickDetector(), undo(), moveToEmptyFromSide(), showSolution*()
//zwraca: true - przesunieto / false - w p.p.
bool Plansza::checkAndMove( int id )
{
    if ( MySquare().getPressed() ) return false; //w trakcie przesuwania

    int pos = idToPosition[id];

    //sprawdzam otoczenie klocka, jesli wolne, przesuwam
    if( posX( pos ) < wymiar - 1  &&  pos + 1 == emptyPosition )
        squares[id]->move(1,0);
    else if( posX( pos ) > 0  &&  pos - 1 == emptyPosition )
        squares[id]->move(-1,0);
    else if( posY( pos ) > 0  &&  pos - wymiar == emptyPosition )
        squares[id]->move(0,-1);
    else if( posY( pos ) < wymiar - 1  &&  pos + wymiar == emptyPosition )
        squares[id]->move(0,1);
    else return false;

    //(*) positionToId[ emptyPosition ] = 0
    std::swap( positionToId[pos], positionToId[ emptyPosition ] );
    std::swap( idToPosition[id], emptyPosition  );

    qDebug()<<"Plansza: przesunieto puzel:"<<id;

    return true;
}


//przyjmuje sygnały od kliknietych puzli, emituje do mainwindow (licznik ruchow)
void Plansza::clickDetector( int id )
{
    qDebug()<<"Plansza: przechwycono klikniecie na puzel:"<<id;

    if( checkAndMove(id) )
    {
        movesCounter++;
        emit moved( movesCounter );
        emit solutionBack( true );
        emit solutionForward( false );
        history.push_back( id );
    }

}


//przesuwa klocek znajdujacy sie na (x,y) od pustego miejsca
//uzywane przy sterowaniu puzli klawiszami
void Plansza::moveToEmptyFromSide( int x, int y )
{
    int X = emptyPosition % wymiar + x;
    int Y = emptyPosition / wymiar + y;
    int pos = X + Y * wymiar;

    //wywołanie spoza planszy:
    if ( X < 0 || X >= wymiar || Y < 0 || Y >= wymiar ) return;

    //imituje klikniecie:
    clickDetector( positionToId[ pos ] );
}


//cofanie ruchu
void Plansza::undo()
{
    if( !history.empty() && checkAndMove( history.back() ) )
    {
        qDebug() << "Plansza: undo()";
        history.pop_back();
    }
}


// S H O W   S O L U T I O N:
bool Plansza::showSolutionBack()
{
    //if ( ! isSolution ) return; //TO DO: computing dla randoma

    //dzięki temu, można kontynuować gre w dowolnym momencie show solution:
    if ( !history.empty() )
    {
        //ucinam rozwiązanie za iteratorem, ponieważ gracz wykonał ruchy
        solution.resize( solutionIter + 1 );

        //przepisuję ruchy gracza do rozwiązania:
        for ( unsigned int i = 0; i < history.size(); i++ )
            solution.push_back( history[i] );

        //czyszczę historię
        history.clear();
        solutionIter = solution.size()-1;

        emit solutionForward( false );
    }

    if ( solutionIter == -1 ) return false; //koniec rozwiazania

    //(*) checkAndMove w if'ie, żeby nie zmniejszał solutionIter,
    //gdy nie wykonano ruchu, z powodu nie skonczonej poprzedniej akcji na scenie
    if ( checkAndMove( solution[ solutionIter ] ) )
    {
        solutionIter--;
        emit solutionForward( true );

        if( solutionIter == -1 )
            emit solutionBack( false );
    }
    return true;
}

bool Plansza::showSolutionForward()
{
    if ( solutionIter >= (int)solution.size() - 1 ) return false; //nie ma co pokazywac

    if ( checkAndMove( solution[ solutionIter + 1 ] ) )
    {
        solutionIter++;
        emit solutionBack( true );
    }

    if ( solutionIter >= (int)solution.size() - 1 )
        emit solutionForward( false );

    return true;
}

