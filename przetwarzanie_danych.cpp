#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class TemperatureSensor {
public:
    TemperatureSensor() {
        std::srand(std::time(0));
    }

    double readTemperature() {
        return 15 + static_cast<double>(std::rand() % 2000) / 100; // Symulacja losowej temperatury 15-35°C
    }
};

void processSensorData(const std::vector<double>& data) {
    double sum = 0;
    for (double temp : data) {
        sum += temp;
    }
    double avg = sum / data.size();

    std::cout << "Average temperature: " << avg << "°C\n";

    for (double temp : data) {
        if (temp > 30.0) {
            std::cout << "Warning: High temperature detected: " << temp << "°C\n";
        }
    }
}

int main() {
    TemperatureSensor sensor;
    std::vector<double> data;

    for (int i = 0; i < 10; ++i) { // Symulacja 10 odczytów
        double temp = sensor.readTemperature();
        data.push_back(temp);
        std::cout << "Read temperature: " << temp << "°C\n";
    }

    processSensorData(data);
    return 0;
}
