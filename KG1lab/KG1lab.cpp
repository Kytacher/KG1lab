#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "Pipeline.h"
#include "camera.h"
#include "math_3d.h"
#include "texture.h"

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
	}
};

GLuint VBO;
GLuint gWVPLocation;
GLuint IBO;
GLuint gSampler;
Texture* pTexture = NULL;
Camera* pGameCamera = NULL;

// инициализация шейдера
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec2 TexCoord;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
out vec2 TexCoord0;                                                                 \n\                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
     gl_Position = gWorld * vec4(Position, 1.0);                                      \n\
    TexCoord0 = TexCoord;                                                           \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec2 TexCoord0;                                                                  \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
uniform sampler2D gSampler;                                                         \n\
                                                                                    \n\                                                                              \n\
void main()                                                                         \n\
{                                                                                   \n\
   FragColor = texture2D(gSampler, TexCoord0.xy);                                  \n\
}";

static void RenderSceneCB()
{
	pGameCamera->OnRender();
	glClear(GL_COLOR_BUFFER_BIT);

	static float sc = 0.0f;
	sc += 0.001f;

	// Создаем: едиинчную матрицу, матричу вращения, матрицу движения, матрицу размера и обьединяеим в итоговую матрицу result
	/*glm::mat4x4 unit;
	unit[0][0] = 1.0f; unit[0][1] = 0.0f; unit[0][2] = 0.0f; unit[0][3] = 0.0f;
	unit[1][0] = 0.0f; unit[1][1] = 1.0f; unit[1][2] = 0.0f; unit[1][3] = 0.0f;
	unit[2][0] = 0.0f; unit[2][1] = 0.0f; unit[2][2] = 1.0f; unit[2][3] = 0.0f;
	unit[3][0] = 0.0f; unit[3][1] = 0.0f; unit[3][2] = 0.0f; unit[3][3] = 1.0f;

	glm::mat4x4 rotate;
	rotate[0][0] = sinf(sc); rotate[0][1] = -sinf(sc); rotate[0][2] = 0.0f; rotate[0][3] = 0.0f;
	rotate[1][0] = 0.0f; rotate[1][1] = 1.0f; rotate[1][2] = 0.0f; rotate[1][3] = 0.0f;
	rotate[2][0] = cosf(sc); rotate[2][1] = cosf(sc); rotate[2][2] = 1.0f; rotate[2][3] = 0.0f;
	rotate[3][0] = 0.0f; rotate[3][1] = 0.0f; rotate[3][2] = 0.0f; rotate[3][3] = 1.0f;

	glm::mat4x4 move;
	move[0][0] = 1.0f; move[0][1] = 0.0f; move[0][2] = 0.0f; move[0][3] = sinf(sc);
	move[1][0] = 0.0f; move[1][1] = 1.0f; move[1][2] = 0.0f; move[1][3] = cosf(sc);
	move[2][0] = 0.0f; move[2][1] = 0.0f; move[2][2] = 1.0f; move[2][3] = 0.0f;
	move[3][0] = 0.0f; move[3][1] = 0.0f; move[3][2] = 0.0f; move[3][3] = 1.0f;

	glm::mat4x4 resize;
	resize[0][0] = sinf(sc); resize[0][1] = 0.0f; resize[0][2] = 0.0f; resize[0][3] = 0.0f;
	resize[1][0] = 0.0f; resize[1][1] = sinf(sc); resize[1][2] = 0.0f; resize[1][3] = 0.0f;
	resize[2][0] = 0.0f; resize[2][1] = 0.0f; resize[2][2] = sinf(sc); resize[2][3] = 0.0f;
	resize[3][0] = 0.0f; resize[3][1] = 0.0f; resize[3][2] = 0.0f; resize[3][3] = 1.0f;

	glm::mat4x4 result = unit * rotate * move * resize;
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, &result[0][0]); */

	//создаем объект конвейера, настраиваем его и отправляем результат в шейдер.
	Pipeline p;

	p.Scale(sinf(sc) / 2, sinf(sc * 0.5), 1.0f);
	p.WorldPos(sinf(sc) / 2, 0.0f, 0.0f);
	p.Rotate(-cosf(sc), sinf(sc), 1.0f);
	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
	p.SetPerspectiveProj(90.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 200.0f);

//	p.WorldPos(sinf(sc), 0.0f, 0.0f);
//	p.Rotate(sinf(sc) * 90.0f, sinf(sc) * 90.0f, sinf(sc) * 90.0f);
//	p.Scale(sinf(sc * 0.1f), sinf(sc * 0.1f), sinf(sc * 0.1f));
//	p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

	//glm::vec3 CameraPos(0.0f, 0.0f, -3.0f);
	//glm::vec3 CameraTarget(0.0f, 0.0f, 2.0f);
	//glm::vec3 CameraUp(0.0f, 1.0f, 0.0f);
	//p.SetCamera(CameraPos, CameraTarget, CameraUp);

//	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());


	//используем атрибуты вершин
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//привязка буфера для рисования
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//устанавливаем атрибуты вершин
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	//привязка буфера для рисования
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//отрисовка
	pTexture->Bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	//отключаме атрбуты вершин
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
//	for (int i = 0; i < 10; i++)
//		glDrawArrays(GL_POLYGON, 0, i);
	//меняем местами фоновый буфер и буфер рендера местами
	glutSwapBuffers();
}

static void SpecialKeyboardCB(int Key, int x, int y)
{
	pGameCamera->OnKeyboard(Key);
}

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 'q':
		exit(0);
	}
}

static void PassiveMouseCB(int x, int y)
{
	pGameCamera->OnMouse(x, y);
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

void createVertexBuffer() {
	Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
						   Vertex(Vector3f(0.0f, -1.0f, -1.15475), Vector2f(0.5f, 0.0f)),
						   Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
						   Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void createIndexBuffer() {
	unsigned int indices[] = {
			0, 3, 1,
			1, 3, 2,
			2, 3, 0,
			0, 2, 1
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWVPLocation != 0xFFFFFFFF);
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	assert(gSampler != 0xFFFFFFFF);
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

//	glutGameModeString("1280x1024@32");
//	glutEnterGameMode();

	//отрисовка
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutSpecialFunc(SpecialKeyboardCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	glutKeyboardFunc(KeyboardCB);

	pGameCamera = new Camera(600, 600);
	//инициализация glew
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//фигурка
	createVertexBuffer();
	createIndexBuffer();
//	glm::vec3 Vertices[3] = {{-0.3f, -0.3f, 0.0f}, {0.3f, -0.3f, 0.0f}, {0.0f, 0.3f, 0.0f}};
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);
	CompileShaders();
	glUniform1i(gSampler, 0);

	pTexture = new Texture(GL_TEXTURE_2D, "D://test.png");

	if (!pTexture->Load()) {	
		return 1;
	}
    
	glutMainLoop();
}


