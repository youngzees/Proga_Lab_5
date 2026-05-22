#include "warehouse.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Warehouse initWarehouse() {
    // Создаём 4D вектор, все ячейки пустые (productName = "", quantity = 0)
    Warehouse warehouse(ZONES, 
        std::vector<std::vector<std::vector<Cell>>>(SHELVES,
            std::vector<std::vector<Cell>>(SECTIONS,
                std::vector<Cell>(SHELVES_LEVELS, {"", 0})
            )
        )
    );
    return warehouse;
}

bool parseAddress(const std::string& address, int& zone, int& shelf, int& section, int& level) {
    if (address.length() < 5) return false;
    
    // Зона: первый символ (A, B, C...), для варианта 4 только A
    char zoneChar = address[0];
    if (zoneChar == 'A') zone = 0;
    else if (zoneChar == 'B') zone = 1;
    else if (zoneChar == 'C') zone = 2;
    else return false;
    
    if (zone < 0 || zone >= ZONES) return false;
    
    // Парсим остальные символы: стеллаж (2 цифры), секция (1), полка (1)
    // В примере A1739: A, потом 17 (2 цифры), 3 (1), 9 (1) → итого 4 цифры.
    if (address.length() != 5) return false; // A + 4 цифры = 5 символов
    
    try {
        shelf = std::stoi(address.substr(1, 2)) - 1;  // 17 → индекс 16
        section = std::stoi(address.substr(3, 1)) - 1; // 3 → индекс 2
        level = std::stoi(address.substr(4, 1)) - 1;
        // По варианту 4: полки 1..4, значит адрес должен быть A170304 (17 стеллаж, 03 секция, 04 полка)
    } catch (...) {
        return false;
    }
    
    // Проверка границ (нумерация с 1, переводим в индексы 0..N-1)
    if (shelf < 0 || shelf >= SHELVES) return false;
    if (section < 0 || section >= SECTIONS) return false;
    if (level < 0 || level >= SHELVES_LEVELS) return false;
    
    return true;
}

bool addProduct(Warehouse& warehouse, const std::string& productName, int quantity, const std::string& address) {
    int zone, shelf, section, level;
    if (!parseAddress(address, zone, shelf, section, level)) {
        std::cout << "Ошибка: Неверный адрес ячейки" << std::endl;
        return false;
    }
    
    Cell& cell = warehouse[zone][shelf][section][level];
    
    // Проверка: ячейка занята другим товаром
    if (cell.quantity > 0 && cell.productName != productName) {
        std::cout << "Ошибка: Ячейка " << address << " занята товаром " << cell.productName << std::endl;
        return false;
    }
    
    // Проверка лимита
    if (cell.quantity + quantity > MAX_CAPACITY) {
        std::cout << "Ошибка: Превышена вместимость ячейки (максимум " << MAX_CAPACITY << ")" << std::endl;
        return false;
    }
    
    // Добавляем
    if (cell.quantity == 0) {
        cell.productName = productName;
    }
    cell.quantity += quantity;
    
    std::cout << "Добавлено " << quantity << " " << productName << " в " << address << std::endl;
    return true;
}

bool removeProduct(Warehouse& warehouse, const std::string& productName, int quantity, const std::string& address) {
    int zone, shelf, section, level;
    if (!parseAddress(address, zone, shelf, section, level)) {
        std::cout << "Ошибка: Неверный адрес ячейки" << std::endl;
        return false;
    }
    
    Cell& cell = warehouse[zone][shelf][section][level];
    
    // Проверка: ячейка пуста
    if (cell.quantity == 0) {
        std::cout << "Ошибка: Товар " << productName << " не найден в ячейке " << address << std::endl;
        return false;
    }
    
    // Проверка: товар не совпадает
    if (cell.productName != productName) {
        std::cout << "Ошибка: Товар " << productName << " не найден в ячейке " << address << " (там " << cell.productName << ")" << std::endl;
        return false;
    }
    
    // Проверка: достаточно ли количества
    if (cell.quantity < quantity) {
        std::cout << "Ошибка: Недостаточно товаров для удаления (в ячейке " << cell.quantity << ")" << std::endl;
        return false;
    }
    
    // Удаляем
    cell.quantity -= quantity;
    int remaining = cell.quantity;
    
    if (cell.quantity == 0) {
        cell.productName = "";
        std::cout << "Удалено " << quantity << " " << productName << " (ячейка стала пустой)" << std::endl;
    } else {
        std::cout << "Удалено " << quantity << " " << productName << " (остаток: " << remaining << ")" << std::endl;
    }
    
    return true;
}

