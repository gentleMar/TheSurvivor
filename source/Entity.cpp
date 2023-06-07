//
// Created by Administrator on 2023/4/28 0028.
//

#include "Entity.h"
#include "CombatText.h"
#include "Map.h"

extern Collection entities;
extern int entityUid;
extern QTime currentTime;
extern int deltaT;
extern QList<CombatText *> combatTexts;
extern Map map;
extern double permanentSpeedBuff;
extern double permanentAttackBonusBuff;
extern double permanentDefenceBuff;
extern double speedBuff;
extern int coin;

extern Q_WIDGETS_EXPORT void
qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);

double Entity::getX() const {
    return bound.x();
}

double Entity::getY() const {
    return bound.y();
}

QRectF Entity::getBound() const {
    return bound;
}

QPointF Entity::getLocation() const {
    return bound.topLeft();
}

QPointF Entity::getCenter() const {
    return bound.center();
}

QPixmap Entity::getPresentSpirit() {
    refreshPresentSpirit();
    return present;
}


void Entity::refreshPresentSpirit() {
    present = spirits.copy(
            QRect(0, (currentTime.msec() / 125) % spiritNum * spiritHeight, spiritWidth, spiritHeight));
    switch (spiritModel) {
        case rotatable:
            if (directionE.length() != 0) {
                present = present.transformed(
                        QTransform(directionE.x(), directionE.y(), -directionE.y(),
                                   directionE.x(), 0, 0),
                        Qt::SmoothTransformation);
            }
            break;
        case horizontalRotatable:
            if (directionE.length() != 0) {
                if (directionE.x() < 0) {
                    present = present.transformed(
                            QTransform(-directionE.x(), directionE.y(), -directionE.y(),
                                       -directionE.x(), 0, 0),
                            Qt::SmoothTransformation);
                } else {
                    present = present.transformed(
                            QTransform(directionE.x(), directionE.y(), -directionE.y(),
                                       directionE.x(), 0, 0),
                            Qt::SmoothTransformation);
                }
            }
        case horizontal:
            present = present.transformed(
                    QTransform(directionE.x() < 0 ? -1 : 1, 0, 0, 1, 0, 0),
                    Qt::SmoothTransformation);
            break;
        case unchangeable:
            break;
        default:
            Error("No such spirits model");
    }
}

bool Entity::collide(const Entity &other) {
    return bound.intersects(other.bound);
}

void Entity::rotateTo(QPointF point) {
    QVector2D vector(point - bound.center());
    directionE = vector / vector.length();
}

void Entity::rotateTo(const Entity &other) {
    rotateTo(other.getCenter());
}

void Entity::rotateUp() {
    rotateTo((bound.topLeft() + bound.topRight()) / 2);
}

void Entity::rotateBottom() {
    rotateTo((bound.bottomLeft() + bound.bottomRight()) / 2);
}

void Entity::rotateLeft() {
    rotateTo((bound.topLeft() + bound.bottomLeft()) / 2);
}

void Entity::rotateRight() {
    rotateTo((bound.topRight() + bound.bottomRight()) / 2);
}

