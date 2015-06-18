#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef WIN32
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#endif

#include <iostream>
#include <GLFW/glfw3.h>

GLFWwindow* window;
float aspect_ratio = 1.0f;
long frame_count = 0;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	aspect_ratio = height ? width / (float)height : 1.f;
}

void draw()
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

	glBegin(GL_POINTS);
	glVertex3d(100, 100, 0);
	glEnd();

	frame_count++;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS) return;

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	default:
		return;
	}
}

int main(int argc, char* argv[])
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		std::cout << "error : glfwInit() failed..." << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_RESIZABLE, 0); // disable resize
	window = glfwCreateWindow(200, 200, "1px - glfw3 version", NULL, NULL);
	if (window == NULL) {
		std::cout << "error : glfwCreateWindow() failed..." << std::endl;
		return -1;
	}

	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

