//
// Created by Administrator on 2023/4/21 0021.
//

#ifndef THESURVIVOR_COMBATTEXT_H
#define THESURVIVOR_COMBATTEXT_H

#include <QString>
#include <QPointF>
#include <QPainter>
#include <QVector2D>
#include <Util.h>

class CombatText {
protected:
    int combatTextID;
    QString text;
    QPointF startPoint;
    QPointF endPoint;
    QPointF currentPoint;
    QColor color;
    QFont font;
    int existingTime;
public:
    CombatText(QString _text,QPointF _startPoint,QColor _color);

    QColor getColor();

    QString getText();

    QRectF getBound();

    QFont getFont();

    void changeState();
};




#endif //THESURVIVOR_COMBATTEXT_H


