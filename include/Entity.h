//
// Created by Administrator on 2023/4/28 0028.
//

#ifndef THESURVIVOR_ENTITY_H
#define THESURVIVOR_ENTITY_H

#include <QRandomGenerator>
#include <QRectF>
#include <QVector2D>
#include <QPixmap>
#include <QList>
#include <QPainter>
#include <QtMath>
#include <QMatrix2x2>
#include <Util.h>
#include "Weapon.h"

class Collection;

class Entity {
protected:
    QString name;
    QRectF bound;
    QVector2D velocity;
    QPixmap spirits;
    QPixmap present;
    QVector2D directionE;

    enum SpiritModel {
        rotatable,
        horizontalRotatable,
        horizontal,
        unchangeable
    } spiritModel;

    double baseMaxSpeed;
    double maxSpeed;
    double activeAccelerationValue;
    double f;

    unsigned UID;
    int spiritNum;
    int spiritWidth;
    int spiritHeight;

    bool fixedSpeed;
    bool activeMoving;
    bool ghost;
public:
    Entity(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound);

    QString getName() const;

    double getX() const;

    double getY() const;

    QRectF getBound() const;

    QPointF getLocation() const;

    QPointF getCenter() const;

    QPixmap getPresentSpirit();

    unsigned getUID() const;

    unsigned trySeekNearestPlayerUID() const;

    unsigned trySeekNearestEnemyUID() const;

    Collection &getEntitiesGroup();

    void setPosition(double x, double y);

    virtual void move();

    virtual void action();

    virtual void refreshPresentSpirit();

    bool collide(const Entity &other);

    bool detect(const Entity &other, double r);

    void rotateTo(QPointF point);

    void rotateTo(const Entity &other);

    void rotateUp();

    void rotateBottom();

    void rotateLeft();

    void rotateRight();

    void startActiveMoving();

    void stopActiveMoving();

    void setSpeed(double speed);

    void moveVector2D(const QVector2D &vector2D);

    virtual void die();

    void setVelocity(const QVector2D &vector2D);
};

class CombatEntity : public Entity {
protected:
    double baseMaxHP;
    double maxHp;
    double HP;
    double baseDefence;
    double defence;
    double baseAttackBonus;
    double attackBonus;
    double collisionAttack;
    double weaponSpeed;
    double criticalAttackChange;
    double criticalAttackBonus;
    bool hasCollisionAttack;
    QList<Weapon *> weapons;

    bool fired;
    int firedTimeLeft;
    int fireDamageIntervalLeft;
    bool frozen;
    int frozenTimeLeft;

    int hurtTimeLeft;
public:
    CombatEntity(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound);

    void action() override;

    virtual void beHurt(double damage);

    virtual void reduceHP(double hp);

    virtual void refreshBuff();

    void refreshPresentSpirit() override;

    void beFiredWithDamage(int damage);

    void beFrozenWithDamage(int damage);

    double getWeaponSpeed() const;

    double getAttackBonus() const;

    double getHP() const;

    double getMaxHP() const;

    void heal(double amount);

    void addWeaponSpeed(double num);

    virtual void useWeapons();

    void addWeapon(Weapon *weapon);
};

class Player : public CombatEntity {
protected:
    double experience;
    int level;
    int maxImmuneToCollisionDamageTime;
    int immuneToCollisionDamageTime;
public:
    Player(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound);

    int getExperience() const;

    void refreshBuff() override;

    void beCollided(double damage);

    void beHurt(double damage) override;

    void reduceHP(double hp) override;

    void action() override;

    void move() override;

    void die() override;

    void addExperience(int exp);

    bool levelUp();
};

class Enemy : public CombatEntity {
protected:
    QPointF targetPoint;
    bool noTarget;
public:
    Enemy(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound);

    void action() override;

    void reduceHP(double hp) override;

    virtual void changeTargetPoint();

    void setTargetPointWithNearestPlayersPosition();

    void die() override;
};

class Projectile : public Entity {
protected:
    enum AimModel {
        trackEnemy,
        atPosition
    } aim;

    double damage;

    double maxDistance;

    double distance;

    bool targetDecided;

    bool hasDistanceLimitation;
public:
    Projectile(int _spiritWidth, int _spiritHeight, int _spiritNum, QRectF _bound);

    void die() override;
};

class Block : public Entity {
public:
    Block(QPointF position);

    void action() override;

    void die() override;
};

class Crystal : public Entity {
public:
    Crystal(QPointF position);

    void die() override;
};

class Star : public Crystal {
    double experience;

    bool activated;
public:
    Star(QPointF position, double _experience);

    void action() override;
};

class Meat : public Crystal {
public:
    Meat(QPointF position);

    void action() override;
};

class Coin : public Crystal {
    bool activated;

public:
    Coin(QPointF position);

    void action() override;
};

class Collection {
private:
    QList<Player *> players;
    QList<Enemy *> enemies;
    QList<Projectile *> projectiles;
    QList<Block *> blocks;
    QList<Crystal *> crystals;

    int countUID;
public:
    Collection();

    int generateUID();

    QList<Player *> &getPlayers();

    QList<Enemy *> &getEnemies();

    QList<Projectile *> &getProjectiles();

    QList<Block *> &getBlocks();

    QList<Crystal *> &getCrystals();

    Player *getPlayer(unsigned uid);

    Enemy *getEnemy(unsigned uid);

    Projectile *getProjectile(unsigned uid);

    Block *getBlock(unsigned uid);

    Crystal *getCrystal(unsigned uid);

    void addPlayer(Player *player);

    void addEnemy(Enemy *enemy);

    void addProjectile(Projectile *projectile);

    void addBlock(Block *block);

    void addCrystal(Crystal *crystal);

    void removePlayer(int uid);

    void removeEnemy(int uid);

    void removeProjectile(int uid);

    void removeBlock(int uid);

    void removeCrystal(int uid);

    void clear();

};

class BowStudent : public Player {
public:
    BowStudent(QPointF position);
};

class SwordStudent : public Player {
public:
    SwordStudent(QPointF position);
};

class Slime : public Enemy {
public:
    Slime(QPointF position);

    void changeTargetPoint() override;
};

class Yharon : public Enemy {
public:
    Yharon(QPointF position);

    void changeTargetPoint() override;
};

class AcidMonster : public Enemy {
public:
    AcidMonster(QPointF position);

    void action() override;

    void changeTargetPoint() override;
};

class Arrow : public Projectile {
public:
    Arrow(QPointF position, CombatEntity *owner);

    void action() override;
};

class AcidBubble : public Projectile {
public:
    AcidBubble(QPointF position, CombatEntity *owner);

    void action() override;
};

class FireBall : public Projectile {
public:
    FireBall(QPointF position, CombatEntity *owner);

    void action() override;
};

class IceBall : public Projectile {
public:
    IceBall(QPointF position, CombatEntity *owner);

    void action() override;
};

#endif //THESURVIVOR_ENTITY_H
