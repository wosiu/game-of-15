#include "plansza.h"
#include "mysquare.h"
#include <QDebug>
#include <algorithm>    // std::random_shuffle

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
void Plansza::generate( int mode = 0 )
{

    for ( int _id = 0; _id < squareNumber; _id++ )
        idToPosition[_id] = _id;

    //poziom trudnosci:
    if ( mode == 0 ) //kompletny rand
    {
        do {
            qDebug() << "Plansza: generate(): generuje parzysta permutacje";
            std::random_shuffle( idToPosition, idToPosition + squareNumber );
        } while( !checkInversions() );
    }

    for ( int _id = 0; _id < squareNumber; _id++ )
        positionToId[ idToPosition[_id] ] = _id;

    emptyPosition = idToPosition[ 0 ];


}

//konstruktor
Plansza::Plansza( QGraphicsScene *scene ) :
    QObject( scene )
{
    scena = scene;
    movesCounter = 0;

    generate();

    //wypelnianie square'ami:
    for ( int id = 1; id < squareNumber; id++ )
    {
        squares[id] = new MySquare( (idToPosition[id] % wymiar) * MySquare().rozmiar, //wspolrzedna x
                                 (idToPosition[id] / wymiar) * MySquare().rozmiar, //wspolrzedna y
                                 id, this );

        scena->addItem( squares[id] );

        connect( squares[id], SIGNAL( clicked(int) ), this, SLOT( clickDetector(int) ) );
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

//sprawdza (czy wszystkie operacje na planszy zostały skończone
//oraz czy klocek  o [id] ma w BEZPOŚREDNIM sasiedztwie empty)
//i fizycznie wykonuje ruch
//uzuwana w clickDetector(), undo(), moveToEmptyFromSide
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

    std::swap( positionToId[pos], positionToId[ emptyPosition ] ); //positionToId[ emptyPosition ] = 0
    std::swap( idToPosition[id], emptyPosition  );

    qDebug()<<"Plansza: przesunieto puzel:"<<id;

    return true;
}

void Plansza::clickDetector( int id )
{
    qDebug()<<"Plansza: przechwycono klikniecie na puzel:"<<id;

    if( checkAndMove(id) )
    {
        movesCounter++;
        emit moved( movesCounter );
        history.push_back( id );
    }

}

// klawiszo obsluga:

//cofanie ruchu
void Plansza::undo()
{
    if( !history.empty() && checkAndMove( history.back() ) )
        history.pop_back();
}


//przesuwa klocek znajdujacy sie na (x,y) od pustego miejsca
void Plansza::moveToEmptyFromSide( int x, int y )
{
    int X = emptyPosition % wymiar + x;
    int Y = emptyPosition / wymiar + y;
    int pos = X + Y * wymiar;

    //wywołanie spoza planszy:
    if ( X < 0 || X >= wymiar || Y < 0 || Y >= wymiar /*|| pos <= 0 || pos >= squareNumber*/ ) return;

    //imituje klikniecie:
    clickDetector( positionToId[ pos ] );
}
