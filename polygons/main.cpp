#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

bool comparePoints(Point first_point, Point second_point) {
    return (first_point.x < second_point.x) || (first_point.x == second_point.x && first_point.y < second_point.y);
}

bool operator == (const Point& point, const Point& point2) {
    return point.x == point2.x && point.y == point2.y;
}

int compare_relatively_min(Point min_p, Point q, Point r) {
    int res = (q.y - min_p.y) * (r.x - q.x) - (q.x - min_p.x) * (r.y - q.y);
    if (res == 0) {
        return 0;
    }
    if (res > 0){
        return 1;
    }
    return 2;
}

void sortVertex(std::vector<Point>& points) { // сортировка вершин многоугольника против часовой стрелки
    std::sort(points.begin(), points.end(), comparePoints);

    Point p0 = points[0];
    std::sort(points.begin() + 1, points.end(), [p0](Point a, Point b) {
        int res = compare_relatively_min(p0, a, b);
        if (res == 0) {
            return (a.x + a.y < b.x + b.y);
        }
        return (res == 2);
    });
}

bool is_point_on_line(const Point t0, const Point t1, const Point a) {
    double eq1 = (a.x - t0.x)/(t1.x - t0.x);
    double eq2 = (a.y - t0.y)/(t1.y - t0.y);
    if ((eq1 == eq2) || (a.x == t0.x && a.x == t1.x)) {
        return false;
    }
    return true;
}

bool is_polygon_correct(std::vector<Point>& polygon) {
    sortVertex(polygon);
    int count_minus = 0;
    int count_plus = 0;
    int size = polygon.size();
    for (int i = 1; i < polygon.size(); i++){
        double side_1_x = polygon[i % size].x - polygon[(i - 1) % size].x;
        double side_1_y = polygon[i % size].y - polygon[(i - 1) % size].y;
        double side_2_x = polygon[(i+1) % size].x - polygon[i % size].x;
        double side_2_y = polygon[(i+1) % size].y - polygon[i % size].y;
        if ((side_1_x * side_2_y - side_1_y * side_2_x) >= 0) {
            count_plus ++;
        }
        else {
            count_minus ++;
        }
    }
    if (count_plus == 0 || count_minus == 0) {
        return true;
    }
    else {
        return false;
    }

}

bool is_3points_on_side(std::vector<Point>& polygon) {
    sortVertex(polygon);
    int size = polygon.size();
    bool flag = true;
    for (int i = 0; i < size; i++){
        if (i > 0){
            if (!is_point_on_line(polygon[i - 1],
                                  polygon[(i + 1) % size],
                                      polygon[i])){
                flag = false;
                break;
            }
        }
        else{
            if (!is_point_on_line(polygon[size - 1], polygon[0], polygon[1])){
                flag = false;
                break;
            }
        }
    }
    return flag;
}

std::vector<std::vector<Point>> polygons;

void addPolygon(const std::string& input) { // парсер
    std::vector<Point> polygon;
    std::istringstream iss(input);
    std::string token;
    std::getline(iss, token, '(');
    bool flag_of_even = true;
    while (std::getline(iss, token, ',')) {
        double x = std::stod(token);
        if (!std::getline(iss, token, ',')) {
            std::cout << "Error! Please enter even number of coordinates\n";
            flag_of_even = false;
            break;
        }
        double y = std::stod(token);
        polygon.push_back(Point(x, y));
    }
    if (polygon.size() < 3) {
        std::cout << "Error! Please enter at least 3 points\n";
    }
    else if (flag_of_even) {
        if (!is_3points_on_side(polygon)) {
            std::cout << "Error! 3 points should not lie on a same line\n";
        }
        else if (!is_polygon_correct(polygon)) {
            std::cout << "Error! Please enter a convex polygon\n";
        }
        else {
            polygons.push_back(polygon);
        }
    }
}

double fixNegZero(double value) {
    if (value == 0.0) {
        return 0.0;
    }
    return value;
}

std::vector<Point> delete_duplicates(const std::vector<Point>& points){ //n^2
    std::vector<Point> res;
    bool flag = true;
    for(int i = 0; i < points.size(); i++){
        Point temp = points[i];
        flag = true;
        for(int j = i+1; j < points.size(); j++){
            if(temp == points[j]){
                flag = false;
            }
        }
        if(flag){
            res.push_back(temp);
        }
    }
    return res;
}

