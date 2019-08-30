//
//  GeneticAlgorithm.hpp
//  Tetris
//
//  Created by Tran Sam on 8/11/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef GeneticAlgorithm_hpp
#define GeneticAlgorithm_hpp

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "utils.hpp"

using namespace std;

struct Genome {
    vector<double> weights;
    
    double  fitness;
    
    Genome(): fitness(0) {}
    
    Genome(vector<double> w, double f): weights(w), fitness(f) {};
    
    friend bool operator<(const Genome& leftHandSide, const Genome& rightHandSide)
    {
        return (leftHandSide.fitness < rightHandSide.fitness);
    }

};

class GeneticAlgorithm {
private:
    vector<Genome> population;
    
    int     populationSize;
    int     chromosomeLength;
    
    double  fitnessTotal;
    double  fitnessBest;
    double  fitnessAverage;
    double  fitnessWorst;
    int     genomeBest;
    
    double  rateMutation;
    double  rateCrossover;
    int     generateCount;
    
    void    Crossover(const vector<double> &mum,
                      const vector<double> &dad,
                      vector<double>       &baby1,
                      vector<double>       &baby2);
    
    
    void    Mutate(vector<double> &chromo);
    
    Genome  GetChromoRoulette();
    
    //use to introduce elitism
    void    GrabNBest(int             NBest,
                      const int       NumCopies,
                      vector<Genome>  &vecPop);
    
    
    void    CalculateBestWorstAvTot();
    
    void    Reset();
    
public:
    
    GeneticAlgorithm(int      populationSize,
                     double   rateMutation,
                     double   rateCrossover,
                     int      weightCount):
                                            populationSize { populationSize },
                                            rateMutation { rateMutation },
                                            rateCrossover { rateCrossover },
                                            chromosomeLength { weightCount } {};
    
    //this runs the GA for one generation.
    vector<Genome>  Epoch(vector<Genome> &old_pop);
    
    
    //-------------------accessor methods
    vector<Genome>  GetChromos() const { return population; };
    double          AverageFitness() const { return fitnessTotal / populationSize; };
    double          BestFitness() const { return fitnessBest; };
    int             getGenerationCount() const { return generateCount; };
    
    vector<Genome>  setChromosFitnessEach(vector<Genome> &current_pop, int chromosID, double fitness);
    
    vector<Genome>  initializePopulation();
    
    
};

#endif /* GeneticAlgorithm_hpp */
