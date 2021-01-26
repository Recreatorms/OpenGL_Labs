#include <iostream>
#include <time.h>
#include <cstdlib>
#include <GL/glut.h>
#include <math.h>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "./third_party/stb_image.h"

#define WIN_HEIGHT 750
#define WIN_WIDTH 750
#define SIDE_LENGTH 10
#define NUMBER_OF_PARTS 3
#define TEXTURES_COUNT 8

GLfloat speed = 0.5;
GLfloat ox_rotation = 0;
GLfloat dx_rotation = speed;
GLfloat oy_rotation = 0;
GLfloat dy_rotation = 0;

GLfloat lightPos[] = { 0, 0, 0, 1 };
bool lightOn = true;
GLfloat sunRotation = 0;
GLfloat dxSunRotation = 1;
int sun_mode = 1;

GLfloat opened = 0;
bool isOpened = false;
GLfloat detachLength = 1.0;

bool transparent = false;
bool cutOct = false;

GLuint cutOctahedron;
GLuint cutOctahedronOpen;

bool textured = false;
GLuint textures[TEXTURES_COUNT];
int textsum = sizeof(textures) / sizeof(textures[0]);
struct text_img {
	unsigned char * data;
	int sizeX, sizeY, n;
};
char text_name[8][21] {
	"./textures/text0.jpg", "./textures/text1.jpg", "./textures/text2.jpg", "./textures/text3.jpg", "./textures/text4.jpg", "./textures/text5.jpg", "./textures/text6.jpg", "./textures/text7.jpg"
};

GLfloat oct_textures_coordinates[] = {
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		0.5, 1.0
};
void load_textures()
{
	text_img* texture = new text_img[8];
	//size_t texture_pos = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(8, textures);

	for (int i(0), texture_pos(0); i < 8; i++) {
		texture[i].data = stbi_load(text_name[texture_pos], &texture[i].sizeX, &texture[i].sizeY, &texture[i].n, STBI_rgb);
		if (texture[i].data == nullptr) {
			std::cout << "it's seems some troubles with textures loading\n";
			exit(0);
		}

		texture_pos++;

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture[i].sizeX, texture[i].sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture[i].data);
	}

	delete[] texture;
}
/*
void initTextures() {
	int width[TEXTURES_COUNT],
		height[TEXTURES_COUNT];

	glGenTextures(TEXTURES_COUNT, textures);
	unsigned char *images[TEXTURES_COUNT];

	std::stringstream filename;
	for (int i = 0; i < TEXTURES_COUNT; i++) {
		filename << "../textures/text" << i << ".jpg";
		images[i] = SOIL_load_image(filename.str().c_str(), &width[i], &height[i],
			nullptr, SOIL_LOAD_RGB);

		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0,
			GL_RGB, GL_UNSIGNED_BYTE, images[i]);

		// Texture rendering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		filename.str("");
	}

	for (auto &image : images) {
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Object unlinking
	}
}*/
namespace OCT_FACES {
	const GLubyte front[] = { 0, 1, 2 };
	const GLubyte right_side[] = { 3, 4, 5 };
	const GLubyte back[] = { 6, 7, 8 };
	const GLubyte left_side[] = { 9, 10, 11 };
	const GLubyte lower_front[] = { 12, 13, 14 };
	const GLubyte lower_right_side[] = { 15, 16, 17 };
	const GLubyte lower_back[] = { 18, 19, 20 };
	const GLubyte lower_left_side[] = { 21, 22, 23 };
}

const GLfloat oct_normals[] = {
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, // Front 
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, // Right side 
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, // Back 
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, // Left side 
		1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, // Lower front 
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, // Lower right side
		1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, // Lower back
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, // Lower left side
		-1.0f, -1.0f, -1.0f,
};

