//
//  PieceFactory.h
//  Tetris
//
//  Created by Tran Sam on 6/22/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef PieceFactory_h
#define PieceFactory_h

#pragma once
#include "Piece.h"
#include <random>

enum class Shape{L, T, Z, S, I, J, O, Empty};

class PieceFactory {
public:
    PieceFactory();
    ~PieceFactory();
    
    // Piece creation
    Piece   getPiece(const Shape& s, const int c, int x, int y) const;
    
    // Randomized piece creation
    Piece   getRandomPiece(const int x, const int y) const;
};

#endif /* PieceFactory_h */
