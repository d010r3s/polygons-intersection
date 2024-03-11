#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <cmath>
#include <algorithm>

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

bool isPointInsideTriangle(const Point& p, const Point& a, const Point& b, const Point& c) {
    double areaOrig = std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    double area1 = std::abs(p.x * (b.y - c.y) + b.x * (c.y - p.y) + c.x * (p.y - b.y));
    double area2 = std::abs(a.x * (p.y - c.y) + p.x * (c.y - a.y) + c.x * (a.y - p.y));
    double area3 = std::abs(a.x * (b.y - p.y) + b.x * (p.y - a.y) + p.x * (a.y - b.y));
    return std::abs(areaOrig - (area1 + area2 + area3)) < 1e-9;
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

std::vector<Point> findIntersections(const std::vector<Point>& triangle1, const std::vector<Point>& triangle2) {
    std::vector<Point> intersections;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto result = findIntersection(triangle1[i], triangle1[(i + 1) % 3], triangle2[j], triangle2[(j + 1) % 3]);
            if (result.first) {
                intersections.push_back(result.second);
            }
        }
    }
    for (const auto& vertex : triangle1) {
        if (isPointInsideTriangle(vertex, triangle2[0], triangle2[1], triangle2[2])) {
            intersections.push_back(vertex);
        }
    }
    for (const auto& vertex : triangle2) {
        if (isPointInsideTriangle(vertex, triangle1[0], triangle1[1], triangle1[2])) {
            intersections.push_back(vertex);
        }
    }
    return intersections;
}

std::vector<std::vector<Point>> triangles = {{{0, 0}, {6, 0}, {3, 6}}, {{2, 2}, {4, 2}, {3, 4}}, {{2, 2}, {-4, -2}, {-3, -4}}, {{-2, -2}, {4, 2}, {3, 4}}, {{5, 5}, {4, 6}, {-1, -2}}};

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

    return msg.wParam;
}
