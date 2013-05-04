#include "plansza.h"
#include "mysquare.h"
#include <QDebug>
//#include <ctime>        // std::time
//#include <cstdlib>      // std::rand, std::srand
#include <algorithm>    // std::random_shuffle

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

void Plansza::generate( int mode = 0 )
{

    for ( int id = 0; id < squareNumber; id++ )
        idToPosition[id] = id;

    //poziom trudnosci:
    if ( mode == 0 ) //kompletny rand
    {
        do {
            qDebug() << "Plansza: generate(): generuje parzysta permutacje";
            std::random_shuffle( idToPosition, idToPosition + squareNumber );
        } while( !checkInversions() );
    }

    emptyPosition = idToPosition[ 0 ];
}

Plansza::Plansza( QGraphicsScene *scene ) :
    QObject( scene )
{
    scena = scene;
    movesCounter = 0;

    generate();

    //wypelnianie:
    for ( int id = 1; id < squareNumber; id++ )
    {
        squares[id] = new MySquare( (idToPosition[id] % wymiar) * MySquare().rozmiar, //wspolrzedna x
                                 (idToPosition[id] / wymiar) * MySquare().rozmiar, //wspolrzedna y
                                 id, this );

        scena->addItem( squares[id] );

        connect( squares[id], SIGNAL( clicked(int) ), this, SLOT( clickDetector(int) ) );
    }
    //scena->removeItem( squares[15] );
}

void Plansza::clickDetector(int id)
{
    qDebug()<<"Plansza: przechwycono klikniecie na puzel:"<<id;

    int pos = idToPosition[id];

    //sprawdzam otoczenie klocka, jesli wolne, przesuwam
    if( pos + 1 == emptyPosition )
        squares[id]->move(1,0);
    else if( pos - 1 == emptyPosition )
        squares[id]->move(-1,0);
    else if( pos - wymiar == emptyPosition )
        squares[id]->move(0,-1);
    else if( pos + wymiar == emptyPosition )
        squares[id]->move(0,1);
    else return;

    //jesli zamieniono, to:
    movesCounter++;
    emit moved(movesCounter);

    std::swap( idToPosition[id], emptyPosition  );
    qDebug()<<"Plansza: przesunieto puzel:"<<id;

}
