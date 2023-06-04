//
// Created by Administrator on 2023/4/21 0021.
//

#include <QRandomGenerator>
#include "Menu.h"

extern int coin;

MenuButton::MenuButton(QWidget *parent, QRect bound, QString groundFileName) : QPushButton(parent) {
    _bound = bound;
    setGeometry(_bound);
    ground = loadImage("button/" + groundFileName);
}

void MenuButton::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, ground);
}

StartButton::StartButton(QWidget *parent, QRect bound) : MenuButton(parent, bound, "Start") {
}

BuffButton::BuffButton(QWidget *parent, QRect bound) : MenuButton(parent, bound, "Buff") {
}

MainMenu::MainMenu(QWidget *parent) : QWidget(parent), startButton(this, QRect(HALF_MY_WINDOW_WIDTH - 175,
                                                                               HALF_MY_WINDOW_HEIGHT + 15, 350, 120)),
                                      buffButton(this, QRect(HALF_MY_WINDOW_WIDTH - 175,
                                                             HALF_MY_WINDOW_HEIGHT + 125, 350, 120)) {
    setGeometry(0, 0, MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);
    hall = loadImage("menu/Hall");
    connect(&startButton, &StartButton::clicked, this, &MainMenu::startButtonPressed);
    connect(&buffButton, &BuffButton::clicked, this, &MainMenu::buffButtonPressed);
}

void MainMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(HALF_MY_WINDOW_WIDTH - 400, HALF_MY_WINDOW_HEIGHT - 400, hall);
}


BowManButton::BowManButton(QWidget *parent, QRect bound) : MenuButton(parent, bound, "BowStudent") {
}

SwordManButton::SwordManButton(QWidget *parent, QRect bound) : MenuButton(parent, bound, "SwordStudent") {

}

CharacterMenu::CharacterMenu(QWidget *parent) : QWidget(parent), bowManButton(this, QRect(HALF_MY_WINDOW_WIDTH - 380,
                                                                                          HALF_MY_WINDOW_HEIGHT - 240,
                                                                                          320, 480)),
                                                swordManButton(this, QRect(HALF_MY_WINDOW_WIDTH + 60,
                                                                           HALF_MY_WINDOW_HEIGHT - 240,
                                                                           320, 480)) {
    setGeometry(0, 0, MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);
    connect(&bowManButton, &BowManButton::clicked, this, &CharacterMenu::bowManButtonPressed);
    connect(&bowManButton, &BowManButton::clicked, this, &CharacterMenu::hide);
    connect(&swordManButton, &SwordManButton::clicked, this, &CharacterMenu::swordManButtonPressed);
    connect(&swordManButton, &SwordManButton::clicked, this, &CharacterMenu::hide);
}

void CharacterMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    QFont font("汉仪力量黑简");

    font.setPixelSize(45);
    painter.setFont(font);
    QFontMetrics metrics(font);
    QString text = "选择你的角色";
    QRectF bound = metrics.boundingRect(text);
    bound.setWidth(1.1 * bound.width());
    bound.moveCenter(QPointF(HALF_MY_WINDOW_WIDTH, HALF_MY_WINDOW_HEIGHT - 310 + bound.height() / 2));
    painter.drawText(bound, text);
}

PermanentBuffButton::PermanentBuffButton(QWidget *parent, QRect bound, QString title, QString info, QString coinAmount)
        : MenuButton(parent,
                     bound,
                     "BackGround") {
    _title = title;
    _info = info;
    _coinAmount = coinAmount;
}

void PermanentBuffButton::paintEvent(QPaintEvent *event) {
    MenuButton::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::black);
    QFont font("汉仪力量黑简");

    font.setPixelSize(45);
    painter.setFont(font);
    QFontMetrics metrics(font);
    QRectF bound = metrics.boundingRect(_title);
    bound.setWidth(1.1 * bound.width());
    bound.moveTo(10, 10);
    painter.drawText(bound, _title);

    font.setPixelSize(25);
    painter.setFont(font);
    QFontMetrics metrics1(font);
    QRectF bound1 = metrics1.boundingRect(_info);
    bound1.setWidth(1.1 * bound1.width());
    bound1.moveTo(10, 100 - bound1.height() / 2);
    painter.drawText(bound1, _info);

    font.setPixelSize(15);
    painter.setFont(font);
    QFontMetrics metrics2(font);
    QString text = _coinAmount + " 硬币";
    QRectF bound2 = metrics1.boundingRect(text);
    bound2.setWidth(1.1 * bound2.width());
    bound2.moveTo(bound.right() + 10, bound.center().y() - bound2.height() / 2);
    painter.drawText(bound2, text);
}

