﻿#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

GLuint VBO;


void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	
	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");
	glutDisplayFunc(RenderSceneCB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);



	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	} 
	glm::vec3 v[3] = { {-0.4f, -0.3f, 0.2f}, {0.7f, -0.2f, 0.1f}, {0.1f, 0.6f, 0.1f} };

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), &v[0] , GL_STATIC_DRAW);
	glutMainLoop();
}