void Entity::move() {
    if (fixedSpeed) {
        if (activeMoving) {
            velocity = directionE * maxSpeed;
        } else {
            velocity = QVector2D(0, 0);
        }
    } else {
        double vx = velocity.x();
        double vy = velocity.y();
        if (velocity.length() == 0) {
            if (activeMoving) {
                if (f < activeAccelerationValue) {
                    vx += (activeAccelerationValue - f) * directionE.x() * deltaT;
                    if (qAbs(vx) > qAbs(maxSpeed * directionE.x())) {
                        vx = maxSpeed * directionE.x();
                    }
                    velocity.setX(vx);
                }
                if (f < activeAccelerationValue) {
                    vy += (activeAccelerationValue - f) * directionE.y() * deltaT;
                    if (qAbs(vy) > qAbs(maxSpeed * directionE.y())) {
                        vy = maxSpeed * directionE.y();
                    }
                    velocity.setY(vy);
                }
            }
        } else {
            QVector2D ev = velocity / velocity.length();
            if (activeMoving && directionE.x() != 0) {
                if (directionE.x() * velocity.x() > 0) {
                    if (qAbs(maxSpeed * directionE.x()) > qAbs(velocity.x())) {
                        vx += (-f * ev.x()) * deltaT;
                        vx += activeAccelerationValue * directionE.x() * deltaT;
                        if (vx * velocity.x() < 0) {
                            vx = 0;
                        }
                        if (qAbs(vx) > qAbs(maxSpeed * directionE.x())) {
                            vx = maxSpeed * directionE.x();
                        }
                    } else if (qAbs(maxSpeed * directionE.x()) < qAbs(velocity.x())) {
                        if (f <= activeAccelerationValue) {
                            vx += (-f * ev.x()) * deltaT;
                            if (velocity.x() * vx < 0 || qAbs(vx) < qAbs(maxSpeed * directionE.x())) {
                                vx = maxSpeed * directionE.x();
                            }
                        } else {
                            if ((vx + (-f * ev.x()) * deltaT) * velocity.x() < 0 ||
                                qAbs((vx + (-f * ev.x()) * deltaT)) <
                                qAbs(maxSpeed * directionE.x())) {
                                vx = maxSpeed * directionE.x() + (activeAccelerationValue * directionE.x() +
                                                                  (-f * ev.x())) * (deltaT -
                                                                                    (qAbs(velocity.x()) -
                                                                                     qAbs(maxSpeed *
                                                                                          directionE.x())) /
                                                                                    f);
                                if (vx * velocity.x() < 0) {
                                    vx = 0;
                                }
                            } else {
                                vx += (-f * ev.x()) * deltaT;
                            }
                        }
                    }
                } else if (directionE.x() * velocity.x() < 0) {
                    vx += (-f * ev.x()) * deltaT;
                    vx += activeAccelerationValue * directionE.x() * deltaT;
                    if (vx * directionE.x() > 0 && qAbs(vx) > qAbs(maxSpeed * directionE.x())) {
                        vx = maxSpeed * directionE.x();
                    }
                } else if (velocity.x() == 0) {
                    if (f < activeAccelerationValue) {
                        vx += (activeAccelerationValue - f) * directionE.x() * deltaT;
                        if (qAbs(vx) > qAbs(maxSpeed * directionE.x())) {
                            vx = maxSpeed * directionE.x();
                        }
                    }
                }
            } else {
                vx += (-f * ev.x()) * deltaT;
                if (vx * velocity.x() < 0) {
                    vx = 0;
                }
            }

            if (activeMoving && directionE.y() != 0) {
                if (directionE.y() * velocity.y() > 0) {
                    if (qAbs(maxSpeed * directionE.y()) > qAbs(velocity.y())) {
                        vy += (-f * ev.y()) * deltaT;
                        vy += activeAccelerationValue * directionE.y() * deltaT;
                        if (vy * velocity.y() < 0) {
                            vy = 0;
                        }
                        if (qAbs(vy) > qAbs(maxSpeed * directionE.y())) {
                            vy = maxSpeed * directionE.y();
                        }
                    } else if (qAbs(maxSpeed * directionE.y()) < qAbs(velocity.y())) {
                        if (f <= activeAccelerationValue) {
                            vy += (-f * ev.y()) * deltaT;
                            if (velocity.y() * vy < 0 || qAbs(vy) < qAbs(maxSpeed * directionE.y())) {
                                vy = maxSpeed * directionE.y();
                            }
                        } else {
                            if ((vy + (-f * ev.y()) * deltaT) * velocity.y() < 0 ||
                                qAbs((vy + (-f * ev.y()) * deltaT)) <
                                qAbs(maxSpeed * directionE.y())) {
                                vy = maxSpeed * directionE.y() + (activeAccelerationValue * directionE.y() +
                                                                  (-f * ev.y())) * (deltaT -
                                                                                    (qAbs(velocity.y()) -
                                                                                     qAbs(maxSpeed *
                                                                                          directionE.y())) /
                                                                                    f);
                                if (vy * velocity.y() < 0) {
                                    vy = 0;
                                }
                            } else {
                                vy += (-f * ev.y()) * deltaT;
                            }
                        }
                    }
                } else if (directionE.y() * velocity.y() < 0) {
                    vy += (-f * ev.y()) * deltaT;
                    vy += activeAccelerationValue * directionE.y() * deltaT;
                    if (vy * directionE.y() > 0 && qAbs(vy) > qAbs(maxSpeed * directionE.y())) {
                        vy = maxSpeed * directionE.y();
                    }
                } else if (velocity.y() == 0) {
                    if (f < activeAccelerationValue) {
                        vy += (activeAccelerationValue - f) * directionE.y() * deltaT;
                        if (qAbs(vy) > qAbs(maxSpeed * directionE.y())) {
                            vy = maxSpeed * directionE.y();
                        }
                    }
                }
            } else {
                vy += (-f * ev.y()) * deltaT;
                if (vy * velocity.y() < 0) {
                    vy = 0;
                }
            }
        }
        velocity.setX(vx);
        velocity.setY(vy);
    }
    if (ghost) {
        moveVector2D(velocity * deltaT);
    } else {
        double origLeft = bound.left(), origRight = bound.right(), origTop = bound.top(), origBottom = bound.bottom(), nextLeft =
                getX() + velocity.x() * deltaT, nextRight =
                nextLeft + spiritWidth, nextTop = getY() + velocity.y() * deltaT, nextBottom = nextTop + spiritHeight;
        if (velocity.x() > 0) {
            for (Block *block: entities.getBlocks()) {
                double blockLeft = block->getBound().left(), blockRight = block->getBound().right(), blockTop = block->getBound().top(), blockBottom = block->getBound().bottom();
                if (((origTop > blockTop && origTop < blockBottom) ||
                     (origBottom > blockTop && origBottom < blockBottom)) && origRight <= blockLeft &&
                    nextRight > blockLeft) {
                    nextRight = block->bound.left();
                    velocity.setX(0);
                }
            }
            bound.moveRight(nextRight);
        }
        if (velocity.x() < 0) {
            for (Block *block: entities.getBlocks()) {
                double blockLeft = block->getBound().left(), blockRight = block->getBound().right(), blockTop = block->getBound().top(), blockBottom = block->getBound().bottom();
                if (((origTop > blockTop && origTop < blockBottom) ||
                     (origBottom > blockTop && origBottom < blockBottom)) && origLeft >= blockRight &&
                    nextLeft < blockRight) {
                    nextLeft = block->bound.right();
                    velocity.setX(0);
                }
            }
            bound.moveLeft(nextLeft);
        }
        if (velocity.y() > 0) {
            for (Block *block: entities.getBlocks()) {
                double blockLeft = block->getBound().left(), blockRight = block->getBound().right(), blockTop = block->getBound().top(), blockBottom = block->getBound().bottom();
                if (((origLeft > blockLeft && origLeft < blockRight) ||
                     (origRight > blockLeft && origRight < blockRight)) && origBottom <= blockTop &&
                    nextBottom > blockTop) {
                    nextBottom = block->bound.top();
                    velocity.setY(0);
                }
            }
            bound.moveBottom(nextBottom);
        }
        if (velocity.y() < 0) {
            for (Block *block: entities.getBlocks()) {
                double blockLeft = block->getBound().left(), blockRight = block->getBound().right(), blockTop = block->getBound().top(), blockBottom = block->getBound().bottom();
                if (((origLeft > blockLeft && origLeft < blockRight) ||
                     (origRight > blockLeft && origRight < blockRight)) && origTop >= blockBottom &&
                    nextTop < blockBottom) {
                    nextTop = block->bound.bottom();
                    velocity.setY(0);
                }
            }
            bound.moveTop(nextTop);
        }
    }
}

