#pragma once
#define GL_SILENCE_DEPRECATION
#define _USE_MATH_DEFINES
#include<GL/glut.h>
#include "wtypes.h"
#include <iostream>
#include <vector>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp> 

#include "Objects/pixel.h"
#include "scene.h"
#include "geometry.h"
#include "Objects/planet.h"
#include "trace.h"

using namespace std;
Scene scene;
std::vector<Vec3f> buffer;
int counter = 0;

Material     mercury(1.0, MercuryA, MercuryC, 10.);
Material     venus(1.0, VenusA, VenusC, 10.);
Material     earth(1.0, EarthA, EarthC, 10.);
Material     mars(1.0, MarsA, MarsC, 10.);

void initPlanets()
{
	Planet Earth(1, -100, 26, 26, 0, 4.5, earth);
	Planet Mercury(1.1, -100, 8, 8, 45, 2, mercury);
	Planet Venus(1.2, -100, 16, 16, 90, 2.7, venus);
	Planet Mars(1.4, -100, 36, 36, 135, 4, mars);

	planets.push_back(Earth);
	planets.push_back(Mercury);
	planets.push_back(Venus);
	planets.push_back(Mars);
}

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, GLdouble(width), 0, GLdouble(height));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void timer(int t)
{
	boost::this_thread::sleep_for(boost::chrono::milliseconds(t));
	counter += 6;
	
	for (auto &planet : planets) {
		planet.set_current_place(1);
	}

	glutPostRedisplay();
}

void renderScene(void)
{
	timer(10);

	buffer = dorender();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_POINTS);

	/*for (auto dot : scene.getDots()) {
		//сюда в 2д походу
		//либо сразу 2д готовть
		glColor3d(dot.color.r, dot.color.g, dot.color.b);
		glVertex2d(dot.x, dot.y);
	}*/

	for (size_t j = 0; j < 1000; j++) {
		for (size_t i = 0; i < 1000; i++) {
			Vec3f& c = buffer[i + j * 1000];

			glColor3d(c.x, c.y, c.z);
			glVertex2d(i, j);

			/*float max = std::max(c[0], std::max(c[1], c[2]));
			if (max > 1) c = c * (1. / max);
			for (size_t j = 0; j < 3; j++) {
				ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
			}*/
		}
	}

	glEnd();

	glutSwapBuffers();
	glutReshapeWindow(1000, 1000);

}

void keyboard(unsigned char key, int xmouse, int ymouse)
{
	switch (key) {
	case 'w':
	{
		camera.y++;
	}
	break;

	case 's':
	{
		camera.y--;
	}
	break;

	case 'a':
	{
		camera.x--;
	}
	break;

	case 'd':
	{
		camera.x++;
	}
	break;

	default:
		break;
	}
	glutPostRedisplay(); //request display() call ASAP
}

void specialKeyboard(int key, int xmouse, int ymouse)
{
	switch (key) {
	
	case GLUT_KEY_LEFT:
	{
		ay += 0.1 * 0.175;
	}
	break;
	
	case GLUT_KEY_RIGHT:
	{
		ay -= 0.1 * 0.175;
	}
	break;
	
	case GLUT_KEY_UP:
	{
		ax += 0.1 * 0.175;
	}
	break;
	
	case GLUT_KEY_DOWN:
	{
		ax -= 0.1 * 0.175;
	}
	break;

	case GLUT_KEY_PAGE_UP:
	{
		camera.z++;
	}
	break;
	
	case GLUT_KEY_PAGE_DOWN:
	{
		camera.z--;
	}
	break;

	default:
		break;
	}
	glutPostRedisplay(); //request display() call ASAP
}

class Display 
{
	int width = 1024;
	int height = 800;
	int posx = 0;
	int posy = 0;
	const char* name = "окно";

public:
	void init(int argc, char** argv) 
	{
		initPlanets();

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //GLUT_DOUBLE когда дойду до анимации //GLUT_DEPTH - Буфер глубины 
		GetDesktopResolution(width, height);
		glutInitWindowPosition(10, 10);
		glutInitWindowSize(1000, 1000);
		glutCreateWindow(name);
		//glMatrixMode(GL_PROJECTION);
		//glClearColor(0.0, 0.0, 0.0, 0.0);
		//gluOrtho2D(0.0, 350.0, 0.0, 350.0);
		//glutReshapeFunc(reshape);
		//glutKeyboardFunc(keyboard);

		glutDisplayFunc(renderScene);
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialKeyboard);
		glutReshapeFunc(reshape);
		glutIdleFunc(renderScene);

		glutReshapeWindow(width-100, height-100);
	}
};