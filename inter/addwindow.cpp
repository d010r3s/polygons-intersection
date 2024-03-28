#include "addwindow.h"
#include <QMessageBox>
#include <QString>
#include "polygonsinfo.h"
#include "ui_addwindow.h"
#include <windows.h>
#include <gl/gl.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <ui_polygonsinfo.h>


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



double calculateAngle(const Point& centroid, const Point& point) {
    return atan2(point.y - centroid.y, point.x - centroid.x);
}



addwindow::addwindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addwindow)
{
    ui->setupUi(this);
}

addwindow::~addwindow()
{
    delete ui;
}

void addwindow::on_pushButton_2_clicked()
{
    QString input = ui->enterpolygon->text();
    std::vector<Point> polygon;
    std::string inp = input.toLocal8Bit().constData();

    std::istringstream iss(inp);
    std::string token;
    std::getline(iss, token, '(');
    bool flag_of_even = true;
    while (std::getline(iss, token, ',')) {
        double x = std::stod(token);
        if (!std::getline(iss, token, ',')) {
            QMessageBox::information(this, "Error", "Error! Please enter even number of coordinates");
            flag_of_even = false;
            break;
        }
        double y = std::stod(token);
        polygon.push_back(Point(x, y));
    }

    if (polygon.size() < 3){
        QMessageBox::information(this, "Error", "Error! Please enter at least 3 points");
    }
    else if (flag_of_even){
        if(!is_3points_on_side(polygon)){
            QMessageBox::information(this, "Error", "Error! 3 points should not lie on a same line");
        }
        else if (!is_polygon_correct(polygon)) {
            QMessageBox::information(this, "Error", "Error! Please enter a convex polygon");

        }
        else{
            polygons.push_back(polygon);
            QMessageBox::information(this, "Success", "Your polygon has been added to the list");
        }

    }

}

void addwindow::on_pushButton_clicked()
{
    QString arrayString;
    for(int i = 0; i < polygons.size(); ++i)
    {
        arrayString += "{";
        for (int j=0; j < polygons[i].size(); j++){
            if (j != polygons[i].size()-1){
        arrayString +=  "(" + QString::number(polygons[i][j].x) + "," + QString::number(polygons[i][j].y) + ")" + " ; ";
            }
            else{
                arrayString +=  "(" + QString::number(polygons[i][j].x) + "," + QString::number(polygons[i][j].y) + ")";
            }
        }
        arrayString += "}\n";
    }
    //QMessageBox::information(this, "Points", arrayStr);

    PolygonsInfo window1;
    window1.setLabel(arrayString);
    window1.setModal(true);
    window1.exec();

}

