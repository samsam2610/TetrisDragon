//
//  GeneticAlgorithm.cpp
//  Tetris
//
//  Created by Tran Sam on 8/11/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include "GeneticAlgorithm.hpp"

#define MAX_PERTURBATION 0.1
#define NUM_COPIES_ELITE 30
#define NUM_ELITE 2

vector<Genome> GeneticAlgorithm::initializePopulation() {
    fitnessTotal = 0;
    generateCount = 0;
    genomeBest = 0;
    Reset();
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(Genome());
        
        for (int j = 0; j < chromosomeLength; ++j) {
            population[i].weights.push_back(RandomClamped());
        }
    }
    
    return population;
    
}

void GeneticAlgorithm::Mutate(vector<double> &chromo) {
    for (int i = 0; i < chromo.size(); ++i) {
        if (RandFloat() < rateMutation) {
            chromo[i] += RandomClamped() * MAX_PERTURBATION;
        }
    }
}

void GeneticAlgorithm::CalculateBestWorstAvTot() {
    fitnessTotal = 0;
    
    double tempHighestScore;
    double tempLowestScore;
    
    for (int i = 0; i < populationSize; ++i) {
        if (population[i].fitness > tempHighestScore) {
            tempHighestScore = population[i].fitness;
            genomeBest = i;
            fitnessBest = tempHighestScore;
        } else if(population[i].fitness < tempLowestScore) {
            tempLowestScore = population[i].fitness;
            fitnessWorst = tempLowestScore;
        }
        
        fitnessTotal += population[i].fitness;
    }
    
    fitnessAverage = fitnessTotal/populationSize;
}

Genome GeneticAlgorithm::GetChromoRoulette() {
    //generate a random number between 0 & total fitness count
    double Slice = (double)(RandFloat() * fitnessTotal);
    
    //this will be set to the chosen chromosome
    Genome TheChosenOne;

    //go through the chromosones adding up the fitness so far
    double FitnessSoFar = 0;
    
    for (int i=0; i < populationSize; ++i)
    {
        FitnessSoFar += population[i].fitness;
        
        //if the fitness so far > random number return the chromo at
        //this point
        if (FitnessSoFar >= Slice)
        {
            TheChosenOne = population[i];
            break;
        }
        
    }
    
    return TheChosenOne;
}

void GeneticAlgorithm::Crossover(const vector<double> &mum,
                       const vector<double> &dad,
                       vector<double>       &baby1,
                       vector<double>       &baby2) {
    //just return parents as offspring dependent on the rate
    //or if parents are the same
    if ((RandFloat() > rateCrossover) || (mum == dad)) {
        baby1 = mum;
        baby2 = dad;
        
        return;
    }
    
    //determine a crossover point
    int cp = RandInt(0, chromosomeLength - 1);
    
    //create the offspring
    for (int i = 0; i < cp; ++i) {
        baby1.push_back(mum[i]);
        baby2.push_back(dad[i]);
    }
    
    for (int i = cp; i < mum.size(); ++i) {
        baby1.push_back(dad[i]);
        baby2.push_back(mum[i]);
    }
    
    return;
}

void GeneticAlgorithm::GrabNBest(int              NBest,
                                 const int        NumCopies,
                                 vector<Genome>   &Pop) {
    //add the required amount of copies of the n most fittest
    //to the supplied vector
    while(NBest--)
    {
        for (int i=0; i < NumCopies; ++i)
        {
            Pop.push_back(population[(populationSize - 1) - NBest]);
        }
    }
}

vector<Genome> GeneticAlgorithm::Epoch(vector<Genome> &old_pop)
{
    //assign the given population to the classes population
    population = old_pop;
    
    //reset the appropriate variables
    Reset();
    
    //sort the population (for scaling and elitism)
    sort(population.begin(), population.end());
    
    //calculate best, worst, average and total fitness
    CalculateBestWorstAvTot();
    
    //create a temporary vector to store new chromosones
    vector <Genome> populationNewTemp;
    
    //Now to add a little elitism we shall add in some copies of the
    //fittest genomes. Make sure we add an EVEN number or the roulette
    //wheel sampling will crash
    if (!(NUM_COPIES_ELITE * NUM_ELITE % 2))
    {
        GrabNBest(NUM_ELITE, NUM_COPIES_ELITE, populationNewTemp);
    }
    
    
    //now we enter the GA loop
    
    //repeat until a new population is generated
    while (populationNewTemp.size() < populationSize)
    {
        //grab two chromosones
        Genome mum = GetChromoRoulette();
        Genome dad = GetChromoRoulette();
        
        //create some offspring via crossover
        vector<double>        baby1, baby2;
        
        Crossover(mum.weights, dad.weights, baby1, baby2);
        
        //now we mutate
        Mutate(baby1);
        Mutate(baby2);
        
        //now copy into vecNewPop population
        populationNewTemp.push_back(Genome(baby1, 0));
        populationNewTemp.push_back(Genome(baby2, 0));
    }
    
    //finished so assign new pop back into m_vecPop
    population = populationNewTemp;
    generateCount += 1;
    
    return population;
}

vector<Genome> GeneticAlgorithm::setChromosFitnessEach(vector<Genome> &current_pop, int chromosID, double fitness) {
    current_pop[chromosID].fitness = fitness;
    return current_pop;
}

void GeneticAlgorithm::Reset()
{
    fitnessTotal = 0;
    fitnessBest = 0;
    fitnessWorst = 9999999;
    fitnessAverage = 0;
}

