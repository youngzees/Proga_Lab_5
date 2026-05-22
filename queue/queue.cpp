#include "queue.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// Глобальная очередь (можно вынести в main и передавать, но для простоты так)
static std::vector<Visitor> visitorsQueue;

void enqueue(const std::string& ticketNumber, int duration) {
    visitorsQueue.push_back({ticketNumber, duration});
}

void distribute(int windowsCount) {
    // Вектор суммарного времени для каждого окна
    std::vector<int> windowTime(windowsCount, 0);
    
    // Вектор списков талонов для каждого окна
    std::vector<std::vector<std::string>> windowTickets(windowsCount);
    
    // Для каждого посетителя в очереди
    for (const auto& visitor : visitorsQueue) {
        // Находим окно с минимальным временем
        int bestWindow = 0;
        for (int i = 1; i < windowsCount; ++i) {
            if (windowTime[i] < windowTime[bestWindow]) {
                bestWindow = i;
            }
        }
        
        // Добавляем посетителя в выбранное окно
        windowTickets[bestWindow].push_back(visitor.ticketNumber);
        windowTime[bestWindow] += visitor.duration;
    }
    
    // Вывод результатов
    for (int i = 0; i < windowsCount; ++i) {
        std::cout << "Окно " << (i + 1) << " (" << windowTime[i] << " минут): ";
        
        for (size_t j = 0; j < windowTickets[i].size(); ++j) {
            if (j > 0) std::cout << ", ";
            std::cout << windowTickets[i][j];
        }
        std::cout << std::endl;
    }
}

void clearQueue() {
    visitorsQueue.clear();
}