void drawOct() {
	using namespace OCT_FACES;
	GLfloat oct_vertices[] = {
		-SIDE_LENGTH - opened, 0.0f + opened, 0.0f + opened,
		0.0f - opened, SIDE_LENGTH + opened, 0.0f + opened,
		0.0f - opened, 0.0f + opened, SIDE_LENGTH + opened,

		0.0f + opened, 0.0f + opened, SIDE_LENGTH + opened,
		0.0f + opened, SIDE_LENGTH + opened, 0.0f + opened,
		SIDE_LENGTH + opened, 0.0f + opened, 0.0f + opened,

		SIDE_LENGTH + opened, 0.0f + opened, 0.0f - opened,
		0.0f + opened, SIDE_LENGTH + opened, 0.0f - opened,
		0.0f + opened, 0.0f + opened, -SIDE_LENGTH - opened,

		0.0f - opened, 0.0f + opened, -SIDE_LENGTH - opened,
		0.0f - opened, SIDE_LENGTH + opened, 0.0f - opened,
		-SIDE_LENGTH - opened, 0.0f + opened, 0.0f - opened,

		-SIDE_LENGTH - opened, 0.0f - opened, 0.0f + opened,
		0.0f - opened, -SIDE_LENGTH - opened, 0.0f + opened,
		0.0f - opened, 0.0f - opened, SIDE_LENGTH + opened,

		0.0f + opened, 0.0f - opened, SIDE_LENGTH + opened,
		0.0f + opened, -SIDE_LENGTH - opened, 0.0f + opened,
		SIDE_LENGTH + opened, 0.0f - opened, 0.0f + opened,

		SIDE_LENGTH + opened, 0.0f - opened, 0.0f - opened,
		0.0f + opened, -SIDE_LENGTH - opened, 0.0f - opened,
		0.0f + opened, 0.0f - opened, -SIDE_LENGTH - opened,

		0.0f - opened, 0.0f - opened, -SIDE_LENGTH - opened,
		0.0f - opened, -SIDE_LENGTH - opened, 0.0f - opened,
		-SIDE_LENGTH - opened, 0.0f - opened, 0.0f - opened,
	};

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, oct_textures_coordinates);

	glVertexPointer(3, GL_FLOAT, 0, oct_vertices);
	glNormalPointer(GL_FLOAT, 0, oct_normals);

	if (!textured) {
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);  // green
		glNormal3d(-1, -1, 1);
		glVertex3f(0 + opened, SIDE_LENGTH + opened, 0 - opened);
		glVertex3f(0 + opened, 0 + opened, -SIDE_LENGTH - opened);
		glVertex3f(SIDE_LENGTH + opened, 0 + opened, 0 - opened);

		glColor3f(0.0, 1.0, 0.0);  // red
		glNormal3d(-1, -1, -1);
		glVertex3f(0 + opened, SIDE_LENGTH + opened, 0 + opened);
		glVertex3f(SIDE_LENGTH + opened, 0 + opened, 0 + opened);
		glVertex3f(0 + opened, 0 + opened, SIDE_LENGTH + opened);

		glColor3f(0.0, 0.0, 1.0);  // blue
		glNormal3d(1, -1, -1);
		glVertex3f(0 - opened, SIDE_LENGTH + opened, 0 + opened);
		glVertex3f(0 - opened, 0 + opened, SIDE_LENGTH + opened);
		glVertex3f(-SIDE_LENGTH - opened, 0 + opened, 0 + opened);

		glColor3f(1.0, 0.0, 1.0);  // purple
		glNormal3d(1, -1, -1);
		glVertex3f(0 + opened, 0 - opened, -SIDE_LENGTH - opened);
		glVertex3f(SIDE_LENGTH + opened, 0 - opened, 0 - opened);
		glVertex3f(0 + opened, -SIDE_LENGTH - opened, 0 - opened);

		glColor3f(1.0, 1.0, 0.0);  // yellow
		glNormal3d(-1, 1, -1);
		glVertex3f(0 + opened, 0 - opened, SIDE_LENGTH + opened);
		glVertex3f(SIDE_LENGTH + opened, 0 - opened, 0 + opened);
		glVertex3f(0 + opened, -SIDE_LENGTH - opened, 0 + opened);

		glColor3f(0.0, 1.0, 1.0);  // cyan
		glNormal3d(-1, -1, 1);
		glVertex3f(0 - opened, 0 - opened, SIDE_LENGTH + opened);
		glVertex3f(-SIDE_LENGTH - opened, 0 - opened, 0 + opened);
		glVertex3f(0 - opened, -SIDE_LENGTH - opened, 0 + opened);

		glColor3f(1.0, 1.0, 1.0);  // white
		glNormal3d(-1, 1, -1);
		glVertex3f(0 - opened, SIDE_LENGTH + opened, 0 - opened);
		glVertex3f(0 - opened, 0 + opened, -SIDE_LENGTH - opened);
		glVertex3f(-SIDE_LENGTH - opened, 0 + opened, 0 - opened);

		glColor3f(1.0, 0.0, 0.0);  // rgb
		glNormal3d(1, 1, 1);
		glVertex3f(0 - opened, 0 - opened, -SIDE_LENGTH - opened);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-SIDE_LENGTH - opened, 0 - opened, 0 - opened);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0 - opened, -SIDE_LENGTH - opened, 0 - opened);

		glEnd();
	}
	else {
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, lower_back);

		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, lower_left_side);

		glBindTexture(GL_TEXTURE_2D, textures[2]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, lower_right_side);

		glBindTexture(GL_TEXTURE_2D, textures[3]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, lower_front);

		glBindTexture(GL_TEXTURE_2D, textures[4]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, back);

		glBindTexture(GL_TEXTURE_2D, textures[5]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, front);

		glBindTexture(GL_TEXTURE_2D, textures[6]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, right_side);

		glBindTexture(GL_TEXTURE_2D, textures[7]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, left_side);

		glDisable(GL_TEXTURE_2D);
	}
}

