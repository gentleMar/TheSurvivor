//
// Created by Administrator on 2023/4/28 0028.
//

#include "Weapon.h"
#include "Entity.h"

extern Collection entities;
extern int deltaT;

Weapon::Weapon(CombatEntity *_owner) {
    owner = _owner;
}

void Weapon::prepare() {
    if (deltaT >= CDLeft) {
        CDLeft = 0;
    } else {
        CDLeft -= deltaT;
    }
}

int Weapon::getWeaponID() const {
    return weaponID;
}

void Weapon::addAmount() {
    ++amount;
}

void Weapon::fire(Projectile *projectile) {
    entities.addProjectile(projectile);
    CDLeft = CD / owner->getWeaponSpeed() / amount;
}

Bow::Bow(CombatEntity *_owner) : Weapon(_owner) {
    CD = 2000;
    CDLeft = 2000;
    amount = 1;
    weaponID = 0;
}

void Bow::beUsed() {
    prepare();
    if (CDLeft == 0 && !entities.getEnemies().empty()) {
        fire(new Arrow(owner->getCenter(), owner));
    }
}

Acid::Acid(CombatEntity *_owner) : Weapon(_owner) {
    CD = 2000;
    CDLeft = 2000;
    amount = 1;
    weaponID = 1;
}

void Acid::beUsed() {
    prepare();
    if (CDLeft == 0 && !entities.getPlayers().empty()) {
        fire(new AcidBubble(owner->getCenter(), owner));
    }
}

FireMagicBook::FireMagicBook(CombatEntity *_owner) : Weapon(_owner) {
    CD = 3000;
    CDLeft = 3000;
    amount = 1;
    weaponID = 2;
}

void FireMagicBook::beUsed() {
    prepare();
    if (CDLeft == 0 && !entities.getEnemies().empty()) {
        fire(new FireBall(owner->getCenter(), owner));
    }
}

IceMagicBook::IceMagicBook(CombatEntity *_owner) : Weapon(_owner) {
    CD = 3500;
    CDLeft = 3500;
    amount = 1;
    weaponID = 3;
}

void IceMagicBook::beUsed() {
    prepare();
    if (CDLeft == 0 && !entities.getEnemies().empty()) {
        fire(new IceBall(owner->getCenter(), owner));
    }
}