void Entity::action() {

}

void Entity::startActiveMoving() {
    activeMoving = true;
}

void Entity::stopActiveMoving() {
    activeMoving = false;
}

void Entity::setSpeed(double speed) {
    velocity *= speed / velocity.length();
}

void Entity::setVelocity(const QVector2D &vector2D) {
    velocity = vector2D;
}

Entity::Entity(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound) {
    spiritWidth = _spiritWidth;
    spiritHeight = _spiritHeight;
    spiritNum = _spiritNum;
    bound = _bound;
    UID = ++entityUid;
}

void CombatEntity::beHurt(double damage) {
    hurtTimeLeft = 500;
    if (defence < damage) {
        reduceHP(damage - defence);
    }
}

double CombatEntity::getWeaponSpeed() const {
    return weaponSpeed;
}

double CombatEntity::getAttackBonus() const {
    return attackBonus;
}

void CombatEntity::useWeapons() {
    for (Weapon *weapon: weapons) {
        weapon->beUsed();
    }
}

CombatEntity::CombatEntity(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound) : Entity(_spiritWidth,
                                                                                                        _spiritHeight,
                                                                                                        _spiritNum,
                                                                                                        _bound) {
    fired = false;
    frozen = false;
    fireDamageIntervalLeft = 0;
    hurtTimeLeft = 0;
}

void CombatEntity::addWeapon(Weapon *weapon) {
    for (Weapon *weapon1: weapons) {
        if (weapon->getWeaponID() == weapon1->getWeaponID()) {
            weapon1->addAmount();
            return;
        }
    }
    weapons.append(weapon);
}

double CombatEntity::getHP() const {
    return HP;
}

double CombatEntity::getMaxHP() const {
    return maxHp;
}

void CombatEntity::heal(double amount) {
    if (HP < maxHp) {
        int healAmount = (HP + amount > maxHp) ? maxHp - HP : amount;
        HP += healAmount;
        combatTexts.append(
                new CombatText("+" + QString::number(healAmount), (bound.topLeft() + bound.topRight()) / 2,
                               QColor(Qt::darkGreen)));
    }
}

void CombatEntity::refreshBuff() {
    if (fired) {
        if (deltaT >= fireDamageIntervalLeft) {
            reduceHP(1);
            fireDamageIntervalLeft = 1000;
        } else {
            fireDamageIntervalLeft -= deltaT;
        }
        if (deltaT >= firedTimeLeft) {
            firedTimeLeft = 0;
            fired = false;
        } else {
            firedTimeLeft -= deltaT;
        }
    }

    if (frozen) {
        if (deltaT >= frozenTimeLeft) {
            frozenTimeLeft = 0;
            frozen = false;
        } else {
            frozenTimeLeft -= deltaT;
        }
    }

    if (frozen) {
        maxSpeed = baseMaxSpeed * 0.6;
    } else {
        maxSpeed = baseMaxSpeed;
    }
}

void CombatEntity::reduceHP(double hp) {
    HP -= hp;
}

