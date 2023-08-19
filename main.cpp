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
	cameraPos += vec;

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
		glm::vec3(86.690f,20.735f,45.331f), // Regulus
glm::vec3(96.732f,24.929f,4.636f), // Denebola
glm::vec3(85.243f,33.942f,39.769f), // Algieba
glm::vec3(91.964f,34.830f,18.154f), // Zosma
glm::vec3(94.607f,26.373f,18.816f), // Chertan
glm::vec3(84.611f,28.840f,44.825f), // Eta Leonis
glm::vec3(82.652f,39.742f,39.864f), // Adhafera
glm::vec3(75.891f,43.660f,48.316f), // Rasalas
glm::vec3(76.630f,40.142f,50.164f), // Epsilon Leonis


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
glm::vec3(3.729f,-64.457f,-76.363f), // Iota Scorpii
		
		//taurus
glm::vec3(-34.389f,28.417f,89.498f), // Aldebaran
glm::vec3(-41.886f,27.428f,86.564f), // Gamma Tauri
glm::vec3(-49.771f,40.841f,76.517f), // Pleiades
glm::vec3(-38.084f,30.141f,87.413f), // Delta Tauri
glm::vec3(-12.850f,47.881f,86.846f), // Elnath
glm::vec3(-10.428f,36.055f,92.689f), // Zeta Tauri
glm::vec3(-47.268f,21.659f,85.420f), // Lambda Tauri
glm::vec3(-59.924f,15.731f,78.496f), // Omicron Tauri
glm::vec3(-35.761f,27.588f,89.219f), // Theta square Tauri
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

//pegasus
glm::vec3(-87.225f,48.803f,3.181f), // Alpheratz
glm::vec3(-96.269f,26.309f,6.337f), // Algenib
glm::vec3(-93.692f,26.304f,-23.019f), // Markab
glm::vec3(-85.503f,47.228f,-21.417f), // Scheat
glm::vec3(-92.235f,19.005f,-33.639f), // Homam
glm::vec3(-88.438f,10.516f,-45.478f), // Biham
glm::vec3(-81.978f,17.343f,-54.579f), // Enif
glm::vec3(-81.735f,50.501f,-27.733f), // Matar
glm::vec3(-74.271f,54.889f,-38.353f), // Pi Pegasi
glm::vec3(-86.958f,40.172f,-28.716f), // Lambda Pegasi
glm::vec3(-79.785f,42.978f,-42.277f), // Iota Pegasi
glm::vec3(-75.002f,43.451f,-49.867f), // Kappa Pegasi

//canis major
glm::vec3(31.288f, -48.944f, 81.398f), // Aludra
glm::vec3(33.206f, -44.555f, 83.140f), // Omega Canis Majoris
glm::vec3(26.589f, -44.485f, 85.522f), // Wezen
glm::vec3(22.144f, -48.438f, 84.637f), // Adhara
glm::vec3(8.115f, -50.105f, 86.161f), // Furud
glm::vec3(25.029f, -40.442f, 87.966f), // Omicron² Canis Majoris
glm::vec3(18.736f, -28.763f, 93.924f), // Sirius
glm::vec3(9.550f, -30.828f, 94.649f), // Mirzam
glm::vec3(15.030f, -32.979f, 93.202f), // Nu² Canis Majoris
glm::vec3(14.997f, -39.022f, 90.843f), // Xi² Canis Majoris
glm::vec3(23.643f, -29.328f, 92.633f), // Iota Canis Majoris
glm::vec3(26.512f, -26.977f, 92.571f), // Multiphen
glm::vec3(23.233f, -20.882f, 94.996f), // Theta Canis Majoris

