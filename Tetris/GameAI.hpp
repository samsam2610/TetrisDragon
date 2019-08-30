//
//  GameAI.hpp
//  Tetris
//
//  Created by Tran Sam on 7/22/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef GameAI_hpp
#define GameAI_hpp

#pragma once
#include <stdio.h>
#include "Boardgame.hpp"
#include <iostream>
#include "NeuralNet.hpp"


class GameAI {
private:
    BoardGame & board;

    void    initComp();
    int     *score;
    BoardGame *planningBoard;
    
    int     modeAI;
    double  scorePile;
    
    double  conHeight = -0.8;
    double  conLine = 2;
    double  conHole = -0.3;
    double  conBumpy = -0.2;
    double  conBlockage = -0.5;
    
    int     countLeftMove;
    int     countRightMove;
    int     countRotation;
    
    int     planningX;
    int     planningY;
    int     planningRotate;
    Piece   planningPiece;
    bool    bestPositionFound = false;
    
    double  calculateScore(Pile pile);
    double  calculateScoreNN(Pile pile);
    
        
    vector<double>    obtainNNInputs(Pile pile);
    NeuralNet neuralBrain;


    
public:
    GameAI(BoardGame & board, int modeAI) : board{ board }, modeAI{ modeAI } {};
    
    //Setters
    void    setScore(int *score) { this->score = score; };
    
    void    setConHeight(double conHeight) { this->conHeight = conHeight; };
    void    setConLine(double conLine) { this->conLine = conLine; };
    void    setConHole(double conHole) { this->conHole = conHole; };
    void    setConBumpy(double conBumpy) { this->conBumpy = conBumpy; };
    void    setConBlockage(double conBlockage) { this->conBlockage = conBlockage; };
    
    void    setNeuralNet(NeuralNet neuralBrain) { this->neuralBrain = neuralBrain; };

    
    double  getScorePile() { return scorePile; };
    
    void    findMoveSet(BoardGame *currentBoard);
    void    makeMoves();
    void    print(vector<vector<int>> const &input);
    
};
#endif /* GameAI_hpp */