bool isPointInsidePolygon(const std::vector<Point>& points, const Point& c) {
    bool res = false;
    Point p1 = points[0];
    Point p2;
    for (int i = 1; i <= points.size(); i++) {
        p2 = points[i % points.size()];
        if (c.y > std::min(p1.y, p2.y)) {
            if (c.y <= std::max(p1.y, p2.y)) {
                if (c.x <= std::max(p1.x, p2.x)) {
                    double x_intersection = (c.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
                    if (p1.x == p2.x || c.x <= x_intersection) {
                        res = !res;
                    }
                }
            }
        }


        p1 = p2;
    }
    return res;
}

std::pair<bool, Point> findIntersection(const Point& A, const Point& B, const Point& C, const Point& D) {
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1 * A.x + b1 * A.y;

    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2 * C.x + b2 * C.y;

    double delta = a1 * b2 - a2 * b1;
    if (std::fabs(delta) < 1e-9) {
        return {false, {}};
    }

    double x = (b2 * c1 - b1 * c2) / delta;
    double y = (a1 * c2 - a2 * c1) / delta;

    if (x < std::min(A.x, B.x) || x > std::max(A.x, B.x) || x < std::min(C.x, D.x) || x > std::max(C.x, D.x) ||
        y < std::min(A.y, B.y) || y > std::max(A.y, B.y) || y < std::min(C.y, D.y) || y > std::max(C.y, D.y)) {
        return {false, {}};
    }

    return {true, {x, y}};
}

std::vector<Point> findIntersections(const std::vector<Point>& polygon1, const std::vector<Point>& polygon2) {
    std::vector<Point> intersections;

    for (int i = 0; i < polygon1.size(); ++i) {
        for (int j = 0; j < polygon2.size(); ++j) {
            auto result = findIntersection(polygon1[i], polygon1[(i + 1) % polygon1.size()], polygon2[j],polygon2[(j + 1) % polygon2.size()]);
            if (result.first) {
                intersections.push_back(result.second);
            }
        }
    }

    for (const auto& vertex : polygon1) {
        if (isPointInsidePolygon(polygon2, vertex)) {
            intersections.push_back(vertex);
        }
    }

    for (const auto& vertex : polygon2) {
        if (isPointInsidePolygon(polygon1, vertex)) {
            intersections.push_back(vertex);
        }
    }

    if (!(intersections.empty())) {
        sortVertex(intersections);
        intersections = delete_duplicates(intersections);
    }

    return intersections;
}

Point Centroid(const std::vector<Point>& points) {
    Point centroid;
    double sumX = 0, sumY = 0;

    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
    }

    centroid.x = sumX / points.size();
    centroid.y = sumY / points.size();

    return centroid;
}

double calculateAngle(const Point& centroid, const Point& point) {
    return atan2(point.y - centroid.y, point.x - centroid.x);
}

void sortPoints(std::vector<Point>& points) {
    Point centroid = Centroid(points);

    std::sort(points.begin(), points.end(), [&centroid](const Point& a, const Point& b) {
        return calculateAngle(centroid, a) < calculateAngle(centroid, b);
    });
}


//std::vector<std::vector<Point>> polygons = {{{1, 1}, {2, 3}, {4, 3}, {5, 1}, {4, -1}, {2, -1}},
//                                            {{4, -4}, {-7, -2}, {5, 4}},
//                                            {{2, 2}, {4, 2}, {4,4}, {2, 4}},
//                                            {{1, 1}, {3, 1}, {3, 3}, {1, 3}},
//};

