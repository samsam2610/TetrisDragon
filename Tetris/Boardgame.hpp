//
//  Boardgame.hpp
//  Tetris
//
//  Created by Tran Sam on 6/23/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef Boardgame_hpp
#define Boardgame_hpp

#include <stdio.h>
#pragma once
#include "Pile.hpp"
#include "PieceFactory.h"
#include "Observ.hpp"

class BoardGame: public Subject {
private:
    int     width;
    int     height;
    
    int     score = 0;
    int     lines = 0;
    int     level = 1;
    int     lockDelay = 0;
    int     pieceCount = 0;
    
    Pile    pile;
    PieceFactory pieceFactory;
    Piece   piece;
    Piece   followingPiece;
    
public:
    BoardGame(int width, int height) :width{ width }, height{ height }, pile{ Pile(width, height) }, pieceFactory{ PieceFactory () } {}
    ~BoardGame();
    
    //getters
    int getWidth() const { return width; };
    int getHeight() const { return  height; };
    int getScore() const { return score; };
    int getLines() const { return lines; };
    int getLevel() const { return level; };
    int getPieceCount() const { return pieceCount; };
    
    Piece getPiece() const { return piece; };
    Piece getFollowingPiece() const { return followingPiece; };
    Pile getPile() const { return pile; };
    
    //setters
    int setLockDelay(const int lockDelay) { this->lockDelay = lockDelay; };
    //generates the next pieces
    void nextPiece();
    
    //performed action
    bool tick();
    
    //check landed piece
    bool landed();
    
    bool checkLanded() const;
    
    //move piece right bool
    bool movePieceRight();
    
    //move piece left bool
    bool movePieceLeft();
    
    //move piece down bool
    bool movePieceDown();
    
    //rotate piece
    void rotatePiece();
    
    //Instant drop
    void instantDrop();
    
    //check piece's collisions in general
    bool checkCollision(const Piece& p) const;
    
    //check piece's collision with pile
    bool checkPileCollision(const Piece& p) const;
    
    //check out of bound
    bool outOfBounds(const Piece& p) const;
    
    //resets
    void clear();
    
};
#endif /* Boardgame_hpp */
