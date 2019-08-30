//
//  GUI.cpp
//  Tetris
//
//  Created by Tran Sam on 7/1/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#include "GUI.h"

static SDL_Surface *mScreen;                                        // Screen

void GUI::keyPressEvent(SDL_Event& event) {
    int pieceVelocity = 0;
    switch(event.type) {
        case SDL_KEYDOWN:
            try {
                if (gameStateRunning) {
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        board.movePieceLeft();
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_RIGHT) {
                        board.movePieceRight();
                        break;
                    }
                   if (event.key.keysym.sym == SDLK_UP) {
                        board.movePieceDown();
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_DOWN) {
                        board.rotatePiece();
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_RSHIFT) {
                        board.setLockDelay(5);
                        break;
                    }
                    
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        board.instantDrop();
                        break;
                    }
                    

                } else if(gameStateRunning == false) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        startGame();
                        break;
                    }
                    if (event.key.keysym.sym == SDLK_CAPSLOCK) {
                        gameStateAI = true;
                        
                        if (gameModeAI == 2) {
                            neuralBrain.createNN();
                            myAI.setNeuralNet(neuralBrain);
                        }
                        
                        startGame();
                        break;
                    }

                }
            } catch(PlaceException e) {
                
                endGame();
            }
            default:
                break;

    }
    
//    drawBoard();
    
}

void GUI::graphics_init(int x, int y) {
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1) {
    }
    if(SDL_CreateWindowAndRenderer(x, y, 0, &window, &renderer) == -1) {
        printf("Could not create window: %s\n", SDL_GetError());
    }
    SDL_SetWindowTitle(window, "Tetris");
    if(TTF_Init()==-1) {
        ;
    }
}

void GUI::graphics_draw_rect(int x, int y, int w, int h) {
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);
}

void GUI::graphics_draw_texture(SDL_Texture *texture, int x, int y) {
    SDL_Rect drect = { x, y, 0, 0 };
    SDL_QueryTexture(texture, NULL, NULL, &drect.w, &drect.h);
    SDL_RenderCopy(renderer, texture, NULL, &drect);
}

void GUI::graphic_draw_circle(SDL_Point center, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}

void GUI::graphics_draw_text(std::string inputText, SDL_Color textColor, int x, int y) {
    
    SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, inputText.c_str(), textColor);
    
    if (textSurface == NULL) {
        fprintf(stderr,
                "\nTTF_RenderText_Solid Error:  %s\n",
                SDL_GetError());
        exit(1);
    }
    
    SDL_Texture* mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    if (mTexture == NULL) {
        fprintf(stderr,
                "\nSDL_CreateTextureFromSurface Error:  %s\n",
                SDL_GetError());
        exit(1);
    }
    
    int mWidth = textSurface->w;
    int mHeight = textSurface->h;
    
    // render text
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    
    SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
    
    SDL_DestroyTexture(mTexture);
    SDL_FreeSurface(textSurface);
}

void GUI::drawStatistics(std::string nameStatistic, std::string valueStatistic, SDL_Color textColor, int x, int y) {
    
    graphics_draw_text(nameStatistic, textColor, x, y);
    
    graphics_draw_text(valueStatistic, textColor, x, y + 25);
    
}


