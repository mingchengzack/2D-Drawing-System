#include "line.h"
#include "point.h"
#include "utility.h"
#include "polygon.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

Line::Line()
{
}

Line::Line(Point s, Point e): start(s), end(e)
{
}

float Line::findLeftX()
{
	if (start.getX() > end.getX())
		return end.getX();
	else
		return start.getX();
}

float Line::findRightX()
{
	if (start.getX() < end.getX())
		return end.getX();
	else
		return start.getX();
}

float Line::getDy()
{
	return (end.getY() - start.getY());
}

float Line::getDx()
{
	return (end.getX() - start.getX());
}

float Line::findYmax()
{
	if (start.getY() > end.getY())
		return start.getY();
	else
		return end.getY();
}

float Line::findYmin()
{
	if (start.getY() < end.getY())
		return start.getY();
	else
		return end.getY();
}

float Line::getX0()
{
	return start.getX();
}

float Line::getY0()
{
	return start.getY();
}

float Line::getXF()
{
	return end.getX();
}

float Line::getYF()
{
	return end.getY();
}

void Line::getVertice()
{
	cout << start.getX() << ',' << start.getY() << "to" << end.getX() << ',' << end.getY() << endl;
}

void Line::clipLine(int option)
{
	//compute codes for start point and end point
	int code1 = start.ComputeCode(), code2 = end.ComputeCode(); 
	float x0 = start.getX(), y0 = start.getY(), x1 = end.getX(), y1 = end.getY();
	bool accept = false;

	while (true)
	{
		if (!(code1 | code2))
		{
			accept = true;
			break;
		}
		else if (code1 & code2)
		{
			break;

		}
		else
		{
			//calculate the line segment to clip
			float x, y;
			int codeout = code1 ? code1 : code2;

			//find the intersection 
			if (codeout & TOP)  //point is top
			{
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = (float) ymax;
			}
			else if (codeout & BOTTOM) //below
			{
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = (float)ymin;
			}
			else if (codeout & RIGHT) //right
			{
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = (float)xmax;
			}
			else if (codeout & LEFT) //left
			{
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = (float) xmin;
			}
			if (codeout == code1)
			{
				x0 = x;
				y0 = y;
				Point P(x0, y0);
				code1 = P.ComputeCode();
			}
			else
			{
				x1 = x;
				y1 = y;
				Point P(x1, y1);
				code2 = P.ComputeCode();
			}
		}
	}
	if (accept)
	{
		//draw the clipped line
		Line clip(Point(x0, y0), Point(x1, y1));
		clip.drawLine(option);
	}
}

void Line::drawLine(int option)
{
	if (option == 1)
		drawLineDDA();
	else if (option == 2)
		drawLineBresenham();
}

void Line::drawLineBresenham() // use Bresenham algorithm to draw line
{
	int x0 = rd(start.getX()), xEnd = rd(end.getX()), y0 = rd(start.getY()), yEnd = rd(end.getY());
	float slope = (end.getY() - start.getY()) / (end.getX() - start.getX());
	int dx = (int)fabs(xEnd - x0), dy = (int)fabs(yEnd - y0);
	
	//for a normal line
	if (dx != 0 && dy != 0)
	{
		//for line with |slope| < 1
		if (fabs(slope) < 1)
		{
			int p = 2 * dy - dx;
			int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
			int x, y;

			// Determine which endpoint to use as start position 
			if (start.getX() > end.getX())
			{
				x = xEnd;
				y = yEnd;
				xEnd = x0;
			}
			else
			{
				x = x0;
				y = y0;
			}
			MakePix(x, y, 1.0, 0, 0);

			while (x < xEnd)
			{
				x++;
				if (p < 0)
					p += twoDy;
				else
				{
					//decrement y if slope is negative
					if (slope < 0)
						y--;
					//increment if slope is positive
					else
						y++;
					p += twoDyMinusDx;
				}
				MakePix(x, y, 1.0, 0, 0);
			}
		}
		//for line with |slope| > 1: switch x and y
		else
		{
			int p = 2 * dx - dy;
			int twoDx = 2 * dx, twoDxMinusDy = 2 * (dx - dy);
			int x, y;

			// Determine which endpoint to use as start position 
			if (start.getY() > end.getY())
			{
				y = yEnd;
				x = xEnd;
				yEnd = y0;
			}
			else
			{
				y = y0;
				x = x0;
			}
			MakePix(x, y, 1.0, 0, 0);

			while (y < yEnd)
			{
				y++;
				if (p < 0)
					p += twoDx;
				else
				{
					//decrement x if slope is negative
					if (slope < 0)
						x--;
					//increment x if slope is positive
					else
						x++;
					p += twoDxMinusDy;
				}
				MakePix(x, y, 1.0, 0, 0);
			}
		}
	}
	
	//horizontal line
	else if (dy == 0)
	{
		int x, y;
		y = rd(start.getY());
		// Determine which endpoint to use as start position 
		if (start.getX() > end.getX())
		{
			x = xEnd;
			xEnd = x0;
		}
		else
		{
			x = x0;
		}
		for (int i = x; i <= xEnd; i++)
			MakePix(i, y, 1.0, 0, 0);
	}
	//vertical line
	else if (dx == 0)
	{
		int x, y;
		x = rd(start.getX());
		// Determine which endpoint to use as start position 
		if (start.getY() > end.getY())
		{
			y = yEnd;
			yEnd = y0;
		}
		else
		{
			y = y0;
		}
		for (int i = y; i <= yEnd; i++)
			MakePix(x, i, 1.0, 0, 0);
	}
}

void Line::drawLineDDA() // using DDA to draw the line
{
	float dx = end.getX() - start.getX(), dy = end.getY() - start.getY(), steps;
	int i;
	float xIncrement, yIncrement, x = start.getX(), y = start.getY();

	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	xIncrement = dx / steps;
	yIncrement = dy / steps;
	MakePix(rd(x), rd(y), 0, 0, 1.0);
	for (i = 0; i < steps; i++)
	{
		x += xIncrement;
		y += yIncrement;
		MakePix(rd(x), rd(y), 0, 0, 1.0);
	}
}