void CombatEntity::beFiredWithDamage(int damage) {
    if (frozen) {
        frozen = false;
        combatTexts.append(
                new CombatText("温差", (bound.topLeft() + bound.topRight()) / 2,
                               QColor(Qt::red)));
        beHurt(1.5 * damage);
    } else {
        beHurt(damage);
        fired = true;
        firedTimeLeft = 5000;
        fireDamageIntervalLeft = 1000;
    }
}

void CombatEntity::beFrozenWithDamage(int damage) {
    if (fired) {
        fired = false;
        combatTexts.append(
                new CombatText("温差", (bound.topLeft() + bound.topRight()) / 2,
                               QColor(Qt::darkBlue)));
        beHurt(1.5 * damage);
    } else {
        beHurt(damage);
        frozen = true;
        frozenTimeLeft = 5000;
    }
}

void CombatEntity::action() {
    if (deltaT >= hurtTimeLeft) {
        hurtTimeLeft = 0;
    } else {
        hurtTimeLeft -= deltaT;
    }
    if (HP <= 0) {
        die();
    }
}

void CombatEntity::refreshPresentSpirit() {
    Entity::refreshPresentSpirit();
    QPixmap temp(present.size());
    temp.fill(Qt::transparent);

    QPainter painter(&temp);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, present);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(temp.rect(), QColor(0, 0, 0, (510 - hurtTimeLeft) / 2));

    present = temp;
}

void CombatEntity::addWeaponSpeed(double num) {
    weaponSpeed += num;
}

void Player::beCollided(double damage) {
    if (immuneToCollisionDamageTime == 0) {
        beHurt(damage);
        immuneToCollisionDamageTime = maxImmuneToCollisionDamageTime;
    }
}

unsigned Entity::getUID() const {
    return UID;
}

void Entity::die() {
    delete this;
}

void Entity::setPosition(double x, double y) {
    bound.moveTo(x, y);
}

Collection &Entity::getEntitiesGroup() {
    return entities;
}

unsigned Entity::trySeekNearestPlayerUID() const {
    if (!entities.getPlayers().empty()) {
        unsigned uid;
        bool flag = false;
        double length = 0;
        for (Player *iterator: entities.getPlayers()) {
            double tempLength = QVector2D(getCenter() - iterator->getCenter()).length();
            if (!flag || tempLength < length) {
                length = tempLength;
                uid = iterator->getUID();
                flag = true;
            }
        }
        return uid;
    }
    throw 0;
}

unsigned Entity::trySeekNearestEnemyUID() const {
    if (!entities.getEnemies().empty()) {
        unsigned uid;
        bool flag = false;
        double length = 0;
        for (Enemy *iterator: entities.getEnemies()) {
            double tempLength = QVector2D(getCenter() - iterator->getCenter()).length();
            if (!flag || tempLength < length) {
                length = tempLength;
                uid = iterator->getUID();
                flag = true;
            }
        }
        return uid;
    }
    throw 0;
}

void Entity::moveVector2D(const QVector2D &vector2D) {
    bound.moveCenter(getCenter() + vector2D.toPointF());
}

QString Entity::getName() const {
    return name;
}

bool Entity::detect(const Entity &other, double r) {
    return QVector2D(other.getCenter() - getCenter()).length() <= r;
}

Player::Player(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound) : CombatEntity(_spiritWidth,
                                                                                                  _spiritHeight,
                                                                                                  _spiritNum, _bound) {
    level = 1;
    experience = 0;
}

Enemy::Enemy(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound) : CombatEntity(_spiritWidth,
                                                                                                _spiritHeight,
                                                                                                _spiritNum, _bound) {
}

Block::Block(QPointF position) : Entity(112, 112, 1, QRectF(position.x(), position.y(), 112, 112)) {
    activeMoving = false;
    spirits = loadImage("block/Block");
    spiritModel = unchangeable;
}

void Block::die() {
    entities.removeBlock(UID);
    Entity::die();
}

void Block::action() {

}

Projectile::Projectile(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound) : Entity(_spiritWidth,
                                                                                                    _spiritHeight,
                                                                                                    _spiritNum,
                                                                                                    _bound) {
    distance = 0;
    fixedSpeed = true;
    targetDecided = false;
    activeMoving = true;
}

void Projectile::die() {
    entities.removeProjectile(UID);
    Entity::die();
}


Crystal::Crystal(QPointF position) : Entity(20, 20, 1, QRectF(position.x(), position.y(), 20, 20)) {

}

void Crystal::die() {
    entities.removeCrystal(UID);
    Entity::die();
}

void Player::action() {
    move();
    useWeapons();
    if (deltaT > immuneToCollisionDamageTime) {
        immuneToCollisionDamageTime = 0;
    } else {
        immuneToCollisionDamageTime -= deltaT;
    }
    CombatEntity::action();
}

void Player::die() {
    entities.removePlayer(UID);
    Entity::die();
}

void Player::move() {
    Entity::move();
    if (bound.x() < 0) {
        bound.moveLeft(0);
    }
    if (bound.y() < 0) {
        bound.moveTop(0);
    }
    if (bound.x() + spiritWidth > map.getWidth()) {
        bound.moveRight(map.getWidth());
    }
    if (bound.y() + spiritHeight > map.getHeight()) {
        bound.moveBottom(map.getHeight());
    }
}

