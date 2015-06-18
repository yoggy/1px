//
// 1px.c - freeglut version
//
// freeglut
//   http://freeglut.sourceforge.net/
//
// Martin Payne's Windows binaries (MSVC and MinGW)
//   http://www.transmissionzero.co.uk/software/freeglut-devel/
//
// GLUTによる「手抜き」OpenGL入門
//   http://www.wakayama-u.ac.jp/~tokoi/opengl/libglut.html
//
#ifdef WIN32
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <gl/freeglut.h>

#include <stdio.h>

int frame_count = 0;

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 200.0, 0, 200.0, 1.f, -1.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch ((frame_count / 15) % 4) {
	case 0:
		glColor3d(1.0, 0.0, 0.0);
		break;
	case 1:
		glColor3d(0.0, 1.0, 0.0);
		break;
	case 2:
		glColor3d(0.0, 0.0, 1.0);
		break;
	case 3:
		glColor3d(0.0, 0.0, 0.0);
		break;
	}
	glBegin(GL_QUADS);
	glVertex3d(100, 100, 0);
	glVertex3d(101, 100, 0);
	glVertex3d(101, 101, 0);
	glVertex3d(100, 101, 0);
	glEnd();

	glFlush();
	glutSwapBuffers();

	frame_count++;
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

void idle(void)
{
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(200, 200);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}