//aquarius
glm::vec3(-94.009f, 0.007f, -34.092f), // Eta Aquarii
glm::vec3(-96.132f, -17.291f, -21.439f), // Psi1 Aquarii
glm::vec3(-91.574f, -34.133f, -21.194f), // B1 Aquarii
glm::vec3(-85.595f, -23.785f, -45.911f), // Iota Aquarii
glm::vec3(-92.832f, -22.860f, -29.320f), // Tau Aquarii
glm::vec3(-87.839f, -0.405f, -47.793f), // Sadalmelik
glm::vec3(-79.603f, -9.508f, -59.774f), // Sadalsuud
glm::vec3(-92.365f, -27.030f, -27.169f), // Skat
glm::vec3(-89.129f, -13.326f, -43.341f), // Ancha
glm::vec3(-67.296f, -16.503f, -72.103f), // Albali
glm::vec3(-90.934f, -2.188f, -41.549f), // Sadachbia
glm::vec3(-94.932f, -12.980f, -28.624f), // Lambda Aquarii
glm::vec3(-92.764f, -0.178f, -37.347f), // Zeta Aquarii

//sagittarius
glm::vec3(-25.885f, -46.387f, -84.724f), // Tau Sagittarii
glm::vec3(-21.421f, -44.260f, -87.076f), // Nunki
glm::vec3(-23.394f, -49.755f, -83.529f), // Ascella
glm::vec3(-18.275f, -45.343f, -87.235f), // Phi Sagittarii
glm::vec3(-8.627f, -42.927f, -89.905f), // Kaus Borealis
glm::vec3(-9.823f, -49.741f, -86.194f), // Kaus Media
glm::vec3(-8.673f, -56.475f, -82.069f), // Kaus Australis
glm::vec3(-6.606f, -59.841f, -79.846f), // Eta Sagittarii
glm::vec3(-2.701f, -50.640f, -86.187f), // Alnasl (Gamma2 Sagittarii)

//cygnus
glm::vec3(-19.774f, 80.185f, -56.386f), // Kappa Cygni
glm::vec3(-23.989f, 78.546f, -57.053f), // Iota Cygni
glm::vec3(-31.230f, 70.936f, -63.188f), // Delta Cygni
glm::vec3(-44.330f, 64.713f, -62.025f), // Sadr
glm::vec3(-45.511f, 71.145f, -53.546f), // Deneb
glm::vec3(-55.219f, 56.006f, -61.760f), // Epsilon Cygni
glm::vec3(-65.191f, 50.487f, -56.580f), // Zeta Cygni
glm::vec3(-72.899f, 48.289f, -48.517f), // Mu Cygni
glm::vec3(-40.019f, 57.545f, -71.324f), // Eta Cygni
glm::vec3(-34.059f, 46.962f, -81.453f), // Albireo

//aries
glm::vec3(-82.898f, 33.230f, 44.986f), // Gamma1 Arietis
glm::vec3(-81.963f, 35.703f, 44.804f), // Sheratan
glm::vec3(-77.762f, 39.949f, 48.551f), // Hamal
glm::vec3(-65.190f, 45.953f, 60.320f), // 41 Arietis

//capricornus
glm::vec3(-81.385f, -27.583f, -51.143f), // Deneb Algedi
glm::vec3(-78.726f, -28.496f, -54.682f), // Nashira
glm::vec3(-74.199f, -28.767f, -60.555f), // Iota Capricorni
glm::vec3(-71.689f, -38.012f, -58.445f), // Zeta Capricorni
glm::vec3(-69.582f, -29.465f, -65.499f), // Theta Capricorni
glm::vec3(-61.136f, -45.115f, -65.016f), // Omega Capricorni
glm::vec3(-60.243f, -42.562f, -67.522f), // Psi Capricorni
glm::vec3(-56.174f, -25.508f, -78.701f), // Dabih
glm::vec3(-52.221f, -21.578f, -82.507f), // Algedi

