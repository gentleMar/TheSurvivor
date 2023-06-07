//
// Created by Administrator on 2023/4/25 0025.
//

#include "Game.h"

Collection entities = Collection();
int entityUid = 0;
QTime currentTime = QTime::currentTime();;
int deltaT = 0;
QList<CombatText *> combatTexts;
Map map = Map(25, 25, 128, 128);
int coin = 0;
double permanentSpeedBuff = 0;
double permanentAttackBonusBuff = 0;
double permanentDefenceBuff = 0;
double speedBuff = 0;

Game::Game(QWidget *parent) : QWidget(parent) {
    setGeometry(0, 0, MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);
    loadGameData();
    mspt = 1000 / tps;
    enemyGenerationInterval = 100;
    coinGettingInterval = 1000;
    mainPlayer = nullptr;
    connect(&enemyGenerationTimer, &QTimer::timeout, this, &Game::beReadyToGenerateEnemy);
    connect(&ticker, &QTimer::timeout, this, &Game::calculate);
}

void Game::calculate() {
    ticker.stop();
    // if no player alive, the game will end
    if (entities.getPlayers().length() != 0) {
        QCoreApplication::processEvents();
        // if it is gaming, calculate the game state
        if (gaming) {
            // add coin according to the game time
            if (deltaT >= coinGettingInterval) {
                coinGettingInterval = 1000;
                ++coin;
            } else {
                coinGettingInterval -= deltaT;
            }

            // main player moving state change
            mainPlayer->stopActiveMoving();
            if (turnRight || turnLeft || turnBottom || turnUp) {
                QPointF directionPoint(0, 0);
                if (turnRight) {
                    directionPoint += QPointF(1, 0);
                }
                if (turnLeft) {
                    directionPoint += QPointF(-1, 0);
                }
                if (turnBottom) {
                    directionPoint += QPointF(0, 1);
                }
                if (turnUp) {
                    directionPoint += QPointF(0, -1);
                }
                if (QVector2D(directionPoint).length() != 0) {
                    mainPlayer->rotateTo(mainPlayer->getCenter() + directionPoint);
                    mainPlayer->startActiveMoving();
                }
            }

            // entities performs their actions
            entitiesPerformAction();
            if (readyToGenerateEnemy) {
                generateEnemies();
                readyToGenerateEnemy = false;
            }

            // combat texts change their state
            for (CombatText *combatText: combatTexts) {
                combatText->changeState();
            }
            mainPlayerLevelChange();
            repaint();
        }
        int dt = currentTime.msecsTo(QTime::currentTime());
        currentTime = QTime::currentTime();
        if (dt < mspt) {
            // it means that calculation costs less than the ms per tick, use a timer to sleep
            ticker.start(mspt - dt);
            deltaT = mspt;
        } else {
            // if calculation costs no less than ms per tick, calculate immediately
            deltaT = dt;
            calculate();
        }
    } else {
        // save data and emit the gameFinished signal, Main Menu will be open
        saveGameData();
        emit gameFinished();
    }
}

void Game::entitiesPerformAction() {
    // combat entity will refresh their buffs, all will perform their action
    for (Player *player: entities.getPlayers()) {
        player->refreshBuff();
        player->action();
    }
    for (Enemy *enemy: entities.getEnemies()) {
        enemy->refreshBuff();
        enemy->action();
    }
    for (Projectile *projectile: entities.getProjectiles()) {
        projectile->action();
    }
    for (Block *block: entities.getBlocks()) {
        block->action();
    }
    for (Crystal *crystal: entities.getCrystals()) {
        crystal->action();
    }
}

void Game::restartGame() {
    entities.clear();
    combatTexts.clear();
    mainPlayer = new BowStudent(QPointF(map.getWidth() / 2 - 20, map.getHeight() / 2 - 30));
    entities.addPlayer(mainPlayer);

    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(100, 500)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(20, 320)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(1200, -60)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(80, 70)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(20, -600)));

    readyToGenerateEnemy = false;
    enemyGenerationTimer.start(enemyGenerationInterval);
    gaming = true;
    grabKeyboard();
    currentTime = QTime::currentTime();
    deltaT = mspt;
    ticker.start(mspt);
}

