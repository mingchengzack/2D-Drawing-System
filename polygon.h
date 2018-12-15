#ifndef POLYGON_H
#define POLYGON_H
#include <iostream>
#include <fstream>
#include <vector>
#include "point.h"
#include "utility.h"
#include "line.h"

using namespace std;

class Poly
{
	vector<Point> vertice;
	vector<Line> edges;
	bool fill;
	int size;
	friend istream& operator>>(istream& is, Poly& polygon);
	friend ostream& operator<<(ostream& os, const Poly& polygon);
public:
	void setFill() { fill = true; }
	bool ifFilled() { return fill; }
	void drawPolygon(int option);
	void fillPolygon(int option); //rasterization using scanline with edge tables and active list
	void clipPolygon(int option); //clip the polygon given xmin, ymin, xmax, ymax which are global variables]
	void translate(float dx, float dy); //translate with vector(dx, dy)
	void scale(float factor); //scale with factor with respect to centroid
	void rotate(float degree); //rotate with respect to centroid in degrees
	Point findCentroid(); //find the centroid of the polygon
	int findYmax();
	int findYmin();
	Poly(int numberOfEdges);
	Poly(vector<Point> vertices);
	~Poly();
};
#endif
