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
  
  
  
  
