# 2D Drawing System
Includes DDA and Bresenham line drawing algorithms, the algorithm for rasterizing polygons, the algorithm for two-dimensional clipping, and the two-dimensional transformations for scaling a polygon with respect to its centroid,translating a polygon by a translation vector and rotating a polygon about its centroid by an arbitrary angle.
To compile my project, just use make command and the makefile to compile. The name of my executable is called Project1.

To run my program, just type in ./Project1. I have a input file called "test", which contains three different polygons, 
and I have a output file called "output" for the transformed polygons in the same format as "test".

I have a fixed window size for my program which is 200 x 200;
My program is using glutCreateMenu(), so after the program is run, I first show the 3 polygons in the window with shape.
Then the user can right-click the window to see the menu. For the menu, I have 5 sub menus: 

1. Set the Line mode, which 
allows the user to select between DDA and Bresenham to draw lines. (DDA line is drawn in blue, Bresenham line is drawn in red).

2. Line drawing. This submenu allows the user to test both DDA and Bresenham algorithms. After the user clicks one of those,
it will prompt the user to enter the start point coordinates and end point coordinates in x y format via command line.
Attention : I implemnted all the user inputs assuming the input format is correct. Please use the correct format to test the feature!!

3. Clipping. This submenu allows the user to test both line clipping and polygon clipping. For line clipping, the user is prompted to enter the
x, y coordinates in x y format as well as the clipping window in xmin ymin xmax ymax format. The program will also display the clipping window as a
different color with the line depending on which line draw mode is (for example, if it is DDA, then the clipping window will be in Red), which allows
for better testing to see if the output is correct. For clipping polygon, the user only needs to specify the clipping window size, then all polygons after 
clipping will show on the window. (if the polygon is not rasterized yet then the respective clipping polygon is also not rasterized). The key thing about 
this feature is that the clipping window is not set for polygons transformation, so only in this submenu, my program will show the clipping window and 
clipped polygons and line.

4.Rasterization. This submenu allows the user to specify which polygons to specify which polygons to rasterize. Since I am implementing the menus with my input file
, which only contains 3 polygons. According to the order of the polygons in the input file, I named them A, B, C respectively. (For a input file with 4 polygons or more, 
this menu item would not work. However, if the menu items can be changed accordingly, this program can accommadate any number of polygons.) The user can choose a polygon 
out of A, B, C to rasterize. Again, I am assuming the user will enter the correct format, please use the correct format for the input (A, B or C). After a polygon is rasterized
, it cannot be un-rasterized, since I did not implement this feature, but restart the program will make the polygons un-rasterized.

5. Transformation. This submenu allows the user to transform each polygon. I have translationg, scaling and rotation, 3 submenus under this submenu. For each of these transformations,
the user needs to specify which polygon to manipulate as 4., (A, B or C). Then for each feature, the user needs to specify the translation vector in the format of dx dy, 
the scaling factor, or the rotation degrees in degreess(not in radians!!). In this menu, the window will always display the transformed polygon unless the user moved to menu 1,
2, 3. (If the user moves to menu 1 or 2, all polygons will disappear, but the changes made to them are saved. If the user moves to menu 3, after the user specifies the clipping
windows(the viewport), it will show the clipped polygon after the transformation in the specified window size.


For DDA algorithm, I implemented in line.cpp, line 293 - 312, with class line.
For Bresenham algorithm, I implemented in line.cpp, line 166 -291, with class line.
For rasterization algorithm, I implemented in polygon.cpp, line 56 - 215, with class polygon.
For line clipping algorithm, I implemented in line.cpp with class line, using computing the 4bit code function implemented in point.cpp, 
line 49 - 62, line 88 - 156, with class point.
For polygon clipping algorithm, I implemented in polygon.cpp, line 217 - 338. 
For translation, I implemented in polygon.cpp, line 340 - 353 with class polygon, using translation function for class point in point.cpp, line 24 - 28.
For scaling, I implemented in polygon.cpp, 1ine 355 - 373 with class polygon, using scaling funciton for class point in point.cpp, line 30 - 34.
For rotation, I implemented in polygon.cpp, line 375 - 392, using rotation function for class point in point.cpp, line 36 - 42.

I believe I implemented all parts in the project except that I consider clipping window as the same as the viewport. Also, my rasterization algorithm is
not suitable for all polygons especially with concave polygons, but it will works for most of the convex polygons. For this algorithim, I implemented using
a scan line based algorithm, and I used edge table and active list to keep track of the edges that are being processed or scanned. Since I am assuming every
scan line will only get maximum two interceps for edges, it would not work for concave polygons which might have more than two intercepts. Because my program
always assumes the user enters the right format for input, if the user enters the wrong format, something strange will happened or even crash. Just restart the 
program when this happens.
