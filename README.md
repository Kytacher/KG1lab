ICG LAB 1

Лабораторная работа № 1 по дисциплине "Инженерная и компьютерная графика"

1) Установка библиотек OpenGL
freeglut
glew
glm

2) Создание окна
```c++ 
  glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");
	glutDisplayFunc(RenderSceneCB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
  ```
  
