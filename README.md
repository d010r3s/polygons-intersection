# Polygons Intersection - 5th AMI's sophomore year c++ project 
:robot: :robot: :robot:
## Table of contents
* [Terms of reference](#terms-of-reference)
* [Theoretical aspects](#theoretical-aspects)
* [Functional aspects](#functional-aspects)

## Terms of Reference
### 1st version
- [x] Parameters are set via text with ```triangle(x0,y0,x1,y1,x2,y2)``` to create a triangle and ```intersect()``` to intersect all of the triangles
- [x] The result is the visualization of the created triangles and a list of coordinates of the figure formed by the intersection
### 2nd version
- [x] Parameters are set via text with ```polygon(x0,y0,x1,y1,x2,y2,...)``` to create a polygon and ```intersect()``` to intersect all of the polygons
- [x] The result is the visualization of the created polygons and a list of coordinates of the figure formed by the intersection

## Theoretical aspects

![confused math lady](https://i.kym-cdn.com/entries/icons/original/000/021/464/14608107_1180665285312703_1558693314_n.jpg)

### Polygon convexity check
We verify this by checking the cross product of vectors formed by consecutive edges.

### Sorting vertices
Vertices of a polygon are sorted counter-clockwise.

### Lines intersection
We use the parametric equation of a line and solve for the intersection point.

### Polygon intersection
We calculate the points where edges intersect and determine which parts of the polygons contribute to the intersected area.
	
## Functional aspects
![user](https://i.imgur.com/2rqNKnh.png)

### User input
We check for correct input format, validate the geometric properties of the polygons and provide feedback on input errors.

### Graphical representation
We utilize OpenGL for rendering polygons.

### Interactivity
We interact with the user through a console interface, then a graphical window displays the polygons and their intersections.
