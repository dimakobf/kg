#include <iostream>

#include "display.h"
#include "trace.h"

#include "image.h"

int main(int argc, char** argv) 
{

	Display window;

	window.init(argc, argv);

	glutMainLoop();
	return 0;
}