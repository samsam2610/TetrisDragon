//
//  triangle.c
//  Tetris
//
//  Created by Tran Sam on 7/1/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include <stdio.h>
#include "GUI.h"
#include "TrainerAI.hpp"


int main() {
    
    BoardGame b{10, 20};
    GUI g{ b };
    g.run();
    TrainerAI trainer{b, 300, 0.1, 0.5, 30};
//        (BoardGame & board, int populationSize, double rateMutation, double rateCrossover, int weightCount)
//    trainer.epoch(100);
    return 0;
}

