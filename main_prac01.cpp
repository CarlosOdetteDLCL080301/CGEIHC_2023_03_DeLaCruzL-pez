/*---------------------------------------------------------*/
/* ----------------   Práctica 1 --------------------------*/
/*-----------------    2023-2   ---------------------------*/
/*------------- (De La Cruz López Carlos Odette) ---------------*/
/*------------- (20 Febrero 2023) ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO[2], VAO[2], EBO[2];
GLuint shaderProgramYellow, shaderProgramColor;
/*Es un shader sencillo, tiene solo una entrada, se le explica el tipo de shader que usará, en este caso recibe de tres en tres*/
static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderYellow = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 1.0f, 0.0f);									\n\
}";
/*Las lineas anteriores sirven para cambiar el color de nuestro objeto, o el "fragmento de shaders"*/
static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()/*Se mostrara la informacion de los vertices que queremos que se muestren*//*Se le conoce como "Lista de vertices"*/
{
	float vertices[] = 
	{
		// positions         //Color
		0.0f,	0.0f,	0.0f,	1.0f,	1.0f,	1.0f,			//0		Esto controla la posicion de nuestro "punto" que se mostrará en la pantalla,en este caso se muestra en el centro, el formato del lienzo es (x,y,z, profundidad)
		0.7f,	0.5f,	0.0f,	1.0f,	0.65f,	0.3f,			//1
		-0.8f,	0.7f,	0.0f,	1.0f,	0.0f,	1.0f,			//2
		-0.8f,	-0.67f,	0.0f,	1.0f,	1.0f,	0.0f,			//3
		-0.0f,	-0.8f,	0.0f,	1.0f,	0.0f,	0.0f,			//4
		0.4f,	-0.8f,	0.0f,	1.0f,	1.0f,	1.0f,			//5
		
	};
	/*La regla de union si se cambia el orden puede que se obtenga un resultado distinto*/
	unsigned int indices[] =
	{
		0, 1, 4, 2, 3
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);



	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));/*En esta se definirá los colores de nuestro arreglo, aqui es gracias a la entrada 1*/
	glEnableVertexAttribArray(1);/**/

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &myFragmentShaderYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(fragmentShaderColor);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();
	/**/
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 1", NULL, NULL);/*Redimensiona el ancho y la altura, para mostrar la ventana con la de nuestro monitor, se le da el identificador de practica 1 esto aparece como titulo de nuestra ventana */
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }/*Si se corre el programa, nos mostrará la pantalla*/
	glfwSetWindowPos(window, 0, 30);/*Mostrara en la posicion, desde el origen del monitor 0,30 (comparar la posicion en HTML)*/
    glfwMakeContextCurrent(window);/*Se usaran muchos buffers de color, profundidad, entre otros*/
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//My Functions
	//Setup Data to use
	myData();/*Se le pasará datos al sistema*/
	//To Setup Shaders
	setupShaders();
    
    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);/*glClearColor(r,g,b,canalApha)*/
        glClear(GL_COLOR_BUFFER_BIT);

		//Display Section
		glUseProgram(shaderProgramColor);/*Cuando une los shaders OpenGL*/

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPointSize(10.0);/*Esto indica que el tamaño de los puntos sea el colocado, es de la dimension del pixel*/
		glLineWidth(1.0f);
		//glDrawElements(GL_POINTS, 5, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);/*Es un comando para armar la geometria, recibe tres argumentos (tipo de dibujo que quiero mostrar se puede usar puntos o lineas con GL_POINTS, GL_LINES, de los pasos que se le paso al sistema es el valor inicial desde donde se comienza, a partir de ahí solo dibujara un objeto hasta el elemento seleccionado)*//*GL_LINE_STRIP lo que hace es conectar todos los puntos que se colocaron en la lista de vertices*//*GL_LINE_LOOP lo que hace es unir todos los puntos hasta cerrarlo completamente con todos los puntos y no recurrir a GL_LINE_STRIP para colocar otra linea que funja como el cierre entre los puntos*//*GL_TRIANGLES nos crea un poligono como un traingulo en este caso*//*GL_TRIANGLE_STRIP lo que hace es conectar cada de los puntos de nuestra lista de vertices generando triangulos, ejemplo, nuestra lista de indice N hara triangulos con i, i+1, i+2, siempre y cuando con i se pueda realizar un triangulo*//*GL_TRIANGLE_FAN tomara los puntos como pivote, y realizará un triangulo uniendo a los demsa puntos*/
		//glDrawArrays(GL_POINTS, 4, 1);

		glBindVertexArray(0);
		glUseProgram(0);

		//End of Display Section

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}