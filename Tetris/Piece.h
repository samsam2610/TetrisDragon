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
using namespace std;

class Piece
{
private:
    
    int     id;
    int     x; // horizontal position
    int     y; // vertical position
    int     countRotation = 0;
    
    vector<vector<int>> shape;
    static int IDs;
    char    name;

public:
    Piece() {};
    
    Piece(const int x, const int y) :id{generateId()}, x{x}, y{y}, name{name} {};

    Piece(const Piece &p2) {
    	id = p2.id;
    	x = p2.x;
    	y = p2.y;
    	shape = p2.shape;
        name = p2.name;
    };

    // getters
    int     getId() const { return id; };
    int     getX() const { return x; };
	int     getY() const { return y; };
    char    getName() const { return name; };
    int     getRotationCount() const { return countRotation; };
    vector<vector<int>> getShape() const { return shape; };


	// setters
    void    setX(const int x) { this->x = x; };
    void    setY(const int y) { this->y = y; };
    void    setShape(const vector<vector<int>>& newShape) { shape = newShape; };
    void    setName(const char newName) { this->name = newName; };

	// generates a new id
	const static int generateId() {
		IDs++;
		return IDs;
	}

	//rotates the shape clockwise
	void    rotate();

	//moves right
	void    moveRight();

	//moves left
	void    moveLeft();

	//moves the shape down
	void    moveDown();

	//moves the shape up
	void    moveUp();

	// check piece's dimension
	bool    hasBody() const;
};
#endif /* piece_h */
