//
//  Observ.hpp
//  Tetris
//
//  Created by Tran Sam on 6/23/19.
//  Copyright © 2019 Tran Sam. All rights reserved.
//

#ifndef Observ_hpp
#define Observ_hpp

#include <stdio.h>
#pragma once
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual void update() = 0;
};

class Subject {
private:
    std::vector<Observer*> obs;
    
public:
    void attach(Observer* o) {
        obs.push_back(o);
    };
    
    void detach(Observer* o) {
        obs.erase(std::remove(obs.begin(), obs.end(), o), obs.end());
    };
    
    void notify() {
        for (std::vector<Observer*>::const_iterator iter = obs.begin(); iter != obs.end(); ++iter)
            if ((*iter) != 0)
                (*iter) -> update();
    };
};

#endif /* Observ_hpp */