void Player::addExperience(int exp) {
    experience += exp;
}

void Player::beHurt(double damage) {
    hurtTimeLeft = 500;
    if (defence < damage) {
        reduceHP(damage - defence);
    } else {
        combatTexts.append(
                new CombatText("格挡", (bound.topLeft() + bound.topRight()) / 2,
                               QColor(Qt::gray)));
    }
}

void Player::refreshBuff() {
    if (fired) {
        if (deltaT >= fireDamageIntervalLeft) {
            reduceHP(1);
            fireDamageIntervalLeft = 1000;
        } else {
            fireDamageIntervalLeft -= deltaT;
        }
        if (deltaT >= firedTimeLeft) {
            firedTimeLeft = 0;
            fired = false;
        } else {
            firedTimeLeft -= deltaT;
        }
    }

    if (frozen) {
        if (deltaT >= frozenTimeLeft) {
            frozenTimeLeft = 0;
            frozen = false;
        } else {
            frozenTimeLeft -= deltaT;
        }
    }

    if (frozen) {
        maxSpeed = baseMaxSpeed * 0.6;
    } else {
        maxSpeed = baseMaxSpeed;
    }

    maxSpeed *= 1 + permanentSpeedBuff + speedBuff;
    attackBonus = baseAttackBonus + permanentAttackBonusBuff;
    defence = baseDefence + permanentDefenceBuff;
}

void Player::reduceHP(double hp) {
    combatTexts.append(
            new CombatText("-" + QString::number(hp), (bound.topLeft() + bound.topRight()) / 2,
                           QColor(Qt::red)));
    CombatEntity::reduceHP(hp);
}

bool Player::levelUp() {
    if (experience >= 100) {
        experience -= 100;
        ++level;
        return true;
    }
    return false;
}

int Player::getExperience() const {
    return experience;
}

void Enemy::action() {
    changeTargetPoint();
    move();
    useWeapons();
    if (hasCollisionAttack) {
        for (Player *player: entities.getPlayers()) {
            if (collide(*player)) {
                player->beCollided(collisionAttack * (1 + attackBonus));
            }
        }
    }
    CombatEntity::action();
}

void Enemy::die() {
    int ran = QRandomGenerator::global()->bounded(5);
    if (ran == 0) {
        entities.addCrystal(new Meat(getCenter()));
    } else if (ran == 1) {
        entities.addCrystal(new Coin(getCenter()));
    } else {
        entities.addCrystal(new Star(getCenter(), 10));
    }

    entities.removeEnemy(UID);
    Entity::die();
}

void Enemy::setTargetPointWithNearestPlayersPosition() {
    try {
        Player *nearestPlayer = entities.getPlayer(trySeekNearestPlayerUID());
        noTarget = false;
        targetPoint = nearestPlayer->getCenter();
        return;
    } catch (int) {
        noTarget = true;
    }
}

void Enemy::changeTargetPoint() {
}


void Enemy::reduceHP(double hp) {
    combatTexts.append(
            new CombatText("-" + QString::number(hp), (bound.topLeft() + bound.topRight()) / 2,
                           QColor(Qt::black)));
    CombatEntity::reduceHP(hp);
}

QList<Player *> &Collection::getPlayers() {
    return players;
}

void Collection::addPlayer(Player *player) {
    players.append(player);
}

QList<Enemy *> &Collection::getEnemies() {
    return enemies;
}

void Collection::addEnemy(Enemy *enemy) {
    enemies.append(enemy);
}

QList<Projectile *> &Collection::getProjectiles() {
    return projectiles;
}

void Collection::addProjectile(Projectile *projectile) {
    projectiles.append(projectile);
}

QList<Block *> &Collection::getBlocks() {
    return blocks;
}

void Collection::addBlock(Block *block) {
    blocks.append(block);
}

QList<Crystal *> &Collection::getCrystals() {
    return crystals;
}

void Collection::addCrystal(Crystal *crystal) {
    crystals.append(crystal);
}

void Collection::removePlayer(int uid) {
    for (QList<Player *>::iterator iterator = players.begin(); iterator != players.end(); ++iterator) {
        if ((*iterator)->getUID() == uid) {
            players.erase(iterator);
            return;
        }
    }
}

void Collection::removeEnemy(int uid) {
    for (QList<Enemy *>::iterator iterator = enemies.begin(); iterator != enemies.end(); ++iterator) {
        if ((*iterator)->getUID() == uid) {
            enemies.erase(iterator);
            return;
        }
    }
}

void Collection::removeProjectile(int uid) {
    for (QList<Projectile *>::iterator iterator = projectiles.begin(); iterator != projectiles.end(); ++iterator) {
        if ((*iterator)->getUID() == uid) {
            projectiles.erase(iterator);
            return;
        }
    }
}

