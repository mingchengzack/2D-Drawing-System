#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include "utility.h"

using namespace std;

//rounding function
int rd(const float a) { return int(a + 0.5); }

//turn pixel(x,y) to (r, g, b) value
void MakePix(int x, int y, float r, float g, float b)
{
	//if it is inside the window
	if (x >= 0 && x < 200 && y >= 0 && y < 200)
	{
		PixelBuffer[600 * y + x * 3] = r;
		PixelBuffer[600 * y + x * 3 + 1] = g;
		PixelBuffer[600 * y + x * 3 + 2] = b;
	}
}