PermanentSpeedBuffButton::PermanentSpeedBuffButton(QWidget *parent, QRect bound) : PermanentBuffButton(parent, bound,
                                                                                                       "速度",
                                                                                                       "最大速度永久性地+10%",
                                                                                                       "15") {

}

PermanentAttackBonusBuffButton::PermanentAttackBonusBuffButton(QWidget *parent, QRect bound) : PermanentBuffButton(
        parent, bound,
        "攻击",
        "攻击加成永久性地+10%",
        "10") {

}

PermanentDefenceBuffButton::PermanentDefenceBuffButton(QWidget *parent, QRect bound) : PermanentBuffButton(parent,
                                                                                                           bound,
                                                                                                           "防御",
                                                                                                           "防御力永久性地+1",
                                                                                                           "5") {

}

PermanentBuffMenu::PermanentBuffMenu(QWidget *parent) : QWidget(parent),
                                                        permanentSpeedBuffButton(this, QRect(HALF_MY_WINDOW_WIDTH - 300,
                                                                                             HALF_MY_WINDOW_HEIGHT -
                                                                                             350, 600, 200)),
                                                        permanentAttackBonusBuffButton(this,
                                                                                       QRect(HALF_MY_WINDOW_WIDTH - 300,
                                                                                             HALF_MY_WINDOW_HEIGHT -
                                                                                             100,
                                                                                             600, 200)),
                                                        permanentDefenceBuffButton(this,
                                                                                   QRect(HALF_MY_WINDOW_WIDTH - 300,
                                                                                         HALF_MY_WINDOW_HEIGHT + 150,
                                                                                         600, 200)),
                                                        permanentBuffMenuToMainMenuButton(this,
                                                                                          QRect(HALF_MY_WINDOW_WIDTH +
                                                                                                400,
                                                                                                HALF_MY_WINDOW_HEIGHT -
                                                                                                32, 64, 64)) {
    setGeometry(0, 0, MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);
    connect(&permanentSpeedBuffButton, &PermanentSpeedBuffButton::clicked, this,
            &PermanentBuffMenu::permanentSpeedBuffButtonPressed);
    connect(&permanentAttackBonusBuffButton, &PermanentAttackBonusBuffButton::clicked, this,
            &PermanentBuffMenu::permanentAttackBuffButtonPressed);
    connect(&permanentDefenceBuffButton, &PermanentDefenceBuffButton::clicked, this,
            &PermanentBuffMenu::permanentDefenceBuffButtonPressed);
    connect(&permanentBuffMenuToMainMenuButton, &PermanentBuffMenuToMainMenuButton::clicked, this,
            &PermanentBuffMenu::permanentBuffMenuToMainMenuButtonPressed);
}

void PermanentBuffMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    QFont font("汉仪力量黑简");
    font.setPixelSize(30);
    painter.setFont(font);
    QFontMetrics metrics(font);
    QString text = "拥有硬币: ";
    QRectF bound = metrics.boundingRect(text);
    QRectF bound1 = metrics.boundingRect(QString::number(coin));
    text += QString::number(coin);
    bound.setWidth(1.1 * bound.width() + bound1.width());
    bound.moveCenter(QPointF(HALF_MY_WINDOW_WIDTH - 400 - bound.width() / 2, HALF_MY_WINDOW_HEIGHT));
    painter.drawText(bound, text);
}

PermanentBuffMenuToMainMenuButton::PermanentBuffMenuToMainMenuButton(QWidget *parent, QRect bound) : MenuButton(parent,
                                                                                                                bound,
                                                                                                                "Return") {

}

AwardButton::AwardButton(QWidget *parent, QRect bound, QString title, QString info) : MenuButton(parent,
                                                                                                 bound,
                                                                                                 "BackGround") {
    _title = title;
    _info = info;
}

void AwardButton::paintEvent(QPaintEvent *event) {
    MenuButton::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::black);
    QFont font("汉仪力量黑简");

    font.setPixelSize(45);
    painter.setFont(font);
    QFontMetrics metrics(font);
    QRectF bound = metrics.boundingRect(_title);
    bound.setWidth(1.1 * bound.width());
    bound.moveTo(10, 10);
    painter.drawText(bound, _title);

    font.setPixelSize(25);
    painter.setFont(font);
    QFontMetrics metrics1(font);
    QRectF bound1 = metrics1.boundingRect(_info);
    bound1.setWidth(1.1 * bound1.width());
    bound1.moveTo(10, 100 - bound1.height() / 2);
    painter.drawText(bound1, _info);
}

AddBowButton::AddBowButton(QWidget *parent, QRect bound) : AwardButton(parent, bound, "弓",
                                                                       "获得发射跟踪箭的弓，如果你已拥有则加快攻速") {

}