//aquila
glm::vec3(-48.272f, 11.258f, -86.851f), // Alshain
glm::vec3(-45.909f, 15.503f, -87.476f), // Altair
glm::vec3(-43.952f, 18.418f, -87.915f), // Tarazed
glm::vec3(-36.536f, 5.496f, -92.924f), // Delta Aquilae
glm::vec3(-47.425f, 1.850f, -88.020f), // Eta Aquilae
glm::vec3(-54.577f, -1.323f, -83.783f), // Theta Aquilae
glm::vec3(-27.756f, 24.038f, -93.015f), // Zeta Aquilae
glm::vec3(-25.030f, 25.909f, -93.286f), // Epsilon Aquilae
glm::vec3(-28.694f, -8.435f, -95.423f), // Lambda Aquilae

//cancer
glm::vec3(69.219f, 20.396f, 69.229f), // Acubens
glm::vec3(62.870f, 31.004f, 71.317f), // Asellus Australis
glm::vec3(55.530f, 15.964f, 81.619f), // Beta Cancri
glm::vec3(61.143f, 36.457f, 70.231f), // Asellus Borealis
glm::vec3(58.748f, 47.970f, 65.173f), // Iota Cancri
glm::vec3(51.441f, 45.610f, 72.619f), // Chi Cancri

//lyra
glm::vec3(-12.508f, 62.638f, -76.942f), // Vega
glm::vec3(-18.073f, 55.024f, -81.521f), // Sheliak
glm::vec3(-21.397f, 54.049f, -81.369f), // Sulafat
glm::vec3(-15.649f, 61.029f, -77.656f), // Zeta1 Lyrae
glm::vec3(-19.019f, 60.070f, -77.653f), // Delta Lyrae
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
		-8.370f, -3.403f, 99.591f,     // Alnitak //6

		//leo
		86.690f, 20.735f, 45.331f, // Regulus // 7
96.732f, 24.929f, 4.636f, // Denebola // 8
85.243f, 33.942f, 39.769f, // Algieba // 9
91.964f, 34.830f, 18.154f, // Zosma // 10
94.607f, 26.373f, 18.816f, // Chertan // 11
84.611f, 28.840f, 44.825f, // Eta Leonis // 12
82.652f, 39.742f, 39.864f, // Adhafera // 13
75.891f, 43.660f, 48.316f, // Rasalas // 14
76.630f, 40.142f, 50.164f, // Epsilon Leonis // 15

//ursa major
45.913f, 88.089f, 11.504f, // Dubhe // 16
53.581f, 83.282f, 13.895f, // Merak // 17
59.208f, 80.572f, 1.594f, // Phecda // 18
54.297f, 83.896f, -3.662f, // Megrez // 19
54.443f, 82.855f, -13.079f, // Alioth // 20
53.664f, 81.835f, -20.573f, // Mizar // 21
58.149f, 75.825f, -29.485f, // Alkaid // 22

// 23 //cassiopeia
-53.895f, 83.533f, 10.843f, // Schedar // 23
-51.089f, 85.941f, 2.037f, // Caph // 24
-47.207f, 87.290f, 12.330f, // Gamma Cassiopeiae // 25
-45.118f, 86.980f, 19.970f, // Ruchbah // 26
-38.765f, 89.625f, 21.554f, // Segin // 27

// 28 //scorpius
9.150f, -60.321f, -79.232f, // Shaula // 28
5.440f, -63.013f, -77.458f, // Kappa Scorpii // 29
6.517f, -69.441f, -71.663f, // Sargas // 30
13.742f, -68.518f, -71.529f, // N Scorpii // 31
20.566f, -67.430f, -70.924f, // Zeta² Scorpii // 32
24.864f, -56.353f, -78.779f, // Epsilon Scorpii // 33
31.218f, -47.286f, -82.398f, // Tau Scorpii // 34
34.485f, -44.510f, -82.641f, // Antares // 35
46.923f, -33.857f, -81.560f, // Acrab // 36
46.043f, -38.462f, -80.004f, // Dschubba // 37
45.022f, -43.193f, -78.150f, // Pi Scorpii // 38
3.729f, -64.457f, -76.363f, // Iota Scorpii // 39

