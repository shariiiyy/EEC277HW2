#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>


const static std::string FRAGMENT_SHADER =
	    "#version 410 core                                                 \n"
	    "                                                                  \n"
	    "out vec4 color;                                                   \n"
	    "                                                                  \n"
	    "void main(void)                                                   \n"
	    "{                                                                 \n"
	    "    color = vec4(0.0, *****, 1.0, 1.0);                           \n"
	    "}                                                                 \n";

const GLchar* VERTEX_SHADER =
	{
	    "#version 410 core                                                 \n"
	    "                                                                  \n"
	    "void main(void)                                                   \n"
	    "{                                                                 \n"
	    "    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
	    "                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
	    "                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
	    "                                                                  \n"
	    "    gl_Position = vertices[gl_VertexID];                          \n"
	    "}                                                                 \n"
};

const std::string ops[] = { "sin", "cos" , "log", "exp" };


double run_test(GLFWwindow* window, std::string op, float start, float end, float step);
double getCPUValue(std::string op, float x);
GLFWwindow* initialize_WIN();
std::string getString(float i);
std::string getFragShader(std::string op, float i);
float getGPUResult(GLFWwindow* window, std::string op, float input_number);



