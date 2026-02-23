#include "lifeGrid.h"
#include <algorithm>

lifeGrid::lifeGrid(int w, int h)
    : width{ w }, height{ h }, grid(w* h, 0), next(w* h, 0) {
} //constructor

void lifeGrid::setHood(std::string hood) {
    if (hood == "Moore") useMoore = true;
    else useMoore = false;
}

uint8_t lifeGrid::getCellState(int x, int y) const { //does boundary checking too
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;
    return grid[y * width + x];
}

uint8_t lifeGrid::getCellStateWrapping(int x, int y) const { //does boundary checking too
    int wrappedX = x;
    if (wrappedX < 0) wrappedX = width - 1;
    else if (wrappedX >= width) wrappedX = 0; //just in case they are negative
    int wrappedY = y;
    if (wrappedY < 0) wrappedY = height - 1;
    else if (wrappedY >= height) wrappedY = 0;
    return grid[wrappedY * width + wrappedX];
}

void lifeGrid::setCell(int x, int y, uint8_t v) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        grid[y * width + x] = v;
}

void lifeGrid::clear() {
    std::fill(grid.begin(), grid.end(), 0);
}

//doesnt check diagnols
int lifeGrid::countNeighboursVonNeumann(int x, int y) const {
    if (x > 0 && x < width - 1 && y >0 && y < height - 1) { //if we arent the boundary
        int index = y * width + x;
        const uint8_t* g = grid.data();
        const int w = width; //to stop it from constantly going tthrough this.width overhead
        return (g[index - w] & 1) + (g[index - 1] & 1) +
            (g[index + w] & 1) + (g[index + 1] & 1);
    }
    int count = 0;

    for (int i = -1; i <= 1; i++) {
        if (i == 0) continue;
        if (wrapping) count += (getCellStateWrapping(x + i, y) & 1);
        else count += (getCellState(x + i, y) & 1);
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
int lifeGrid::countNeighbours(int x, int y) const { //later we can make it this check is inside the update loop
    if (x > 0 && x < width - 1 && y >0 && y < height - 1) { //if we arent the boundary
        int index = y * width + x;
        const uint8_t* g = grid.data();
        const int w = width; //to stop it from constantly going tthrough this.width overhead
        return (g[index - w - 1] & 1) + (g[index - w] & 1) + (g[index - w + 1] & 1) +
            (g[index - 1] & 1) + (g[index + 1] & 1) +
            (g[index + w - 1] & 1) + (g[index + w] & 1) + (g[index + w + 1] & 1);
    }
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
    int (lifeGrid:: * countFunction)(int, int) const;
    countFunction = (useMoore) ? &lifeGrid::countNeighbours : &lifeGrid::countNeighboursVonNeumann; //decides the algo
    if (wrapping) {
        for (int y = 0; y < height; y++) {
            int currentY = y * width; //to save having to repeatedly compute this later
            for (int x = 0; x < width; x++) {
                int neighbours = (this->*countFunction)(x, y);
                uint8_t currentState = grid[currentY + x];
                next[currentY + x] = currentRule->apply(currentState, neighbours); //calculates the new grid based off rule
            }
        }
    }
    else {
        for (int y = 0; y < height; y++) {
            int currentY = y * width;
            for (int x = 0; x < width; x++) {
                int neighbours = (this->*countFunction)(x, y);
                uint8_t currentState = grid[currentY + x];
                next[currentY + x] = currentRule->apply(currentState, neighbours); //calculates the new grid based off rule
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
void BihamMiddletonLevineGrid::calcNext(int x, int y, bool goingRight) {
    int index = y * width + x;
    uint8_t cellValue = grid[index];
    if (goingRight) {
        if (cellValue == 1) { // car going right
            int targetIdx;
            if (x == width - 1) {
                targetIdx = y * width; // wrap to the left
            }
            else {
                targetIdx = index + 1;
            }

            // check if next spot occupied
            if (grid[targetIdx] == 0) {
                next[targetIdx] = 1;
            }
            else {
                next[index] = 1; // dont move
            }
        }
        else if (cellValue == 2) {
            next[index] = 2; // copy other car
        }
    }
    else {
        if (cellValue == 2) { // car going down
            int targetIdx;
            if (y == height - 1) {
                targetIdx = x; // wrap to the top
            }
            else {
                targetIdx = index + width;
            }

            if (grid[targetIdx] == 0) {
                next[targetIdx] = 2;
            }
            else {
                next[index] = 2;
            }
        }
        else if (cellValue == 1) {
            next[index] = 1; // copy other car
        }
    }
}

void BihamMiddletonLevineGrid::update(basicAutomataRule* currentRule) {
    if (!currentRule) return;
    std::fill(next.begin(), next.end(), 0);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            calcNext(x, y, true);
        }
    }
    std::swap(grid, next);
    std::fill(next.begin(), next.end(), 0);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            calcNext(x, y, false);
        }
    }
    std::swap(grid, next);
}
void BihamMiddletonLevineGrid::populate(float perc) {
    for (int i = 0; i < width * height; i++) {
        if ((float)rand() / RAND_MAX < perc)
            grid[i] = (rand() % 2 == 0) ? 1 : 2;
        else
            grid[i] = 0;
    }
   
}

// NagelSchreckenbergGrid
void NagelSchreckenbergGrid::calcNext(int x, int y) {
    
    int index = y * width + x;
    if (grid[index] == 0) //empty cell
        return;
    int velocity = grid[index];
    if (velocity < 0) //velocity is stored as -1 to differentiate it from empty cells
        velocity = 0;
    if (velocity < maxVelocity)
		velocity++; // accelerate
    
    for (int i = 1; i <= velocity; i++){ //check it does not pass the car in front
        int checkIndex = ((x + i) % width) + y * width;//wraps around
        if (grid[checkIndex] != 0) {
            velocity = i - 1;
            break;
        }
    }
    
    if (velocity > 0)
        velocity -= ((float)rand() / RAND_MAX < p) ? 1 : 0; //randomly slow down for some reason lol
    
    int nextIndex = ((x + velocity) % width) + y * width; 
    if (velocity == 0)
        next[nextIndex] = -1;
    else
        next[nextIndex] = velocity;
    
}


void NagelSchreckenbergGrid::update(basicAutomataRule* currentRule) {
    if (!currentRule) return;
    std::fill(next.begin(), next.end(), 0);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            calcNext(x, y);
        }
    }
    std::swap(grid, next);
    
}
void NagelSchreckenbergGrid::setMaxVelocity(int maxV) {
    maxVelocity = maxV;
}
//brownian grid
void BrownianGrid::update(basicAutomataRule* currentRule) {
    if (!currentRule) return;
   // std::fill(next.begin(), next.end(), 0);
    for (int i = activeWalkers.size()-1; i >= 0; i--) {
        for (int step = 0; step < 20; step++) {
            if (calcNext(i)) {
                //activeWalkers.erase(activeWalkers.begin() + i);
                activeWalkers[i] = activeWalkers.back();//deletes a walker, faster than eras ebecause thats o(n)
                activeWalkers.pop_back();
                if (activeWalkers.size() == 0)
                    addNew();
                break;
            }
        }
        
    }
    

    
    
}
uint8_t  BrownianGrid::calcNext(int walkerIndex) {
    Walker walker = activeWalkers[walkerIndex];
    int index = walker.y * width + walker.x;
    int w = width;
    if (walker.x > 0 && walker.x < w - 1 && walker.y > 0 && walker.y < height - 1) {
        if ((grid[index - w - 1] | grid[index - w] | grid[index - w + 1] |
            grid[index - 1] | grid[index + 1] |
            grid[index + w - 1] | grid[index + w] | grid[index + w + 1]) & 1)
        {
            grid[index] = 1; //crystalise
            return 1;
        }
    }
    int dir = rand() & 3;
    if (dir > 1) 
        walker.y += neumannOffsets[dir];
    else
        walker.x += neumannOffsets[dir];
    if (walker.x < 0) walker.x = width - 1;
    else if (walker.x >= width ) walker.x = 0;
    if (walker.y < 0) walker.y = height - 1;
    else if (walker.y >= height ) walker.y = 0;
    grid[index] = 0;

    grid[walker.y * width + walker.x] = 2;
    activeWalkers[walkerIndex] = walker;
    return 0;
}
void BrownianGrid::addNew() {
    Walker walker;

    while (true){
         walker.x= rand() % width;
        walker.y = rand() % height;
        if (grid[walker.y * width + walker.x] != 1) {
            grid[walker.y * width + walker.x] = 2;
            activeWalkers.push_back(walker);
            return;
        }
        
    }


}
void BrownianGrid::setCell(int x,int y,uint8_t v) {
    if (x >= 0 && x < width && y >= 0 && y < height)
    {//later add a check so only 1 gets added at a time
        if (v == 1) {
            grid[y * width + x] = v;
        }
        else if(v == 2) {
            Walker w = { x,y };
            grid[y * width + x] = v;
            activeWalkers.push_back(w);
        }
        else {
            grid[y * width + x] = v;
        }
    }
        
}
void BrownianGrid::clear() {
    activeWalkers.clear();
    std::fill(grid.begin(), grid.end(), 0);
}
void BrownianGrid::populate(float perc) {
    for (int y = 0; y <  height; y++) {
        int currentY = y * width;
        for (int x = 0; x < width; x++) {
            if (((float)rand() / RAND_MAX < perc) && grid[currentY+x] == 0) {
                Walker w = { x,y };
                grid[currentY + x] = 2;
                activeWalkers.push_back(w);

            }
        }
        
        
    }
}