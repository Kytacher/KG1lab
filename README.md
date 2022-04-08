ICG LAB 1

Лабораторная работа № 1 по дисциплине "Инженерная и компьютерная графика"

1) Установка библиотек OpenGL
- freeglut
- glew
- glm

2) Создание окна
```c++ 
        glutInit(&argc, argv); // инициализация glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // двойная буферизации и буфера цвета
	glutInitWindowSize(1024, 768); // размер окна
	glutInitWindowPosition(100, 100); // начальная позиция
	glutCreateWindow("Tutorial 01"); // окно с заголовком "Tutorial 01"
	glutDisplayFunc(RenderSceneCB); //
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // цвет фона окна (красный, зеленый, синий, прозрачность)
	glutMainLoop(); //
  ```
  
