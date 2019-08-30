//
//  Pile.hpp
//  Tetris
//
//  Created by Tran Sam on 6/23/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef Pile_hpp
#define Pile_hpp

#pragma once
#include <stdio.h>
#include "Piece.h"
#include <string>

class PlaceException {
    string  msg;
public:
    PlaceException(std::string m) : msg{ m } {}
    string  getMsg() const { return msg; }
};

class Pile {
private:
    int     width;
    int     height;
    vector<vector<int>> pile;
    vector<int> valueHeightEach;
    
    int     valueHole;
    int     valueCompleteLine;
    int     valueHeightSum;
    int     valueBumpy;
    int     valueBlockage;
    
    void    calculateValueHole();
    void    calculateValueCompleteLine();
    void    calculateValueHeightSum();
    void    calculateValueHeightEach();
    void    calculateValueBumpy();
    void    calculateValueBlockage();
    
    
public:
    Pile(int width, int height) :width{ width }, height{ height } {
        pile.resize(height);
        valueHeightEach.resize(width);
        for (int i = 0; i < pile.size(); i++) {
            pile[i].resize(width, 0);
        }
    };
    
    // getters
    int     getWidth() const { return width; };
    int     getHeight() const { return height; };
    vector<vector<int>> getPile() const { return pile; };
    
    
    // check for completed rows
    int     checkRow();
    
    // remove specific rows;
    void    removeRow(const int row);
    
    // adds a piece to a pile
    void    addPiece(const Piece& p);
    
    //clears the pile
    void    clear();
    
    //Get pile's characteristics
    int     getValueHole() { return valueHole; };
    int     getValueCompleteLine() { return valueCompleteLine; };
    int     getValueHeightSum() { return valueHeightSum; };
    vector<int> getValueHeightEach() { return valueHeightEach; };
    int     getValueBumpy() { return valueBumpy; };
    int     getValueBlockage() { return valueBlockage; };
    
    vector<double> getSetFeatures();
    
    // Calculate pile's characteristics
    void    calculateFeature();

};
#endif /* Pile_hpp */
