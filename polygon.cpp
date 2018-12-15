#include "polygon.h"
#include "utility.h"

using namespace std;

istream & operator>>(istream & is, Poly & polygon)
{
	// store the polygons and draw the outline of the polygon
	float x, y;
	for (int i = 0; i < polygon.size; i++)
	{
		is >> x >> y;
		Point p(x, y);
		polygon.vertice.insert(polygon.vertice.begin() + i, p);
	}
	for (int i = 0; i < polygon.size - 1; i++)
	{
		Line l(polygon.vertice[i], polygon.vertice[i + 1]);
		polygon.edges.insert(polygon.edges.begin() + i, l);
	}
	Line l(polygon.vertice[0], polygon.vertice[polygon.size - 1]);
	polygon.edges.insert(polygon.edges.end(), l);
	// connect the first edge to the last edge
	return is;
}

ostream & operator<<(ostream & os, const Poly & polygon)
{
	os << polygon.size << endl;
	for (int i = 0; i < polygon.size; i++)
	{
		Point p(polygon.vertice[i]);
		os << p.getX() << ' ' << p.getY() << endl;
	}
	os << '\n';
	return os;
}


//the function to connect the vertice and draw the shape of the polygon
void Poly::drawPolygon(int option)
{
	for (int i = 0; i < size - 1; i++)
	{
		Line l(vertice[i], vertice[i + 1]);
		//draw all the edges
		l.drawLine(option);
		// add edges
	}
	Line l(vertice[0], vertice[size - 1]);
	l.drawLine(option);
	// connect the first edge to the last edge
}

//rasterization using scanline with edge tables and active list
void Poly::fillPolygon(int option)
{
	drawPolygon(2);
	int yMin = findYmin();
	vector<Line> ET;
	vector<Line> AET;	
    //initialize edge tables
	int y = yMin;
	float x1, x2;
	int currentY = rd(edges[0].findYmin());
	ET.insert(ET.begin(), edges[0]); // insert and sort the edge table by minimum Y value of each edge
	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < i; j++)
		{
			currentY = rd(ET[j].findYmin());
			if (rd(edges[i].findYmin()) < currentY)
			{
				ET.insert(ET.begin() + j, edges[i]);
				break;
			}
			else if (j == i - 1)
			{
				ET.push_back(edges[i]);
			}
		}
	}
	// get rid of the horizontal lines
	for (unsigned int i = 0; i < ET.size(); i++)
	{
		if (ET[i].getDy() == 0)
		{
		   ET.erase(ET.begin() + i);
		   i--;
		}
	}

	if (rd(ET[0].getYF()) == y)
	{
		x1 = ET[0].getXF();
		x2 = ET[0].getXF();
	}
	
	else
	{
		x1 = ET[0].getX0();
		x2 = ET[0].getX0();
	}
		
	if (ET.size() > 1 && rd(ET[1].getYF()) == y)
	{
		if (ET[1].findLeftX() > ET[0].findLeftX())
			x2 = ET[1].getXF();
		else if(ET[1].findLeftX() < ET[0].findLeftX())
			x1 = ET[1].getXF();
	}
	else if(ET.size() > 1 && rd(ET[1].getY0()) == y)
	{
		if (ET[1].findLeftX() > ET[0].findLeftX())
			x2 = ET[1].getX0();
		else if (ET[1].findLeftX() < ET[0].findLeftX())
			x1 = ET[1].getX0();
	}
	while (!ET.empty())
	{
		//remove edges from the active list if y == ymax 
		if (!AET.empty())
		{
			for (unsigned int i = 0; i < AET.size(); i++)
			{
				if (rd(AET[i].findYmax()) == y)
				{
					for (unsigned int j = 0; j < ET.size(); j++)
					{
						if (ET[j].getXF() == AET[i].getXF() && ET[j].getX0() == AET[i].getX0() && ET[j].getYF() == AET[i].getYF() && ET[j].getY0() == AET[i].getY0())
							ET.erase(ET.begin() + j);
					}
					AET.erase(AET.begin() + i);
					if(AET.size() != 0)
						i--;
				}
			}
		}

		//add edge from edge table to active list if y == ymin
		for (unsigned int i = 0; i < ET.size(); i++)
		{
			if (rd(ET[i].findYmin()) == y)
			{
				AET.push_back(ET[i]);
			}
		}

		//sort AET depending on their values
		if (AET.size() > 1 && AET[0].getY0() == AET[1].getY0())
		{
			if (AET[0].getXF() > AET[1].getXF())
			{
				AET.insert(AET.begin(), AET[1]);
				AET.pop_back();
			}
		}

		else if (AET.size() > 1 && AET[0].getY0() == AET[1].getYF())
		{
			if (AET[0].getXF() > AET[1].getX0())
			{
				AET.insert(AET.begin(), AET[1]);
				AET.pop_back();
			}
		}

		else if (AET.size() > 1 && AET[0].getYF() == AET[1].getYF())
		{
			if (AET[0].getX0() > AET[1].getX0())
			{
				AET.insert(AET.begin(), AET[1]);
				AET.pop_back();
			}
		}
		else if (AET.size() > 1 && AET[0].getYF() == AET[1].getY0())
		{
			if (AET[0].getX0() > AET[1].getXF())
			{
				AET.insert(AET.begin(), AET[1]);
				AET.pop_back();
			}
		}
		else
		{
			if (AET.size() > 1 && AET[0].findLeftX() > AET[1].findLeftX())
			{
				AET.insert(AET.begin(), AET[1]);
				AET.pop_back();
			}
		}


		//fill the polygon pixel between two x values of the two interceptions
		if (!AET.empty())
		{
			for (int x = rd(x1); x <= rd(x2); x++)
			{
				if(option == 1)
					MakePix(x, y, 0, 0, 1.0);
				else
					MakePix(x, y, 1.0, 0, 0);
			}
			//increment x values for the two edges based on slope
			if (rd(AET[0].getDx()) != 0)
				x1 = x1 + AET[0].getDx() / AET[0].getDy();

			if (AET.size() > 1)
			{
				if (rd(AET[1].getDx()) != 0)
					x2 = x2 + AET[1].getDx() / AET[1].getDy();
			}
		}
		y++;
	}
}

