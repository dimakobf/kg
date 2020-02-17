#pragma once
#define GL_SILENCE_DEPRECATION
#define _USE_MATH_DEFINES
#include<GL/glut.h>
#include "wtypes.h"
#include <iostream>
#include <vector>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp> 

#include "scene.h"
#include "geometry.h"
#include "Objects/planet.h"
#include "trace.h"
using namespace std;
std::vector<Vec3f> buffer;
int speed = 1;
int frame = 0, time1, timebase = 0;
const int width = 1000;
const int height = 1000;

Material     mercury(1.0, MercuryA, MercuryC, 10.);
Material     venus(1.0, VenusA, VenusC, 10.);
Material     earth(1.0, EarthA, EarthC, 10.);
Material     mars(1.0, MarsA, MarsC, 10.);

Material     moon(1.0, MoonA, MoonC, 10.);


auto image_earth = get_image("images/2k_earth_daymap.jpg");
auto image_mercury = get_image("images/2k_mercury.jpg");
auto image_venus = get_image("images/2k_venus_atmosphere.jpg");
auto image_mars = get_image("images/2k_mars.jpg");

auto image_moon = get_image("images/2k_moon.jpg");
auto image_sun = get_image("images/2k_sun.jpg");

void initPlanets()
{
	Planet Earth(1, -100, 26, 26, 0, 4.5, earth, &image_earth);
	Planet Mercury(1.1, -100, 8, 8, 45, 2, mercury, &image_mercury);
	Planet Venus(1.2, -100, 16, 16, 90, 2.7, venus, &image_venus);
	Planet Mars(1.4, -100, 36, 36, 135, 4, mars, &image_mars);

	planets.push_back(Earth);
	planets.push_back(Mercury);
	planets.push_back(Venus);
	planets.push_back(Mars);

	Material sun(1.0, Vec4f(1.0, 0.1, 0.0, 0.5), Vec3f(1.0, 1.0, 0.0), 0.);
	sun.light_source = true;
	Planet s(0, -100, 0, 0, 0, 2, sun, &image_sun);
	planets.push_back(s);

	Moon moon(Earth.sphere.center, 2, -100, 6, 6, 0, 1, moon, &image_moon);
	moons.push_back(moon);

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
	
	for (auto &planet : planets) {
		planet.set_current_place(1);
	}
	
	for (auto &m : moons) {
		m.set_current_place(planets[0].sphere.center, 1);
	}

	glutPostRedisplay();
}

void renderScene(void)
{
	timer(10);

	dorender(buffer);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_POINTS);

	for (size_t j = 0; j < 1000; j++) {
		for (size_t i = 0; i < 1000; i++) {
			Vec3f& c = buffer[i + j * 1000];

			glColor3d(c.x, c.y, c.z);
			glVertex2d(i, j);

		}
	}

	glEnd();

	frame++;
	time1 = glutGet(GLUT_ELAPSED_TIME);
	if (time1 - timebase > 1000) {
		std::cout << "FPS: " << (frame * 1000.0 / (time1 - timebase)) << std::endl;
		timebase = time1;
		frame = 0;
	}

	glutSwapBuffers();
	glutReshapeWindow(1000, 1000);

}

void keyboard(unsigned char key, int xmouse, int ymouse)
{
	switch (key) {
	case 'w':
	{
		camera.y += speed;
	}
	break;

	case 's':
	{
		camera.y -= speed;
	}
	break;

	case 'a':
	{
		camera.x -= speed;
	}
	break;

	case 'd':
	{
		camera.x += speed;
	}
	break;

	case '1':
	{
		speed = 1;
	}
	break;
	
	case '2':
	{
		speed = 2;
	}
	break;
	
	case '3':
	{
		speed = 3;
	}
	break;

	case '4':
	{
		speed = 4;
	}
	break;
	
	case '5':
	{
		speed = 5;
	}
	break;

	case '0':
	{
		camera.x = 0;
		camera.y = 0;
		camera.z = 0;
		ax = 0;
		ay = 0;
	}
	break;

	case '=':
	{
		camera.z += speed * speed;
	}
	break;

	case '-':
	{
		camera.z -= speed * speed;
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
		ay += 0.1 * 0.175 * speed;
	}
	break;
	
	case GLUT_KEY_RIGHT:
	{
		ay -= 0.1 * 0.175 * speed;
	}
	break;
	
	case GLUT_KEY_UP:
	{
		ax += 0.1 * 0.175 * speed;
	}
	break;
	
	case GLUT_KEY_DOWN:
	{
		ax -= 0.1 * 0.175 * speed;
	}
	break;

	case GLUT_KEY_PAGE_UP:
	{
		camera.z += speed * speed;
	}
	break;
	
	case GLUT_KEY_PAGE_DOWN:
	{
		camera.z -= speed * speed;
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
		buffer.resize(1000 * 1000);
		initPlanets();

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); //GLUT_DOUBLE когда дойду до анимации //GLUT_DEPTH - Буфер глубины 
		GetDesktopResolution(width, height);
		glutInitWindowPosition(10, 10);
		glutInitWindowSize(1000, 1000);
		glutCreateWindow(name);

		glutDisplayFunc(renderScene);
		glutKeyboardFunc(keyboard);
		glutSpecialFunc(specialKeyboard);
		glutReshapeFunc(reshape);
		glutIdleFunc(renderScene);

		glutReshapeWindow(width-100, height-100);
	}
};