#ifndef PLANSZA_H
#define PLANSZA_H

#include <QObject>
#include "mysquare.h"

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

signals:
    void moved(int);

public slots:
    void clickDetector(int id);

};

#endif // PLANSZA_H
