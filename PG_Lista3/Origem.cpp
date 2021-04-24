#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/transformations.vs", "../shaders/transformations.frag");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shader.ID, "contourColor");
	assert(colorLoc > -1);
	glUseProgram(shader.ID);
	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f); //para cor de contorno

	// Para enviar o identificador de modo de desenho
	GLint rasterCodeLoc = glGetUniformLocation(shader.ID, "rasterCode");

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		//aqui recupera o tamanho atual da janela, para correção do aspect ratio mais tarde
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Matriz ortográfica - Câmera 2D
		glm::mat4 ortho = glm::mat4(1);
		ortho = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

		// Obtem seus identificadores de localização no shader
		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		GLint projLoc = glGetUniformLocation(shader.ID, "projection");
		// Passa suas informações efetivamente para o shader
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

		glLineWidth(2);
		glPointSize(10);

		// Primeiro triângulo
		glm::mat4 model1 = glm::mat4(1);
		model1 = glm::translate(model1, glm::vec3(30.0f, 30.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

		glUniform1i(rasterCodeLoc, 0);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUniform1i(rasterCodeLoc, 1);
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		glBindVertexArray(0);

		// Segundo triângulo
		glm::mat4 model2 = glm::mat4(1);
		model2 = glm::translate(model2, glm::vec3(150.0f, 150.0f, 0.0f));
		model2 = glm::scale(model2, glm::vec3(1.6f, 3.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		glUniform1i(rasterCodeLoc, 0);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUniform1i(rasterCodeLoc, 1);
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		glBindVertexArray(0);

		// Terceiro triângulo
		glm::mat4 model3 = glm::mat4(1);
		model3 = glm::translate(model3, glm::vec3(300.0f, 300.0f, 0.0f));
		model3 = glm::scale(model3, glm::vec3(4.0f, 1.0f, 1.0f));
		model3 = glm::rotate(model3, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));

		glUniform1i(rasterCodeLoc, 0);
		glUseProgram(shader.ID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUniform1i(rasterCodeLoc, 1);
		glDrawArrays(GL_LINE_LOOP, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat vertices[] = {
		  0.0f,   0.0f, 0.0f, 0.0, 1.0, 0.0,
		 20.0f, 100.0f, 0.0f, 1.0, 0.0, 0.0,
		100.0f,   20.f, 0.0f, 0.0, 0.0, 1.0
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), ((GLvoid*)(3 * sizeof(GLfloat))));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

