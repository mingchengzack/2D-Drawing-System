#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include "utility.h"
#include "line.h"
#include "polygon.h"

using namespace std;

static int mode;
static int MainWindow;
static int lineDrawing;
static int rasterization;
static int clipping;
static int transformation;
static int MainMenu;

int option = 2; // the option to use DDA or Bresenham

void createMenu();
void menu(int value);
void clear();
void display();
int val;
float* PixelBuffer;
int numberOfPolygons;
vector<Poly> Polygons;
int xmin, ymin, xmax, ymax;

void createMenu()
{
	mode = glutCreateMenu(menu);
	glutAddMenuEntry("Set to DDA", 1);
	glutAddMenuEntry("Set to Bresenham", 2);

	lineDrawing = glutCreateMenu(menu);
	glutAddMenuEntry("DDA", 3);
	glutAddMenuEntry("Bresenham", 4);

	clipping = glutCreateMenu(menu);
	glutAddMenuEntry("Line Clipping", 5);
	glutAddMenuEntry("Polygon Clipping", 6);

	rasterization = glutCreateMenu(menu);
	glutAddMenuEntry("A", 7);
	glutAddMenuEntry("B", 8);
	glutAddMenuEntry("C", 9);

	transformation = glutCreateMenu(menu);
	glutAddMenuEntry("Translation", 10);
	glutAddMenuEntry("Scaling", 11);
	glutAddMenuEntry("Rotation", 12);

	MainMenu = glutCreateMenu(menu);

	glutAddSubMenu("Set Line Draw Mode", mode);
	glutAddSubMenu("Line Drawing", lineDrawing);
	glutAddSubMenu("Clipping", clipping);
	glutAddSubMenu("Rasterization", rasterization);
	glutAddSubMenu("Transformation", transformation);
	
	//choose 0 to exit
	glutAddMenuEntry("Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int value)
{
	//exit
	if(value != val)
		clear();
	if (value == 0)
	{
		glutDestroyWindow(MainWindow);
		ofstream outf;
		outf.open("output");
		outf << numberOfPolygons << endl;
		outf << '\n';
		
		for (unsigned int i = 0; i < Polygons.size(); i++)
		{
			outf << Polygons[i];
		}
		outf.close();
		exit(0);
	}
	//set to DDA mode
	if (value == 1)
	{
		option = 1;
	}
	//set to Bresenham mode
	else if (value == 2)
	{
		option = 2;
	}
	//draw a line using DDA
	else if (value == 3)
	{
		float x0, y0, xEnd, yEnd;
		cout << "Enter the start point in the format of x y: ";
		cin >> x0 >> y0;
		cout << "Enter the end point in the format of x y: ";
		cin >> xEnd >> yEnd;
		Line l(Point(x0, y0), Point(xEnd, yEnd));
		l.drawLineDDA();
	}
	//draw a line using Bresenham
	else if (value == 4)
	{
		float x0, y0, xEnd, yEnd;
		cout << "Enter the start point in the format of x y: ";
		cin >> x0 >> y0;
		cout << "Enter the end point in the format of x y: ";
		cin >> xEnd >> yEnd;
		Line l(Point(x0, y0), Point(xEnd, yEnd));
		l.drawLineBresenham();
	}
	//draw a clipped line with user specified clipping window
	else if (value == 5)
	{
		clear();
		//get the line from user
		float x0, y0, xEnd, yEnd;
		cout << "Enter the start point in the format of x y: ";
		cin >> x0 >> y0;
		cout << "Enter the end point in the format of x y: ";
		cin >> xEnd >> yEnd;
		
		//get the clipping window from user
		cout << "Enter the clipping window size in the following format \n(xmin ymin xmax ymax in the range of 200 x 200 of the window size) :";
		cin >> xmin >> ymin >> xmax >> ymax;

		vector<Point> vertices;
		Point p1((float)xmin, (float)ymin);
		Point p2((float)xmin, (float)ymax);
		Point p3((float)xmax, (float)ymax);
		Point p4((float)xmax, (float)ymin);

		vertices.insert(vertices.begin(), p1);
		vertices.insert(vertices.begin() + 1, p2);
		vertices.insert(vertices.begin() + 2, p3);
		vertices.insert(vertices.begin() + 3, p4);
		Poly window(vertices);
		window.drawPolygon(option);

		Line l(Point(x0, y0), Point(xEnd, yEnd));
		l.clipLine(option);

	}
	//draw a clipped polygon with user specified clipping window
	else if (value == 6)
	{
		clear();
		cout << "Enter the clipping window size in the following format \n(xmin ymin xmax ymax in the range of 200 x 200 of the window size) :";
		cin >> xmin >> ymin >> xmax >> ymax;
		vector<Point> vertices;
		Point p1((float)xmin, (float)ymin);
		Point p2((float)xmin, (float)ymax);
		Point p3((float)xmax, (float)ymax);
		Point p4((float)xmax, (float)ymin);

		vertices.insert(vertices.begin(), p1);
		vertices.insert(vertices.begin() + 1, p2);
		vertices.insert(vertices.begin() + 2, p3);
		vertices.insert(vertices.begin() + 3, p4);
		Poly window(vertices);
		if (option == 1)
			window.drawPolygon(2);
		else
			window.drawPolygon(1);

		for (int i = 0; i < numberOfPolygons; i++)
			Polygons[i].clipPolygon(option);
	}
	//choose A to fill
	else if (value == 7)
	{
		clear();
		Polygons[2].setFill();
		for (int i = 0; i < numberOfPolygons; i++)
		{
			if (Polygons[i].ifFilled())
				Polygons[i].fillPolygon(option);
			else
				Polygons[i].drawPolygon(option);
		}
	}
	//choose B to fill
	else if (value == 8)
	{
		clear();
		Polygons[0].setFill();
		for (int i = 0; i < numberOfPolygons; i++)
		{
			if (Polygons[i].ifFilled())
				Polygons[i].fillPolygon(option);
			else
				Polygons[i].drawPolygon(option);
		}
	}
	//choose C to fill
	else if(value == 9)
	{
		clear();
		Polygons[1].setFill();
		for (int i = 0; i < numberOfPolygons; i++)
		{
			if (Polygons[i].ifFilled())
				Polygons[i].fillPolygon(option);
			else
				Polygons[i].drawPolygon(option);
		}
	}
	//choose one polygon to translate
	else if (value == 10)
	{
		clear();
		char choice;
		cout << "Choose a polygon to manipulate (A, B, C, from left to right): ";
		cin >> choice;

		float dx, dy;
		cout << "Enter the translation vector in the format of dx dy: ";
		cin >> dx >> dy;

		if (choice == 'A')
		{
			Polygons[2].translate(dx, dy);
		}
		else if (choice == 'B')
		{
			Polygons[0].translate(dx, dy);
		}
		else if (choice == 'C')
		{
			Polygons[1].translate(dx, dy);
		}
		for (int i = 0; i < numberOfPolygons; i++)
		{
			if (Polygons[i].ifFilled())
				Polygons[i].fillPolygon(option);
			else
				Polygons[i].drawPolygon(option);
		}
	}
	//choose a polygon to scale with respect to centroid
	else if (value == 11)
	{
		clear();
		char choice;
		cout << "Choose a polygon to manipulate (A, B, C, from left to right): ";
		cin >> choice;

		float factor;
		cout << "Enter the scale vector: ";
		cin >> factor;

		if (choice == 'A')
		{
			Polygons[2].scale(factor);
		}
		else if (choice == 'B')
		{
			Polygons[0].scale(factor);
		}
		else if (choice == 'C')
		{
			Polygons[1].scale(factor);
		}
		for (int i = 0; i < numberOfPolygons; i++)
		{
			if (Polygons[i].ifFilled())
				Polygons[i].fillPolygon(option);
			else
				Polygons[i].drawPolygon(option);
		}

	}
	//choose a polygon to rotate with respect to centroid
	else if (value == 12)
	{
		clear();
		char choice;
		cout << "Choose a polygon to manipulate (A, B, C, from left to right): ";
		cin >> choice;

		float degree;
		cout << "Enter the rotation degree (in degrees): ";
		cin >> degree;

		if (choice == 'A')
		{
			Polygons[2].rotate(degree);
		}
		else if (choice == 'B')
		{
			Polygons[0].rotate(degree);
		}
		else if (choice == 'C')
		{
			Polygons[1].rotate(degree);
		}
		for (int i = 0; i < numberOfPolygons; i++)
		{
			if (Polygons[i].ifFilled())
				Polygons[i].fillPolygon(option);
			else
				Polygons[i].drawPolygon(option);
		}
	}
	val = value;
	glutPostRedisplay();
}

void clear()
{
	for (int i = 0; i < 120000; i++)
		PixelBuffer[i] = 0;
}

int main(int argc, char *argv[])
{
	//allocate buffer
	PixelBuffer = new float[200 * 200 * 3];

	int numberOfEdges;
	ifstream inf("test2");
	inf >> numberOfPolygons;

	//read the polygons and display them at the beginning of the program
	for (int i = 0; i < numberOfPolygons; i++)
	{
		inf >> numberOfEdges;
		Poly polygon(numberOfEdges);
		inf >> polygon; // store the edges and draw the outline of the polygon
		Polygons.insert(Polygons.begin() + i, polygon);
		polygon.drawPolygon(option);
	}
	inf.close();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	//set window size to 500*500
	glutInitWindowSize(200, 200);
	//set window position
	glutInitWindowPosition(100, 100);
	//create and set main window title
	MainWindow = glutCreateWindow("Project 1");

	createMenu();

	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL

	glutDisplayFunc(display);

	glutMainLoop();//main display loop, will display until terminate

	return 0;
}

//main display loop, this function will be called again and again by OpenGL

void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(200, 200, GL_RGB, GL_FLOAT, PixelBuffer);
	
	//window refresh
	glFlush();
}
