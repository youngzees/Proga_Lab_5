#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <vector>

struct Cell {
    std::string productName;
    int quantity; // 0..10, 0 означает пустую ячейку
};

// Конфигурация склада (вариант 4)
const int ZONES = 1;
const int SHELVES = 12;      // стеллажи
const int SECTIONS = 7;      // вертикальные секции
const int SHELVES_LEVELS = 4; // полки
const int MAX_CAPACITY = 10;

// Тип склада: [зона][стеллаж][секция][полка]
using Warehouse = std::vector<std::vector<std::vector<std::vector<Cell>>>>;

// Инициализация склада (все ячейки пустые)
Warehouse initWarehouse();

// Преобразование адреса (например "A120304") в индексы (зона, стеллаж, секция, полка)
// Возвращает true, если адрес корректен
bool parseAddress(const std::string& address, int& zone, int& shelf, int& section, int& level);

// Добавление товара
bool addProduct(Warehouse& warehouse, const std::string& productName, int quantity, const std::string& address);

// Удаление товара
bool removeProduct(Warehouse& warehouse, const std::string& productName, int quantity, const std::string& address);

// Получение информации о складе
void printInfo(const Warehouse& warehouse);

#endif