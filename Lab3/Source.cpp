#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GL/glut.h>
#include <math.h>

#define WIN_HEIGHT 750
#define WIN_WIDTH 750

GLfloat ox_rotation = 0;
GLfloat dx_rotation = 0.5;
GLfloat oy_rotation = 0;
GLfloat dy_rotation = 0;

GLfloat lightPos[] = { 0, 0, 0, 1};
bool lightOn = true;
GLfloat sunRotation = 0;
GLfloat dxSunRotation = 1;
int sun_mode = 1;

GLfloat opened = 0;
bool isOpened = false;

void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH, GL_NICEST);

	glTranslatef(0, 0, -30);
	glRotatef((sunRotation), 0, 1, 0);
	glTranslatef(0, 0, -20);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightPos);
	//glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 128);
	//glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 30);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
	glColor3d(1, 1, 0);
	gluQuadricDrawStyle(quadObj, GLU_LINE);
	gluSphere(quadObj, 2, 20, 20);

	glLoadIdentity();
	glTranslatef(0, 0, -20);
	glRotatef((ox_rotation), 0, 1, 0);
	glRotatef((oy_rotation), 1, 0, 0);

	if (isOpened) {
		if (opened < 1.5)
			opened += 0.1;
		}
	else {
		if (opened > 0.0) {
			opened -= 0.1;
			if (opened < 0.0)
				opened = 0;
		}
	}
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);  // green
	glNormal3d(-1, -1, 1);
	glVertex3f(0 + opened , 10 + opened, 0 - opened);
	glVertex3f(0 + opened, 0 + opened, -10 - opened);
	glVertex3f(10 + opened, 0 + opened, 0 - opened);

	glColor3f(0.0, 1.0, 0.0);  // red
	glNormal3d(-1, -1, -1);
	glVertex3f(0 + opened, 10 + opened, 0 + opened);
	glVertex3f(10 + opened, 0 + opened, 0 + opened);
	glVertex3f(0 + opened, 0 + opened, 10 + opened);

	glColor3f(0.0, 0.0, 1.0);  // blue
	glNormal3d(1, -1, -1);
	glVertex3f(0 - opened, 10 + opened, 0 + opened);
	glVertex3f(0 - opened, 0 + opened, 10 + opened);
	glVertex3f(-10 - opened, 0 + opened, 0 + opened);

	glColor3f(1.0, 0.0, 1.0);  // purple
	glNormal3d(1, -1, -1);
	glVertex3f(0 + opened, 0 - opened, -10 - opened);
	glVertex3f(10 + opened, 0 - opened, 0 - opened);
	glVertex3f(0 + opened, -10 - opened, 0 - opened);

	glColor3f(1.0, 1.0, 0.0);  // yellow
	glNormal3d(-1, 1, -1);
	glVertex3f(0 + opened, 0 - opened, 10 + opened);
	glVertex3f(10 + opened, 0 - opened, 0 + opened);
	glVertex3f(0 + opened, -10 - opened, 0 + opened);

	glColor3f(0.0, 1.0, 1.0);  // cyan
	glNormal3d(-1, -1, 1);
	glVertex3f(0 - opened, 0 - opened, 10 + opened);
	glVertex3f(-10 - opened, 0 - opened, 0 + opened);
	glVertex3f(0 - opened, -10 - opened, 0 + opened);

	glColor3f(1.0, 1.0, 1.0);  // white
	glNormal3d(-1, 1, -1);
	glVertex3f(0 - opened, 10 + opened, 0 - opened);
	glVertex3f(0 - opened, 0 + opened, -10 - opened);
	glVertex3f(-10 - opened, 0 + opened, 0 - opened);

	glColor3f(1.0, 0.0, 0.0);  // rgb
	glNormal3d(1, 1, 1);
	//  (-1, -1, -1)
	glVertex3f(0 - opened, 0 - opened, -10 - opened);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-10 - opened, 0 - opened, 0 - opened);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0 - opened, -10 - opened, 0 - opened);

	glEnd();

	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h) {
	if (h == 0)
		h = 1;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (GLdouble)w / h, 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, (GLdouble)w, (GLdouble)h);

	Draw();
}

void TimerFunction(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
	ox_rotation += dx_rotation;
	oy_rotation += dy_rotation;
	sunRotation += dxSunRotation;
}

void glutNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		dx_rotation = -0.5;
		break;
	case 'd':
		dx_rotation = 0.5;
		break;
	case 'w':
		dy_rotation = -0.5;
		break;
	case 's':
		dy_rotation = 0.5;
		break;
	case 'q':
		dy_rotation = 0;
		dx_rotation = 0;
		break;
	case 'e':
		if (!isOpened) 
			isOpened = true;		
		else
			isOpened = false;
		break;
	case 'r':
		if (sun_mode >= 0) {
			sun_mode--;
			dxSunRotation--;
		}
		else {
			sun_mode = 1;
			dxSunRotation = 1;
		}
		break;
	case 'z':
		exit(0);
		break;
	case 'f':
		if (lightOn)
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
		lightOn = !lightOn;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	std::cout << "W - Turn up\nA - Turn left\nS - turn down\nD - Turn right\nQ - Stop rotation of octahedron\nE - Open octahedron\nF - Turn the light on/off\nR - Change sun rotation\nZ - Close the programm\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Octahedron");
	glutDisplayFunc(Draw);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(5, TimerFunction, 0.5);
	glutKeyboardFunc(glutNormalKeys);

	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}