//
// Created by Administrator on 2023/4/28 0028.
//

#ifndef THESURVIVOR_WEAPON_H
#define THESURVIVOR_WEAPON_H

#include <QPointF>
#include <QTimer>

class Projectile;

class CombatEntity;

class Weapon {
protected:
    CombatEntity *owner;
    double CD;
    unsigned weaponID;
    int amount;
    int CDLeft;
    bool ready;
public:
    Weapon(CombatEntity *_owner);

    virtual void beUsed() = 0;

    void fire(Projectile *projectile);

    void prepare();

    int getWeaponID() const;

    void addAmount();
};

class Bow : public Weapon {
public:
    Bow(CombatEntity *_owner);

    void beUsed() override;
};

class Acid : public Weapon {
public:
    Acid(CombatEntity *_owner);

    void beUsed() override;
};

class FireMagicBook : public Weapon {
public:
    FireMagicBook(CombatEntity *_owner);

    void beUsed() override;
};

class IceMagicBook : public Weapon {
public:
    IceMagicBook(CombatEntity *_owner);

    void beUsed() override;
};


#endif //THESURVIVOR_WEAPON_H
