#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <vector>

struct Visitor {
    std::string ticketNumber;
    int duration;
};

// Добавление посетителя в очередь
void enqueue(const std::string& ticketNumber, int duration);

// Распределение и вывод результатов
void distribute(int windowsCount);

#endif