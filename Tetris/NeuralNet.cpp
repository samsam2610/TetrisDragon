//
//  NeuralNet.cpp
//  Tetris
//
//  Created by Tran Sam on 8/10/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#define NUM_INPUTS 5
#define NUM_OUTPUTS 1
#define NUM_HIDDEN_LAYERS 2
#define NEURONS_PER_LAYER 3
#define DBIAS -1
#define ACTIVATION_RESPONSE 1

#include "NeuralNet.hpp"

Neuron::Neuron(int inputCount): inputPerNeuronCount(inputCount + 1) {
    for (int i = 0; i < inputCount; ++i) {
        inputWeightVect.push_back(RandomClamped());
    }
}

NeuronLayer::NeuronLayer(int neuronCount, int inputPerNeuronCount): neuronPerLayerCount(neuronCount) {
    for (int i = 0; i < neuronCount; ++i) {
        neuronLayerVect.push_back(Neuron(inputPerNeuronCount));
    }
}

NeuralNet::NeuralNet() {
    inputPerNeuronCount = NUM_INPUTS;
    outputCount = NUM_OUTPUTS;
    hiddenLayerCount = NUM_HIDDEN_LAYERS;
    neuronPerHiddenLayerCount = NEURONS_PER_LAYER;
    
    elementCount = NUM_HIDDEN_LAYERS*NEURONS_PER_LAYER*NUM_INPUTS;
    createNN();
}

void NeuralNet::createNN() {
    if (hiddenLayerCount > 0) {
        // create first layer
        layerVect.push_back(NeuronLayer(neuronPerHiddenLayerCount, inputPerNeuronCount));
    
        // create hidden layers
        for (int i = 0; i < hiddenLayerCount - 1; ++i) {
            layerVect.push_back(NeuronLayer(neuronPerHiddenLayerCount, neuronPerHiddenLayerCount));
        }
    
        // create output layer
        layerVect.push_back(NeuronLayer(outputCount, neuronPerHiddenLayerCount));
    } else {
        // create output layer
        layerVect.push_back(NeuronLayer(outputCount, inputPerNeuronCount));
    }
    
}

vector<double> NeuralNet::getWeightNN() const {
    vector<double> weights;
    
    // For each layer
    for (int i = 0; i < hiddenLayerCount + 1; ++i) {
        
        // for each neuron
        for (int j = 0; j < layerVect[i].neuronPerLayerCount; ++j) {
            
            // for each weight
            for (int k = 0; k < layerVect[i].neuronLayerVect[j].inputPerNeuronCount; ++k) {
                weights.push_back(layerVect[i].neuronLayerVect[j].inputWeightVect[k]);
            }
        }
    }
    
    return weights;
}

void NeuralNet::putWeight(vector<double> &weights) {
    int cWeight = 0;
    
    // For each layer
    for (int i = 0; i < hiddenLayerCount + 1; ++i) {
        
        // for each neuron
        for (int j = 0; j < layerVect[i].neuronPerLayerCount; ++j) {
            
            // for each weight
            for (int k = 0; k < layerVect[i].neuronLayerVect[j].inputPerNeuronCount; ++k) {
                layerVect[i].neuronLayerVect[j].inputWeightVect[k] = weights[cWeight++];
            }
        }
    }
    return;
}

int NeuralNet::getWeightCount() const {
    int weightCount = 0;
    
    // For each layer
    for (int i = 0; i < hiddenLayerCount + 1; ++i) {
        
        // for each neuron
        for (int j = 0; j < layerVect[i].neuronPerLayerCount; ++j) {
            
            // for each weight
            for (int k = 0; k < layerVect[i].neuronLayerVect[j].inputPerNeuronCount; ++k) {
                weightCount++;
            }
        }
    }
    
    return weightCount;
}

vector<double> NeuralNet::calculateOutput(vector<double> &inputs) {
    vector<double> outputs;
    
    int weightCount = 0;
    
    if (inputs.size() != inputPerNeuronCount) {
        return outputs;
    }
    
    for (int i = 0; i < hiddenLayerCount + 1; ++i) {
        
        if (i > 0) {
            inputs = outputs;
        }

        outputs.clear();
        
        weightCount = 0;
        
        for (int j = 0; j < layerVect[i].neuronPerLayerCount; ++j) {
            double inputNet = 0;
            
            double inputCount = layerVect[i].neuronLayerVect[j].inputPerNeuronCount;
            
            
            for (int k = 0; k < inputCount - 1; ++k) {
                inputNet += layerVect[i].neuronLayerVect[j].inputWeightVect[k] * inputs[weightCount++];
            }
            
            inputNet +=layerVect[i].neuronLayerVect[j].inputWeightVect[inputCount - 1]*DBIAS;
            outputs.push_back(Sigmoid(inputNet, ACTIVATION_RESPONSE));
            
            weightCount = 0;
            
        }
    }
    
    return outputs;
    
}

double NeuralNet::Sigmoid(double netinput, double response)
{
    return ( 1 / ( 1 + exp(-netinput / response)));
}
