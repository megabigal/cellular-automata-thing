#ifndef LIFEGRID_H
#define LIFEGRID_H

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>
#include "rules.h" 
#include <iostream>
class lifeGrid {
private:
    int width, height;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> next; //holds the next iteration
    bool useMoore = true;
    bool wrapping = true;
public:
    
    lifeGrid(int w, int h);

    bool isMoore() const { return useMoore; }
    void toggleHood() { useMoore = !useMoore; }
    void setHood(std::string hood);
    void toggleWrapping() { wrapping = !wrapping; }

     int getWidth() const { return width; }
    int getHeight() const { return height; }
    inline int getIndex(int x, int y) const { return y * width + x; }

    uint8_t getCellState(int x, int y) const;
    uint8_t getCellStateWrapping(int x, int y) const;
    void setCell(int x, int y, uint8_t v);
    void clear();

    int countNeighboursVonNeumann(int x, int y) const;
    int countNeighbours(int x, int y) const;
    void update(basicAutomataRule* currentRule);

    void populate(float perc);
    void invert();
};

#endif