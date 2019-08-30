//
//  GUI.hpp
//  Tetris
//
//  Created by Tran Sam on 7/1/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef GUI_h
#define GUI_h

#define BLOCK_SIZE 20
#define EXTRA_WIDTH 700
#define EXTRA_HEIGHT 0
#define STAGE_X 6 * BLOCK_SIZE
#define STAGE_Y 2 * BLOCK_SIZE
#define SCREEN_W 22 * BLOCK_SIZE
#define SCREEN_H 24 * BLOCK_SIZE
#define QUEUE_X 17 * BLOCK_SIZE
#define QUEUE_Y 2 * BLOCK_SIZE
#define HOLD_X 1 * BLOCK_SIZE
#define HOLD_Y 2 * BLOCK_SIZE

#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include "Boardgame.hpp"
#include "GameAI.hpp"
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <iostream>
#include "ResourcePath.hpp"
#include "NeuralNet.hpp"
#include "GeneticAlgorithm.hpp"
#include "TrainerAI.hpp"
using namespace std;


class GUI {
private:
    // Bucket size
    BoardGame & board;
    
    BoardGame *pboard;
    int     blockSize;
    int     text_count = 0;
    bool    gameStateRunning = false;
    bool    gameStateAI = false;
    int     gameModeAI = 3;
    Uint32  timeFrame = 0;
    Uint32  timeGame = 0;
    SDL_Event mainEvent;
    NeuralNet neuralBrain;
    GameAI  myAI { board, gameModeAI };
    
    Genome  currentGenome;
    
    vector<double>  inputNN_Current;
    vector<double>  inputNN_Before;
    vector<double>  inputDelta;
    
    double scorePile_Current = 0;
    double scorePile_Before = 0;
    double scorePile_Delta = 0;
    
    TrainerAI trainer{board, 2000, 0.1, 0.8, neuralBrain.getWeightCount()};
    
    char    *base_path = SDL_GetBasePath();
    string  fontPath = "Inconsolata-Regular.ttf";
    int     fontSize = 30;
    TTF_Font* gFont; 


    
    long frameTime;
    
    // Draw Graphic
    void    graphics_init(int x, int y);
    void    graphics_quit();
    void    graphics_set_color(unsigned int color);
    void    graphics_draw_rect(int x, int y, int w, int h);
    void    graphics_draw_texture(SDL_Texture *texture, int x, int y);
    void    graphics_draw_int(int n, int x, int y);
    void    graphics_draw_text(std::string inputText, SDL_Color textColor, int x, int y);
    void    graphic_draw_circle(SDL_Point center, int radius, SDL_Color color);
    void    drawStatistics(std::string nameStatistic, std::string valueStatistic, SDL_Color textColor, int x, int y);
    
    // Game continuos
    void    tick();
    void    initComp();
    void    initSlots();
    void    keyPressEvent(SDL_Event& keyevent);
    
    void    updatePileStats();
    void    updatePileDelta();
    



    
public:
    GUI(BoardGame & board) : board{ board } {
        initComp();
    }
    void    startGame();
    void    endGame();
    
    // Draw Game
    void    drawBoard();
    void    drawPiece(Piece p, int offsetX, int offsetY, char pieceType);
    void    drawPile();
    void    drawScore();
    void    drawGrid();
    void    drawNeuralNet(int countInput, int countLayer, int countNeuronPerLayer, int countOutput, vector<NeuronLayer> layerVect);

    void    run();
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    // Color map
    map<char, vector<int>> colorTable;
    
};
#endif /* GUI_hpp */