void Game::restartGameWithSwordStudent() {
    entities.clear();
    combatTexts.clear();

    mainPlayer = new SwordStudent(QPointF(map.getWidth() / 2 - 20, map.getHeight() / 2 - 30));
    entities.addPlayer(mainPlayer);

    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(100, 500)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(20, 320)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(1200, -60)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(80, 70)));
    entities.addBlock(new Block(mainPlayer->getCenter() + QPointF(20, -600)));

    readyToGenerateEnemy = false;
    enemyGenerationTimer.start(enemyGenerationInterval);
    gaming = true;
    grabKeyboard();
    currentTime = QTime::currentTime();
    deltaT = mspt;
    ticker.start(mspt);
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_D) {
        turnRight = true;
    } else if (event->key() == Qt::Key_A) {
        turnLeft = true;
    } else if (event->key() == Qt::Key_S) {
        turnBottom = true;
    } else if (event->key() == Qt::Key_W) {
        turnUp = true;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_D) {
        turnRight = false;
    } else if (event->key() == Qt::Key_A) {
        turnLeft = false;
    } else if (event->key() == Qt::Key_S) {
        turnBottom = false;
    } else if (event->key() == Qt::Key_W) {
        turnUp = false;
    } else if (event->key() == Qt::Key_Escape) {
        entities.clear();
        combatTexts.clear();
    }
}

void Game::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // refresh camera
    if (mainPlayer != nullptr) {
        focusPoint = QPoint(mainPlayer->getCenter().toPoint());
    } else {
        focusPoint = QPoint(map.getWidth() / 2, map.getHeight() / 2);
    }

    cameraArea = QRect(
            focusPoint.x() < HALF_MY_WINDOW_WIDTH ? 0 : focusPoint.x() > map.getWidth() - HALF_MY_WINDOW_WIDTH
                                                        ?
                                                        map.getWidth() - MY_WINDOW_WIDTH : focusPoint.x() -
                                                                                           HALF_MY_WINDOW_WIDTH,
            focusPoint.y() < HALF_MY_WINDOW_HEIGHT ? 0 : focusPoint.y() >
                                                         map.getHeight() - HALF_MY_WINDOW_HEIGHT ?
                                                         map.getHeight() - MY_WINDOW_HEIGHT : focusPoint.y() -
                                                                                              HALF_MY_WINDOW_HEIGHT,
            MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);

    // paint map tiles
    int topLeftM = cameraArea.topLeft().x() / map.getTileWidth(),
            topLeftN = cameraArea.topLeft().y() / map.getTileHeight(),
            bottomRightM = cameraArea.bottomRight().x() / map.getTileWidth(),
            bottomRightN = cameraArea.bottomRight().y() / map.getTileHeight();
    for (int i = topLeftM; i <= bottomRightM; ++i) {
        for (int j = topLeftN; j <= bottomRightN; ++j) {
            painter.drawPixmap(map.getTile(i, j).getPosition() - cameraArea.topLeft(), map.getTile(i, j).getTexture());
        }
    }

    // paint entities
    for (Block *block: entities.getBlocks()) {
        paintEntity(block);
    }
    for (Player *player: entities.getPlayers()) {
        paintEntity(player);
        if (player->getHP() < player->getMaxHP()) {
            QPainter painter1(this);
            painter1.setBrush(QBrush(QColor(Qt::red)));
            painter1.drawRect(
                    QRectF(player->getCenter().x() - 30 - cameraArea.x(),
                           player->getCenter().y() + player->getBound().height() / 2 + 10 - cameraArea.y(),
                           60.0 * player->getHP() / player->getMaxHP(), 10));

            QPainter painter2(this);
            painter2.setPen(QColor(Qt::black));
            painter2.drawRect(
                    QRectF(player->getCenter().x() - 30 - cameraArea.x(),
                           player->getCenter().y() + player->getBound().height() / 2 + 10 - cameraArea.y(),
                           60, 10));
        }
    }
    for (Enemy *enemy: entities.getEnemies()) {
        paintEntity(enemy);
    }
    for (Crystal *crystal: entities.getCrystals()) {
        paintEntity(crystal);
    }
    for (Projectile *projectile: entities.getProjectiles()) {
        paintEntity(projectile);
    }

    // paint CombatTexts
    for (CombatText *combatText: combatTexts) {
        paintCombatText(combatText);
    }

    // paint experience bar
    if (gaming && mainPlayer != nullptr) {
        QPainter painter1(this);
        painter1.setBrush(QBrush(QColor(Qt::blue)));
        painter1.drawRect(
                QRectF(0, 0, MY_WINDOW_WIDTH * (double) (mainPlayer->getExperience()) / 100.0, 10));
    }
}

void Game::paintEntity(Entity *entity) {
    QPainter painter(this);
    QRect rect;
    int a = qSqrt(qPow(entity->getBound().width(), 2) + qPow(entity->getBound().height(), 2));
    rect = QRect(0, 0, a, a);
    rect.moveCenter(entity->getCenter().toPoint());
    if (cameraArea.intersects(rect)) {
        painter.drawPixmap(entity->getX() - cameraArea.x(), entity->getY() - cameraArea.y(),
                           entity->getPresentSpirit());
    }
}