void drawCutOct(GLuint display_list) {
	glLoadIdentity();

	glTranslatef(0, 0, -20);
	glRotatef((ox_rotation), 0, 1, 0);
	glRotatef((oy_rotation), 1, 0, 0);

	glCallList(display_list);
}
void create_cut_triangles_list(GLuint display_list, GLfloat partLength, GLfloat maxDetachLength) {
	glLoadIdentity();
	glNewList(display_list, GL_COMPILE);
	//if (isOpened) {
	//	if (opened < maxDetachLength)
	//		opened += 0.1;
	//}
	//else {
	//	if (opened > 0.0) {
	//		opened -= 0.1;
	//		if (opened < 0.0)
	//			opened = 0;
	//	}
	//}

	for (GLfloat l = -(SIDE_LENGTH + partLength),
		i = -partLength;
		i >= -(SIDE_LENGTH + 4 * 0.1); l += partLength, i -= partLength) {

		i -= 0.1;

		// rgb
		glBegin(GL_POLYGON);

		glNormal3f(1.0f, 1.0f, 1.0f);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(-maxDetachLength, i - maxDetachLength, l + 2 * partLength - maxDetachLength);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3d(-maxDetachLength, i + partLength - maxDetachLength, l + partLength - maxDetachLength);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3d(l + partLength - maxDetachLength, i + partLength - maxDetachLength, -maxDetachLength);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3d(l + 2 * partLength - maxDetachLength, i - maxDetachLength, -maxDetachLength);

		glEnd();


		// purple
		glColor3f(1.0, 0.0, 1.0);
		glBegin(GL_POLYGON);

		glNormal3f(1.0f, 1.0f, -1.0f);

		glVertex3d(maxDetachLength, i - maxDetachLength, l + 2 * partLength - maxDetachLength);
		glVertex3d(maxDetachLength, i + partLength - maxDetachLength, l + partLength - maxDetachLength);
		glVertex3d(-(l + partLength) + maxDetachLength, i + partLength - maxDetachLength, -maxDetachLength);
		glVertex3d(-(l + 2 * partLength) + maxDetachLength, i - maxDetachLength, -maxDetachLength);

		glEnd();


		// cyan
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_POLYGON);

		glNormal3f(-1.0f, -1.0f, 1.0f);

		glVertex3d(-maxDetachLength, i - maxDetachLength, -(l + 2 * partLength) + maxDetachLength);
		glVertex3d(-maxDetachLength, i + partLength - maxDetachLength, -(l + partLength) + maxDetachLength);
		glVertex3d(l + partLength - maxDetachLength, i + partLength - maxDetachLength, maxDetachLength);
		glVertex3d(l + 2 * partLength - maxDetachLength, i - maxDetachLength, maxDetachLength);

		glEnd();


		// yellow
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POLYGON);

		glNormal3f(-1.0f, 1.0f, -1.0f);

		glVertex3d(maxDetachLength, i - maxDetachLength, -(l + 2 * partLength) + maxDetachLength);
		glVertex3d(maxDetachLength, i + partLength - maxDetachLength, -(l + partLength) + maxDetachLength);
		glVertex3d(-(l + partLength) + maxDetachLength, i + partLength - maxDetachLength, maxDetachLength);
		glVertex3d(-(l + 2 * partLength) + maxDetachLength, i - maxDetachLength, maxDetachLength);

		glEnd();

	}
	for (GLfloat l = -(SIDE_LENGTH + partLength), i = partLength;
		i <= SIDE_LENGTH + 4 * 0.1; l += partLength, i += partLength) {

		//white
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POLYGON);

		glNormal3f(-1.0f, -1.0f, -1.0f);

		glVertex3d(-maxDetachLength, i + maxDetachLength, l + 2 * partLength - maxDetachLength);
		glVertex3d(-maxDetachLength, i - partLength + maxDetachLength, l + partLength - maxDetachLength);
		glVertex3d(l + partLength - maxDetachLength, i - partLength + maxDetachLength, -maxDetachLength);
		glVertex3d(l + 2 * partLength - maxDetachLength, i + maxDetachLength, -maxDetachLength);

		glEnd();

		//red
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_POLYGON);

		glNormal3f(-1.0f, 1.0f, 1.0f);

		glVertex3d(maxDetachLength, i + maxDetachLength, l + 2 * partLength - maxDetachLength);
		glVertex3d(maxDetachLength, i - partLength + maxDetachLength, l + partLength - maxDetachLength);
		glVertex3d(-(l + partLength) + maxDetachLength, i - partLength + maxDetachLength, -maxDetachLength);
		glVertex3d(-(l + 2 * partLength) + maxDetachLength, i + maxDetachLength, -maxDetachLength);

		glEnd();

		//blue
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);

		glNormal3f(1.0f, -1.0f, -1.0f);

		glVertex3d(-maxDetachLength, i + maxDetachLength, -(l + 2 * partLength) + maxDetachLength);
		glVertex3d(-maxDetachLength, i - partLength + maxDetachLength, -(l + partLength) + maxDetachLength);
		glVertex3d(l + partLength - maxDetachLength, i - partLength + maxDetachLength, maxDetachLength);
		glVertex3d(l + 2 * partLength - maxDetachLength, i + maxDetachLength, maxDetachLength);

		glEnd();

		//green
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);

		glNormal3f(1.0f, 1.0f, 1.0f);

		glVertex3d(maxDetachLength, i + maxDetachLength, -(l + 2 * partLength) + maxDetachLength);
		glVertex3d(maxDetachLength, i - partLength + maxDetachLength, -(l + partLength) + maxDetachLength);
		glVertex3d(-(l + partLength) + maxDetachLength, i - partLength + maxDetachLength, maxDetachLength);
		glVertex3d(-(l + 2 * partLength) + maxDetachLength, i + maxDetachLength, maxDetachLength);

		glEnd();

		i += 0.1;
	}

	glEndList();
}

