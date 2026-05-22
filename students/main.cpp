#include "students.h"
#include <iostream>
#include <string>

int main() {
    std::string line;
    
    clearData();
    
    std::cout << "Система учёта студентов" << std::endl;
    std::cout << "Команды: NEW_STUDENTS N, SUSPICIOUS N, IMMORTAL N, TOP-LIST" << std::endl;
    
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