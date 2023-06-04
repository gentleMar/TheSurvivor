//
// Created by Administrator on 2023/4/28 0028.
//

#include "Map.h"

QImage defaultTileTexture = QImage("../resource/tile/Default.png");

Tile::Tile(QRect _bound) {
    bound = _bound;
    texture = &defaultTileTexture;
}

QPoint Tile::getPosition() const {
    return bound.topLeft();
}

QPixmap Tile::getTexture() const {
    return QPixmap::fromImage(*texture);
}

void Tile::setBound(QRect _bound) {
    bound = _bound;
}


Map::Map(int c, int l, int tw, int th) {
    column = c;
    line = l;
    tileWidth = tw;
    tileHeight = th;
    width = column * tileWidth;
    height = line * tileHeight;
    bound = QRect(0, 0, width, height);
    fillWithDefaultTile();
}

void Map::fillWithDefaultTile() {
    for (int i = 0; i < column; ++i) {
        QList<Tile> list;
        for (int j = 0; j < line; ++j) {
            Tile tile(QRect(i * tileWidth, j * tileHeight, tileWidth, tileHeight));
            list.append(tile);
        }
        tiles.append(list);
    }
}

void Map::setTile(int m, int n, Tile &tile) {
    if (0 <= m < column && 0 <= n < line) {
        tiles[m][n] = tile;
        tile.setBound(QRect(m * tileWidth, n * tileHeight, tileWidth, tileHeight));
    } else {
        Error("Placing tile failed");
    }
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

Tile Map::getTile(int m, int n) const {
    return tiles[m][n];
}

int Map::getColumn() const {
    return column;
}

int Map::getLine() const {
    return line;
}

int Map::getTileWidth() const {
    return tileWidth;
}

int Map::getTileHeight() const {
    return tileHeight;
}

QRect Map::getBound() const {
    return bound;
}


