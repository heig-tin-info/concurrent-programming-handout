#include <thread>
#include <iostream>

class Point {
public:
    int x, y;

    Point() : x(0), y(0) {}

    void deplacer(int deltaX, int deltaY) {
        x += deltaX; // Imaginez un contexte switch ici
        y += deltaY;
    }

    void afficher() const {
        std::cout << "Point(" << x << ", " << y << ")" << std::endl;
    }
};

void deplacerPoint(Point& point, int deltaX, int deltaY) {
    for (int i = 0; i < 1000; ++i) {
        point.deplacer(deltaX, deltaY);
    }
}

int main() {
    Point monPoint;
    std::thread t1(deplacerPoint, std::ref(monPoint), 1, 1);
    std::thread t2(deplacerPoint, std::ref(monPoint), -1, -1);

    t1.join();
    t2.join();

    monPoint.afficher();

    return 0;
}
