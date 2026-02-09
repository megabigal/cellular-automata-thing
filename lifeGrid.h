/*#ifndef LIFEGRID_H  // Header Guard: Prevents double-loading
#define LIFEGRID_H

#include <vector>
#include <cstdint>
#include "rules.h"
class lifeGrid {
private:
    int width, height;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> next;

public:
    lifeGrid(int w, int h) : width{ w }, height{ h }, grid(w* h, 0), next(w* h, 0) {}
    int getIndex(int x, int y) const;
    uint8_t getCellState(int x, int y) const;
    int countNeighbours(int x, int y) const;
    void update(basicAutomataRule* currentRule);
    void setCell(int x, int y, uint8_t v);
    void populate(float perc);
    int getHeight() const;
    int getWidth() const;
};


#endif*/
#ifndef LIFEGRID_H
#define LIFEGRID_H

#include <vector>
#include <cstdint>
#include <cstdlib>
#include "rules.h" 

class lifeGrid {
private:
    int width, height;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> next; //holds the next iteration

public:
    lifeGrid(int w, int h) : width{ w }, height{ h },
        grid(w* h, 0), next(w* h, 0) {}//constructor

    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getIndex(int x, int y) const { return y * width + x; }

    uint8_t getCellState(int x, int y) const {//does boundary checking too
        if (x < 0 || x >= width || y < 0 || y >= height) return 0;
        return grid[getIndex(x, y)];
    }

    void setCell(int x, int y, uint8_t v) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            grid[getIndex(x, y)] = v;
    }

    void clear() {
        std::fill(grid.begin(), grid.end(), 0);
    }

    int countNeighbours(int x, int y) const {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue;//skip current
                count += (getCellState(x + j, y + i) & 1);
            }
        }
        return count;
    }

    void update(basicAutomataRule* currentRule) {
        if (!currentRule) return;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbors = countNeighbours(x, y);
                uint8_t currentState = getCellState(x, y);
                next[getIndex(x, y)] = currentRule->apply(currentState, neighbors); //calculates the new grid based off rule
            }
        }
        std::swap(grid, next); 
    }

    void populate(float perc) { //random population of the grid
        for (int i = 0; i < width * height; i++) {
            grid[i] = ((float)rand() / RAND_MAX < perc) ? 1 : 0;
        }
    }
    void invert() {
        for (int i = 0; i < width * height; i++) {
            grid[i] = 1 + -grid[i];
        }
    }
};

#endif