void Game::paintCombatText(CombatText *combatText) {
    QPainter painter(this);
    painter.setPen(combatText->getColor());
    painter.setFont(combatText->getFont());
    painter.drawText(QRectF(combatText->getBound().x() - cameraArea.x(), combatText->getBound().y() - cameraArea.y(),
                            combatText->getBound().width(), combatText->getBound().height()), combatText->getText());
}

void Game::generateEnemies() {
    if (gaming && !entities.getPlayers().empty()) {
        Enemy *enemyToBeGenerated;
        int playerRan = QRandomGenerator::global()->bounded(entities.getPlayers().length());
        double angleRan = QRandomGenerator::global()->bounded(2 * M_PI);
        QVector2D vector(cos(angleRan), sin(angleRan));
        vector *= qSqrt(qPow(HALF_MY_WINDOW_WIDTH, 2) + qPow(HALF_MY_WINDOW_HEIGHT, 2)) + 200;
        vector += QVector2D(entities.getPlayers()[playerRan]->getCenter());
        QPointF generatePoint(vector.toPointF());
        switch (QRandomGenerator::global()->bounded(3)) {
            case 0:
                enemyToBeGenerated = new Slime(generatePoint);
                break;
            case 1:
                enemyToBeGenerated = new Yharon(generatePoint);
                break;
            case 2:
                enemyToBeGenerated = new AcidMonster(generatePoint);
                break;
            default:
                Error("Enemy generation random number N/A");
        }
        entities.addEnemy(enemyToBeGenerated);
    }
    enemyGenerationTimer.start(enemyGenerationInterval);
}

void Game::beReadyToGenerateEnemy() {
    readyToGenerateEnemy = true;
}

void Game::loadGameData() {
    QFile loadFile("../save.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        Error("Couldn't open save file");
        return;
    }

    QJsonObject save = QJsonDocument(QJsonDocument::fromJson(loadFile.readAll())).object();

    if (const QJsonValue v = save["coin"]; v.isDouble())
        coin = v.toDouble();
    if (const QJsonValue v = save["permanentSpeedBuff"]; v.isDouble())
        permanentSpeedBuff = v.toDouble();
    if (const QJsonValue v = save["permanentAttackBonusBuff"]; v.isDouble())
        permanentAttackBonusBuff = v.toDouble();
    if (const QJsonValue v = save["permanentDefenceBuff"]; v.isDouble())
        permanentDefenceBuff = v.toDouble();
}

void Game::saveGameData() {
    QFile saveFile("../save.json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        Error("Couldn't open save file");
        return;
    }

    QJsonObject save;

    save["coin"] = coin;
    save["permanentSpeedBuff"] = permanentSpeedBuff;
    save["permanentAttackBonusBuff"] = permanentAttackBonusBuff;
    save["permanentDefenceBuff"] = permanentDefenceBuff;

    QJsonDocument saveDoc(save);
    saveFile.write(saveDoc.toJson());
}

void Game::purchasePermanentSpeedBuff() {
    if (coin >= 15) {
        permanentSpeedBuff += 0.1;
        coin -= 15;
        emit purchaseSucceeded();
    }
}

void Game::purchasePermanentAttackBonusBuff() {
    if (coin >= 10) {
        permanentAttackBonusBuff += 0.1;
        coin -= 10;
        emit purchaseSucceeded();
    }
}

void Game::purchasePermanentDefenceBuff() {
    if (coin >= 5) {
        permanentDefenceBuff += 1;
        coin -= 5;
        emit purchaseSucceeded();
    }
}

void Game::chooseAddBow() {
    mainPlayer->addWeapon(new Bow(mainPlayer));
}

void Game::chooseAddFireMagicBook() {
    mainPlayer->addWeapon(new FireMagicBook(mainPlayer));
}

void Game::chooseAddIceMagicBook() {
    mainPlayer->addWeapon(new IceMagicBook(mainPlayer));
}

void Game::chooseSpeedBuff() {
    speedBuff += 0.1;
}

void Game::chooseWeaponSpeedBuff() {
    if (mainPlayer != nullptr){
        mainPlayer->addWeaponSpeed(0.1);
    }
}

void Game::mainPlayerLevelChange() {
    if (mainPlayer->levelUp()) {
        emit mainPlayerLevelUp();
        gaming = false;
    }
}

void Game::continueGame() {
    gaming = true;
}
