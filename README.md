# Polygons Intersection - 1st AMI's sophomore year c++ project 
:robot:
## Table of contents
* [Terms of reference](#terms-of-reference)
* [Intersection of Triangles](#intersection-of-triangles)
* [Intersection of Polygons](#intersection-of-polygons)

## Terms of Reference
### 1st version
- [x] Parameters are set via text with ```triangle(x0,y0,x1,y1,x2,y2)``` to create a triangle and ```intersect()``` to intersect all of the triangles
- [x] The result is the visualization of the created triangles and a list of coordinates of the vertices of the figure formed by the intersection
### 2nd version
- [x] Parameters are set via text with ```polygon(x0,y0,x1,y1,x2,y2,...)``` to create a polygon and ```intersect()``` to intersect all of the polygons
- [x] The result is the visualization of the created polygons and a list of coordinates of the vertices of the figure formed by the intersection

## Intersection of Triangles
A cubic spline is a spline constructed of piecewise third-order polynomials which pass through a set of m control points. The second derivative of each polynomial is commonly set to zero at the endpoints, since this provides a boundary condition that completes the system of m-2 equations. This produces a so-called "natural" cubic spline and leads to a simple tridiagonal system which can be solved easily to give the coefficients of the polynomials.

![Formula used](https://i.imgur.com/0lU4qyO.png)
![Formula used](https://i.imgur.com/8lzXqOJ.png)

[source](http://statistica.ru/branches-maths/interpolyatsiya-splaynami-teor-osnovy/)


![Cubic Spline](https://blogs.sas.com/content/iml/files/2020/05/cubicInterp1.png)
	
## Intersection of Polygons
In the case of parametric cubic
splines, each spline segment is represented by two equations in the independent variable s:
```
x = f1(s) = a_x*(s-s0)^3 + b_x*(s-s0)^2 + c_x*(s-s0) + d_x
x = a_x*t^3 + b_x*t^2 + c_x*t + d_x
t = s - s0

y = f2(s) = a_y*(s-s0)^3 + b_y*(s-s0)^2 + c_y*(s-s0) + d_y
y = a_y*t^3 + b_y*t^2 + c_y*t + d_y
t = s - s0
```
, 

where s0 represents the value of 

[source](https://www.physicsforums.com/attachments/parametric-spline-tutorialv2-pdf.12898/) 


#### Methods used:
* [Tridiagonal matrix algorithm](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm)
###### Tridiagonal matrix algorithm
![Tridiagonal matrix algorithm](https://www.cfd-online.com/W/images/math/5/b/7/5b73418f4d5bd7154e369a6d6f4ae2fd.png)
* [Fitting parametric cubic splines to a set of points](https://www.physicsforums.com/attachments/parametric-spline-tutorialv2-pdf.12898)
###### Fitting parametric cubic spline to a set of points
![Set](https://i.imgur.com/iUAsmUc.png)

## Intersection point of two splines and minimum distance between two splines

#### Methods used:
* Creating an additional third spline merged from the two given
* [Newton-Raphson method](https://en.wikipedia.org/wiki/Newton%27s_method)
* [Gradient-Descent Method](https://hal.archives-ouvertes.fr/hal-03854553/file/annpr.pdf)
###### Newton-Raphson Method 
![Newton Method](https://media.geeksforgeeks.org/wp-content/cdn-uploads/newtonRaphsonMethod.png)
###### Gradient-Descent Method
![Gradient descent Method](https://miro.medium.com/max/1400/1*jNyE54fTVOH1203IwYeNEg.png)
