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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    static const int rozmiar;
    void move(int x, int y);
    bool getPressed();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void checkAndMove();
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    int startX, startY;
    int id; //indeks w tablicy plansza oraz wyswietlana cyfra
    static bool pressed;

private slots:
    void unpress();

signals:
    void clicked(int clickedId);
};

#endif // MYSQUARE_H
