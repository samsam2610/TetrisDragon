//
//  TrainerAI.cpp
//  Tetris
//
//  Created by Tran Sam on 8/18/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include "TrainerAI.hpp"


void TrainerAI::initComp() {
    pboard = &board;
    population = myGenome.initializePopulation();
    myBrain.createNN();
    ofstream myRecord;
    myRecord.open("Population Record.csv");
    myRecord << populationSize << ", " << rateMutation << ", " << rateCrossover << ", " << weightCount << endl;
    cout << populationSize << ", " << rateMutation << ", " << rateCrossover << ", " << weightCount << endl;

    myRecord.close();
}

void TrainerAI::tick() {
    
    try {
        if (gameStateRunning) {
            board.tick();
            myAI.findMoveSet(pboard);
            myAI.makeMoves();
        }
        
    }
    catch (PlaceException e) {
        endGame();
    }
    
}

void TrainerAI::startGame() {
    board.clear();
    gameStateRunning = true;
    board.nextPiece();
}

void TrainerAI::endGame() {
    fitness = board.getScore() + board.getLevel() + board.getPieceCount() + board.getLines();
    score = board.getScore();
    board.clear();
    gameStateRunning = false;
}

void TrainerAI::perGeneration() {
//    population = myGenome.GetChromos();
    fitnessHighest = 0;
    for (int i = 0; i < populationSize; ++i) {
        Genome currentGenome = population[i];
        vector<double> weights = currentGenome.weights;
        myBrain.putWeight(weights);
        myAI.setNeuralNet(myBrain);
        
        startGame();
        while (true) {
            tick();
            if (gameStateRunning == false) {
                if (fitness > fitnessHighest) {
                    fitnessHighest = fitness;
                    bestGenome = currentGenome;
                }
                break;
            }
        }
        population = myGenome.setChromosFitnessEach(population, i, score);
        cout << "current geneme ID is " << i << " with fitness is " << fitness << " and true score is " << score << endl;
    }
    population = myGenome.Epoch(population);
    cout << "Highest score is: " << fitnessHighest << endl;
}

void TrainerAI::epoch(int numberOfCycle) {
    for (int i = 0; i < numberOfCycle; i++) {
        perGeneration();
        recordResult();
        generationCount += 1;
    }
}

void TrainerAI::epochSingle() {
    perGeneration();
    recordResult();
    generationCount += 1;

}

void TrainerAI::recordResult() {
    ofstream myRecord;
    myRecord.open("Population Record.csv", ios_base::app);
    myRecord << generationCount << ", " << fitnessHighest << ", ";
    
    vector<double> weights = bestGenome.weights;

    for (int i = 0; i < weights.size(); ++i) {
        myRecord << weights[i] << ", ";
    }
    myRecord << endl;
    myRecord.close();
}

void TrainerAI::debuggingMode(vector<double> weights) {
    for (int i = 0; i < weights.size(); ++i) {
        cout << weights[i] << ", ";
    }

}
