#include "engine.h"
#include <ctime>

int main() {
//seeding the rng, seed is calculated based of current time and is just casted so its an unsigned int
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    GameEngine engine(1200, 800);

    engine.run();

    return 0;
}