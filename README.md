# ICG LAB 1

## Лабораторная работа № 1 по дисциплине "Инженерная и компьютерная графика"

### 1) Установка библиотек OpenGL
- freeglut
- glew
- glm

### 2) Создание окна
```c++ 
        glutInit(&argc, argv); //
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // двойная буферизации и буфера цвета
	glutInitWindowSize(1024, 768); // размер окна
	glutInitWindowPosition(100, 100); // начальная позиция
	glutCreateWindow("Tutorial 01"); // окно с заголовком "Tutorial 01"
	glutDisplayFunc(RenderSceneCB); // функция отрисовки
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // цвет фона окна (красный, зеленый, синий, прозрачность)
	glutMainLoop(); //
  ```
  Функиця отрисовки: 
  ```c++
        glClear(GL_COLOR_BUFFER_BIT); //очистка окна
	glutSwapBuffers(); //меняем буферы местами
  ```
  ### 3) Рисование трегольника
  Глобальная переменная: 
  ```c++
        GLuint VBO;
  ```
  В функцию RenderSceneCB добавим:
  ```c++
        glEnableVertexAttribArray(0); // исп атрибутов конвейера
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // привязываем буфер
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // настройка конвейера 
	glDrawArrays(GL_TRIANGLES, 0, 3); // отрисовка тругольника
	glDisableVertexAttribArray(0); // откл атрибутво конвейера
  ```
  В main добавим: 
  ```c++
        glm::vec3 v[3] = { {-0.4f, -0.3f, 0.2f}, {0.7f, -0.2f, 0.1f}, {0.1f, 0.6f, 0.1f} }; // вектора вершин
	glGenBuffers(1, &VBO); // генерация обьекта переменного типа
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // указываем OpenGL, что в VBO будет храниться массив вершин
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), &v[0] , GL_STATIC_DRAW); // заполняем буфер
  ```
  
  
  
  # ICG LAB 2

## Лабораторная работа № 2 по дисциплине "Инженерная и компьютерная графика"

### 1) Изменение функции RenderSceneCB.
```c++ 
       static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float sc = 0.0f;
	sc += 0.001f;

	// Создаем: едиинчную матрицу, матричу вращения, матрицу движения, матрицу размера и обьединяеим в итоговую матрицу result
	glm::mat4x4 unit;
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
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &result[0][0]);

	//создаем объект конвейера, настраиваем его и отправляем результат в шейдер. Попробуйте изменить параметры и посмотрите на результат
	Pipeline p;
	p.Scale(sinf(sc * 0.1f), sinf(sc * 0.1f), sinf(sc * 0.1f));
	p.WorldPos(sinf(sc), 0.0f, 0.0f);
	p.Rotate(sinf(sc) * 90.0f, sinf(sc) * 90.0f, sinf(sc) * 90.0f);
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
  ```
  ### 2) Код шейдера.
  ```c++ 
  2 static const char* pVS = "                                                          \n\
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
  ```
  
  ### 3) Добавлена функция AddShader.
  ```c++
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
  ```
  
### 4) Добавлена функция CompileShaders.
```c++
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
  ```
  
  ### 5) Добавлен класс Pipeline, для различных преобразований шейдеров.
  ```c++
  class Pipeline
{
private:
    glm::vec3 scale;
    glm::vec3 worldPos;
    glm::vec3 rotateInfo;
    glm::mat4 transformation;

public:
    Pipeline()
    {
        scale = glm::vec3(1.0f, 1.0f, 1.0f);
        worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        scale.x = ScaleX;
        scale.y = ScaleY;
        scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        worldPos.x = x;
        worldPos.y = y;
        worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        rotateInfo.x = RotateX;
        rotateInfo.y = RotateY;
        rotateInfo.z = RotateZ;
    }

    const glm::mat4* getTransformation()
    {
        glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;
        InitScaleTransform(ScaleTrans);
        InitRotateTransform(RotateTrans);
        InitTranslationTransform(TranslationTrans);
        transformation = TranslationTrans * RotateTrans * ScaleTrans;
        return &transformation;
    }

    void InitScaleTransform(glm::mat4& m) const
    {
        m[0][0] = scale.x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = scale.y; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = scale.z; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    void InitRotateTransform(glm::mat4& m) const
    {
        glm::mat4 rx, ry, rz;

        const float x = ToRadian(rotateInfo.x);
        const float y = ToRadian(rotateInfo.y);
        const float z = ToRadian(rotateInfo.z);

        rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
        rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
        rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
        rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

        ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
        ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
        ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
        ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

        rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
        rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
        rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
        rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

        m = rz * ry * rx;
    }

    void InitTranslationTransform(glm::mat4& m) const
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = worldPos.x;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = worldPos.y;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = worldPos.z;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }
};
```