// 40 //taurus
-34.389f, 28.417f, 89.498f, // Aldebaran // 40
-41.886f, 27.428f, 86.564f, // Gamma Tauri // 41
-49.771f, 40.841f, 76.517f, // Pleiades // 42
-38.084f, 30.141f, 87.413f, // Delta Tauri // 43
-12.850f, 47.881f, 86.846f, // Elnath // 44
-10.428f, 36.055f, 92.689f, // Zeta Tauri // 45
-47.268f, 21.659f, 85.420f, // Lambda Tauri // 46
-59.924f, 15.731f, 78.496f, // Omicron Tauri // 47
-35.761f, 27.588f, 89.219f, // Theta square Tauri // 48
-36.048f, 32.910f, 87.278f, // Ain // 49
-29.919f, 39.096f, 87.042f, // Tau Tauri // 50


// 51 //gemini
39.162f, 46.993f, 79.107f, // Pollux // 51
34.060f, 52.829f, 77.775f, // Castor // 52
40.457f, 41.104f, 81.693f, // Kappa Gem // 53
36.345f, 45.181f, 81.472f, // Upsilon Gem // 54
32.889f, 46.561f, 82.161f, // Iota Gem // 55
27.250f, 50.272f, 82.038f, // Tau Gem // 56
20.715f, 55.861f, 80.314f, // Theta Gem // 57
34.907f, 37.428f, 85.911f, // Wasat // 58
32.305f, 28.368f, 90.286f, // Lambda Gem // 59
25.970f, 35.042f, 89.987f, // Mekbuda // 60
19.019f, 22.274f, 95.615f, // Xi Gem // 61
15.709f, 28.234f, 94.636f, // Alhena // 62
17.249f, 42.467f, 88.876f, // Mebsuta // 63
14.069f, 34.382f, 92.844f, // Nu Gem // 64
9.656f, 38.284f, 91.875f, // Tejat // 65
5.746f, 38.284f, 92.202f, // Propus // 66
3.806f, 39.547f, 91.769f, // 1 Geminorum // 67

// 68 //ursa minor
-0.795f, 99.994f, 0.802f, // Polaris // 68
20.073f, 96.201f, -18.508f, // Kochab // 69
19.963f, 95.016f, -23.948f, // Pherkad // 70
0.871f, 99.823f, -5.890f, // Yildun // 71
4.506f, 99.019f, -13.229f, // Epsilon Ursae Minoris // 72
11.816f, 97.719f, -17.644f, // Zeta Ursae Minoris // 73
10.847f, 96.926f, -22.084f, // Eta Ursae Minoris // 74

// 75 //pegasus
- 87.225f, 48.803f, 3.181f, // Alpheratz // 75
-96.269f, 26.309f, 6.337f, // Algenib // 76
-93.692f, 26.304f, -23.019f, // Markab // 77
-85.503f, 47.228f, -21.417f, // Scheat // 78
-92.235f, 19.005f, -33.639f, // Homam // 79
-88.438f, 10.516f, -45.478f, // Biham // 80
-81.978f, 17.343f, -54.579f, // Enif // 81
-81.735f, 50.501f, -27.733f, // Matar // 82
-74.271f, 54.889f, -38.353f, // Pi Pegasi // 83
-86.958f, 40.172f, -28.716f, // Lambda Pegasi // 84
-79.785f, 42.978f, -42.277f, // Iota Pegasi // 85
-75.002f, 43.451f, -49.867f, // Kappa Pegasi // 86

