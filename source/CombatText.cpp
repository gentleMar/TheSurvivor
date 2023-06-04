//
// Created by Administrator on 2023/4/21 0021.
//

#include "CombatText.h"
#include <QRandomGenerator>
#include <QFontDatabase>

extern int deltaT;
extern QList<CombatText *> combatTexts;
int textID = 0;

CombatText::CombatText(QString _text, QPointF _startPoint, QColor _color) {
    text = _text;
    startPoint = _startPoint;
    currentPoint = startPoint;
    double angleRan = QRandomGenerator::global()->bounded(2 * M_PI / 3);
    angleRan += 7 * M_PI / 6;
    endPoint = startPoint + QPointF(cos(angleRan), sin(angleRan)) * 50;
    existingTime = 0;
    color = _color;
    font = QFont("汉仪力量黑简");
    font.setPixelSize(QRandomGenerator::global()->bounded(16, 27));
    combatTextID = ++textID;
}

QColor CombatText::getColor() {
    return color;
}

QString CombatText::getText() {
    return text;
}

QRectF CombatText::getBound() {
    QFontMetrics metrics(font);
    QRectF bound = metrics.boundingRect(text);
    bound.setWidth(1.1 * bound.width());
    bound.moveCenter(currentPoint);
    return bound;
}

void CombatText::changeState() {
    existingTime += deltaT;
    currentPoint = startPoint +
                   (existingTime >= 1000 ? 1 : existingTime * 0.001) * QVector2D(endPoint - startPoint).toPointF();
    color.setAlpha(existingTime <= 1500 ? 255 : existingTime >= 2500 ? 0 : 0.255 * (2500 - existingTime));
    if (existingTime >= 2500) {
        for (QList<CombatText *>::iterator iterator = combatTexts.begin(); iterator != combatTexts.end(); ++iterator) {
            if ((*iterator)->combatTextID == combatTextID) {
                combatTexts.erase(iterator);
                return;
            }
        }
    }
}

QFont CombatText::getFont() {
    return font;
}



