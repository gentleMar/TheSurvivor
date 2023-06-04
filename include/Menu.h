//
// Created by Administrator on 2023/4/21 0021.
//

#ifndef THESURVIVOR_MENU_H
#define THESURVIVOR_MENU_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPushButton>
#include <Util.h>

class MenuButton : public QPushButton {
Q_OBJECT

protected:
    QPixmap ground;
    QRect _bound;

    void paintEvent(QPaintEvent *event) override;

public:
    MenuButton(QWidget *parent, QRect bound, QString groundFileName);
};

class StartButton : public MenuButton {
public:
    StartButton(QWidget *parent, QRect bound);
};

class BuffButton : public MenuButton {
public:
    BuffButton(QWidget *parent, QRect bound);
};

class MainMenu : public QWidget {
Q_OBJECT;
private:
    QPixmap hall;
    StartButton startButton;
    BuffButton buffButton;
public:
    MainMenu(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

signals:

    void startButtonPressed();

    void buffButtonPressed();
};

class BowManButton : public MenuButton {
public:
    BowManButton(QWidget *parent, QRect bound);
};

class SwordManButton : public MenuButton {
public:
    SwordManButton(QWidget *parent, QRect bound);
};

class CharacterMenu : public QWidget {
Q_OBJECT;
private:
    BowManButton bowManButton;
    SwordManButton swordManButton;
public:
    CharacterMenu(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

signals:

    void bowManButtonPressed();

    void swordManButtonPressed();
};

class PermanentBuffButton : public MenuButton {
protected:
    QString _title;
    QString _info;
    QString _coinAmount;
public:
    PermanentBuffButton(QWidget *parent, QRect bound, QString title, QString info, QString coinAmount);

    void paintEvent(QPaintEvent *event) override;
};

class PermanentSpeedBuffButton : public PermanentBuffButton {
public:
    PermanentSpeedBuffButton(QWidget *parent, QRect bound);
};

class PermanentAttackBonusBuffButton : public PermanentBuffButton {
public:
    PermanentAttackBonusBuffButton(QWidget *parent, QRect bound);
};

class PermanentDefenceBuffButton : public PermanentBuffButton {
public:
    PermanentDefenceBuffButton(QWidget *parent, QRect bound);
};

class PermanentBuffMenuToMainMenuButton : public MenuButton {
public:
    PermanentBuffMenuToMainMenuButton(QWidget *parent, QRect bound);
};


class PermanentBuffMenu : public QWidget {
Q_OBJECT;
private:
    PermanentSpeedBuffButton permanentSpeedBuffButton;
    PermanentAttackBonusBuffButton permanentAttackBonusBuffButton;
    PermanentDefenceBuffButton permanentDefenceBuffButton;
    PermanentBuffMenuToMainMenuButton permanentBuffMenuToMainMenuButton;
public:
    PermanentBuffMenu(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

signals:

    void permanentSpeedBuffButtonPressed();

    void permanentAttackBuffButtonPressed();

    void permanentDefenceBuffButtonPressed();

    void permanentBuffMenuToMainMenuButtonPressed();
};

class AwardButton : public MenuButton {
protected:
    QString _title;
    QString _info;
public:
    AwardButton(QWidget *parent, QRect bound, QString title, QString info);

    void paintEvent(QPaintEvent *event) override;
};

class AddBowButton : public AwardButton {
public:
    AddBowButton(QWidget *parent, QRect bound);
};

class AddFireMagicBookButton : public AwardButton {
public:
    AddFireMagicBookButton(QWidget *parent, QRect bound);
};

class AddIceMagicBookButton : public AwardButton {
public:
    AddIceMagicBookButton(QWidget *parent, QRect bound);
};

class SpeedBuffButton : public AwardButton {
public:
    SpeedBuffButton(QWidget *parent, QRect bound);
};

class WeaponSpeedBuffButton : public AwardButton {
public:
    WeaponSpeedBuffButton(QWidget *parent, QRect bound);
};

class AwardMenu : public QWidget {
Q_OBJECT;
private:
    AddBowButton addBowButton;
    AddFireMagicBookButton addFireMagicBookButton;
    AddIceMagicBookButton addIceMagicBookButton;
    SpeedBuffButton speedBuffButton;
    WeaponSpeedBuffButton weaponSpeedBuffButton;
public:
    AwardMenu(QWidget *parent = nullptr);

    void createNewAndShow();
signals:
    void addBowButtonPressed();

    void addFireMagicBookButtonPressed();

    void addIceMagicBookButtonPressed();

    void speedBuffButtonPressed();

    void weaponSpeedBuffButtonPressed();
};

#endif //THESURVIVOR_MENU_H