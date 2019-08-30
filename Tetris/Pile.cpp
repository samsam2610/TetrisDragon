//
//  Pile.cpp
//  Tetris
//
//  Created by Tran Sam on 6/23/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include "Pile.hpp"

int Pile::checkRow() {
    int removed = 0;
    
    for (int i = 0; i < height; i++) {
        bool full = true;
        for (int j = 0; j < width; j++) {
            if (pile[i][j] == 0) {
                full = false;
            }
        }
        
        if (full == true) {
            removeRow(i);
            removed++;
        }
    }
    
    return removed;
}

void Pile::calculateValueCompleteLine() {
    valueCompleteLine = checkRow();
}

void Pile::calculateValueHole() {
    valueHole = 0;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (pile[i][j] == 0) {
                for (int ii = i; ii >= 0; ii --) {
                    if (pile[ii][j] > 0) {
                        valueHole += 1;
                        break;
                    }
                }
                
            }
        }
    }
}

void Pile::calculateValueBlockage() {
    valueBlockage = 0;
    if (valueHole > 0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (pile[i][j] == 0) {
                    for (int ii = i; ii >= 0; ii --) {
                        if (pile[ii][j] > 0) {
                            valueBlockage += 1;
                        }
                    }
                    
                }
            }
        }

    }
}

void Pile::calculateValueHeightSum() {
    valueHeightSum = 0;

    for (auto& n : valueHeightEach)
        valueHeightSum += n;
    
}

void Pile::calculateValueHeightEach() {
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            if  (pile[i][j] > 0) {
                valueHeightEach[j] = height - i;
                break;
            }
        }
    }
}

void Pile::calculateValueBumpy() {
    valueBumpy = 0;
    
    for (int j = 0; j < width; j++ ){
        if ((j + 1) < width) {
            valueBumpy += abs(valueHeightEach[j] - valueHeightEach[j+1]);
        }
    }
}

void Pile::calculateFeature() {
    calculateValueHeightEach();
    calculateValueHeightSum();
    calculateValueBumpy();
    calculateValueHole();
    calculateValueBlockage();
    calculateValueCompleteLine();
}

void Pile::removeRow(int row) {
    for (int i = row; i >= 1; i--) {
        for (int j = 0; j <= width; j++) {
            pile[i][j] = pile[i-1][j];
        }
    }
    
    for (int j = 0; j < width; j++) {
        pile[0][j] = 0;
    }
}

void Pile::addPiece(const Piece& p) {
    int x = p.getX();
    int y = p.getY();
    
    vector<vector<int>> shape = p.getShape();
    
    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j] > 0) {
                if (y + i < 0 || x + j < 0)
                     throw PlaceException{ "out of bound" };
                pile[y + i][x + j] = shape[i][j];
            }
        }
    }
    
}


void Pile::clear() {
    for (auto& row:pile) {
        for (auto& block:row) {
            block = 0;
        }
    }
    
}

vector<double> Pile::getSetFeatures() {
    vector<double> listFeatures;
    
    calculateFeature();
    listFeatures.push_back((double)getValueHole());
    listFeatures.push_back((double)getValueCompleteLine());
    listFeatures.push_back((double)getValueHeightSum());
    listFeatures.push_back((double)getValueBumpy());
    listFeatures.push_back((double)getValueBlockage());
    
    return listFeatures;
}

