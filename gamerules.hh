#ifndef GAMERULES_HH
#define GAMERULES_HH
#include <string>
#include <card.hh>

namespace GameRules {

    bool sameSuitInOrder(std::string bottom, std::string top, const int count);
    bool differentColourInOrder(std::string bottom, std::string top, const int count);
}

void checkIfCompleted(std::string topSuit, std::string topValue);

#endif // GAMERULES_HH
