//
// Created by Administrator on 2023/4/21 0021.
//

#ifndef THESURVIVOR_UTIL_H
#define THESURVIVOR_UTIL_H

//#define NO_INFO
#define MY_WINDOW_WIDTH 1280
#define HALF_MY_WINDOW_WIDTH 640
#define MY_WINDOW_HEIGHT 800
#define HALF_MY_WINDOW_HEIGHT 400
#define MY_TPS 125

#include <QWidget>
#include <QString>
#include <QPixmap>
#include <QTime>


void Info(const QString& text);

void Error(const QString& text);

QString resourcePath(const QString &fileName);

QPixmap loadImage(const QString &fileName);


#endif //THESURVIVOR_UTIL_H
