#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};
bool comparePoints(Point first_point, Point second_point) {
    return (first_point.x < second_point.x) || (first_point.x == second_point.x && first_point.y < second_point.y);
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



bool isPointInsidePoligon(const std::vector<Point>& points, const Point& c) {
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
std::vector<Point> findIntersections(const std::vector<Point>& poligon1, const std::vector<Point>& poligon2) {
    std::vector<Point> intersections;
    for (int i = 0; i < poligon1.size(); ++i) {
        for (int j = 0; j < poligon2.size(); ++j) {
            auto result = findIntersection(poligon1[i], poligon1[(i + 1) % poligon1.size()], poligon2[j],poligon2[(j + 1) % poligon2.size()]);
            if (result.first) {

                intersections.push_back(result.second);

            }

        }
    }
    for (const auto& vertex : poligon1) {
        if (isPointInsidePoligon(poligon2, vertex)) {
            intersections.push_back(vertex);
        }
    }
    for (const auto& vertex : poligon2) {
        if (isPointInsidePoligon(poligon1, vertex)) {
            intersections.push_back(vertex);
        }
    }
    sortVertex(intersections);
    std::vector<Point> res_inter = {intersections[0]};
    for (int i = 0; i < intersections.size()-1; i++){
        if (intersections[i].x != intersections[i+1].x || intersections[i].y != intersections[i+1].y){
            res_inter.push_back(intersections[i+1]);
        }
    }


    return res_inter;
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




std::vector<std::vector<Point>> triangles = {
        {{-4,-4}, {-3, 6}, {5, 6}},
        {{-2, 3}, {4, 9}, {4, 2}},
        {{1, -3}, {1, 7}, {7, 4}}
};
void general_inter(){ // нахождение общей фигуры пересечения и вывод вершин
    std::vector<Point> inter = findIntersections(triangles[0], triangles[1]);

    std::cout<<"---"<<std::endl;
    for (int i = 2; i < triangles.size(); i++){
        inter = findIntersections(inter, triangles[i]);



        }
    for (int i = 0; i < inter.size(); i++){
        std::cout<<inter[i].x << ' ' << inter[i].y << std::endl;
    }
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
    wcex.lpszClassName = "Triangle";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    hwnd = CreateWindowEx(0, "Triangle", "Triangles", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 680, 680, NULL, NULL, hInstance, NULL);
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
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            float tick = 0.1f / 10.0f;

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
            float colorIncrement = 1.0f / (triangles.size() + 1);
            for (size_t i = 0; i < triangles.size(); ++i) {
                glBegin(GL_TRIANGLES);
                glColor3f((i + 1) * colorIncrement, 0.0f, 0.0f); // каждый треугольник нового цвета
                for (const auto& point : triangles[i]) glVertex2f(point.x / 10.0f, point.y / 10.0f);
                glEnd();
            }

            std::vector<std::vector<Point>> intersections;
            for (size_t i = 0; i < triangles.size() - 1; ++i) {
                for (size_t j = i + 1; j < triangles.size(); ++j) {
                    auto currentIntersection = findIntersections(triangles[i], triangles[j]);
                    if (!currentIntersection.empty()) {
                        sortPoints(currentIntersection);
                        intersections.push_back(currentIntersection);
                    }
                }

            }
            colorIncrement = 1.0f / (intersections.size() + 1);
            for (size_t i = 0; i < intersections.size(); ++i) {
                glBegin(GL_POLYGON);
                glColor3f(colorIncrement * (i + 1), 0.0f, 1.0f); // каждая область пересечения нового цвета
                for (const auto& point : intersections[i]) glVertex2f(point.x / 10.0f, point.y / 10.0f);
                glEnd();
            }

            SwapBuffers(hDC);
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);
    general_inter();

    return msg.wParam;
}
