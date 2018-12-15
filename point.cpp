#include "point.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265
using namespace std;

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(const Point &p)
{
	x = p.x;
	y = p.y;
}

Point::Point(float x, float y) : x(x), y(y)
{
}

void Point::translate(float dx, float dy)
{
	x += dx;
	y += dy;
}

void Point::scale(float factor)
{
	x = x * factor;
	y = y * factor;
}

void Point::rotate(float degree)
{
	float xtemp = x;
	float ytemp = y;
	x = (float) (xtemp * cos(degree * PI / 180.0) - ytemp * sin(degree * PI / 180.0));
	y = (float) (xtemp * sin(degree * PI / 180.0) + ytemp * cos(degree * PI / 180.0));
}

Point::~Point()
{
}

//compute the 4bit code for a point(x, y) in a window of (xmin, ymin) x (xmax, ymax)
int Point::ComputeCode()
{
	int code = INSIDE;

	if (x < xmin)
		code |= LEFT;  //left of the window
	else if (x > xmax)
		code |= RIGHT; //right of the window
	if (y < ymin)
		code |= BOTTOM; //bottom of the window
	else if (y > ymax)
		code |= TOP;  //top of the window
	return code;
}