std::vector<Point> general_inter() {
    std::vector<Point> inter = findIntersections(polygons[0], polygons[1]);
    for (int i = 2; i < polygons.size(); i++) {
        inter = findIntersections(inter, polygons[i]);
        if (inter.empty()) {
            return {};
        }
    }

    // cортировка точек общего пересечения
    if (!inter.empty()) {
        Point centroid = Centroid(inter);
        std::sort(inter.begin(), inter.end(), [&centroid](const Point& a, const Point& b) {
            return calculateAngle(centroid, a) < calculateAngle(centroid, b);
        });
    }
    return inter;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    *hDC = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    // ввод координат
    std::cout << "Hi there! I will help you find intersection area of multiple polygons. \n" <<
                 "Ask me and you shall receive! \n" <<
                 "Although you should keep in mind a couple of rules: polygons must be convex and entered in the following format: \n" <<
                 "'polygon(x0,y0,x1,y1,x2,y2,...)'. When all polygons are entered, type 'intersect()'.\n" <<
                 "Of course, the number of coordinates entered must be even, and 3 points should not be lying on the same line.\n" <<
                 "The coordinates themselves must be finite real numbers from -10 to 10 with a dot ('.') as a decimal separator. \n" <<
                 "Don't forget the brackets! \n" <<
                 "Input: ";

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input[input.size() - 1] != ')'){
            std::cout << "Error! Please enter closing bracket at the end of the string\n";
            std::getline(std::cin, input);
        }
        if (input == "intersect()" and polygons.size() < 2) {
            std::cout << "Error! Please enter at least 2 polygons.\n";
            return 0;
        }
        else if (input == "intersect()" and polygons.size() >= 2) {
            break;
        }
        addPolygon(input);
    }

    // вывод общей площади пересечения
    std::vector<Point> generalIntersection = general_inter();
    if (generalIntersection.empty()) {
        std::cout << "General intersection is empty" << std::endl;
    }
    else {
        std::cout << "Coordinates of the general intersection area:" << std::endl;
        for (int i = 0; i < generalIntersection.size(); i++) {
            if (i > 0) {
                if (is_point_on_line(generalIntersection[i - 1],
                                     generalIntersection[(i + 1) % generalIntersection.size()],
                                     generalIntersection[i])){
                    std::cout << "(" << fixNegZero(generalIntersection[i].x) << ", " << fixNegZero(generalIntersection[i].y) << ")" << std::endl;
                }
            }
            else {
                if (is_point_on_line(generalIntersection[generalIntersection.size() - 1], generalIntersection[0], generalIntersection[1])){
                    std::cout << "(" << fixNegZero(generalIntersection[i].x) << ", " << fixNegZero(generalIntersection[i].y) << ")" << std::endl;
                }
            }
        }
        std::cout << "Thank you for using our project! \n" <<
                     "Learn how this works in our GitHub: https://github.com/d010r3s/polygons-intersection";
    }

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Polygon";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    hwnd = CreateWindowEx(0, "Polygon", "Polygons", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 680, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    EnableOpenGL(hwnd, &hDC, &hRC);

    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            float tick = 0.1f / 10.0f;
            float colorIncrement = 1.0f / (polygons.size() + 1);
            for (size_t i = 0; i < polygons.size(); ++i) {
                glBegin(GL_POLYGON);
                glColor4f((i + 1) * colorIncrement, 0.0f, 0.0f, 0.5f);
                for (const auto& point : polygons[i]) {
                    glVertex2f(point.x / 10.0f, point.y / 10.0f);
                }
                glEnd();
            }

            std::vector<std::vector<Point>> intersections;

            for (size_t i = 0; i < polygons.size(); ++i) {
                for (size_t j = i + 1; j < polygons.size(); ++j) {
                    auto currentIntersection = findIntersections(polygons[i], polygons[j]);
                    if (!currentIntersection.empty()) {
                        sortPoints(currentIntersection);
                        intersections.push_back(currentIntersection);
                    }
                }
            }

            colorIncrement = 1.0f / (intersections.size() + 1);
            for (size_t i = 0; i < intersections.size(); ++i) {
                glBegin(GL_POLYGON);
                glColor4f(colorIncrement * (i + 1), 0.0f, 1.0f, 0.5f); // каждая область пересечения нового цвета
                for (const auto& point : intersections[i]) glVertex2f(point.x / 10.0f, point.y / 10.0f);
                glEnd();
            }

            // засечки по X
            for(float i = -10.0f; i <= 11.0f; i += 1.0f) {
                glBegin(GL_LINES);
                glVertex2f(i / 10.0f, -tick);
                glVertex2f(i / 10.0f, tick);
                glEnd();
            }

            // засечки по Y
            for(float i = -10.0f; i <= 11.0f; i += 1.0f) {
                glBegin(GL_LINES);
                glVertex2f(-tick, i / 10.0f);
                glVertex2f(tick, i / 10.0f);
                glEnd();
            }

            // ось Х
            glBegin(GL_LINES);
            glVertex2f(-10.0f / 10.0f, 0.0f);
            glVertex2f(11.0f / 10.0f, 0.0f);
            glEnd();

            // ось Y
            glBegin(GL_LINES);
            glVertex2f(0.0f, -10.0f / 10.0f);
            glVertex2f(0.0f, 11.0f / 10.0f);
            glEnd();

            // отрисовка области общего пересечения белым
            std::vector<Point> commonIntersection = general_inter();
            if (!commonIntersection.empty()) {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                glBegin(GL_POLYGON);
                for (const auto& point : commonIntersection) glVertex2f(point.x / 10.0f, point.y / 10.0f);
                glEnd();
            }

            SwapBuffers(hDC);
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);
    return msg.wParam;
}
