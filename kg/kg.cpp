#include <iostream>

#include "display.h"
#include "draw.h"
#include "trace.h"

int main(int argc, char** argv) 
{

	Display window;

	Color color;
	color.set_color(1, 0, 0);
	
	//line(0, 0, 100, 100, scene, color);

	window.init(argc, argv);

	glutMainLoop();
	return 0;
}