void Collection::removeBlock(int uid) {
    for (QList<Block *>::iterator iterator = blocks.begin(); iterator != blocks.end(); ++iterator) {
        if ((*iterator)->getUID() == uid) {
            blocks.erase(iterator);
            return;
        }
    }
}

void Collection::removeCrystal(int uid) {
    for (QList<Crystal *>::iterator iterator = crystals.begin(); iterator != crystals.end(); ++iterator) {
        if ((*iterator)->getUID() == uid) {
            crystals.erase(iterator);
            return;
        }
    }
}

Collection::Collection() {
    countUID = 1;
}


int Collection::generateUID() {
    return countUID++;
}

Player *Collection::getPlayer(unsigned int uid) {
    for (Player *player: players) {
        if (player->getUID() == uid) {
            return player;
        }
    }
    throw 0;
}

Enemy *Collection::getEnemy(unsigned int uid) {
    for (Enemy *enemy: enemies) {
        if (enemy->getUID() == uid) {
            return enemy;
        }
    }
    throw 0;
}

Projectile *Collection::getProjectile(unsigned int uid) {
    for (Projectile *projectile: projectiles) {
        if (projectile->getUID() == uid) {
            return projectile;
        }
    }
    throw 0;
}

Block *Collection::getBlock(unsigned int uid) {
    for (Block *block: blocks) {
        if (block->getUID() == uid) {
            return block;
        }
    }
    throw 0;
}

Crystal *Collection::getCrystal(unsigned int uid) {
    for (Crystal *crystal: crystals) {
        if (crystal->getUID() == uid) {
            return crystal;
        }
    }
    throw 0;
}

void Collection::clear() {
    players.clear();
    enemies.clear();
    projectiles.clear();
    blocks.clear();
    crystals.clear();
}


BowStudent::BowStudent(QPointF position) : Player(40, 60, 1, QRectF(position.x(), position.y(), 40, 60)) {
    name = "Bow Student";
    baseMaxHP = 150;
    maxHp = baseMaxHP;
    HP = maxHp;
    baseDefence = 3;
    defence = baseDefence;
    baseAttackBonus = 0.1;
    attackBonus = baseAttackBonus;
    weaponSpeed = 25;
    fixedSpeed = false;
    baseMaxSpeed = 0.2;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.0012;
    f = 0.0006;
    criticalAttackChange = 0.1;
    criticalAttackBonus = 1;
    activeMoving = false;
    ghost = false;
    spirits = loadImage("player/BowStudent");
    spiritModel = horizontal;
    maxImmuneToCollisionDamageTime = 1000;
    immuneToCollisionDamageTime = 0;
    addWeapon(new Bow(this));
}

Slime::Slime(QPointF position) : Enemy(34, 25, 2, QRectF(position.x(), position.y(), 34, 25)) {
    name = "Slime";
    baseMaxHP = 100;
    maxHp = baseMaxHP;
    HP = maxHp;
    baseDefence = 3;
    defence = baseDefence;
    baseAttackBonus = 0;
    attackBonus = baseAttackBonus;
    weaponSpeed = 1;
    fixedSpeed = false;
    baseMaxSpeed = 0.1;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.0012;
    f = 0.0005;
    criticalAttackChange = 0;
    criticalAttackBonus = 0;
    activeMoving = true;
    ghost = false;
    spirits = loadImage("enemy/Slime");
    spiritModel = unchangeable;
    hasCollisionAttack = true;
    collisionAttack = 10;
}

void Slime::changeTargetPoint() {
    setTargetPointWithNearestPlayersPosition();
    if (!noTarget) {
        rotateTo(targetPoint);
        activeMoving = true;
    } else {
        activeMoving = false;
    }
}

Yharon::Yharon(QPointF position) : Enemy(220, 177, 7, QRectF(position.x(), position.y(), 220, 177)) {
    name = "Yharon";
    baseMaxHP = 150;
    maxHp = baseMaxHP;
    HP = maxHp;
    baseDefence = 5;
    defence = baseDefence;
    baseAttackBonus = 0;
    attackBonus = baseAttackBonus;
    weaponSpeed = 1;
    fixedSpeed = false;
    baseMaxSpeed = 0.5;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.0012;
    f = 0.001;
    criticalAttackChange = 0;
    criticalAttackBonus = 0;
    activeMoving = true;
    ghost = true;
    spirits = loadImage("enemy/Yharon");
    spiritModel = horizontalRotatable;
    hasCollisionAttack = true;
    collisionAttack = 10;
    noTarget = true;
}

void Yharon::changeTargetPoint() {
    if (!noTarget && QVector2D(targetPoint - getCenter()).length() < 100) {
        noTarget = true;
    }
    if (noTarget) {
        setTargetPointWithNearestPlayersPosition();
        if (!noTarget) {
            QVector2D e = QVector2D(targetPoint - getCenter());
            e /= e.length();
            targetPoint = (QVector2D(targetPoint) + e * 2000).toPointF();
            rotateTo(targetPoint);
            activeMoving = true;
        } else {
            activeMoving = false;
        }
    }
}

