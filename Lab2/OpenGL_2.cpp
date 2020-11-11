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

void draw_house() {
	
	glColor3ub(255, 255, 0);
	glRectf(0.11f, -0.49f, 0.59f, 0.01f);

	// house roof
	glColor3ub(255, 0, 0);

	GLfloat verts[6];
	verts[0] = 0.0f;
	verts[1] = -0.1f;
	verts[2] = 0.35f;
	verts[3] = 0.3f;
	verts[4] = 0.7f;
	verts[5] = -0.1f;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_VERTEX_ARRAY);

	// window
	glColor3f(0, 0, 0);
	glRectf(0.25f, -0.18f, 0.45f, -0.38f);
	if (day)
		glColor3f(0.0, 0.4, 0.7); // window
	else
		glColor3f(1.0, 1.0, 0.0);
	glRectf(0.26f, -0.19f, 0.44f, -0.37f);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex2f(0.35, -0.18);
	glVertex2f(0.35, -0.38);
	glVertex2f(0.25, -0.28);
	glVertex2f(0.45, -0.28);
	glEnd();
	glFlush();
}

void drawSun() {
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
	for (float i = 0; i < 6.28; i += 0.1)
		glVertex2f(offsetX + r * cos(i) + shiftX, offsetY + r * sin(i));
	glEnd();


}
void background() {

	glColor3ub(0, 160, 240);
	GLfloat sky[8];
	sky[0] = -1.0f;
	sky[1] =  0.0f;
	sky[2] = -1.0f;
	sky[3] =  1.0f;
	sky[4] =  1.0f;
	sky[5] =  1.0f;
	sky[6] =  1.0f;
	sky[7] =  0.0f;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, sky);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glColor3ub(20, 170, 0);
	GLfloat verts[8];
	verts[0] = -1.0f;
	verts[1] = -1.0f;
	verts[2] = -1.0f;
	verts[3] =  0.0f;
	verts[4] =  1.0f;
	verts[5] =  0.0f;
	verts[6] =  1.0f;
	verts[7] = -1.0f;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

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
}
void scene() {

	background();
	draw_house();
	drawSun();

}

void renderScene(void) {

	scene();
	glFlush();
	glutSwapBuffers();


}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Laba2_s_massivami_vershin");
	/*myinit();*/

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glutDisplayFunc(renderScene);
	glutTimerFunc(16, timer, 0);
	//glutReshapeFunc(reshape);
	glutMainLoop();
}