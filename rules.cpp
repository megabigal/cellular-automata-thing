#include "rules.h"

// conways
std::string ConwayRule::getName() const {
    return "Conway's Game of Life";
}

uint8_t ConwayRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) {
        return (neighbours == 2 || neighbours == 3) ? 1 : 0;
    }
    return (neighbours == 3) ? 1 : 0;
}
//seeds
std::string SeedsRule::getName() const {
    return "Seeds";
}

uint8_t SeedsRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 2) return 1;
    return 0;
}

// day and night
std::string DayAndNightRule::getName() const {
    return "Day And Night";
}
std::array<float, 3> DayAndNightRule::getDefaultColour(uint8_t state) {
    return { 1.0f, 0.863f, 0.0f };
}
uint8_t DayAndNightRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) {
        switch (neighbours) {
        case 3: case 4: case 6: case 7: case 8: return 1;
        default: return 0;
        }
    }
    switch (neighbours) {
    case 3: case 6: case 7: case 8: return 1;
    default: return 0;
    }
}
//life without death
std::string LifeWithoutDeathRule::getName() const { return "Life Without Death"; }
uint8_t LifeWithoutDeathRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 3) return 1;
    if (currentState == 1) return 1;
    return 0;
}
// brians brain
std::string BriansBrainRule::getName() const {
    return "Brian's Brain";
}

uint8_t BriansBrainRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 2) return 1;
    if (currentState == 1) return 2;//dying
    return 0;
}
std::array<float, 3> BriansBrainRule::getDefaultColour(uint8_t state) {
    switch (state) {
    case 1: return { 0.0f, 0.0f, 1.0f };
    case 2: return { 1.0f, 1.0f, 1.0f };
    default: return { 0.0,0.0f,1.0f };
	}
}
int BriansBrainRule::getMaxState() const {
    return 2;
}
// ulam-warburton
std::string UlamWarburtonRule::getName() const {
    return "Ulam-Warburton";
}

std::string UlamWarburtonRule::getDefaultHood() const {
    return "Neumann";
}

uint8_t UlamWarburtonRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) return 1;
    if (currentState == 0 && neighbours == 1) return 1;
    return 0;
    
}
//mazectric
std::string MazectricRule::getName() const {
    return "Mazectric";
}

uint8_t MazectricRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1 && neighbours > 0 && neighbours < 5) return 1;
    if (currentState == 0 && neighbours == 3) return 1;
    return 0;
}
//Iceballs
std::string IceballsRule::getName() const {
    return "Iceballs";
}
uint8_t IceballsRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) {
        switch (neighbours) {
        case 5: case 6: case 7: case 8: return 1;
        default: return 0;
        }
    }
    switch (neighbours) {
    case 2: case 5: case 6: case 7: case 8: return 1;
    default: return 0;
    }
}
std::array<float, 3> IceballsRule::getDefaultColour(uint8_t state) {
    return { 1.0f,1.0f,1.0f };
}
//star wars
std::string StarWarsRule::getName() const {
    return "Star Wars";
}

uint8_t  StarWarsRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours == 2) return 1;
    if (currentState == 1) {
        switch (neighbours) {
        case 3: case 4: case 5: return 1;
        default: return 2;
        }
    }
    if (currentState == 2)
        return 4;
    return 0;
}
std::array<float, 3>  StarWarsRule::getDefaultColour(uint8_t state) {
    switch (state) {
    case 1: return { 1.0f, 1.0f, 0.0f };
    case 2: return { 1.0f, 0.498f, 0.0f };
	case 4: return { 1.0f, 0.0f, 0.0f };
    default: return { 0.0,0.0f,1.0f };
    }
}
int StarWarsRule::getMaxState() const {
    return 4;
}
//greenberg hastings
std::string GreenbergHastingsRule::getName() const {
    return "Greenberg-Hastings";
}
uint8_t GreenbergHastingsRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0 && neighbours > 0) return 1;
    if (currentState == 1) return 2;
    if (currentState == 2) return 0;
    return 0;
}
std::array<float, 3> GreenbergHastingsRule::getDefaultColour(uint8_t state) {
    switch (state) {
    case 1: return { 0.0f, 1.0f, 0.0f };
    case 2: return { 0.0f, 0.0f, 1.0f };
    default: return { 1.0,0.0f,0.0f };
    }
}
std::string GreenbergHastingsRule::getDefaultHood() const {
    return "Neumann";
}
int GreenbergHastingsRule::getMaxState() const {
    return 2;
}
//Biham-Middleton-Levine traffic model
std::string BihamMiddletonLevineTrafficRule::getName() const {
    return "Biham–Middleton–Levine traffic Model";
}

uint8_t BihamMiddletonLevineTrafficRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) return 1;
    return 0;
}
std::array<float, 3> BihamMiddletonLevineTrafficRule::getDefaultColour(uint8_t state) {
    switch (state) {
    case 1: return { 0.0f, 0.0f, 1.0f };
    case 2: return { 1.0f,0.0f,0.0f };
    default: return { 0.0,0.0f,1.0f };
    }
}

int BihamMiddletonLevineTrafficRule::getMaxState() const {
    return 2;
}
//Nagel–Schreckenberg
std::string NagelSchreckenbergTrafficRule::getName() const {
    return "Nagel–Schreckenberg traffic Model";
}

uint8_t NagelSchreckenbergTrafficRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 1) return 1;
    return 0;
}
std::array<float, 3> NagelSchreckenbergTrafficRule::getDefaultColour(uint8_t state) {
    //return { 1.0f,1.0f,1.0f };
    switch (state) {
    case -1: return { 0.0f, 0.0f, 0.4f }; 
    case 1: return { 0.0f, 0.2f, 0.7f }; 
    case 2: return { 0.0f, 0.5f, 1.0f }; 
    case 3: return { 0.0f, 0.8f, 1.0f }; 
    case 4: return { 0.0f, 1.0f, 1.0f }; 
    default: return { 0.0f, 1.0f, 1.0f }; 
    }

}
int NagelSchreckenbergTrafficRule::getMaxState() const {
    return 5;
}



//wireworld
std::string WireWorldRule::getName() const {
    return "wire world";
}
int WireWorldRule::getMaxState() const {
    return 4;
}
uint8_t WireWorldRule::apply(uint8_t currentState, int neighbours) {
    if (currentState == 0) return 0;
    if (currentState == 1) return 2;
    if (currentState == 2) return 4;
    if (currentState == 4 && (neighbours == 2 || neighbours == 1)) return 1;
    else return 4;
    return 0;
}
std::array<float, 3> WireWorldRule::getDefaultColour(uint8_t state) {
    switch (state) {
    case 1: return { 0.0f, 0.6f, 1.0f }; // Electron Head: Bright Blue
    case 2: return { 1.0f, 0.2f, 0.2f }; // Electron Tail: Red
    case 4: return { 1.0f, 0.8f, 0.0f }; // Conductor: Gold/Yellow
    default: return { 0.0f, 0.0f, 0.0f }; // Empty: Black
    }

}
//Diffusion-Limited-Aggregation 
std::string DiffusionLimitedAggregationRule::getName() const {
    return "Diffusion-Limited-Aggregation";
}
int DiffusionLimitedAggregationRule::getMaxState() const {
    return 2;
}
uint8_t DiffusionLimitedAggregationRule::apply(uint8_t currentState, int neighbours) {
    return currentState;
}
std::array<float, 3> DiffusionLimitedAggregationRule::getDefaultColour(uint8_t state) {
    switch (state) {
    case 1: return { 0.0f, 0.6f, 1.0f }; // Electron Head: Bright Blue
    case 2: return { 1.0f, 0.2f, 0.2f }; // Electron Tail: Red
    case 4: return { 1.0f, 0.8f, 0.0f }; // Conductor: Gold/Yellow
    default: return { 0.0f, 0.0f, 0.0f }; // Empty: Black
    }

}