#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

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
#include"stb_image.h"



int height = 1200;
int width = 1200;

glm::vec3 sunPos = glm::vec3(0.0f, 0.0f, 100.0f);

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

//Cursor mode flag
bool NORMAL_CURSOR = 0;

//camera's field of view
float fov = 60.0f;

//earth's rotation
float motion = 0.04167f;

float latitude = 21.7172;
float longitude = 85.3240;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
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
	if (NORMAL_CURSOR)
	{
		return;
	}

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
	float cameraSpeed = 25.0f;
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - prevTime;
	prevTime = currentTime;
	cameraSpeed *= deltaTime;

	sunPos.x = 100* glm::cos(glm::radians((float)glfwGetTime()));
	sunPos.z = 100*glm::sin(glm::radians((float)glfwGetTime()));

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
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		motion = -2;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{	
		motion = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		motion = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		NORMAL_CURSOR = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		NORMAL_CURSOR = 0;
		
	}
	yaw = yaw + motion * deltaTime;
	cameraFront = directionVector();
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

	float lon = glm::dot(cameraFront, sunPos);
	glm::vec3 vec;
	vec.x = cos(glm::radians(lon)) * cos(glm::radians(latitude));
	vec.y = sin(glm::radians(latitude));
	vec.z = sin(glm::radians(lon)) * cos(glm::radians(latitude));
	vec = glm::normalize(vec);
	cameraFront = vec;
	cameraPos += cameraFront;

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

	glm::vec2 textranslate = glm::vec2(-longitude/360.0f, -(latitude)/180.0f);

	float surfaceVertices[] = {
		//vertices				//texture
		0.0f,110.0f,110.0f,     1.0f+textranslate.x,0.0f+textranslate.y,
		0.0f,-110.0f,110.0f,	1.0f+textranslate.x,1.0f+textranslate.y,	
		0.0f,-110.0f,-110.0f,	0.0f+textranslate.x,1.0f+textranslate.y,	
		0.0f,110.0f,-110.0f,	0.0f+textranslate.x,0.0f+textranslate.y
	};

	unsigned int surfaceIndices[] = {
		0,1,3,
		1,2,3
	};

	glm::vec3 cubePositions[] =
	{
		//Orion

		glm::vec3(-2.091f,12.880f,99.145f), // Betelgeuse
		glm::vec3(-19.507f,-14.263f,97.036f), // Rigel
		glm::vec3(-15.069f,11.060f,98.237f), // Bellatrix
		glm::vec3(-12.187f,-0.524f,99.253f), // Mintaka
		glm::vec3(-10.364f,-2.094f,99.439f), // Alnilam
		glm::vec3(-5.267f,-16.797f,98.438f), // Saiph
		glm::vec3(-8.370f,-3.403f,99.591f), // Alnitak

		//leo
		glm::vec3(86.448f,20.740f,45.788f), // Regulus
		glm::vec3(96.674f,25.156f,4.622f), // Denebola
		glm::vec3(83.859f,33.956f,42.598f), // Algieba
		glm::vec3(91.775f,35.086f,18.609f), // Zosma


		//ursa major
		glm::vec3(45.913f,88.089f,11.504f), // Dubhe
		glm::vec3(53.581f,83.282f,13.895f), // Merak
		glm::vec3(59.208f,80.572f,1.594f), // Phecda
		glm::vec3(54.297f,83.896f,-3.662f), // Megrez
		glm::vec3(54.443f,82.855f,-13.079f), // Alioth
		glm::vec3(53.664f,81.835f,-20.573f), // Mizar
		glm::vec3(58.149f,75.825f,-29.485f), // Alkaid

		//cassiopeia
glm::vec3(-53.895f,83.533f,10.843f), // Schedar
glm::vec3(-51.089f,85.941f,2.037f), // Caph
glm::vec3(-47.207f,87.290f,12.330f), // Gamma Cassiopeiae
glm::vec3(-45.118f,86.980f,19.970f), // Ruchbah
glm::vec3(-38.765f,89.625f,21.554f), // Segin
glm::vec3(-45.741f,87.563f,15.505f), // Navi


		//scorpius
glm::vec3(9.150f,-60.321f,-79.232f), // Shaula
glm::vec3(5.440f,-63.013f,-77.458f), // Kappa Scorpii
glm::vec3(6.517f,-69.441f,-71.663f), // Sargas
glm::vec3(13.742f,-68.518f,-71.529f), // N Scorpii
glm::vec3(20.566f,-67.430f,-70.924f), // Zeta² Scorpii
glm::vec3(24.864f,-56.353f,-78.779f), // Epsilon Scorpii
glm::vec3(31.218f,-47.286f,-82.398f), // Tau Scorpii
glm::vec3(34.485f,-44.510f,-82.641f), // Antares
glm::vec3(46.923f,-33.857f,-81.560f), // Acrab
glm::vec3(46.043f,-38.462f,-80.004f), // Dschubba
glm::vec3(45.022f,-43.193f,-78.150f), // Pi Scorpii
		
		//taurus
glm::vec3(-34.389f,28.417f,89.498f), // Aldebaran
glm::vec3(-41.886f,27.428f,86.564f), // Gamma Tauri
glm::vec3(-49.771f,40.841f,76.517f), // Pleiades
glm::vec3(-38.084f,30.141f,87.413f), // Delta Tauri
glm::vec3(-12.850f,47.881f,86.846f), // Elnath
glm::vec3(-10.428f,36.055f,92.689f), // Zeta Tauri
glm::vec3(-47.268f,21.659f,85.420f), // Lambda Tauri
glm::vec3(-59.924f,15.731f,78.496f), // Omicron Tauri
glm::vec3(-13.655f,36.069f,92.264f), // Epsilon Tauri
glm::vec3(-36.048f,32.910f,87.278f), // Ain
glm::vec3(-29.919f,39.096f,87.042f), // Tau Tauri
		
		//gemini
	glm::vec3(39.162f,46.993f,79.107f), // Pollux
glm::vec3(34.060f,52.829f,77.775f), // Castor
glm::vec3(40.457f,41.104f,81.693f), // Kappa Gem
glm::vec3(36.345f,45.181f,81.472f), // Upsilon Gem
glm::vec3(32.889f,46.561f,82.161f), // Iota Gem
glm::vec3(27.250f,50.272f,82.038f), // Tau Gem
glm::vec3(20.715f,55.861f,80.314f), // Theta Gem
glm::vec3(34.907f,37.428f,85.911f), // Wasat
glm::vec3(32.305f,28.368f,90.286f), // Lambda Gem
glm::vec3(25.970f,35.042f,89.987f), // Mekbuda
glm::vec3(19.019f,22.274f,95.615f), // Xi Gem
glm::vec3(15.709f,28.234f,94.636f), // Alhena
glm::vec3(17.249f,42.467f,88.876f), // Mebsuta
glm::vec3(14.069f,34.382f,92.844f), // Nu Gem
glm::vec3(9.656f,38.284f,91.875f), // Tejat
glm::vec3(5.746f,38.284f,92.202f), // Propus
glm::vec3(3.806f,39.547f,91.769f), // 1 Geminorum

//ursa minor
glm::vec3(-0.795f,99.994f,0.802f), // Polaris
glm::vec3(20.073f,96.201f,-18.508f), // Kochab
glm::vec3(19.963f,95.016f,-23.948f), // Pherkad
glm::vec3(0.871f,99.823f,-5.890f), // Yildun
glm::vec3(4.506f,99.019f,-13.229f), // Epsilon Ursae Minoris
glm::vec3(11.816f,97.719f,-17.644f), // Zeta Ursae Minoris
glm::vec3(10.847f,96.926f,-22.084f), // Eta Ursae Minoris
	};

	float starvertices[] =
	{
		//orion
		-2.091f, 12.880f, 99.145f,   // Betelgeuse //0
		-19.507f, -14.263f, 97.036f,  // Rigel
		-15.069f, 11.060f, 98.237f,   // Bellatrix
		-12.187f, -0.524f, 99.253f,   // Mintaka
		-10.364f, -2.094f, 99.439f,   // Alnilam
		-5.267f, -16.797f, 98.438f,   // Saiph
		-8.370f, -3.403f, 99.591f     // Alnitak //6
	};

	const unsigned int indices[]
	{
		//orion
		0,2,0,6,6,4,4,3,3,1,6,5,3,2,1,5
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
	Sphere sphere(0.3f,36,18);

	VAO sphereAO;
	sphereAO.Bind();

	EBO sphereEO(sphere.getIndices(), sphere.getIndexSize());

	VBO sphereBO(sphere.getVertices(), sphere.getVertexSize());

	sphereAO.linkVBO(sphereBO, 0, 3, 3);

	sphereAO.linkEBO(sphereEO);

	sphereAO.Unbind();
	sphereBO.Unbind();
	sphereEO.Unbind();

	//asterisms
	VAO AOasterisms;
	AOasterisms.Bind();
	VBO BOasterisms(starvertices,sizeof(starvertices));
	EBO EOasterisms(indices,sizeof(indices));

	AOasterisms.linkEBO(EOasterisms);
	AOasterisms.linkVBO(BOasterisms, 0, 3, 3);

	AOasterisms.Unbind();
	BOasterisms.Unbind();
	EOasterisms.Unbind();


	//big sphere
	Sphere grid(110.0f,18,6,true,2);
	VAO gridAO;
	gridAO.Bind();

	VBO gridBO(grid.getVertices(), grid.getVertexSize());
	EBO gridEO(grid.getLineIndices(), grid.getLineIndexSize());

	gridAO.linkVBO(gridBO, 0, 3, 3);

	gridAO.linkEBO(gridEO);

	gridAO.Unbind();
	sphereBO.Unbind();
	gridEO.Unbind();

	//surface buffers
	VAO AOsurface;
	AOsurface.Bind();

	VBO BOsurface(surfaceVertices, sizeof(surfaceVertices));
	EBO EOsurface(surfaceIndices, sizeof(surfaceIndices));

	//textures
	int img_width, img_height, nrChannels;
	unsigned char* data = stbi_load("Whole_world_-_land_and_oceans.jpg", &img_width, &img_height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Could not load image!" << std::endl;
	}
	stbi_image_free(data);


	AOsurface.linkVBO(BOsurface, 0, 3, 5);
	AOsurface.linkVBO(BOsurface, 1, 2, 5);
	AOsurface.linkEBO(EOsurface);

	AOsurface.Unbind();
	EOsurface.Unbind();
	BOsurface.Unbind();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//ImGui Initializations
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsClassic();





	//related to position
	bool firstframe=true;

	//related to showing surface
	bool showSurface=false;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ImGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		glfwPollEvents();
		glEnable(GL_DEPTH_TEST);

		
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

		//position camera above the surface
		if (showSurface)
		{
			glm::vec3 vec;
			vec.x = cos(glm::radians(lon)) * cos(glm::radians(latitude));
			vec.y = sin(glm::radians(latitude));
			vec.z = sin(glm::radians(lon)) * cos(glm::radians(latitude));
			vec = glm::normalize(vec);
			cameraPos = vec*glm::vec1(3.0f);
		}

		//stars
		shaderprogram.Activate();
		shaderprogram.setuniform1i("tex", 0);
		sphereAO.Bind();
		shaderprogram.setuniform3v("color",glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 model = glm::mat4(1.0f);
		for (int i = 0; i < sizeof(cubePositions)/sizeof(glm::vec3); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			/*model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));*/
			//model = glm::rotate(model, 0 * (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 200.0f);
			shaderprogram.setuniform4fm("model", model);
			shaderprogram.setuniform4fm("view", view);
			shaderprogram.setuniform4fm("projection", projection);
			glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		}


		//Asterisms
		AOasterisms.Bind();
		for (int i = 0; i < sizeof(starvertices) / 3*sizeof(float); i++)
		{
			model = glm::mat4(1.0f);
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 200.0f);
			shaderprogram.setuniform4fm("model", model);
			shaderprogram.setuniform4fm("view", view);
			shaderprogram.setuniform4fm("projection", projection);
			glDrawElements(GL_LINES, sizeof(indices), GL_UNSIGNED_INT, 0);
		}


		//sun
		sphereAO.Bind();
		shaderprogram.setuniform3v("color", glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::mat4(1.0f);
		model = glm::translate(model, sunPos);
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//model = glm::rotate(model, 0 * (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 200.0f);
		shaderprogram.setuniform4fm("model", model);
		shaderprogram.setuniform4fm("view", view);
		shaderprogram.setuniform4fm("projection", projection);
		glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);


		//draw a enclosing sphere for reference. Drawing only the sector and stack lines may help.

		gridAO.Bind();
		glm::vec3 gridcolor = glm::vec3(0.5f, 0.5f, 0.5f);
		shaderprogram.setuniform3v("color", gridcolor);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		shaderprogram.setuniform4fm("model", model);
		shaderprogram.setuniform4fm("view", view);
		shaderprogram.setuniform4fm("projection", projection);
		glDrawElements(GL_LINES, grid.getLineIndexCount(), GL_UNSIGNED_INT, 0);


		if (showSurface)
		{
			//surface
			AOsurface.Bind();
			shaderprogram.setuniform1i("tex", 1);
			shaderprogram.setuniform3v("color", glm::vec3(0.0f, 0.3f, 0.0f));
			float cose_sangle = glm::dot(glm::vec3(-1.0f, 0.0f, 0.0f), glm::normalize(sunPos));
			float e_sangle = acos(cose_sangle);
			model = glm::mat4(1.0f);
			model = glm::rotate(model, e_sangle, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(latitude), glm::vec3(0.0f, 0.0f, -1.0f));
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
			shaderprogram.setuniform4fm("model", model);
			shaderprogram.setuniform4fm("view", view);
			textranslate = glm::vec2(longitude / 360, -(latitude) / 180);
			shaderprogram.setuniform3v("textransform", glm::vec3(textranslate, 1.0f));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		//this else statement is making so that once the checkbutton value is clicked, it does not it does not show surface, the showSurface variable was updated though
		//else
		//{
		//	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		//}
	/*	if (firstframe == true)
		{
			if (latitude > 0)
			{
				cameraPos.z = 1;
			}
			else
			{
				cameraPos.z = -1;
			}
			firstframe = false;
		}*/


		float costheta = glm::dot(glm::normalize(cameraFront), glm::normalize(sunPos));
		if (costheta < 0)
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		else
		{
			glClearColor(0.0f, 0.0f, costheta * 0.4, 1.0f);
		}


		//Imgui
		ImGui::Begin("Controls");
		ImGui::SliderFloat("LAT", &latitude, -90.0f, 90.0f);
		ImGui::SliderFloat("LONG", &longitude, -180.0f, 180.0f);
		ImGui::Checkbox("Surface",&showSurface);
		ImGui::End();


		//ImGUI Render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		glfwSwapBuffers(window);
	}

	VAO1.Delete();
	VBO1.Delete();
	//EBO1.Delete();

	sphereAO.Delete();
	sphereBO.Delete();
	sphereEO.Delete();

	AOasterisms.Delete();
	EOasterisms.Delete();
	BOasterisms.Delete();

	gridAO.Delete();
	gridEO.Delete();
	gridBO.Delete();

	AOsurface.Delete();
	BOsurface.Delete();
	EOsurface.Delete();

	shaderprogram.Delete();
	

	//ImGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}