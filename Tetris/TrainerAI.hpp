//
//  TrainerAI.hpp
//  Tetris
//
//  Created by Tran Sam on 8/18/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef TrainerAI_hpp
#define TrainerAI_hpp

#include <stdio.h>
#include <fstream>
#include <vector>
#include <math.h>
#include "GeneticAlgorithm.hpp"
#include "NeuralNet.hpp"
#include "Boardgame.hpp"
#include "GameAI.hpp"

using namespace std;
class TrainerAI {
private:
    BoardGame & board;
    BoardGame *pboard;
    
    int     populationSize;
    double  rateMutation;
    double  rateCrossover;
    int     weightCount;
    
    int     generationCount = 0;
    
    GeneticAlgorithm myGenome { populationSize, rateMutation, rateCrossover, weightCount };
    
    NeuralNet myBrain {};
    vector<Genome> population;
    Genome  bestGenome;
    
    bool    gameStateRunning;
    double  score = 0;
    double  fitness;
    double  fitnessHighest;
    
    int     gameModeAI = 2;
    GameAI  myAI { board, gameModeAI };

    void    tick();
    void    initComp();
    
    
public:
    TrainerAI(BoardGame & board, int populationSize, double rateMutation, double rateCrossover, int weightCount):
    board{ board }, populationSize{ populationSize }, rateMutation{ rateMutation }, rateCrossover{ rateCrossover }, weightCount { weightCount }
    {
        initComp();
        this->weightCount = myBrain.getElementCount();
    }
    
    void    setGenomePopulation(int populationSize, double rateMutation, double rateCrossover, int weightCount);
    
    Genome  getBestGenome() const { return bestGenome; };
    
    int     getGenerationCount() { return generationCount; };
    void    startGame();
    void    endGame();
    
    void    perGeneration();
    void    epoch(int numberOfCycle);
    void    recordResult();
    void    debuggingMode(vector<double> weights);
    void    epochSingle();
};

#endif /* TrainerAI_hpp */
