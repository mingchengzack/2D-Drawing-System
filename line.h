#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <fstream>
#include "point.h"

using namespace std;

class Line
{
	Point start;
	Point end;
public:
	Line();
	Line(Point s, Point e);
	float findLeftX();
	float findRightX();
	float getDy();
	float getDx();
	float findYmax();
	float findYmin();
	float getX0();
	float getY0();
	float getXF();
	float getYF();
	void getVertice();
	void clipLine(int option);
	void drawLine(int option);
	void drawLineBresenham(); //Bresenham implementation
	void drawLineDDA(); // DDA implementation
}; //class of a line
#endif