// 87 //canis major
31.288f, -48.944f, 81.398f, // Aludra // 87
33.206f, -44.555f, 83.140f, // Omega Canis Majoris // 88
26.589f, -44.485f, 85.522f, // Wezen // 89
22.144f, -48.438f, 84.637f, // Adhara // 90
8.115f, -50.105f, 86.161f, // Furud // 91
25.029f, -40.442f, 87.966f, // Omicron² Canis Majoris // 92
18.736f, -28.763f, 93.924f, // Sirius // 93
9.550f, -30.828f, 94.649f, // Mirzam // 94
15.030f, -32.979f, 93.202f, // Nu² Canis Majoris // 95
14.997f, -39.022f, 90.843f, // Xi² Canis Majoris // 96
23.643f, -29.328f, 92.633f, // Iota Canis Majoris // 97
26.512f, -26.977f, 92.571f, // Multiphen // 98
23.233f, -20.882f, 94.996f, // Theta Canis Majoris // 99

// 100 //aquarius
-94.009f, 0.007f, -34.092f, // Eta Aquarii // 100
-96.132f, -17.291f, -21.439f, // Psi1 Aquarii // 101
-91.574f, -34.133f, -21.194f, // B1 Aquarii // 102
-85.595f, -23.785f, -45.911f, // Iota Aquarii // 103
-92.832f, -22.860f, -29.320f, // Tau Aquarii // 104
-87.839f, -0.405f, -47.793f, // Sadalmelik // 105
-79.603f, -9.508f, -59.774f, // Sadalsuud // 106
-92.365f, -27.030f, -27.169f, // Skat // 107
-89.129f, -13.326f, -43.341f, // Ancha // 108
-67.296f, -16.503f, -72.103f, // Albali // 109
-90.934f, -2.188f, -41.549f, // Sadachbia // 110
-94.932f, -12.980f, -28.624f, // Lambda Aquarii // 111
-92.764f, -0.178f, -37.347f, // Zeta Aquarii // 112

// 113 //sagittarius
-25.885f, -46.387f, -84.724f, // Tau Sagittarii // 113
-21.421f, -44.260f, -87.076f, // Nunki // 114
-23.394f, -49.755f, -83.529f, // Ascella // 115
-18.275f, -45.343f, -87.235f, // Phi Sagittarii // 116
-8.627f, -42.927f, -89.905f, // Kaus Borealis // 117
-9.823f, -49.741f, -86.194f, // Kaus Media // 118
-8.673f, -56.475f, -82.069f, // Kaus Australis // 119
-6.606f, -59.841f, -79.846f, // Eta Sagittarii // 120
-2.701f, -50.640f, -86.187f, // Alnasl (Gamma2 Sagittarii) // 121

// 122 //cygnus
- 19.774f, 80.185f, -56.386f, // Kappa Cygni // 122
-23.989f, 78.546f, -57.053f, // Iota Cygni // 123
-31.230f, 70.936f, -63.188f, // Delta Cygni // 124
-44.330f, 64.713f, -62.025f, // Sadr // 125
-45.511f, 71.145f, -53.546f, // Deneb // 126
-55.219f, 56.006f, -61.760f, // Epsilon Cygni // 127
-65.191f, 50.487f, -56.580f, // Zeta Cygni // 128
-72.899f, 48.289f, -48.517f, // Mu Cygni // 129
-40.019f, 57.545f, -71.324f, // Eta Cygni // 130
-34.059f, 46.962f, -81.453f, // Albireo // 131

// 132 //aries
-82.898f, 33.230f, 44.986f, // Gamma1 Arietis // 132
-81.963f, 35.703f, 44.804f, // Sheratan // 133
-77.762f, 39.949f, 48.551f, // Hamal // 134
-65.190f, 45.953f, 60.320f, // 41 Arietis // 135

// 136 //capricornus
-81.385f, -27.583f, -51.143f, // Deneb Algedi // 136
-78.726f, -28.496f, -54.682f, // Nashira // 137
-74.199f, -28.767f, -60.555f, // Iota Capricorni // 138
-71.689f, -38.012f, -58.445f, // Zeta Capricorni // 139
-69.582f, -29.465f, -65.499f, // Theta Capricorni // 140
-61.136f, -45.115f, -65.016f, // Omega Capricorni // 141
-60.243f, -42.562f, -67.522f, // Psi Capricorni // 142
-56.174f, -25.508f, -78.701f, // Dabih // 143
-52.221f, -21.578f, -82.507f, // Algedi // 144

