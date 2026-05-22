#include "students.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

static std::vector<std::string> allStudents;
static std::set<int> suspiciousList; // номера студентов (1-based)

void processCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "NEW_STUDENTS") {
        int number;
        iss >> number;
        
        if (number > 0) {
            // Добавляем новых студентов
            int startId = allStudents.size() + 1;
            for (int i = 0; i < number; ++i) {
                allStudents.push_back("Студент " + std::to_string(startId + i));
            }
            std::cout << "Добавлено " << number << " студентов" << std::endl;
        }
        else if (number < 0) {
            int toRemove = -number;
            if (suspiciousList.empty()) {
                std::cout << "Ошибка: Нет кандидатов на отчисление" << std::endl;
            }
            else if (suspiciousList.size() < toRemove) {
                std::cout << "Ошибка: невозможно отчислить больше студентов, чем есть их в списках на отчисление" << std::endl;
            }
            else {
                // Удаляем первых toRemove кандидатов
                auto it = suspiciousList.begin();
                for (int i = 0; i < toRemove; ++i) {
                    it = suspiciousList.erase(it);
                }
                std::cout << "Удалено " << toRemove << " студентов" << std::endl;
            }
        }
        else {
            std::cout << "Ошибка: неверное значение" << std::endl;
        }
    }
    else if (cmd == "SUSPICIOUS") {
        int studentNumber;
        iss >> studentNumber;
        
        // Проверка: существует ли студент
        if (studentNumber < 1 || studentNumber > (int)allStudents.size()) {
            std::cout << "Ошибка: Студент " << studentNumber << " не существует" << std::endl;
            return;
        }
        
        // Проверка: уже ли кандидат
        if (suspiciousList.find(studentNumber) != suspiciousList.end()) {
            std::cout << "Ошибка: Студент " << studentNumber << " уже является кандидатом на отчисление" << std::endl;
            return;
        }
        
        // Добавляем
        suspiciousList.insert(studentNumber);
        std::cout << "Студент " << studentNumber << " стал кандидатом на отчисление" << std::endl;
    }
    else if (cmd == "IMMORTAL") {
        int studentNumber;
        iss >> studentNumber;
        
        // Проверка: есть ли в кандидатах
        auto it = suspiciousList.find(studentNumber);
        if (it == suspiciousList.end()) {
            std::cout << "Ошибка: Студент " << studentNumber << " не может стать неприкасаемым, поскольку он не находится в списках на отчисление" << std::endl;
            return;
        }
        
        // Удаляем из кандидатов
        suspiciousList.erase(it);
        std::cout << "Студент " << studentNumber << " стал неприкасаемым" << std::endl;
    }
    else if (cmd == "TOP-LIST") {
        if (suspiciousList.empty()) {
            std::cout << "Список студентов на отчисление: " << std::endl;
        }
        else {
            std::cout << "Список студентов на отчисление: ";
            bool first = true;
            for (int id : suspiciousList) {
                if (!first) std::cout << ", ";
                std::cout << "Студент " << id;
                first = false;
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "Неизвестная команда" << std::endl;
    }
}

void clearData() {
    allStudents.clear();
    suspiciousList.clear();
}