void GUI::graphics_quit() {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GUI::tick() {
    
    try {
        if (gameStateRunning) {
            board.tick();
            if (gameStateAI) {
                myAI.findMoveSet(pboard);
                myAI.makeMoves();
                updatePileDelta();
                updatePileStats();

            }
        }
        
        
        
    }
    catch (PlaceException e) {
        endGame();
    }
    
    drawBoard();
    return 0;
    
}

void GUI::initComp() {
    colorTable.insert(pair<char, vector<int>>('I', vector<int>{20, 20, 175}));
    colorTable.insert(pair<char, vector<int>>('J', vector<int>{0, 0, 155}));
    colorTable.insert(pair<char, vector<int>>('L', vector<int>{237, 121, 48}));
    colorTable.insert(pair<char, vector<int>>('O', vector<int>{246, 209, 49}));
    colorTable.insert(pair<char, vector<int>>('S', vector<int>{0, 155, 0}));
    colorTable.insert(pair<char, vector<int>>('T', vector<int>{175, 20, 20}));
    colorTable.insert(pair<char, vector<int>>('Z', vector<int>{155, 0, 0}));
    colorTable.insert(pair<char, vector<int>>('M', vector<int>{0, 0, 0}));

    fontPath = string(base_path) + fontPath;

    pboard = &board;
    Pile p = board.getPile();
    graphics_init(board.getWidth()*BLOCK_SIZE + EXTRA_WIDTH, board.getHeight()*BLOCK_SIZE + EXTRA_HEIGHT);
    gFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    SDL_Delay(16);
    
    inputNN_Current = p.getSetFeatures();
    inputNN_Before = p.getSetFeatures();
    updatePileDelta();
    //    drawBoard();
}

void GUI::run() {
    bool is_running = true;
    timeFrame = SDL_GetTicks();
    while (is_running) {
        while (SDL_PollEvent(&mainEvent)) {
            if (mainEvent.type == SDL_QUIT) {
                is_running = false;
            } else {
                keyPressEvent(mainEvent);
            }
        }
        if ((SDL_GetTicks() - timeGame) >= 200) {
            tick();
            timeGame = SDL_GetTicks();
        }
        if ((SDL_GetTicks() - timeFrame) >= 17) {
            SDL_RenderClear(renderer);
            drawBoard();
            SDL_RenderPresent(renderer);
            timeFrame = SDL_GetTicks();
        } else {
            SDL_Delay(17);
        }
        if (gameModeAI == 3 && gameStateRunning == false) {
            gameStateAI = true;
            trainer.epochSingle();
            
            currentGenome = trainer.getBestGenome();
            vector<double> weights = currentGenome.weights;
            neuralBrain.createNN();
            neuralBrain.putWeight(weights);
            myAI.setNeuralNet(neuralBrain);
            gameStateRunning = true;
            startGame();
        }

    }

}

void GUI::startGame() {
    board.clear();
    gameStateRunning = true;
    timeGame = SDL_GetTicks();
    board.nextPiece();
}

void GUI::endGame() {
    board.clear();
    gameStateRunning = false;
}

void GUI::drawBoard() {
    
    Pile p = board.getPile();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < board.getHeight(); i++) {
        for (int j = 0; j < board.getWidth(); j++) {
            graphics_draw_rect(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
        }
    }
    graphics_draw_rect((board.getWidth() * BLOCK_SIZE + 1), 0, EXTRA_WIDTH, board.getHeight() * BLOCK_SIZE);
    
    SDL_Color textColor = { 0x11, 0x1F, 0x3F };
    string displayTextScore = "Next piece:";
    graphics_draw_text(displayTextScore, textColor, (board.getWidth() * BLOCK_SIZE + 10), 60);
    
    Piece currentPiece = board.getPiece();
    Piece followingPiece = board.getFollowingPiece();
    if (gameStateRunning) {
        drawPiece(currentPiece, 0, 0, 'c');
        drawPiece(followingPiece, (board.getWidth() * BLOCK_SIZE + 20), 95, 'f');
        drawPile();
        drawScore();

    }
    
    if (gameStateAI) {
        drawNeuralNet(neuralBrain.getInputCount(),
                      neuralBrain.getLayerCount(),
                      neuralBrain.getNeuronPerLayerCount(),
                      neuralBrain.getOutputCount(),
                      neuralBrain.getLayerVect());
    }
    drawGrid();
    
}

void GUI::drawPiece(Piece p, int offsetX, int offsetY, char pieceType) {
    
//    Piece p = board.getPiece();
    auto shape = p.getShape();
    char shapeName = p.getName();
//    printf("Shape name: %c\n", shapeName);
    
    
    if (shapeName != '\0') {
        auto vectorColor = colorTable[shapeName];
//        cout << shapeName << endl;
        SDL_SetRenderDrawColor(renderer, vectorColor[0], vectorColor[1], vectorColor[2], 255);
    }
    
    for (int i = 0; i < shape.size(); i++)
        for (int j = 0; j < shape[i].size(); j++)
            if (shape[i][j] > 0 && p.getY() + i >= 0 && p.getX() + j >= 0) {
                graphics_draw_rect((p.getX() + j)*BLOCK_SIZE + offsetX, (p.getY() + i)*BLOCK_SIZE + offsetY, BLOCK_SIZE, BLOCK_SIZE);
                
            } else if (shape[i][j] > 0 && pieceType == 'f') {
                graphics_draw_rect(j*BLOCK_SIZE + offsetX, i*BLOCK_SIZE + offsetY, BLOCK_SIZE, BLOCK_SIZE);

            }
    
}

void GUI::drawPile() {
    
    Pile p = board.getPile();
    auto pile = p.getPile();
    
    for (int i = 0; i < pile.size(); i++)
        for (int j = 0; j < pile[i].size(); j++)
            if (pile[i][j] > 0) {
                SDL_SetRenderDrawColor(renderer, 206, 206, 206, 255);
                graphics_draw_rect(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            }
    
}

void GUI::drawScore() {
    // Show tetris score after all tetris operations are finished
    SDL_Color textColor = { 0x11, 0x1F, 0x3F };
    
    Pile p = board.getPile();
    
    string string_score = to_string(board.getScore());
    string string_numberHole = to_string(p.getValueHole());
    string string_numberHeight = to_string(p.getValueHeightSum());
    string string_numberBumpy = to_string(p.getValueBumpy());
    string string_numberBlockage = to_string(p.getValueBlockage());
    string string_numbergeneration = to_string(trainer.getGenerationCount());
//    graphics_draw_text(string_score, textColor, (board.getWidth() * BLOCK_SIZE + 20), 35);
    
    string displayTextScore = "Score:";
    string displayTextHole = "Number of holes:";
    string displayTextBumpy = "Pile's bumpiness";
    string displayTextHeight = "Current height sum:";
    string displayTextBlockage = "Current blockage:";
    string displayTextGeneration = "Current Generation:";
//    graphics_draw_text(displayTextScore, textColor, (board.getWidth() * BLOCK_SIZE + 20), 10);
    int textLocation = (board.getWidth() * BLOCK_SIZE + 10);
    drawStatistics(displayTextScore, string_score, textColor, textLocation, 10);
    drawStatistics(displayTextHole, string_numberHole, textColor, textLocation, 180);
    drawStatistics(displayTextBumpy, string_numberBumpy, textColor, textLocation, 230);
    drawStatistics(displayTextHeight, string_numberHeight, textColor, textLocation, 280);
    drawStatistics(displayTextBlockage, string_numberBlockage, textColor, textLocation, 330);
    drawStatistics(displayTextGeneration, string_numbergeneration, textColor, (board.getWidth() * BLOCK_SIZE + 320), 330);

}

void GUI::drawNeuralNet(int countInput, int countHiddenLayer, int countNeuronPerLayer, int countOutput, vector<NeuronLayer> layerVect) {
    int neuralLocation_X = (board.getWidth() * BLOCK_SIZE + 330);
    int neuralLocation_Y = 150;
    
    int spacing_X = 100;
    int spacing_Y = 60;
    int radius = 15;
    SDL_Color   circleColor = { 0x11, 0x1F, 0x3F };
    SDL_Color   circleOutputColor;
    SDL_Color   circleInputColor;


    int halfCountInput = 1;
    int halfCountHidden = 1;
    int halfCountOutput = 1;
    
    if (countInput > 1)
        halfCountInput = floor(countInput/2);
    
    if (countNeuronPerLayer > 1)
        halfCountHidden = floor(countNeuronPerLayer/2);
    
    if (countOutput > 1)
        halfCountOutput = floor(countOutput/2);
    
    SDL_Point centerInput;
    SDL_Point centerHidden;
    SDL_Point centerHidden_next;
    SDL_Point centerOutput;
    int startInput_Y = neuralLocation_Y - halfCountInput*spacing_Y;
    int startHidden_Y = neuralLocation_Y - halfCountHidden*spacing_Y;
    int startOutput_Y = neuralLocation_Y;
    
    double currentWeight;
    
    // Draw neurons in input layer
    centerInput.x = neuralLocation_X;
    for (int i = 0; i < countInput; ++i) {
        centerInput.y = startInput_Y + i*spacing_Y;
        if (inputDelta[i] > 0) {
            circleInputColor = { 245, 66, 66 };
        } else if (inputDelta[i] < 0) {
            circleInputColor = { 66, 245, 69 };
        } else if (inputDelta[i] == 0) {
            circleInputColor = { 0x11, 0x1F, 0x3F };
        }
        graphic_draw_circle(centerInput, radius, circleInputColor);
        
        centerHidden.x = neuralLocation_X + spacing_X;
        for (int j = 0; j < layerVect[0].neuronPerLayerCount; ++j) {
            centerHidden.y = startHidden_Y + j*spacing_Y;
            currentWeight = layerVect[0].neuronLayerVect[j].inputWeightVect[i];
            if (currentWeight > 0) {
                SDL_SetRenderDrawColor(renderer, 16, 46, 235, 255);
            } else if (currentWeight < 0) {
                SDL_SetRenderDrawColor(renderer, 252, 3, 82, 255);
            }
            SDL_RenderDrawLine(renderer, centerInput.x, centerInput.y, centerHidden.x, centerHidden.y);
            SDL_SetRenderDrawColor(renderer, 206, 206, 206, 255);
        }
    }
    
    // Draw neurons in hidden layers
    centerOutput.x = neuralLocation_X + countHiddenLayer*spacing_X + spacing_X;
    for (int i = 0; i < countOutput; ++i) {
        centerOutput.y = startOutput_Y + i*spacing_Y;
        double scorePile = myAI.getScorePile();
        
        if (scorePile_Delta > 0) {
            circleOutputColor = { 66, 245, 69 };
        } else if (scorePile_Delta < 0) {
            circleOutputColor = { 245, 66, 66 };
        } else if (scorePile_Delta == 0) {
             circleOutputColor = { 0x11, 0x1F, 0x3F };
        }
        graphic_draw_circle(centerOutput, radius, circleOutputColor);
    }
    
    // Draw neurons in output layer
    for (int i = 0; i < countHiddenLayer; ++i ){
        centerHidden.x = neuralLocation_X + (i + 1)*spacing_X;
        for (int j = 0; j < countNeuronPerLayer; ++j){
            centerHidden.y = startHidden_Y + j*spacing_Y;
            graphic_draw_circle(centerHidden, radius, circleColor);
            
            if ((i + 1 < countHiddenLayer)) {
                centerHidden_next.x = neuralLocation_X + (i + 2)*spacing_X;
                int ii = i + 1;
                for (int k = 0; k < layerVect[ii].neuronPerLayerCount; ++k) {
                    currentWeight = layerVect[ii].neuronLayerVect[k].inputWeightVect[j];
                    if (currentWeight > 0) {
                        SDL_SetRenderDrawColor(renderer, 16, 46, 235, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 252, 3, 82, 255);
                    }
                    centerHidden_next.y = startHidden_Y + k*spacing_Y;
                    SDL_RenderDrawLine(renderer, centerHidden.x, centerHidden.y, centerHidden_next.x, centerHidden_next.y);
                    SDL_SetRenderDrawColor(renderer, 206, 206, 206, 255);
                }
            } else {
                for (int k = 0; k < countOutput; ++k){
                    currentWeight = layerVect[countHiddenLayer].neuronLayerVect[k].inputWeightVect[j];
                    if (currentWeight > 0) {
                        SDL_SetRenderDrawColor(renderer, 16, 46, 235, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 252, 3, 82, 255);
                    }
                    centerOutput.y = startOutput_Y + k*spacing_Y;
                    SDL_RenderDrawLine(renderer, centerHidden.x, centerHidden.y, centerOutput.x, centerOutput.y);
                }
            }
        }
        
    }



    
}

void GUI::drawGrid() {
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 150);
    for (int i = 0; i < board.getHeight(); i++) {
        SDL_RenderDrawLine(renderer, 0, i * BLOCK_SIZE, board.getWidth() * BLOCK_SIZE , i * BLOCK_SIZE);
    }
    
    for (int j = 0; j < board.getWidth(); j++) {
        SDL_RenderDrawLine(renderer, j * BLOCK_SIZE, 0 , j * BLOCK_SIZE, board.getHeight() * BLOCK_SIZE);
    }

}

void GUI::updatePileDelta() {
    inputDelta.clear();
    Pile p = board.getPile();
    inputNN_Current = p.getSetFeatures();
    for (int i = 0; i < inputNN_Current.size(); ++i) {
        double currentDelta = inputNN_Current[i] - inputNN_Before[i];
        inputDelta.push_back(currentDelta);
    }
    scorePile_Delta = scorePile_Before - scorePile_Current;
}

void GUI::updatePileStats() {
    Pile p = board.getPile();
    for (int i = 0; i < inputNN_Current.size(); ++i) {
        if (inputNN_Current[i] != inputNN_Before[i]) {
            inputNN_Before[i] = inputNN_Current[i];
        }
    }
    scorePile_Before = scorePile_Current;

    inputNN_Current = p.getSetFeatures();
    scorePile_Current = myAI.getScorePile();
}
