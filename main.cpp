#include <iostream>
#include <string>
#include "mcts.h"

int main() {
    std::string i;
    while (std::getline(std::cin, i)) {
        if (i == "q") break;
        
        int s = std::stoi(i);
        int r = ex_mcts(s, 8); 
        
        std::cout << r << "\n";
        std::cout.flush();
    }
    return 0;
}