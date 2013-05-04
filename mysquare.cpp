#include "mysquare.h"
#include <QDebug>

bool MySquare::pressed = false;

MySquare::MySquare(int x, int y, int id, QObject *parent) :
    QObject(parent)
{
    this->id=id;
    this->startX = x;
    this->startY = y;
}

MySquare::MySquare() {}

QRectF MySquare::boundingRect() const
{
    return QRectF(startX,startY,rozmiar,rozmiar);
}

void MySquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush brush(Qt::green);
    painter->fillRect(rec,brush);
    painter->drawRect(rec);

    //debug wzglednych wspolrzednych:
    //QString xStr = QString::number( this->x() );
    //QString yStr = QString::number( this->y() );
    //painter->drawText(rec,xStr + " " + yStr);

    QFont font = painter->font();
    font.setPointSize(30);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText( rec, Qt::AlignCenter, QString::number( id  ) );
}

void MySquare::unpress()
{
    pressed = false;
}

bool MySquare::getPressed()
{
    return pressed;
}

/* Przesuwa square na planszy o [rozmiar]
 * move(1,0) - w prawo
 * move(-1,0) - w lewo
 * itd.. */
void MySquare::move(int x, int y)
{
    if ( pressed ) return;
    pressed = true;

    QTimeLine *timer = new QTimeLine(250);
    timer->setFrameRange(0, rozmiar);
    timer->start();

    connect( timer, SIGNAL(finished()), this, SLOT(unpress()) );

    //tworze animacje:
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    animation->setItem(this);
    animation->setTimeLine(timer);
    animation->setPosAt( 1 , mapToParent( x*rozmiar, y*rozmiar ) );
}

void MySquare::mousePressEvent(QGraphicsSceneMouseEvent *event )
{
    if ( pressed ) return;
    emit clicked( this->id );
}
