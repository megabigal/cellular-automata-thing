#include "lifeGrid.h"
#include <algorithm>
#
lifeGrid::lifeGrid(int w, int h)
    : width{ w }, height{ h }, grid(w* h, 0), next(w* h, 0) {
} //constructor

void lifeGrid::setHood(std::string hood) {
    if (hood == "Moore") useMoore = true;
    else useMoore = false;
}

uint8_t lifeGrid::getCellState(int x, int y) const { //does boundary checking too
    
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;
    return grid[getIndex(x, y)];
}
uint8_t lifeGrid::getCellStateWrapping(int x, int y) const { //does boundary checking too
    
    int wrappedX = x;
    if (wrappedX < 0) wrappedX = width - 1;
    else if (wrappedX >= width) wrappedX = 0; //just in case they are negative
    int wrappedY = y;
    if (wrappedY < 0) wrappedY = height - 1;
    else if (wrappedY >= height) wrappedY = 0;
    return grid[getIndex(wrappedX, wrappedY)];
    
}

void lifeGrid::setCell(int x, int y, uint8_t v) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        grid[getIndex(x, y)] = v;
}

void lifeGrid::clear() {
    std::fill(grid.begin(), grid.end(), 0);
}
//doesnt check diagnols
int lifeGrid::countNeighboursVonNeumann(int x, int y) const {
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        if (i == 0) continue;
        if (wrapping) count += (getCellStateWrapping(x + i, y) & 1);
        else count += (getCellState(x + i, y ) & 1);
    }
    for (int j = -1; j <= 1; j++) {
        if (j == 0) continue; //skip current
        if (wrapping)
            count += (getCellStateWrapping(x, y + j) & 1);
        else
            count += (getCellState(x, y + j) & 1);
    }
    return count;
}
//checks tha diagnols
int lifeGrid::countNeighbours(int x, int y) const {
    int count = 0;
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            if (i == 0 && j == 0) continue; //skip current
            if (wrapping)
                count += (getCellStateWrapping(x + i, y + j) & 1);
            else count += (getCellState(x + i, y + j) & 1);
        }
    }
    return count;
}

void lifeGrid::update(basicAutomataRule* currentRule) {
    if (!currentRule) return;

    int neighbours = 0;
    int (lifeGrid:: * countFunction)(int, int) const;
    countFunction = (useMoore) ? &lifeGrid::countNeighbours : &lifeGrid::countNeighboursVonNeumann; //decides the algo
    if (wrapping) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbours = (this->*countFunction)(x, y);
                uint8_t currentState = getCellStateWrapping(x, y);
                next[getIndex(x, y)] = currentRule->apply(currentState, neighbours); //calculates the new grid based off rule
            }
        }
    }
    else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbours = (this->*countFunction)(x, y);
                uint8_t currentState = getCellState(x, y);
                next[getIndex(x, y)] = currentRule->apply(currentState, neighbours); //calculates the new grid based off rule
            }
        }
    }
    
    std::swap(grid, next);
}

void lifeGrid::populate(float perc) { //random population of the grid
    for (int i = 0; i < width * height; i++) {
        grid[i] = ((float)rand() / RAND_MAX < perc) ? 1 : 0;
    }
}

void lifeGrid::invert() {//woah...crazy...
    for (int i = 0; i < width * height; i++) {
        grid[i] = (grid[i] == 0) ? 1 : 0;
    }
}