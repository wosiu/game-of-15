#ifndef MYSQUARE_H
#define MYSQUARE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QtCore>
#include <QtGui>
#include <QObject>

class MySquare : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    MySquare(int x, int y, int id, QObject *parent);
    MySquare();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    const static int rozmiar = 100;
    void move(int x, int y);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool pressed;
    void checkAndMove();
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int startX, startY;
    int id; //indeks w tablicy plansza oraz wyswietlana cyfra

private slots:
    void unpress();

signals:
    void clicked(int clickedId);
};

#endif // MYSQUARE_H