AddFireMagicBookButton::AddFireMagicBookButton(QWidget *parent, QRect bound) : AwardButton(parent, bound,
                                                                                           "火魔法书",
                                                                                           "获得发射火焰弹的火魔法书，如果已拥有则加快攻速") {

}

AddIceMagicBookButton::AddIceMagicBookButton(QWidget *parent, QRect bound) : AwardButton(parent, bound,
                                                                                         "冰魔法书",
                                                                                         "获得发射寒冰弹的冰魔法书，如果已拥有则加快攻速") {

}

SpeedBuffButton::SpeedBuffButton(QWidget *parent, QRect bound) : AwardButton(parent, bound, "移速",
                                                                             "在这一局中你的最大速度+10%") {

}

WeaponSpeedBuffButton::WeaponSpeedBuffButton(QWidget *parent, QRect bound) : AwardButton(parent, bound, "攻速",
                                                                                         "在这一局中你的武器攻击速度+10%") {

}

AwardMenu::AwardMenu(QWidget *parent) : QWidget(parent), addBowButton(this, QRect(0, 0, 600, 200)),
                                        addFireMagicBookButton(this, QRect(0, 0, 600, 200)),
                                        addIceMagicBookButton(this, QRect(0, 0, 600, 200)),
                                        speedBuffButton(this, QRect(0, 0, 600, 200)),
                                        weaponSpeedBuffButton(this, QRect(0, 0, 600, 200)) {
    setGeometry(0, 0, MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);
    connect(&addBowButton, &AddBowButton::clicked, this, &AwardMenu::addBowButtonPressed);
    connect(&addBowButton, &AddBowButton::clicked, this, &AwardMenu::hide);
    connect(&addFireMagicBookButton, &AddFireMagicBookButton::clicked, this, &AwardMenu::addFireMagicBookButtonPressed);
    connect(&addFireMagicBookButton, &AddFireMagicBookButton::clicked, this, &AwardMenu::hide);
    connect(&addIceMagicBookButton, &AddIceMagicBookButton::clicked, this, &AwardMenu::addIceMagicBookButtonPressed);
    connect(&addIceMagicBookButton, &AddIceMagicBookButton::clicked, this, &AwardMenu::hide);
    connect(&speedBuffButton, &SpeedBuffButton::clicked, this, &AwardMenu::speedBuffButtonPressed);
    connect(&speedBuffButton, &SpeedBuffButton::clicked, this, &AwardMenu::hide);
    connect(&weaponSpeedBuffButton, &WeaponSpeedBuffButton::clicked, this, &AwardMenu::weaponSpeedBuffButtonPressed);
    connect(&weaponSpeedBuffButton, &WeaponSpeedBuffButton::clicked, this, &AwardMenu::hide);
}

void AwardMenu::createNewAndShow() {
    addBowButton.hide();
    addFireMagicBookButton.hide();
    addIceMagicBookButton.hide();
    speedBuffButton.hide();
    weaponSpeedBuffButton.hide();
    QVector<int> nums = {0, 1, 2, 3, 4};
    for (int i = 0; i < 3; ++i) {
        int ran = QRandomGenerator::global()->bounded(nums.length());
        switch (nums[ran]) {
            case 0:
                addBowButton.setGeometry(HALF_MY_WINDOW_WIDTH - 300, HALF_MY_WINDOW_HEIGHT -
                                                                     350 + 250 * i, 600, 200);
                addBowButton.show();

                break;
            case 1:
                addFireMagicBookButton.setGeometry(HALF_MY_WINDOW_WIDTH - 300, HALF_MY_WINDOW_HEIGHT -
                                                                               350 + 250 * i, 600, 200);
                addFireMagicBookButton.show();

                break;
            case 2:
                addIceMagicBookButton.setGeometry(HALF_MY_WINDOW_WIDTH - 300, HALF_MY_WINDOW_HEIGHT -
                                                                              350 + 250 * i, 600, 200);
                addIceMagicBookButton.show();
                break;
            case 3:
                speedBuffButton.setGeometry(HALF_MY_WINDOW_WIDTH - 300, HALF_MY_WINDOW_HEIGHT -
                                                                        350 + 250 * i, 600, 200);
                speedBuffButton.show();
                break;
            case 4:
                weaponSpeedBuffButton.setGeometry(HALF_MY_WINDOW_WIDTH - 300, HALF_MY_WINDOW_HEIGHT -
                                                                        350 + 250 * i, 600, 200);
                weaponSpeedBuffButton.show();
                break;
        }
        nums.removeAt(ran);
    }
    show();
}


