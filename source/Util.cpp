//
// Created by Administrator on 2023/4/21 0021.
//

#include "Util.h"

void Info(const QString &text) {
#ifdef NO_INFO
#else
    qInfo() << "[INFO] " + text;
#endif
}

void Error(const QString &text) {
    qInfo() << "[Error] " + text;
    exit(-1);
}

QString resourcePath(const QString &fileName) {
    return "../resource/" + fileName;
}

QPixmap loadImage(const QString &fileName) {
    QPixmap result = {resourcePath(fileName) + ".png"};
    if (result.isNull()) {
        Error("Loading image " + fileName + " failed");
    }
    return result;
}
