#include "queue.h"
#include <iostream>
#include <string>
#include <sstream>

int main() {
    int windowsCount;
    std::cout << "Введите количество окон:" << std::endl;
    std::cout << ">>> ";
    std::cin >> windowsCount;
    std::cin.ignore(); // Очищаем буфер после cin
    
    int ticketCounter = 1;
    std::string line;
    
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, line);
        
        if (line == "DISTRIBUTE" || line == "distribute") {
            distribute(windowsCount);
            break;
        }
        
        // Парсим команду ENQUEUE
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        
        if (command == "ENQUEUE" || command == "enqueue") {
            int duration;
            iss >> duration;
            
            if (duration <= 0) {
                std::cout << "Ошибка: время должно быть положительным" << std::endl;
                continue;
            }
            
            // Формируем номер талона: T001, T002...
            std::string ticketNumber = "T" + 
                std::string(3 - std::to_string(ticketCounter).length(), '0') + 
                std::to_string(ticketCounter);
            
            enqueue(ticketNumber, duration);
            std::cout << "    " << ticketNumber << std::endl;
            
            ticketCounter++;
        }
        else {
            std::cout << "Неизвестная команда. Используйте ENQUEUE <время> или DISTRIBUTE" << std::endl;
        }
    }
    
    return 0;
}