#ifndef POINT_H
#define POINT_H
//global variable for the clipping window
extern int xmin, ymin, xmax, ymax;

//define the constant for 4bit code
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

//point class to store a point (x,y)
class Point
{
	float x;
	float y;
public:
	Point();
	Point(const Point &p);
	Point(float x, float y);
	void translate(float dx, float dy); //translate with (dx, dy) vector
	void scale(float factor); //unform scale with factor
	void rotate(float degree); //rotate around centroid with degree
	~Point();
	float getX() { return x; }
	float getY() { return y; }
	int ComputeCode();
};

#endif