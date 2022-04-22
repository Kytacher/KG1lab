#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <list>
#include <iostream>
#include "Pipeline.h"
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)


GLuint VBO;
GLuint gWorldLocation;

// инициализация шейдера
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(0.4, 0.0, 1.0, 1.0);                                           \n\
}";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float offset = 0.0f;
	offset += 0.001f;

	// Создаем: едиинчную матрицу, матричу вращения, матрицу движения, матрицу размера и обьединяеим в итоговую матрицу result
	glm::mat4x4 unit;
	unit[0][0] = 1.0f; unit[0][1] = 0.0f; unit[0][2] = 0.0f; unit[0][3] = 0.0f;
	unit[1][0] = 0.0f; unit[1][1] = 1.0f; unit[1][2] = 0.0f; unit[1][3] = 0.0f;
	unit[2][0] = 0.0f; unit[2][1] = 0.0f; unit[2][2] = 1.0f; unit[2][3] = 0.0f;
	unit[3][0] = 0.0f; unit[3][1] = 0.0f; unit[3][2] = 0.0f; unit[3][3] = 1.0f;

	glm::mat4x4 rotate;
	rotate[0][0] = sinf(offset); rotate[0][1] = -sinf(offset); rotate[0][2] = 0.0f; rotate[0][3] = 0.0f;
	rotate[1][0] = 0.0f; rotate[1][1] = 1.0f; rotate[1][2] = 0.0f; rotate[1][3] = 0.0f;
	rotate[2][0] = cosf(offset); rotate[2][1] = cosf(offset); rotate[2][2] = 1.0f; rotate[2][3] = 0.0f;
	rotate[3][0] = 0.0f; rotate[3][1] = 0.0f; rotate[3][2] = 0.0f; rotate[3][3] = 1.0f;

	glm::mat4x4 move;
	move[0][0] = 1.0f; move[0][1] = 0.0f; move[0][2] = 0.0f; move[0][3] = sinf(offset);
	move[1][0] = 0.0f; move[1][1] = 1.0f; move[1][2] = 0.0f; move[1][3] = cosf(offset);
	move[2][0] = 0.0f; move[2][1] = 0.0f; move[2][2] = 1.0f; move[2][3] = 0.0f;
	move[3][0] = 0.0f; move[3][1] = 0.0f; move[3][2] = 0.0f; move[3][3] = 1.0f;

	glm::mat4x4 resize;
	resize[0][0] = sinf(offset); resize[0][1] = 0.0f; resize[0][2] = 0.0f; resize[0][3] = 0.0f;
	resize[1][0] = 0.0f; resize[1][1] = sinf(offset); resize[1][2] = 0.0f; resize[1][3] = 0.0f;
	resize[2][0] = 0.0f; resize[2][1] = 0.0f; resize[2][2] = sinf(offset); resize[2][3] = 0.0f;
	resize[3][0] = 0.0f; resize[3][1] = 0.0f; resize[3][2] = 0.0f; resize[3][3] = 1.0f;

	glm::mat4x4 result = unit * rotate * move * resize;
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &result[0][0]);

	//создаем объект конвейера, настраиваем его и отправляем результат в шейдер. Попробуйте изменить параметры и посмотрите на результат
	Pipeline p;
	p.Scale(sinf(offset * 0.1f), sinf(offset * 0.1f), sinf(offset * 0.1f));
	p.WorldPos(sinf(offset), 0.0f, 0.0f);
	p.Rotate(sinf(offset) * 90.0f, sinf(offset) * 90.0f, sinf(offset) * 90.0f);
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());
	//используем атрибуты вершин
	glEnableVertexAttribArray(0);
	//привязка буфера для рисования
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//устанавливаем атрибуты вершин
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//отрисовка
	for (int i = 0; i < 10; i++)
		glDrawArrays(GL_POLYGON, 0, i);
	//отключаме атрбуты вершин
	glDisableVertexAttribArray(0);
	//меняем местами фоновый буфер и буфер рендера местами
	glutSwapBuffers();
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//создаем шефдер
	GLuint ShaderObj = glCreateShader(ShaderType);
	//проверка на ошибки
	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	//создаем код шефдера
	const GLchar* p[1];
	p[0] = pShaderText;
	//массив длин кодов шефдеров
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//задаем исходники шейдера
	glShaderSource(ShaderObj, 1, p, Lengths);
	//комплируем шейдер
	glCompileShader(ShaderObj);
	//проверка на ошибки
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}
	//обьект шейдера
	glAttachShader(ShaderProgram, ShaderObj);
}


static void CompileShaders()
{
	//создание программы шейдера
	GLuint ShaderProgram = glCreateProgram();
	//проверка на ошибки
	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
	//добавляем шейдер для вершин
	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	//проверка на отклик и ошибки
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	//glew использует шейдер
	glUseProgram(ShaderProgram);
	//сохроняем в переменную
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}



int main(int argc, char** argv)
{
	//окно
	glutInit(&argc, argv);
	//буферизация
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//настройки окна
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");
	//отрисовка
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	//инициализация glew
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//фигурка
	glm::vec3 Vertices[5] = {{-0.3f, -0.3f, 0.0f}, {0.3f, -0.3f, 0.0f}, {0.0f, 0.3f, 0.0f},{0.5f, -0.2f, 0.1f}, {0.1f, 0.1f, 0.4f} };
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);
    
	CompileShaders();
	glutMainLoop();
}


