//
//  NeuralNet.hpp
//  Tetris
//
//  Created by Tran Sam on 8/10/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef NeuralNet_hpp
#define NeuralNet_hpp

#include <stdio.h>
#include <vector>
#include <math.h>
#include "utils.hpp"

using namespace std;

struct Neuron {
    int inputPerNeuronCount;
    
    vector<double>  inputWeightVect;
    
    Neuron(int inputCount);
};

struct NeuronLayer {
    int neuronPerLayerCount;
    
    vector<Neuron> neuronLayerVect;
    
    NeuronLayer(int neuronCount, int inputPerNeuronCount);
};

class NeuralNet {
private:
    int     inputPerNeuronCount;
    int     outputCount;
    int     hiddenLayerCount;
    int     neuronPerHiddenLayerCount;
    int     elementCount;
    
    vector<NeuronLayer> layerVect;
    
public:
    NeuralNet();
    
    void    createNN();
    
    vector<double> getWeightNN() const;
    
    int     getWeightCount() const;
    int     getInputCount() const { return inputPerNeuronCount; };
    int     getOutputCount() const { return outputCount; };
    int     getLayerCount() const { return hiddenLayerCount; };
    int     getNeuronPerLayerCount() const { return neuronPerHiddenLayerCount; };
    int     getElementCount() const { return elementCount; };
    vector<NeuronLayer> getLayerVect() const { return layerVect; };
    
    void    putWeight(vector<double> &weights);
    
    vector<double> calculateOutput(vector<double> &inputs);
    
    inline double Sigmoid(double activation, double response);
    
};

#endif /* NeuralNet_hpp */
