//
//  Boardgame.cpp
//  Tetris
//
//  Created by Tran Sam on 6/23/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include "Boardgame.hpp"

BoardGame::~BoardGame() {
}

void BoardGame::nextPiece() {
    if (!followingPiece.hasBody()) {
        followingPiece = pieceFactory.getRandomPiece(width/2 - 1, 0);
        followingPiece.setY(followingPiece.getShape().size() * (-1));
    }
    
    piece = followingPiece;
    followingPiece = pieceFactory.getRandomPiece(width/2 - 1, 0);
    followingPiece.setY(followingPiece.getShape().size() * (-1));
    
    landed();
    
    notify();
}

bool BoardGame::tick() {
    if(!movePieceDown()) {
        if (!landed()) {
            lockDelay += 1;
        } else {
            lockDelay = 0;
        }
    }
    
    return true;
}

bool BoardGame::landed() {
    if (checkLanded()) {
        if (lockDelay >= 3) {
            lockDelay = 0;
            pile.addPiece(piece);
            int l = pile.checkRow();
            lines += l;
            score += l*level*100;
            if (l >= 4 )
                score += l*level*100;
            level = lines/8 + 1;
            pile.calculateFeature();
            pieceCount += 1;
            nextPiece();
            notify();
            return true;
        }
        
    }
    
    return false;
}

bool BoardGame::checkLanded() const {
    int x = piece.getX();
    int y = piece.getY();
    
    auto s = piece.getShape();
    auto p = pile.getPile();
    
    for(int i = 0; i < s.size(); i++)
        for(int j = 0; j < s[0].size(); j++)
            if(s[i][j] > 0)
                if(((y + i + 1) >= height) || ((y + i + 1) >= 0 && x + j >= 0 && p[y + i + 1][x + j] > 0))
                    return true;
    return false;
}

bool BoardGame::movePieceRight() {
    Piece p2 = Piece{ piece};
    p2.moveRight();
    
    if (!checkCollision(p2)) {
        piece.moveRight();
        landed();
        return true;
    }
    return false;
}

bool BoardGame::movePieceLeft() {
    Piece p2 = Piece{ piece };
    p2.moveLeft();
    
    if (!checkCollision(p2)){
        piece.moveLeft();
        landed();
        return true;
    }
    return false;
}

bool BoardGame::movePieceDown() {
    Piece p2 = Piece{ piece };
    p2.moveDown();
    
    if(!checkCollision(p2)) {
        piece.moveDown();
        landed();
        return true;
    }
    return false;
}

void BoardGame::instantDrop() {
    
    while (movePieceDown()) {}
    tick();
}

void BoardGame::rotatePiece() {
    Piece p2 = Piece{ piece };
    p2.rotate();
    
    if (!checkCollision(p2)) {
        piece.rotate();
        landed();
        return true;
    }
    return false;
}

bool BoardGame::checkCollision(const Piece & p) const {
    return checkPileCollision(p) || outOfBounds(p);
}

bool BoardGame::checkPileCollision(const Piece & p) const {
    if(outOfBounds(p))
        return true;
    
    int x = p.getX();
    int y = p.getY();
    
    auto s = p.getShape();
    
    for(int i = 0; i < s.size(); i++)
        for(int j = 0; j < s[0].size(); j++)
            if (s[i][j] > 0) {
                    int xBlock = x + j;
                    int yBlock = y + i;
                        if ((xBlock>= 0 && xBlock < width) && (yBlock >= 0 && yBlock < height) && (pile.getPile()[yBlock][xBlock] > 0))
                            return true;
                        
                }
    
    return  false;
}

bool BoardGame::outOfBounds(const Piece & p) const {
    int x = p.getX();
    int y = p.getY();
    
    auto s = p.getShape();
    
    for(int i = 0; i < s.size(); i++)
        for(int j = 0; j < s[0].size(); j++)
            if (s[i][j] > 0) {
                int xBlock = x + j;
                int yBlock = y + i;
                if(xBlock >= width || yBlock >= height || xBlock < 0)
                    return true;
            }

    return false;
}


void BoardGame::clear() {
    pile.clear();
    followingPiece = pieceFactory.getPiece(Shape::Empty, 0, 0, 1);
    piece = followingPiece;
    score = 0;
    lines = 0;
    level = 1;
    pieceCount = 0;
}

