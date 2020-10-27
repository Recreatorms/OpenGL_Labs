#include <GL/glut.h>
#include <cmath>


float shiftX = 0.0,
	  shiftY = 0.0,
	  nightOpacity = 0.0;
bool day = true;
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(17, timer, 0);
}

void reshape(int w, int h) {
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}
void renderScene(void) {

	

	glBegin(GL_QUADS);
		glColor3f(0.0, 0.6, 0.9); // sky
		glVertex2i(-1, 0);
		glVertex2i(-1, 1);
		glVertex2i(1, 1);
		glVertex2i(1, 0);

		glColor3f(0.4, 1.0, 0.0); // terrain
		glVertex2f(-1.0, -1.0);
		glVertex2f(-1.0, 0.0);
		glVertex2f(1.0, 0.0);
		glVertex2f(1.0, -1.0);
		
		glColor3f(0.0, 0.0, 0.0); //house (outline)
		glVertex2f(0.1, -0.5);
		glVertex2f(0.1, 0.0);
		glVertex2f(0.6, 0.0);
		glVertex2f(0.6, -0.5);

		glColor3f(1.0, 1.0, 0.0); // house
		glVertex2f(0.11, -0.49);
		glVertex2f(0.11, 0.01);
		glVertex2f(0.59, 0.01);
		glVertex2f(0.59, -0.49);

		glColor3f(0.0, 0.0, 0.0); // window (outline)
		glVertex2f(0.25, -0.18);
		glVertex2f(0.25, -0.38);
		glVertex2f(0.45, -0.38);
		glVertex2f(0.45, -0.18);

				
	glEnd();
	glBegin(GL_TRIANGLES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.00, -0.1);
		glVertex2f(0.35, 0.3);
		glVertex2f(0.7, -0.1);

		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(0.03, -0.09);
		glVertex2f(0.35, 0.28);
		glVertex2f(0.67, -0.09);
	glEnd();
	glBegin(GL_QUADS);

	glColor4f(0.0, 0.0, 0.0, nightOpacity);
		glVertex2i(-1, -1);
		glVertex2i(-1, 1);
		glVertex2i(1, 1);
		glVertex2i(1, -1);
		if (day)
			glColor3f(0.0, 0.4, 0.7); // window
		else
			glColor3f(1.0, 1.0, 0.0);
		glVertex2f(0.26, -0.19);
		glVertex2f(0.26, -0.37);
		glVertex2f(0.44, -0.37);
		glVertex2f(0.44, -0.19);
	glEnd();
	glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(0.35, -0.18);
		glVertex2f(0.35, -0.38);
		glVertex2f(0.25, -0.28);
		glVertex2f(0.45, -0.28);
	glEnd();

	if (shiftX > 1.5)
		if (day)
			nightOpacity += 0.0014;
		else
			nightOpacity -= 0.0009;
	if (shiftX > 3.0) {
		shiftX = 0.0;
		nightOpacity = 0.5;
		if (day)
			day = false;
		else {
			nightOpacity = 0.0;
		day = true;
	}
	}
	shiftX += 0.005;
	glBegin(GL_POLYGON);
		if (day)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(1.0, 1.0, 1.0);
		float r = 0.1,
		      offsetX = -1.5,
			  offsetY = 0.7;
		for (float i = 0; i < 6.28; i+=0.1) 
			glVertex2f(offsetX + r*cos(i) + shiftX, offsetY + r*sin(i));
	glEnd();


	glutSwapBuffers();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("MyFirstOpenGLProgram");
	/*myinit();*/

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, timer, 0);
	//glutReshapeFunc(reshape);
	glutMainLoop();
}
