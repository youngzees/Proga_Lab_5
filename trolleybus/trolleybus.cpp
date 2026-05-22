#include "trolleybus.h"
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

static std::map<std::string, std::vector<std::string>> trolleybuses;

enum class Type {
    CREATE_TRL,
    TRLS_IN_STOP,
    STOPS_IN_TRL,
    TRLS,
    UNKNOWN
};

Type getCommandType(const std::string& cmd) {
    if (cmd == "CREATE_TRL") return Type::CREATE_TRL;
    if (cmd == "TRLS_IN_STOP") return Type::TRLS_IN_STOP;
    if (cmd == "STOPS_IN_TRL") return Type::STOPS_IN_TRL;
    if (cmd == "TRLS") return Type::TRLS;
    return Type::UNKNOWN;
}

void processCommand(const std::string& line) {
    std::istringstream iss(line);
    std::string cmdStr;
    iss >> cmdStr;
    
    Type cmd = getCommandType(cmdStr);
    
    switch (cmd) {
        case Type::CREATE_TRL: {
            std::string name;
            int stopCount;
            iss >> name >> stopCount;
            
            // Проверка на существование
            if (trolleybuses.find(name) != trolleybuses.end()) {
                std::cout << "Ошибка: Троллейбус с именем " << name << " уже создан" << std::endl;
                return;
            }
            
            // Проверка количества остановок
            if (stopCount < 2) {
                std::cout << "Ошибка: Троллейбус не может быть создан с " << stopCount << " остановками (нужно минимум 2)" << std::endl;
                return;
            }
            
            // Читаем остановки
            std::vector<std::string> stops;
            std::set<std::string> uniqueStops;
            for (int i = 0; i < stopCount; ++i) {
                std::string stop;
                iss >> stop;
                stops.push_back(stop);
                uniqueStops.insert(stop);
            }
            
            // Проверка на дубликаты (одинаковые остановки подряд или вообще)
            if (uniqueStops.size() != stops.size()) {
                std::cout << "Ошибка: Троллейбус не может быть создан с одинаковыми остановками" << std::endl;
                return;
            }
            
            // Всё ок, сохраняем
            trolleybuses[name] = stops;
            std::cout << "Троллейбус " << name << " создан" << std::endl;
            break;
        }
        
        case Type::TRLS_IN_STOP: {
            std::string stop;
            iss >> stop;
            
            std::vector<std::string> result;
            for (const auto& pair : trolleybuses) {
                for (const auto& s : pair.second) {
                    if (s == stop) {
                        result.push_back(pair.first);
                        break;
                    }
                }
            }
            
            if (result.empty()) {
                std::cout << "Ошибка: Остановка " << stop << " не найдена" << std::endl;
            } else {
                std::cout << "Троллейбус на остановке " << stop << ": ";
                for (size_t i = 0; i < result.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << result[i];
                }
                std::cout << std::endl;
            }
            break;
        }
        
        case Type::STOPS_IN_TRL: {
            std::string name;
            iss >> name;
            
            if (trolleybuses.find(name) == trolleybuses.end()) {
                std::cout << "Ошибка: Троллейбус " << name << " не найден" << std::endl;
                return;
            }
            
            const auto& stops = trolleybuses[name];
            
            std::cout << "Остановки троллейбуса " << name << ": ";
            for (size_t i = 0; i < stops.size(); ++i) {
                if (i > 0) std::cout << " ";
                std::cout << stops[i];
            }
            std::cout << std::endl;
            
            // Для каждой остановки выводим другие троллейбусы
            for (const auto& stop : stops) {
                std::vector<std::string> others;
                for (const auto& pair : trolleybuses) {
                    if (pair.first == name) continue;
                    for (const auto& s : pair.second) {
                        if (s == stop) {
                            others.push_back(pair.first);
                            break;
                        }
                    }
                }
                
                std::cout << "Остановка " << stop << ": ";
                if (others.empty()) {
                    std::cout << "-";
                } else {
                    for (size_t i = 0; i < others.size(); ++i) {
                        if (i > 0) std::cout << ", ";
                        std::cout << others[i];
                    }
                }
                std::cout << std::endl;
            }
            break;
        }
        
        case Type::TRLS: {
            if (trolleybuses.empty()) {
                std::cout << "Ошибка: Троллейбусы не найдены" << std::endl;
            } else {
                for (const auto& pair : trolleybuses) {
                    std::cout << "Троллейбус " << pair.first << ": ";
                    for (size_t i = 0; i < pair.second.size(); ++i) {
                        if (i > 0) std::cout << " ";
                        std::cout << pair.second[i];
                    }
                    std::cout << std::endl;
                }
            }
            break;
        }
        
        default:
            std::cout << "Неизвестная команда" << std::endl;
            break;
    }
}