void printInfo(const Warehouse& warehouse) {
    // Общее количество ячеек
    int totalCells = ZONES * SHELVES * SECTIONS * SHELVES_LEVELS;
    int totalCapacity = totalCells * MAX_CAPACITY;
    
    // Считаем занятые единицы товара
    int totalOccupiedUnits = 0;
    
    // Для каждой зоны считаем занятые единицы
    std::vector<int> zoneOccupiedUnits(ZONES, 0);
    std::vector<int> zoneTotalCapacity(ZONES, 0);
    
    // Перебираем все ячейки (range-based for)
    for (int z = 0; z < ZONES; ++z) {
        int cellsInZone = SHELVES * SECTIONS * SHELVES_LEVELS;
        zoneTotalCapacity[z] = cellsInZone * MAX_CAPACITY;
        
        for (const auto& shelfVec : warehouse[z]) {
            for (const auto& sectionVec : shelfVec) {
                for (const auto& cell : sectionVec) {
                    if (cell.quantity > 0) {
                        zoneOccupiedUnits[z] += cell.quantity;
                        totalOccupiedUnits += cell.quantity;
                    }
                }
            }
        }
    }
    
    // Проценты
    double totalPercent = (totalCapacity > 0) ? (100.0 * totalOccupiedUnits / totalCapacity) : 0.0;
    
    // Вывод
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Загруженность склада: " << totalPercent << " %" << std::endl;
    
    // Зоны (для варианта 4 зона одна)
    for (int z = 0; z < ZONES; ++z) {
        double zonePercent = (zoneTotalCapacity[z] > 0) ? (100.0 * zoneOccupiedUnits[z] / zoneTotalCapacity[z]) : 0.0;
        char zoneLetter = 'A' + z;
        std::cout << "Загруженность зоны " << zoneLetter << ": " << zonePercent << " %" << std::endl;
    }
    
    // Заполненные ячейки
    std::cout << "\nЗаполненные ячейки:" << std::endl;
    bool hasFilled = false;
    
    for (int z = 0; z < ZONES; ++z) {
        char zoneLetter = 'A' + z;
        for (int shelf = 0; shelf < SHELVES; ++shelf) {
            for (int section = 0; section < SECTIONS; ++section) {
                for (int level = 0; level < SHELVES_LEVELS; ++level) {
                    const Cell& cell = warehouse[z][shelf][section][level];
                    if (cell.quantity > 0) {
                        hasFilled = true;
                        // Формируем адрес: зона + стеллаж(2 цифры) + секция(1) + полка(1)
                        std::cout << "  " << zoneLetter
                                  << std::setw(2) << std::setfill('0') << (shelf + 1)
                                  << (section + 1)
                                  << (level + 1)
                                  << ": " << cell.productName << " (" << cell.quantity << ")" << std::endl;
                    }
                }
            }
        }
    }
    
    if (!hasFilled) {
        std::cout << "  (нет заполненных ячеек)" << std::endl;
    }
    
    // Пустые ячейки
    std::cout << "\nПустые ячейки:" << std::endl;
    bool hasEmpty = false;
    int emptyCount = 0;
    const int MAX_EMPTY_DISPLAY = 20;
    
    for (int z = 0; z < ZONES; ++z) {
        char zoneLetter = 'A' + z;
        for (int shelf = 0; shelf < SHELVES; ++shelf) {
            for (int section = 0; section < SECTIONS; ++section) {
                for (int level = 0; level < SHELVES_LEVELS; ++level) {
                    const Cell& cell = warehouse[z][shelf][section][level];
                    if (cell.quantity == 0) {
                        emptyCount++;
                        if (emptyCount <= MAX_EMPTY_DISPLAY) {
                            hasEmpty = true;
                            std::cout << "  " << zoneLetter
                                      << std::setw(2) << std::setfill('0') << (shelf + 1)
                                      << (section + 1)
                                      << (level + 1);
                            if (emptyCount % 5 == 0) std::cout << std::endl;
                            else std::cout << ", ";
                        }
                    }
                }
            }
        }
    }
    
    if (emptyCount > MAX_EMPTY_DISPLAY) {
        std::cout << "... и ещё " << (emptyCount - MAX_EMPTY_DISPLAY) << " ячеек" << std::endl;
    } else if (emptyCount == 0) {
        std::cout << "  (нет пустых ячеек)" << std::endl;
    } else {
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}