//aquila
-48.272f, 11.258f, -86.851f, // Alshain // 145
-45.909f, 15.503f, -87.476f, // Altair // 146
-43.952f, 18.418f, -87.915f, // Tarazed // 147
-36.536f, 5.496f, -92.924f, // Delta Aquilae // 148
-47.425f, 1.850f, -88.020f, // Eta Aquilae // 149
-54.577f, -1.323f, -83.783f, // Theta Aquilae // 150
-27.756f, 24.038f, -93.015f, // Zeta Aquilae // 151
-25.030f, 25.909f, -93.286f, // Epsilon Aquilae // 152
-28.694f, -8.435f, -95.423f, // Lambda Aquilae // 153

//cancer
69.219f, 20.396f, 69.229f, // Acubens // 154
62.870f, 31.004f, 71.317f, // Asellus Australis // 155
55.530f, 15.964f, 81.619f, // Beta Cancri // 156
61.143f, 36.457f, 70.231f, // Asellus Borealis // 157
58.748f, 47.970f, 65.173f, // Iota Cancri // 158
51.441f, 45.610f, 72.619f, // Chi Cancri // 159

//lyra
-12.508f, 62.638f, -76.942f, // Vega // 160
-18.073f, 55.024f, -81.521f, // Sheliak // 161
-21.397f, 54.049f, -81.369f, // Sulafat // 162
-15.649f, 61.029f, -77.656f, // Zeta1 Lyrae // 163
-19.019f, 60.070f, -77.653f // Delta Lyrae // 164



	};

	

	const unsigned int indices[]
	{
		//orion
		0,2,0,6,6,4,4,3,3,1,6,5,3,2,1,5,
		//leo
		8,11,8,10,11,7,11,10,10,9,7,12,12,9,9,13,13,14,14,15,
		//ursa major
		22,21,21,20,20,19,19,18,19,16,18,17,17,16,
		//cassiopeia
		27,26,26,25,25,23,23,24,
		//scorpius
		28,29,29,39,39,30,30,31,31,32,32,33,33,34,34,35,35,36,35,37,35,38,
		//taurus
		45,40,44,50,50,49,49,40,49,43,43,41,43,42,41,46,46,47,40,48,48,41,
		//gemini
		51,54,54,53,54,58,54,55,58,59,59,61,58,60,60,62,55,56,56,57,56,63,63,64,63,65,65,66,66,67,52,56,
		//ursa minor
		68,71,71,72,72,73,73,69,73,74,69,70,70,74,
		//pegasus
		75,76,75,78,76,77,77,78,77,79,79,80,80,81,78,84,78,82,84,85,85,86,82,83,
		//canis major
		87,88,88,89,89,90,89,92,90,91,92,93,93,94,93,95,93,97,95,96,97,98,97,99,98,99,
		//aquarius
		109,106,106,105,105,110,110,112,112,100,100,111,111,101,101,102,105,108,108,104,104,107,108,103,
		//sagittarius
		113,114,113,115,114,116,115,119,119,120,119,118,118,116,118,121,121,119,118,117,117,116,115,116,
		//cygnus
		122,123,123,124,124,125,125,126,125,127,127,128,128,129,125,130,130,131,
		//aries
		132,133,133,134,134,135,
		//capricornus
		136,137,137,138,138,139,138,140,139,140,140,141,140,143,142,143,143,144,
		//aquila
		145,146,146,147,146,148,148,149,149,150,148,151,151,152,148,153,
		//cancer
		154,155,155,156,155,157,157,158,157,159,
		//lyra
		160,163,163,161,163,164,161,162,162,164
	};


	float magnitudes[] = {
		//orion
		0.45f,   // Betelgeuse
		0.12f,   // Rigel
		1.64f,   // Bellatrix
		2.23f,   // Mintaka
		1.69f,   // Alnilam
		2.09f,   // Saiph
		1.74f,   // Alnitak

		//leo
		1.35f,   // Regulus
		2.14f,   // Denebola
		2.61f,   // Algieba
		2.56f,   // Zosma
		3.32f,   // Chertan
		2.6f,    // Eta Leonis
		3.44f,   // Adhafera
		3.85f,   // Rasalas
		2.97f,   // Epsilon Leonis

		//ursa major
		1.79f,   // Dubhe
		2.34f,   // Merak
		2.40f,   // Phecda
		3.31f,   // Megrez
		1.76f,   // Alioth
		2.04f,   // Mizar
		1.85f,   // Alkaid

		//cassiopeia
		2.24f,   // Schedar
		2.27f,   // Caph
		2.15f,   // Gamma Cassiopeiae
		2.68f,   // Ruchbah
		3.01f,   // Segin

		//scorpius
		1.62f,   // Shaula
		2.39f,   // Kappa Scorpii
		1.86f,   // Sargas
		4.00f,   // N Scorpii
		2.59f,   // Zeta² Scorpii
		2.29f,   // Epsilon Scorpii
		2.82f,   // Tau Scorpii
		1.06f,   // Antares
		2.62f,   // Acrab
		2.29f,   // Dschubba
		3.55f,   // Pi Scorpii
		3.02f,   // Iota Scorpii

		//taurus
		0.87f,   // Aldebaran
		3.65f,   // Gamma Tauri
		1.60f,   // Pleiades
		2.87f,   // Delta Tauri
		1.68f,   // Elnath
		2.97f,   // Zeta Tauri
		3.41f,   // Lambda Tauri
		3.62f,   // Omicron Tauri
		3.84f,   // Theta square Tauri
		3.52f,   // Ain
		3.87f,   // Tau Tauri

		//gemini
		1.14f,   // Pollux
		1.58f,   // Castor
		3.57f,   // Kappa Gem
		4.06f,   // Upsilon Gem
		4.08f,   // Iota Gem
		4.28f,   // Tau Gem
		2.62f,   // Theta Gem
		3.52f,   // Wasat
		4.22f,   // Lambda Gem
		3.42f,   // Mekbuda
		3.34f,   // Xi Gem
		1.93f,   // Alhena
		2.70f,   // Mebsuta
		3.62f,   // Nu Gem
		2.66f,   // Tejat
		3.28f,   // Propus
		2.86f,   // 1 Geminorum

		//ursa minor
		1.98f,   // Polaris
		2.07f,   // Kochab
		3.41f,   // Pherkad
		4.83f,   // Yildun
		5.1f,    // Epsilon Ursae Minoris
		2.99f,   // Zeta Ursae Minoris
		4.16f,   // Eta Ursae Minoris

		//pegasus
		2.06f,   // Alpheratz
		2.83f,   // Algenib
		2.48f,   // Markab
		2.44f,   // Scheat
		2.44f,   // Homam
		2.5f,    // Biham
		2.38f,   // Enif
		3.39f,   // Matar
		2.88f,   // Pi Pegasi
		3.75f,   // Lambda Pegasi
		3.82f,   // Iota Pegasi
		3.84f,   // Kappa Pegasi

		//canis major
		2.45f,   // Aludra
		3.05f,   // Omega Canis Majoris
		1.84f,   // Wezen
		1.50f,   // Adhara
		2.93f,   // Furud
		4.0f,    // Omicron² Canis Majoris
		-1.46f,  // Sirius
		1.98f,   // Mirzam
		3.55f,   // Nu² Canis Majoris
		4.02f,   // Xi² Canis Majoris
		2.37f,   // Iota Canis Majoris
		2.85f,   // Multiphen
		3.27f,   // Theta Canis Majoris

		//aquarius
		3.53f,   // Eta Aquarii
		4.20f,   // Psi1 Aquarii
		3.36f,   // B1 Aquarii
		3.93f,   // Iota Aquarii
		4.23f,   // Tau Aquarii
		2.95f,   // Sadalmelik
		2.87f,   // Sadalsuud
		3.22f,   // Skat
		4.16f,   // Ancha
		3.77f,   // Albali
		3.94f,   // Sadachbia
		3.62f,   // Lambda Aquarii
		2.87f,   // Zeta Aquarii

		//sagittarius
		2.81f,   // Tau Sagittarii
		2.05f,   // Nunki
		2.69f,   // Ascella
		2.87f,   // Phi Sagittarii
		2.70f,   // Kaus Borealis
		2.82f,   // Kaus Media
		1.83f,   // Kaus Australis
		3.11f,   // Eta Sagittarii
		2.93f,   // Alnasl (Gamma2 Sagittarii)

		//cygnus
		3.91f,   // Kappa Cygni
		3.76f,   // Iota Cygni
		2.86f,   // Delta Cygni
		2.23f,   // Sadr
		1.25f,   // Deneb
		2.48f,   // Epsilon Cygni
		2.87f,   // Zeta Cygni
		3.09f,   // Mu Cygni
		3.89f,   // Eta Cygni
		3.08f,   // Albireo

		//aries
		1.95f,   // Gamma1 Arietis
		2.64f,   // Sheratan
		2.00f,   // Hamal
		3.62f,   // 41 Arietis

		//capricornus
		2.87f,   // Deneb Algedi
		3.69f,   // Nashira
		4.05f,   // Iota Capricorni
		3.77f,   // Zeta Capricorni
		3.57f,   // Theta Capricorni
		4.13f,   // Omega Capricorni
		4.13f,   // Psi Capricorni
		3.05f,   // Dabih
		2.93f,   // Algedi

		//aquila
		1.75f,   // Alshain
		0.76f,   // Altair
		2.69f,   // Tarazed
		3.40f,   // Delta Aquilae
		3.18f,   // Eta Aquilae
		3.29f,   // Theta Aquilae
		2.98f,   // Zeta Aquilae
		3.85f,   // Epsilon Aquilae
		3.42f,   // Lambda Aquilae

		//cancer
		4.25f,   // Acubens
		3.94f,   // Asellus Australis
		3.52f,   // Beta Cancri
		4.53f,   // Asellus Borealis
		4.05f,   // Iota Cancri
		4.67f,   // Chi Cancri

		//lyra
		0.03f,   // Vega
		2.71f,   // Sheliak
		3.24f,   // Sulafat
		4.37f,   // Zeta1 Lyrae
		3.47f    // Delta Lyrae
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
	bool showSurface=true;

	//related to showing asterisms
	bool showAsterism = true;

	//related to fixing camera Pos
	bool freeCamera = false;

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
		//restricts camera movement
		if (!freeCamera)
		{
			glm::vec3 vec;
			vec.x = cos(glm::radians(lon)) * cos(glm::radians(latitude));
			vec.y = sin(glm::radians(latitude));
			vec.z = sin(glm::radians(lon)) * cos(glm::radians(latitude));
			vec = glm::normalize(vec);
			cameraPos = vec*glm::vec1(1.0f);
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
			float scalefactor =1.2f*( (magnitudes[i] + 1.46) / 6.56);
			model = glm::scale(model, glm::vec3(1.4f-scalefactor, 1.4f-scalefactor, 1.4f-scalefactor));
			//model = glm::rotate(model, 0 * (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, upVector);
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 200.0f);
			shaderprogram.setuniform4fm("model", model);
			shaderprogram.setuniform4fm("view", view);
			shaderprogram.setuniform4fm("projection", projection);
			glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
		}


		if (showAsterism)
		{
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
		ImGui::Checkbox("Asterisms", &showAsterism);
		ImGui::Checkbox("FreeCamera",&freeCamera);
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