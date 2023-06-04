//
// Created by Administrator on 2023/4/20 0020.
//

#include "MainWindow.h"
#include "Util.h"


Game *game;
MainMenu *mainMenu;
CharacterMenu *characterMenu;
PermanentBuffMenu *permanentBuffMenu;
AwardMenu *awardMenu;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // initialize the main window
    setWindowIcon(QIcon(loadImage("icon/TheSurvivor")));
    setWindowTitle("The Survivor");
    setFixedSize(MY_WINDOW_WIDTH, MY_WINDOW_HEIGHT);

    // create and show the game
    game = new Game(this);
    connect(game,&Game::gameFinished, this,&MainWindow::openMainMenu);
    connect(game,&Game::purchaseSucceeded, this,&MainWindow::refreshPermanentBuffMenu);
    Info("Game was showed");

    // menus
    // Main Menu
    mainMenu = new MainMenu(this);
    connect(mainMenu, &MainMenu::startButtonPressed, this,&MainWindow::openCharacterMenu);
    connect(mainMenu, &MainMenu::buffButtonPressed, this,&MainWindow::openPermanentBuffMenu);

    // Character Menu
    characterMenu = new CharacterMenu(this);
    connect(characterMenu,&CharacterMenu::bowManButtonPressed,game,&Game::restartGame);
    connect(characterMenu,&CharacterMenu::swordManButtonPressed,game,&Game::restartGameWithSwordStudent);

    // Permanent Buff Menu
    permanentBuffMenu = new PermanentBuffMenu(this);
    connect(permanentBuffMenu,&PermanentBuffMenu::permanentSpeedBuffButtonPressed,game,&Game::purchasePermanentSpeedBuff);
    connect(permanentBuffMenu,&PermanentBuffMenu::permanentAttackBuffButtonPressed,game,&Game::purchasePermanentAttackBonusBuff);
    connect(permanentBuffMenu,&PermanentBuffMenu::permanentDefenceBuffButtonPressed,game,&Game::purchasePermanentDefenceBuff);
    connect(permanentBuffMenu, &PermanentBuffMenu::permanentBuffMenuToMainMenuButtonPressed, this, &MainWindow::openMainMenu);
    connect(permanentBuffMenu, &PermanentBuffMenu::permanentBuffMenuToMainMenuButtonPressed, game, &Game::saveGameData);

    // Award Menu
    awardMenu = new AwardMenu(this);
    connect(awardMenu,&AwardMenu::addBowButtonPressed,game,&Game::chooseAddBow);
    connect(awardMenu,&AwardMenu::addBowButtonPressed,game,&Game::continueGame);
    connect(awardMenu,&AwardMenu::addFireMagicBookButtonPressed,game,&Game::chooseAddFireMagicBook);
    connect(awardMenu,&AwardMenu::addFireMagicBookButtonPressed,game,&Game::continueGame);
    connect(awardMenu,&AwardMenu::addIceMagicBookButtonPressed,game,&Game::chooseAddIceMagicBook);
    connect(awardMenu,&AwardMenu::addIceMagicBookButtonPressed,game,&Game::continueGame);
    connect(awardMenu,&AwardMenu::speedBuffButtonPressed,game,&Game::chooseSpeedBuff);
    connect(awardMenu,&AwardMenu::speedBuffButtonPressed,game,&Game::continueGame);
    connect(awardMenu,&AwardMenu::weaponSpeedBuffButtonPressed,game,&Game::chooseWeaponSpeedBuff);
    connect(awardMenu,&AwardMenu::weaponSpeedBuffButtonPressed,game,&Game::continueGame);
    connect(game,&Game::mainPlayerLevelUp, this,&MainWindow::openAwardMenu);

    openMainMenu();
    Info("MainMenu was showed");
}

void MainWindow::openMainMenu() {
    characterMenu->hide();
    permanentBuffMenu->hide();
    awardMenu->hide();
    mainMenu->show();
}

void MainWindow::openCharacterMenu() {
    mainMenu->hide();
    characterMenu->show();
}

void MainWindow::openPermanentBuffMenu() {
    mainMenu->hide();
    permanentBuffMenu->show();
}

void MainWindow::refreshPermanentBuffMenu() {
    permanentBuffMenu->repaint();
}

void MainWindow::openAwardMenu() {
    awardMenu->createNewAndShow();
}