AcidMonster::AcidMonster(QPointF position) : Enemy(62, 36, 5, QRectF(position.x(), position.y(), 62, 36)) {
    name = "Acid Monster";
    baseMaxHP = 70;
    maxHp = baseMaxHP;
    HP = maxHp;
    baseDefence = 1;
    defence = baseDefence;
    baseAttackBonus = 0;
    attackBonus = baseAttackBonus;
    weaponSpeed = 1;
    fixedSpeed = false;
    baseMaxSpeed = 0.1;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.001;
    f = 0.0004;
    criticalAttackChange = 0;
    criticalAttackBonus = 0;
    activeMoving = true;
    ghost = false;
    spirits = loadImage("enemy/AcidMonster");
    spiritModel = horizontal;
    hasCollisionAttack = true;
    collisionAttack = 10;
    addWeapon(new Acid(this));
}

void AcidMonster::changeTargetPoint() {
    setTargetPointWithNearestPlayersPosition();
    if (QVector2D(targetPoint - getCenter()).length() > 200) {
        rotateTo(targetPoint);
        activeMoving = true;
    } else {
        activeMoving = false;
    }
}

void AcidMonster::action() {
    changeTargetPoint();
    move();
    try {
        Player *nearestPlayer = entities.getPlayer(trySeekNearestPlayerUID());
        if (detect(*nearestPlayer, 500)) {
            useWeapons();
        }
    } catch (int) {
    }
    if (hasCollisionAttack) {
        for (Player *player: entities.getPlayers()) {
            if (collide(*player)) {
                player->beCollided(collisionAttack * (1 + attackBonus));
            }
        }
    }
    CombatEntity::action();
}

Arrow::Arrow(QPointF position, CombatEntity *owner) : Projectile(30, 30, 1,
                                                                 QRectF(position.x(), position.y(), 30, 30)) {
    name = "Arrow";
    baseMaxSpeed = 0.1;
    maxSpeed = baseMaxSpeed;
    damage = 10 * (1 + owner->getAttackBonus());
    maxDistance = 1000;
    ghost = true;
    spirits = loadImage("projectile/Arrow");
    spiritModel = rotatable;
    hasDistanceLimitation = true;
}

void Arrow::action() {
    try {
        Enemy *nearestEnemy = entities.getEnemy(trySeekNearestEnemyUID());
        rotateTo(*nearestEnemy);
        if (collide(*nearestEnemy)) {
            nearestEnemy->beHurt(damage);
            Projectile::die();
            return;
        }
    } catch (int) {
    }

    QPointF orig = getCenter();
    move();
    distance += QVector2D(orig - getCenter()).length();
    if (hasDistanceLimitation && distance > maxDistance) {
        Projectile::die();
        return;
    }

    for (Block *block: entities.getBlocks()) {
        if (collide(*block)) {
            Projectile::die();
            return;
        }
    }
}

AcidBubble::AcidBubble(QPointF position, CombatEntity *owner) : Projectile(30, 30, 7,
                                                                           QRectF(position.x(), position.y(), 30,
                                                                                  30)) {
    baseMaxSpeed = 0.1;
    maxSpeed = baseMaxSpeed;
    damage = 10 * (1 + owner->getAttackBonus());
    maxDistance = 600;
    ghost = true;
    spirits = loadImage("projectile/AcidBubble");
    spiritModel = unchangeable;
    hasDistanceLimitation = true;
}

void AcidBubble::action() {
    try {
        Player *nearestPlayer = entities.getPlayer(trySeekNearestPlayerUID());
        if (!targetDecided) {
            rotateTo(*nearestPlayer);
            targetDecided = true;
        } else {
            if (collide(*nearestPlayer)) {
                nearestPlayer->beHurt(damage);
                Projectile::die();
                return;
            }
        }
    } catch (int) {
    }

    QPointF orig = getCenter();
    move();
    distance += QVector2D(orig - getCenter()).length();
    if (hasDistanceLimitation && distance > maxDistance) {
        Projectile::die();
        return;
    }

    for (Block *block: entities.getBlocks()) {
        if (collide(*block)) {
            Projectile::die();
            return;
        }
    }
}

FireBall::FireBall(QPointF position, CombatEntity *owner) : Projectile(30, 30, 1,
                                                                       QRectF(position.x(), position.y(), 30, 30)) {
    baseMaxSpeed = 0.1;
    maxSpeed = baseMaxSpeed;
    damage = 10 * (1 + owner->getAttackBonus());
    maxDistance = 2000;
    ghost = true;
    spirits = loadImage("projectile/FireBall");
    spiritModel = unchangeable;
    hasDistanceLimitation = true;
}

void FireBall::action() {
    try {
        Enemy *nearestEnemy = entities.getEnemy(trySeekNearestEnemyUID());
        if (!targetDecided) {
            rotateTo(*nearestEnemy);
            targetDecided = true;
        } else {
            if (collide(*nearestEnemy)) {
                nearestEnemy->beFiredWithDamage(damage);
                Projectile::die();
                return;
            }
        }
    } catch (int) {
    }

    QPointF orig = getCenter();
    move();
    distance += QVector2D(orig - getCenter()).length();
    if (hasDistanceLimitation && distance > maxDistance) {
        Projectile::die();
        return;
    }

    for (Block *block: entities.getBlocks()) {
        if (collide(*block)) {
            Projectile::die();
            return;
        }
    }
}

