//
//  GameAI.cpp
//  Tetris
//
//  Created by Tran Sam on 7/22/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include "GameAI.hpp"
void GameAI::print(vector<vector<int>> const &input)
{
    for (unsigned int i = 0; i < input.size(); ++i)
    {
        for (unsigned int j = 0; j < input[i].size(); ++j)
        {
            cout << input[i][j];
        }
        cout << std::endl;
    }
}

double GameAI::calculateScore(Pile pile) {
    int valueHole = pile.getValueHole();
    int valueCompleteLine = pile.getValueCompleteLine();
    int valueHeightSum = pile.getValueHeightSum();
    int valueBumpy = pile.getValueBumpy();
    int valueBlockage = pile.getValueBlockage();
    
    scorePile = valueHole*conHole + valueCompleteLine*conLine + valueHeightSum*conHeight + valueBumpy*conBumpy + valueBlockage*conBlockage;
    
    return scorePile;
}

double GameAI::calculateScoreNN(Pile pile) {
    vector<double> listNNInputs = obtainNNInputs(pile);
    vector<double> scoreList = neuralBrain.calculateOutput(listNNInputs);

    scorePile = scoreList[0];
    
    return scorePile;
}


vector<double> GameAI::obtainNNInputs(Pile pile) {
  
    vector<double> listNNInputs;
    
    listNNInputs = pile.getSetFeatures();

    return listNNInputs;
}

void GameAI::findMoveSet(BoardGame *currentBoard) {
    countLeftMove = 0;
    countRightMove = 0;
    countRotation = 0;
    
    double maxScorePile = -100000;
    
    int movingYLimit;
    
    for (int indexRotation = 0; indexRotation < 4; indexRotation++) {
        for (int indexWidth = 0; indexWidth < board.getWidth(); indexWidth++) {
            planningBoard = new BoardGame(*currentBoard);

            int tempCountRotation = 0;
            int tempCountLeftMove = 0;
            int tempCountRightMove = 0;
            
            for (int currentRotation = 0; currentRotation < indexRotation; currentRotation++) {
                planningBoard->rotatePiece();
                tempCountRotation += 1;
            }
            
            if (indexWidth < board.getWidth()/2) {
                movingYLimit = indexWidth;
                for (int currentMoveLeft = 0; currentMoveLeft < movingYLimit; currentMoveLeft++) {
                    planningBoard->movePieceLeft();
                    tempCountLeftMove += 1;
                }
            } else if (indexWidth >= board.getWidth()/2) {
                movingYLimit = indexWidth - board.getWidth()/2;
                for (int currentMoveRight = 0; currentMoveRight < movingYLimit; currentMoveRight++) {
                    planningBoard->movePieceRight();
                    tempCountRightMove += 1;
                }
            }
            
            planningBoard->instantDrop();
            planningBoard->setLockDelay(5);
            planningBoard->tick();
            Pile pile = planningBoard->getPile();
            
            auto pileShape = pile.getPile();
//            print(pileShape);
            
            double currentScorePile;
            if (modeAI == 1) {
                currentScorePile  = calculateScore(pile);
            } else if (modeAI > 1) {
                currentScorePile = calculateScoreNN(pile);
            }
            
            
            if (currentScorePile > maxScorePile) {
                maxScorePile = currentScorePile;
                if (tempCountLeftMove > 0) {
                    countRightMove = 0;
                    countLeftMove = tempCountLeftMove;
                } else if (tempCountRightMove > 0) {
                    countRightMove = tempCountRightMove;
                    countLeftMove = 0;
                }
                countRotation = tempCountRotation;
                bestPositionFound = true;
            }
            delete planningBoard;
        }
    }
}

void GameAI::makeMoves() {

    if (bestPositionFound) {
        if (countRightMove > 0) {
            for (int indexMove = 0; indexMove < countRightMove; indexMove++) {
                board.movePieceRight();
            }
        } else if (countLeftMove > 0) {
            for (int indexMove = 0; indexMove < countLeftMove; indexMove++) {
                board.movePieceLeft();
            }
        }
        
        if (countRotation > 0) {
            for (int indexMove = 0; indexMove < countRotation; indexMove++) {
                board.rotatePiece();
            }
        }
        board.instantDrop();
        bestPositionFound = false;
    }

}