void Draw() {
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (transparent) {
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else {
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
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

	//сфера/шар/источник освещения
	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric();
	glColor3d(1, 1, 0);
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluSphere(quadObj, 2, 20, 20);

	//октаэдр
	glLoadIdentity();
	glTranslatef(0, 0, -20);
	glRotatef((ox_rotation), 0, 1, 0);
	glRotatef((oy_rotation), 1, 0, 0);

	if (isOpened) {
		if (opened < detachLength)
			opened += 0.1;
	}
	else {
		if (opened > 0.0) {
			opened -= 0.1;
			if (opened < 0.0)
				opened = 0;
		}
	}
	if (!cutOct)
		drawOct();
	else {
		if (opened == 0)
			drawCutOct(cutOctahedron);
		else
			drawCutOct(cutOctahedronOpen);
	}

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
		dx_rotation = -speed;
		break;
	case 'd':
		dx_rotation = speed;
		break;
	case 'w':
		dy_rotation = -speed;
		break;
	case 's':
		dy_rotation = speed;
		break;
	case 'q':
		dy_rotation = 0;
		dx_rotation = 0;
		break;
	case 'e':
		isOpened = !isOpened;
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
	case 'x':
		transparent = !transparent;
		break;
	case 'c':
		cutOct = !cutOct;
		break;
	case 't':
		textured = !textured;
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
	glClearColor(0, 0, 0, 0);
	std::cout << "W - Turn up\nA - Turn left\nS - turn down\nD - Turn right\nQ - Stop rotation of octahedron\nE - Open octahedron\nF - Turn the light on/off\nR - Change sun rotation\nX - Toggle transparency\nC - cut side of octahedron (demonstration of display lists)\nT - add textures\nZ - Close the programm\n";
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Octahedron");

	GLfloat openedPartLength = SIDE_LENGTH / NUMBER_OF_PARTS; // side length / number of parts
	cutOctahedron = glGenLists(2);
	create_cut_triangles_list(cutOctahedron, openedPartLength, 0.0f);
	glEndList();

	cutOctahedronOpen = cutOctahedron + 1;
	create_cut_triangles_list(cutOctahedronOpen, openedPartLength, detachLength);
	glEndList();

	glutDisplayFunc(Draw);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(5, TimerFunction, 0.5);
	glutKeyboardFunc(glutNormalKeys);

	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	load_textures();

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}
