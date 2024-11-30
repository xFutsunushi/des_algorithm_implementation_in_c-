#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>

class Drone {
public:
    Drone() : x(0), y(0), altitude(0) {}

    void setAltitude(double alt) {
        if (alt < 0) {
            std::cerr << "Altitude cannot be negative.\n";
            return;
        }
        altitude = alt;
    }

    void setPosition(double newX, double newY) {
        x = newX;
        y = newY;
    }

    void flyTo(double targetX, double targetY, double targetAlt) {
        setAltitude(targetAlt);
        while (distanceTo(targetX, targetY) > 0.1 || std::abs(altitude - targetAlt) > 0.1) {
            moveTo(targetX, targetY, targetAlt);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            displayStatus();
        }
        std::cout << "Arrived at destination.\n";
    }

private:
    double x, y, altitude;

    double distanceTo(double targetX, double targetY) {
        return std::sqrt(std::pow(targetX - x, 2) + std::pow(targetY - y, 2));
    }

    void moveTo(double targetX, double targetY, double targetAlt) {
        x += (targetX - x) * 0.1;
        y += (targetY - y) * 0.1;
        altitude += (targetAlt - altitude) * 0.1;
    }

    void displayStatus() {
        std::cout << "Drone position: (" << x << ", " << y << "), Altitude: " << altitude << "\n";
    }
};

int main() {
    Drone drone;
    drone.flyTo(10, 10, 15); // Przykład – dron przemieszcza się do punktu (10, 10) na wysokość 15
    return 0;
}
