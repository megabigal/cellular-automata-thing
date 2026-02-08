#include <iostream>
#include <vector>
#include <windows.h>
#include <cstdint>

class lifeGrid {
private:
    int width, height;
    std::vector<uint8_t> grid;
    std::vector<uint8_t> next;

public:
    lifeGrid(int w, int h) : width{ w }, height{ h }, 
    grid(w* h, 0), next(w* h, 0) {} //constructor

    int getIndex(int x, int y) const {
        return y * width + x;
    }

    //checks boundaries and returns the cell
    uint8_t getCellState(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) return 0;
        return grid[getIndex(x, y)];
    }

    int countNeighbours(int x, int y) const {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue; //skip self
                count += getCellState(x + j, y + i);
            }
        }
        return count;
    }
    void update() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbours = countNeighbours(x, y);
                int index = getIndex(x, y);

                if (grid[index] == 1) {
                    next[index] = (neighbours == 2 || neighbours == 3) ? 1 : 0;
                }
                else {
                    next[index] = (neighbours == 3) ? 1 : 0;
                }
            }
        }
        std::swap(grid, next);
    }

    void setCell(int x, int y, uint8_t v) {
        if (x >= 0 && x < width && y >= 0 && y < height)
            grid[getIndex(x, y)] = v;
    }

    void printGrid() {
        std::string output = "";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                output += (grid[getIndex(x, y)] ? "O" : ".");
            }
            output += "\n";
        }
        std::cout << output;
    }
};

int main() {
    lifeGrid gridMap(2000, 100);

    //glider
    gridMap.setCell(1, 0, 1);
    gridMap.setCell(2, 1, 1);
    gridMap.setCell(0, 2, 1);
    gridMap.setCell(1, 2, 1);
    gridMap.setCell(2, 2, 1);

    while (true) {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
        gridMap.printGrid();
        gridMap.update();
        Sleep(100);
    }
}