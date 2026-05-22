#include "trolleybus.h"
#include <iostream>
#include <string>

int main() {
    std::string line;
    
    std::cout << "Система управления троллейбусами" << std::endl;
    std::cout << "Команды: CREATE_TRL, TRLS_IN_STOP, STOPS_IN_TRL, TRLS" << std::endl;
    
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, line);
        
        if (line == "EXIT" || line == "exit") {
            break;
        }
        
        processCommand(line);
    }
    
    return 0;
}