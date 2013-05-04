#ifndef PLANSZA_H
#define PLANSZA_H

#include <QObject>
#include "mysquare.h"
#include <vector>

#include <QKeyEvent>

class Plansza : public QObject
{
    Q_OBJECT
public:
    Plansza( QGraphicsScene *scene = 0 );

private:
    QGraphicsScene *scena;
    static const int wymiar = 4;
    static const int squareNumber = wymiar * wymiar;
    int movesCounter;
    MySquare* squares[ squareNumber ]; //plansza 4 na 4
    int idToPosition[ squareNumber ];
    int emptyPosition;
    void generate(int mode);
    bool checkInversions();
    std::vector <int> history; //historia ruchow
    bool checkAndMove( int id );

signals:
    void moved(int);

public slots:
    //wykrywa klik i nakazuje przesuwania
    //tu podpiete sygnaly ze square i klawisze
    void clickDetector(int id);
    void undo();
};

#endif // PLANSZA_H
