//
// Created by Administrator on 2023/4/28 0028.
//

#ifndef THESURVIVOR_MAP_H
#define THESURVIVOR_MAP_H

#include <QPixmap>
#include <Util.h>
#include <QPainter>

class Tile {
protected:
    QImage *texture;

    QRect bound;
public:
    Tile(QRect _bound);

    void setBound(QRect _bound);

    QPoint getPosition() const;

    QPixmap getTexture() const;
};

class Map {
protected:
    QList<QList<Tile>> tiles;
    QRect bound;

    int column;
    int line;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
public:
    Map(int c, int l, int tw, int th);

    Tile getTile(int m, int n) const;

    void fillWithDefaultTile();

    void setTile(int m, int n, Tile &tile);

    int getColumn() const;

    int getLine() const;

    int getWidth() const;

    int getHeight() const;

    int getTileWidth() const;

    int getTileHeight() const;

    QRect getBound() const;
};


#endif //THESURVIVOR_MAP_H
