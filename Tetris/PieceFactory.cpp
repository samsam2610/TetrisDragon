//
//  PieceFactory.cpp
//  Tetris
//
//  Created by Tran Sam on 6/22/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include <stdio.h>
#include "PieceFactory.h"

PieceFactory::PieceFactory() {

}

PieceFactory::~PieceFactory() {
    
}

Piece PieceFactory::getPiece(const Shape &s, const int c, int x, int y) const {
    Piece l = Piece(x, y);
    
    if (s == Shape::L) {
        l.setShape({
            { 0, c, 0 },
            { 0, c, 0 },
            { 0, c, c }
        });
        l.setName('L');
    }
    else if (s == Shape::J) {
        l.setShape({
            { 0, c, 0 },
            { 0, c, 0 },
            { c, c, 0 }
        });
        l.setName('J');
    }
    else if (s == Shape::T) {
        l.setShape({
            { c, c, c },
            { 0, c, 0 },
            { 0, 0, 0 }
        });
        l.setName('T');
    }
    else if (s == Shape::S) {
        l.setShape({
            { 0, 0, 0 },
            { 0, c, c },
            { c, c, 0 }
        });
        l.setName('S');
    }
    else if (s == Shape::Z) {
        l.setShape({
            { 0, 0, 0 },
            { c, c, 0 },
            { 0, c, c }
        });
        l.setName('Z');
    }
    else if (s == Shape::O) {
        l.setShape({
            { 0, 0, 0, 0 },
            { 0, c, c, 0 },
            { 0, c, c, 0 },
            { 0, 0, 0, 0 }
        });
        l.setName('O');
    }
    else if (s == Shape::I) {
        l.setShape({
            { 0, c, 0, 0 },
            { 0, c, 0, 0 },
            { 0, c, 0, 0 },
            { 0, c, 0, 0 }
        });
        l.setName('I');
    } else {
        l.setName('M');
    }

    return l;
}

Piece PieceFactory::getRandomPiece(const int x, const int y) const {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 1000.0);
    
    Shape s = Shape((int)dist(mt) % 7);
    
    int color = (int)dist(mt) % 5 + 1;
    
    return getPiece(s, color, x, y);
}
