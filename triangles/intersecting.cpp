#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

// Функция для проверки пересечения двух отрезков и нахождения точки пересечения
std::pair<bool, Point> findIntersection(const Point& A, const Point& B, const Point& C, const Point& D) {
    double a2 = B.y - A.y;
    double a1 = B.x - A.x;
    //double c1 = a1 * A.x + b1 * A.y;

    double b2 = D.y - C.y;
    double b1 = D.x - C.x;
    //double c2 = a2 * C.x + b2 * C.y;

    double det = b1 * a2 - b2 * a1;
    double epsilon = fabs(det) / ((pow(a1, 2) + pow(a2, 2)) * (pow(b1, 2) + pow(b2, 2)));
    if (det == 0.0 || epsilon <= 0.00001) {
        return { false, Point(0, 0) };
    }
    else {
        
        double pscalar1 = D.x * C.y - D.y * C.x;
        double pscalar2 = B.x * A.y - B.y * A.x;
        double r_x = (pscalar1 * a1 - pscalar2 * b1) / det;
        double r_y = (pscalar1 * a2 - pscalar2 * b2) / det;
        double scal1 = ((r_x - A.x) * (r_x - B.x) + (r_y - A.y) * (r_y - B.y));
        double scal2 = ((r_x - C.x) * (r_x - D.x) + (r_y - C.y) * (r_y - D.y));
        if ((scal1 <= 0) && (scal2 <= 0)) {
            return { true, Point(r_x, r_y) };
        }
    }
    return { false, Point(0, 0) };
    
}

std::vector<Point> findIntersections(const Point(&triangle1)[3], const Point(&triangle2)[3]) {
    std::vector<Point> intersections;
    for (int i = 0; i < 3; i ++){
        double eq_1 = (triangle1[0].x - triangle2[i].x)*(triangle1[1].y - triangle1[0].y) - (triangle1[1].x - triangle1[0].x)*(triangle1[0].y - triangle2[i].y);
        double eq_2 = (triangle1[1].x - triangle2[i].x)*(triangle1[2].y - triangle1[1].y) - (triangle1[2].x - triangle1[1].x)*(triangle1[1].y - triangle2[i].y);
        double eq_3 = (triangle1[2].x - triangle2[i].x)*(triangle1[0].y - triangle1[2].y) - (triangle1[0].x - triangle1[2].x)*(triangle1[2].y - triangle2[i].y);
        if ((eq_1 > 0 && eq_2 > 0 && eq_3 > 0) || (eq_1 < 0 && eq_2 < 0 && eq_3 < 0) || (eq_1 == 0 || eq_2 == 0 || eq_3 == 0)){
            intersections.push_back(triangle2[i]);
        }
        eq_1 = (triangle2[0].x - triangle1[i].x)*(triangle2[1].y - triangle2[0].y) - (triangle2[1].x - triangle2[0].x)*(triangle2[0].y - triangle1[i].y);
        eq_2 = (triangle2[1].x - triangle1[i].x)*(triangle2[2].y - triangle2[1].y) - (triangle2[2].x - triangle2[1].x)*(triangle2[1].y - triangle1[i].y);
        eq_3 = (triangle2[2].x - triangle1[i].x)*(triangle2[0].y - triangle2[2].y) - (triangle2[0].x - triangle2[2].x)*(triangle2[2].y - triangle1[i].y);
        if ((eq_1 > 0 && eq_2 > 0 && eq_3 > 0) || (eq_1 < 0 && eq_2 < 0 && eq_3 < 0) || (eq_1 == 0 || eq_2 == 0 || eq_3 == 0)){
            intersections.push_back(triangle1[i]);
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int next_i = (i + 1) % 3;
            int next_j = (j + 1) % 3;
            //std::cout << i << " " << next_i << "\n";
            //std::cout << j<< " " << next_j << "\n";

            auto [intersected, intersectionPoint] = findIntersection(triangle1[i], triangle1[next_i], triangle2[j], triangle2[next_j]);
            if (intersected) {
                intersections.push_back(intersectionPoint);
            }
        }
    }

    

    return intersections;
}

int main() {
    Point triangle1[] = { Point(2, 7), Point(6, 2), Point(2,2) };
    Point triangle2[] = { Point(1, 9), Point(1, -2), Point(6,4) };

    //auto [intersected, intersectionPoint] = findIntersection(triangle1[0], triangle1[1], triangle2[0], triangle2[1]);
    //std::cout << intersectionPoint.x << " " << intersectionPoint.y;

    std::vector<Point> intersectionPoints = findIntersections(triangle1, triangle2);

    std::cout << "Intersection points: ";
    for (const Point& intersectionPoint : intersectionPoints) {
        std::cout << "(" << intersectionPoint.x << ", " << intersectionPoint.y << ") ";
    }
    std::cout << std::endl;

    return 0;
}