void Poly::clipPolygon(int option)
{
	vector<Point> output = vertice;
	// iterate xmin
	vector<Point> input = output;
	output.clear();
	Point S = input.back();
	for (unsigned int i = 0; i < input.size(); i++)
	{
		Point E = input[i];
		float y;
		if (E.getX() >= xmin)
		{
			if (S.getX() < xmin)
			{
				y = S.getY() + (E.getY() - S.getY()) * (xmin - S.getX()) / (E.getX() - S.getX());
				Point intercept((float)xmin, y);
				output.push_back(intercept);
			}
			output.push_back(E);
		}
		else if(S.getX() >= xmin)
		{
			y = E.getY() + (S.getY() - E.getY()) * (xmin - E.getX()) / (S.getX() - E.getX());
			Point intercept((float)xmin, y);
			output.push_back(intercept);
		}
		S = E;
	}
	
	//iterate ymin
	input = output;
	output.clear();
	if (input.size() > 0)
		S = input.back();
	for (unsigned int i = 0; i < input.size(); i++)
	{
		Point E = input[i];
		float x;
		if (E.getY() >= ymin)
		{
			if (S.getY() < ymin)
			{
				x = S.getX() + (E.getX() - S.getX()) * (ymin - S.getY()) / (E.getY() - S.getY());
				Point intercept(x, float(ymin));
				output.push_back(intercept);
			}
			output.push_back(E);
		}
		else if (S.getY() >= ymin)
		{
			x = E.getX() + (S.getX() - E.getX()) * (ymin - E.getY()) / (S.getY() - E.getY());
			Point intercept(x, float(ymin));
			output.push_back(intercept);
		}
		S = E;
	}

	//iterate xmax
	input = output;
	output.clear();
	if (input.size() > 0)
		S = input.back();
	for (unsigned int i = 0; i < input.size(); i++)
	{
		Point E = input[i];
		float y;
		if (E.getX() <= xmax)
		{
			if (S.getX() > xmax)
			{
				y = E.getY() + (S.getY() - E.getY()) * (xmax - E.getX()) / (S.getX() - E.getX());
				Point intercept((float)xmax, y);
				output.push_back(intercept);
			}
			output.push_back(E);
		}
		else if (S.getX() <= xmax)
		{
			y = S.getY() + (E.getY() - S.getY()) * (xmax - S.getX()) / (E.getX() - S.getX());
			Point intercept((float)xmax, y);
			output.push_back(intercept);
		}
		S = E;
	}
	//iterate ymax
	input = output;
	output.clear();
	if(input.size() > 0)
		S = input.back();
	for (unsigned int i = 0; i < input.size(); i++)
	{
		Point E = input[i];
		float x;
		if (E.getY() <= ymax)
		{
			if (S.getY() > ymax)
			{
				x = E.getX() + (S.getX() - E.getX()) * (ymax - E.getY()) / (S.getY() - E.getY());
				Point intercept(x, float(ymax));
				output.push_back(intercept);
			}
			output.push_back(E);
		}
		else if (S.getY() <= ymax)
		{
			x = S.getX() + (E.getX() - S.getX()) * (ymax - S.getY()) / (E.getY() - S.getY());
			Point intercept(x, float(ymax));
			output.push_back(intercept);
		}
		S = E;
	}

	if (output.size() != 0)
	{
		cout << output.size() << endl;
		Poly clippedPoly(output);
		if (fill)
			clippedPoly.fillPolygon(option);
		else
			clippedPoly.drawPolygon(option);
	}
}

