#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shader.h"
#include"VAO.h"
#include"VBO.h"
#include"Sphere.h"


int height = 1200;
int width = 1200;

//user Input
//for cameraSpeed
float prevTime = glfwGetTime();

//for camera rotation
float yaw = -90.0f;
float pitch = 0.0f;

//mouse positions
int lastX = width / 2;
int lastY = height / 2;

//first mouse position flag
bool firstmouse = GL_TRUE;

//camera's field of view
float fov = 60.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);


glm::vec3 directionVector()
{
	glm::vec3 vec;
	vec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	vec.y = sin(glm::radians(pitch));
	vec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	vec = glm::normalize(vec);
	return vec;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstmouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}
	float sensitivity = 0.1f;
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	yaw += xoffset*sensitivity;
	pitch += yoffset*sensitivity;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	cameraFront = directionVector();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 160.0f)
		fov = 160.0f;
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = 8.0f;
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	cameraSpeed *= deltaTime;

	if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(upVector,cameraFront))*cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, upVector))*cameraSpeed;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	float cubevertices[] =
	{
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f,  0.5f,  
	 0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f, -0.5f,  

	-0.5f,  0.5f, -0.5f,  
	 0.5f,  0.5f, -0.5f,  
	 0.5f,  0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f
	};

	glm::vec3 cubePositions[] =
	{
		//Orion

		glm::vec3(2.091f,12.880f,99.145f), // Betelgeuse
		glm::vec3(19.507f,-14.263f,97.036f), // Rigel
		glm::vec3(15.069f,11.060f,98.237f), // Bellatrix
		glm::vec3(12.187f,-0.524f,99.253f), // Mintaka
		glm::vec3(10.364f,-2.094f,99.439f), // Alnilam
		glm::vec3(5.267f,-16.797f,98.438f), // Saiph
		glm::vec3(8.370f,-3.403f,99.591f), // Alnitak

		//leo
		glm::vec3(-86.448f,20.740f,45.788f), // Regulus
		glm::vec3(-96.674f,25.156f,4.622f), // Denebola
		glm::vec3(-83.859f,33.956f,42.598f), // Algieba
		glm::vec3(-91.775f,35.086f,18.609f), // Zosma

		//ursa major
		glm::vec3(-45.913f,88.089f,11.504f), // Dubhe
		glm::vec3(-53.581f,83.282f,13.895f), // Merak
		glm::vec3(-59.208f,80.572f,1.594f), // Phecda
		glm::vec3(-54.297f,83.896f,-3.662f), // Megrez
		glm::vec3(-54.443f,82.855f,-13.079f), // Alioth
		glm::vec3(-53.664f,81.835f,-20.573f), // Mizar
		glm::vec3(-58.149f,75.825f,-29.485f), // Alkaid

		//cassiopiea
		glm::vec3(54.289f,83.417f,9.707f), // Schedar
		glm::vec3(51.239f,85.851f,2.043f), // Caph
		glm::vec3(48.149f,87.221f,8.609f), // Gamma Cassiopeiae
		glm::vec3(48.906f,86.808f,8.524f), // Ruchbah
		glm::vec3(43.022f,89.625f,10.787f), // Segin
		glm::vec3(45.741f,87.563f,15.505f), // Navi

		//scorpius
		//glm::vec3(-9.150f,-60.321f,-79.232f), // Shaula
		//glm::vec3(-29.052f,-63.823f,-71.293f), // Kappa Scorpii
		//glm::vec3(-27.045f,-68.174f,-67.977f), // Sargas
		//glm::vec3(-34.088f,-33.233f,-87.941f), // N Scorpii
		//glm::vec3(-27.885f,-67.379f,-68.429f), // Zeta¹ Scorpii
		//glm::vec3(-27.885f,-67.379f,-68.429f), // Zeta² Scorpii
		//glm::vec3(-24.864f,-56.353f,-78.779f), // Epsilon Scorpii
		//glm::vec3(-38.003f,-47.286f,-79.497f), // Tau Scorpii
		//glm::vec3(-34.485f,-44.510f,-82.641f), // Antares
		//glm::vec3(-46.923f,-33.857f,-81.560f), // Acrab
		//glm::vec3(-46.043f,-38.462f,-80.004f), // Dschubba
		//glm::vec3(-40.026f,-43.193f,-80.823f), // Pi Scorpii
		
		//taurus
		glm::vec3(34.392f,28.417f,89.497f), // Aldebaran
		glm::vec3(12.869f,47.885f,86.842f), // Elnath
		glm::vec3(49.874f,40.849f,76.446f), // Alcyone
		glm::vec3(49.798f,40.865f,76.487f), // Pleione
		glm::vec3(42.763f,26.942f,86.287f), // Hyadum I
		glm::vec3(43.110f,26.926f,86.119f), // Hyadum II
		glm::vec3(49.858f,40.753f,76.507f), // Atlas
		
		//gemini
		//glm::vec3(-34.060f,52.829f,77.775f), // Castor
		//glm::vec3(-39.162f,46.993f,79.107f), // Pollux
		//glm::vec3(-39.366f,45.197f,80.047f), // υ Gem
		//glm::vec3(-39.166f,46.901f,79.160f), // ι Gem
		//glm::vec3(-40.855f,41.422f,81.334f), // κ Gem
		//glm::vec3(-38.939f,37.428f,84.160f), // δ Gem
		//glm::vec3(-37.937f,35.135f,85.594f), // ζ Gem
		//glm::vec3(-42.520f,28.402f,85.938f), // γ Gem
		//glm::vec3(-37.008f,28.234f,88.506f), // λ Gem
		//glm::vec3(-32.205f,22.308f,92.006f), // ξ Gem
		//glm::vec3(-27.487f,50.377f,81.894f), // τ Gem
		//glm::vec3(-27.396f,54.713f,79.095f), // θ Gem
		//glm::vec3(-9.457f,42.467f,90.039f), // ε Gem
		//glm::vec3(-9.097f,34.530f,93.407f), // ν Gem
		//glm::vec3(-9.349f,38.284f,91.907f), // μ Gem
		//glm::vec3(-9.643f,38.284f,91.877f), // η Gem

		//pegasus
		glm::vec3(93.711f,26.219f,-23.039f), // Markab
		glm::vec3(85.585f,47.070f,-21.438f), // Scheat
		glm::vec3(96.446f,26.185f,3.522f), // Algenib
		glm::vec3(81.714f,17.159f,-55.031f), // Enif
		glm::vec3(94.182f,26.219f,-21.031f), // Homam
		glm::vec3(83.191f,49.364f,-25.348f), // Matar
		glm::vec3(76.689f,54.975f,-33.114f), // Sadalbari
		
		////cygnus
		//glm::vec3(45.569f,71.055f,-53.615f), // Deneb
		//glm::vec3(44.373f,64.626f,-62.085f), // Sadr
		//glm::vec3(34.065f,46.901f,-81.485f), // Albireo
		//glm::vec3(53.700f,55.876f,-63.200f), // Gienah
		//glm::vec3(53.188f,57.472f,-62.193f), // Eta Cygni
		//glm::vec3(47.183f,64.626f,-59.977f), // Gama Cygni
		//glm::vec3(44.625f,70.871f,-54.643f), // Delta Cygni
		//
		////sagittarius
		//glm::vec3(8.657f,-56.468f,-82.076f), // Kaus Australis
		//glm::vec3(21.416f,-44.307f,-87.053f), // Nunki
		//glm::vec3(9.505f,-42.941f,-89.809f), // Kaus Borealis
		//glm::vec3(0.475f,-49.227f,-87.043f), // Ascella
		//glm::vec3(17.020f,-50.633f,-84.537f), // Alnasl
		//glm::vec3(17.045f,-70.041f,-69.309f), // Arkab Prior
		//glm::vec3(20.202f,-70.041f,-68.455f) // Arkab Posterior
	};

	float starvertices[] =
	{
		//orion
		2.091f, 12.880f, 99.145f,   // Betelgeuse
		19.507f, -14.263f, 97.036f,  // Rigel
		15.069f, 11.060f, 98.237f,   // Bellatrix
		12.187f, -0.524f, 99.253f,   // Mintaka
		10.364f, -2.094f, 99.439f,   // Alnilam
		5.267f, -16.797f, 98.438f,   // Saiph
		8.370f, -3.403f, 99.591f     // Alnitak

	};


	GLFWwindow* window=glfwCreateWindow(width, height, "Sky Map", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, width, height);


	glfwSwapBuffers(window);

	shader shaderprogram("default.vert", "default.frag");
	
	//VAO for cube
	VAO VAO1;
	VAO1.Bind();

	//EBO EBO1(indices, sizeof(indices));

	VBO VBO1(cubevertices, sizeof(cubevertices));
	//VBO//layout//number of data//total number of data in a stride
	//offset of the data provided by stride given that previous layouts all have 3 data values
	VAO1.linkVBO(VBO1, 0, 3, 3);
	//VAO1.linkEBO(EBO1);

	VAO1.Unbind();
	VBO1.Unbind();
	//EBO1.Unbind();

	//sphere

	Sphere sphere(0.3f,72,36);

	VAO sphereAO;
	sphereAO.Bind();

	EBO sphereEO(sphere.getIndices(), sphere.getIndexSize());

	VBO sphereBO(sphere.getVertices(), sphere.getVertexSize());

	sphereAO.linkVBO(sphereBO, 0, 3, 8);

	sphereAO.linkEBO(sphereEO);

	sphereAO.Unbind();
	sphereBO.Unbind();
	sphereEO.Unbind();


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		shaderprogram.Activate();
		VAO1.Bind();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//draw cubes
		//for (int i = 0; i < 9; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		//	//model = glm::rotate(model, 0 * (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		//	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
		//	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 2000.0f);
		//	shaderprogram.setuniform4fv("model", model);
		//	shaderprogram.setuniform4fv("view", view);
		//	shaderprogram.setuniform4fv("projection", projection);
		//	glEnable(GL_DEPTH_TEST);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);

		//}

		//sphere
		sphereAO.Bind();
		glm::mat4 model = glm::mat4(1.0f);
		for (int i = 0; i < sizeof(cubePositions)/sizeof(glm::vec3); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			/*model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));*/
			//model = glm::rotate(model, 0 * (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 200.0f);
			shaderprogram.setuniform4fv("model", model);
			shaderprogram.setuniform4fv("view", view);
			shaderprogram.setuniform4fv("projection", projection);
			glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);
		}

		//draw a enclosing sphere for reference. Drawing only the sector and stack lines may help.

		glfwSwapBuffers(window);
	}

	VAO1.Delete();
	VBO1.Delete();
	//EBO1.Delete();

	sphereAO.Delete();
	sphereBO.Delete();
	sphereEO.Delete();
	shaderprogram.Delete();


	

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}