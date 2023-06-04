//
// Created by Administrator on 2023/4/20 0020.
//

#ifndef THESURVIVOR_MAINWINDOW_H
#define THESURVIVOR_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <Util.h>
#include <Menu.h>
#include "Game.h"

class MainWindow : public QMainWindow {
private:
Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
public slots:
    void openMainMenu();

    void openCharacterMenu();

    void openPermanentBuffMenu();

    void openAwardMenu();

    void refreshPermanentBuffMenu();
};


#endif //THESURVIVOR_MAINWINDOW_H
