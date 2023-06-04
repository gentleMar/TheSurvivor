//
// Created by Administrator on 2023/4/25 0025.
//

#ifndef THESURVIVOR_GAME_H
#define THESURVIVOR_GAME_H

#include <QWidget>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "Entity.h"
#include "Map.h"
#include "CombatText.h"

class Game : public QWidget {
Q_OBJECT;
private:
    QPoint focusPoint;
    QRect cameraArea;
    Player *mainPlayer;
    QTimer enemyGenerationTimer;
    QTimer ticker;

    int tps = MY_TPS;
    int mspt;
    int enemyGenerationInterval;
    int coinGettingInterval;

    bool turnRight;
    bool turnBottom;
    bool turnLeft;
    bool turnUp;
    bool gaming;
    bool readyToGenerateEnemy;
protected:
    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;
public:
    Game(QWidget *parent);

    void mainPlayerLevelChange();

    void entitiesPerformAction();

    void paintEvent(QPaintEvent *event) override;

    void paintEntity(Entity *entity);

    void paintCombatText(CombatText *combatText);
public slots:
    void calculate();

    void generateEnemies();

    void loadGameData();

    void saveGameData();

    void restartGame();

    void restartGameWithSwordStudent();

    void beReadyToGenerateEnemy();

    //permanent buff
    void purchasePermanentSpeedBuff();

    void purchasePermanentAttackBonusBuff();

    void purchasePermanentDefenceBuff();

    //temporary buff
    void chooseAddBow();

    void chooseAddFireMagicBook();

    void chooseAddIceMagicBook();

    void chooseSpeedBuff();

    void chooseWeaponSpeedBuff();

    void continueGame();
signals:
    void purchaseSucceeded();

    void mainPlayerLevelUp();

    void gameFinished();
};


#endif //THESURVIVOR_GAME_H