void Poly::translate(float dx, float dy)
{
	for (int i = 0; i < size; i++)
		vertice[i].translate(dx, dy);
	
	edges.clear();
	for (int i = 0; i < size - 1; i++)
	{
		Line l(vertice[i], vertice[i + 1]);
		edges.insert(edges.begin() + i, l);
	}
	Line l(vertice[0], vertice[size - 1]);
	edges.insert(edges.end(), l);
}

void Poly::scale(float factor)
{
	float xc = findCentroid().getX(), yc = findCentroid().getY();
	for (int i = 0; i < size; i++)
	{
		translate(xc * -1, yc * -1);
		vertice[i].scale(factor);
		translate(xc, yc);
	}
	edges.clear();
	for (int i = 0; i < size - 1; i++)
	{
		Line l(vertice[i], vertice[i + 1]);
		edges.insert(edges.begin() + i, l);
	}
	Line l(vertice[0], vertice[size - 1]);
	edges.insert(edges.end(), l);
	
}

void Poly::rotate(float degree)
{
	float xc = findCentroid().getX(), yc = findCentroid().getY();
	for (int i = 0; i < size; i++)
	{
		translate(xc * -1, yc * -1);
		vertice[i].rotate(degree);
		translate(xc, yc);
	}
	edges.clear();
	for (int i = 0; i < size - 1; i++)
	{
		Line l(vertice[i], vertice[i + 1]);
		edges.insert(edges.begin() + i, l);
	}
	Line l(vertice[0], vertice[size - 1]);
	edges.insert(edges.end(), l);
}

Point Poly::findCentroid()
{
	float xmean = 0, ymean = 0;
	for (int i = 0; i < size; i++)
	{
		xmean += vertice[i].getX();
		ymean += vertice[i].getY();
	}
	float divider = (float) size;
	xmean /= divider;
	ymean /= divider;
	Point centroid(xmean, ymean);
	return centroid;
}

int Poly::findYmax()
{
	float yMax = edges[0].findYmax();
	for (int i = 1; i < size; i++)
	{
		if (yMax < edges[i].findYmax())
			yMax = edges[i].findYmax();
	}
	return  rd(yMax);
}

int Poly::findYmin()
{
	float yMin = edges[0].findYmin();
	for (int i = 1; i < size; i++)
	{
		if (yMin > edges[i].findYmin())
			yMin = edges[i].findYmin();
	}
	return rd(yMin);
}


Poly::Poly(int numberOfEdges): fill(false), size(numberOfEdges)
{
}

Poly::Poly(vector<Point> vertices) 
{
	fill = false;
	size = vertices.size();
	for (unsigned int i = 0; i < vertices.size(); i++)
		vertice.insert(vertice.begin() + i, vertices[i]);
	for (int i = 0; i < size - 1; i++)
	{
		Line l(vertice[i], vertice[i + 1]);
		edges.insert(edges.begin() + i, l);
	}
	Line l(vertice[0], vertice[size - 1]);
	edges.insert(edges.end(), l);
}

Poly::~Poly()
{
}