IceBall::IceBall(QPointF position, CombatEntity *owner) : Projectile(30, 30, 1,
                                                                     QRectF(position.x(), position.y(), 30, 30)) {
    baseMaxSpeed = 0.1;
    maxSpeed = baseMaxSpeed;
    damage = 10 * (1 + owner->getAttackBonus());
    maxDistance = 2000;
    ghost = true;
    spirits = loadImage("projectile/IceBall");
    spiritModel = unchangeable;
    hasDistanceLimitation = true;
}

void IceBall::action() {
    try {
        Enemy *nearestEnemy = entities.getEnemy(trySeekNearestEnemyUID());
        if (!targetDecided) {
            rotateTo(*nearestEnemy);
            targetDecided = true;
        } else {
            if (collide(*nearestEnemy)) {
                nearestEnemy->beFrozenWithDamage(damage);
                Projectile::die();
                return;
            }
        }
    } catch (int) {
    }

    QPointF orig = getCenter();
    move();
    distance += QVector2D(orig - getCenter()).length();
    if (hasDistanceLimitation && distance > maxDistance) {
        Projectile::die();
        return;
    }

    for (Block *block: entities.getBlocks()) {
        if (collide(*block)) {
            Projectile::die();
            return;
        }
    }
}

Star::Star(QPointF position, double _experience) : Crystal(position) {
    experience = _experience;
    activated = false;
    activeMoving = false;
    baseMaxSpeed = 1;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.001;
    fixedSpeed = false;
    f = 0;
    spirits = loadImage("crystal/Star");
    spiritModel = rotatable;
    ghost = true;
}

void Star::action() {
    try {
        Player *nearestPlayer = entities.getPlayer(trySeekNearestPlayerUID());
        if (collide(*nearestPlayer)) {
            nearestPlayer->addExperience(experience);
            Crystal::die();
        } else {
            if (!activated) {
                if (detect(*nearestPlayer, 100)) {
                    QVector2D vector2D(getCenter() - nearestPlayer->getCenter());
                    setVelocity(vector2D / vector2D.length() * 0.2);
                    activated = true;
                    activeMoving = true;
                }
            }
            if (activated) {
                rotateTo(*nearestPlayer);
                move();
            }
        }
    } catch (int) {

    }
}

Meat::Meat(QPointF position) : Crystal(position) {
    activeMoving = false;
    spiritModel = unchangeable;
    spirits = loadImage("crystal/Meat");
}

void Meat::action() {
    try {
        Player *nearestPlayer = entities.getPlayer(trySeekNearestPlayerUID());
        if (collide(*nearestPlayer)) {
            nearestPlayer->heal(25);
            Crystal::die();
        }
    } catch (int) {
    }

}

SwordStudent::SwordStudent(QPointF position) : Player(40, 60, 1, QRectF(position.x(), position.y(), 40, 60)) {
    name = "Sword Student";
    baseMaxHP = 200;
    maxHp = baseMaxHP;
    HP = maxHp;
    baseDefence = 5;
    defence = baseDefence;
    baseAttackBonus = 0.1;
    attackBonus = baseAttackBonus;
    weaponSpeed = 1;
    fixedSpeed = false;
    baseMaxSpeed = 0.15;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.001;
    f = 0.0006;
    criticalAttackChange = 0.1;
    criticalAttackBonus = 1;
    activeMoving = false;
    ghost = false;
    spirits = loadImage("player/SwordStudent");
    spiritModel = horizontal;
    maxImmuneToCollisionDamageTime = 1000;
    immuneToCollisionDamageTime = 0;
    addWeapon(new FireMagicBook(this));
}

Coin::Coin(QPointF position) : Crystal(position) {
    activated = false;
    activeMoving = false;
    baseMaxSpeed = 1;
    maxSpeed = baseMaxSpeed;
    activeAccelerationValue = 0.001;
    fixedSpeed = false;
    f = 0;
    spirits = loadImage("crystal/Coin");
    spiritModel = rotatable;
    ghost = true;
}

void Coin::action() {
    try {
        Player *nearestPlayer = entities.getPlayer(trySeekNearestPlayerUID());
        if (collide(*nearestPlayer)) {
            coin += 100;
            combatTexts.append(
                    new CombatText("+100", getCenter(),
                                   QColor(Qt::yellow)));
            Crystal::die();
        } else {
            if (!activated) {
                if (detect(*nearestPlayer, 100)) {
                    QVector2D vector2D(getCenter() - nearestPlayer->getCenter());
                    setVelocity(vector2D / vector2D.length() * 0.2);
                    activated = true;
                    activeMoving = true;
                }
            }
            if (activated) {
                rotateTo(*nearestPlayer);
                move();
            }
        }
    } catch (int) {

    }
}