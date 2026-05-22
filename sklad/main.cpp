#include "warehouse.h"
#include <iostream>
#include <string>
#include <sstream>

int main() {
    Warehouse warehouse = initWarehouse();
    
    std::string line;
    std::cout << "Добро пожаловать в систему учёта склада (вариант 4)" << std::endl;
    std::cout << "Команды: ADD <товар> <количество> <адрес>" << std::endl;
    std::cout << "         REMOVE <товар> <количество> <адрес>" << std::endl;
    std::cout << "         INFO" << std::endl;
    std::cout << "         EXIT" << std::endl;
    
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, line);
        
        if (line == "EXIT" || line == "exit") {
            break;
        }
        
        if (line == "INFO" || line == "info") {
            printInfo(warehouse);
            continue;
        }
        
        // Парсим команду ADD или REMOVE
        std::istringstream iss(line);
        std::string command;
        iss >> command;
        
        if (command == "ADD" || command == "add") {
            std::string productName;
            int quantity;
            std::string address;
            
            iss >> productName >> quantity >> address;
            
            if (productName.empty() || quantity <= 0 || address.empty()) {
                std::cout << "Ошибка: Неверный формат команды. Используйте: ADD <товар> <количество> <адрес>" << std::endl;
                continue;
            }
            
            addProduct(warehouse, productName, quantity, address);
        }
        else if (command == "REMOVE" || command == "remove") {
            std::string productName;
            int quantity;
            std::string address;
            
            iss >> productName >> quantity >> address;
            
            if (productName.empty() || quantity <= 0 || address.empty()) {
                std::cout << "Ошибка: Неверный формат команды. Используйте: REMOVE <товар> <количество> <адрес>" << std::endl;
                continue;
            }
            
            removeProduct(warehouse, productName, quantity, address);
        }
        else {
            std::cout << "Неизвестная команда. Используйте ADD, REMOVE, INFO или EXIT" << std::endl;
        }
    }
    
    return 0;
}