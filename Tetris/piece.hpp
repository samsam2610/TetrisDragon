//
//  piece.h
//  Tetris
//
//  Created by Tran Sam on 6/8/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef piece_h
#define piece_h

#pragma once
#include <vector>

class Piece
{
private:
    
    int id;
    int x; // horizontal position
    int y; // vertical position
    
    std::vector<std::vector<int>> shape;
    static int IDs;

public:
    Piece() {};
    
    Piece
#endif /* piece_h */
