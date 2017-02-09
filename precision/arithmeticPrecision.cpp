#include "arithmeticPrecision.h"

int main()
{
	double result = 0;
	std::ofstream file_t("arithmetic_precision.txt",std::ios::out);
	file_t.precision(30);
	file_t << "\nThis file contains average realtive error of sin, cos, log and exp operations." << std::endl;
	
	GLFWwindow* window = initialize_WIN();

	//200 number for each test
	//rgb value has to be between 0 and 1
	result = run_test(window, ops[0], 6.3f, 9.45f, 0.01575f);
	file_t << "sin, " << result<<std::endl;
	
	result=run_test(window, ops[1], 4.78f, 7.78f, 0.015f);
	file_t << "cos, " << result << std::endl;
	
	result = run_test(window, ops[2], 1.01f, 2.7f, 0.00845f);
	file_t << "log, " << result << std::endl;

	result = run_test(window, ops[3], -6.18f, -0.1f, 0.034f);
	file_t << "exp, " << result << std::endl;
	
	
	file_t.close();
	return 0;
}//main

double run_test(GLFWwindow* window, std::string op, float start, float end, float step) {

	std::ofstream file_t(op, std::ios::out);

	file_t.precision(30);
	file_t << "Relative error for "<< op << ", test started with input "<< start<<" ended at "<< end <<" with a step of "<<step <<std::endl;
	file_t << "input(i)\tGPU\t\tCPU\t\trelativeErr"<<std::endl;
	double sumGPU = 0;
	double sumCPU = 0;
	double maxError = 0;
	for (float i = start; i < end; i += step)
	{
		float gpu = getGPUResult(window, op, i);
		double cpu = getCPUValue(op, i);
		sumGPU +=  fabs(gpu);
		sumCPU +=  fabs(cpu);
		//calulating the relative error 
		double absError = fabs( fabs(cpu) - fabs(gpu));
		double relError = (absError == 0.0) ? 0.0 : fabs(absError / cpu);
		relError = (relError > 1.0) ? 1.0 : relError;
		file_t << std::fixed << std::setprecision(10) << i << "\t" << std::fixed << std::setprecision(10) << gpu << "\t" << std::fixed << std::setprecision(10) << cpu <<"\t" << std::fixed << std::setprecision(10) << relError << std::endl;
		if (relError > maxError)
			maxError = relError;

		//std::cout << "CPU: " << cpu << " GPU: " << gpu << " relative errror: "<< relError << std::endl;	

	}
		
	double temp = fabs(sumCPU - sumGPU);
	double avgError = temp / sumCPU;
	std::cout << "The average relative error for " << op << " is " << avgError << std::endl;
	file_t << "The average relative error for " << op << " is " << avgError << std::endl;
	file_t << "The max realtive error for " << op << " is " << maxError << std::endl ;

	
	file_t.close();
	return avgError;


}

double getCPUValue(std::string op, float x) {

	if(!op.compare("sin"))
		return sin(x);
	else if (!op.compare("cos"))
		return cos(x);
	else if (!op.compare("exp"))
		return exp(x);
	else if (!op.compare("log"))
		return log(x);
	else
		return  (double) x;

}


GLFWwindow* initialize_WIN() {

	if (!glfwInit()) {
	 	std::cout << "ERROR:Failed initialize GLFW.\n" << std::endl;
	  	exit(EXIT_FAILURE);
	}

	// Set up OpenGL options.
	// Use OpenGL verion 3.2,
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// GLFW_OPENGL_FORWARD_COMPAT specifies whether the OpenGL context should be forward-compatible, i.e. one where all functionality deprecated in the requested version of OpenGL is removed.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// Indicate we only want the newest core profile, rather than using backwards compatible and deprecated features.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Make the window resize-able.
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a window to put our stuff in.
	GLFWwindow* window = glfwCreateWindow(160, 160, "OpenGL", NULL, NULL);

	// If the window fails to be created, print out the error, clean up GLFW and exit the program.
	if(!window) {
		std::cout << "ERROR:Failed to create GLFW window.\n" << std::endl;
	  	glfwTerminate();
	  	exit(EXIT_FAILURE);
	}

	return window;
}



std::string getString(float i)
{
	std::ostringstream ss;
	ss << i;
	return std::string (ss.str());

}

/*This function edit the fragment shader string to be used with any arithmetic operation*/
std::string getFragShader(std::string op, float i)
{
	std::string shader(FRAGMENT_SHADER);
	std::string mark("*****");
	std::string replace = op + "(" + getString(i) + ")";
	int pos = shader.find(mark, 0);

	if(pos != std::string::npos){
		shader.replace(pos, mark.length(), replace);
	}
	return shader;

}

 /*This function returns the relative error between the CPU and the GPU for a cerain operation*/
float getGPUResult(GLFWwindow* window, std::string op, float input_number)
{

	glfwMakeContextCurrent(window);
	std::string fragShader = getFragShader(op, input_number);
	const char *fshader = fragShader.c_str();
	float gpu = 0;

	// Makes sure all extensions will be exposed in GLEW and initialize GLEW.
	glewExperimental = GL_TRUE;
	glewInit();

	// This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
	GLuint program = glCreateProgram();

	// We create a shader with our fragment shader source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fshader, NULL);
	glCompileShader(fs);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

	}

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &VERTEX_SHADER, NULL);
	glCompileShader(vs);

	// Link shaders
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	// Check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	GLuint vao;
	// Generate vertex arrays for our program. More explanation on this will come in the future.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// We'll specify that we want to use this program that we've attached the shaders to.
	glUseProgram(program);

	float* out = new float[5];

	// Set up our green background color
	static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f};
	// Clear the entire buffer with our green color (sets the background to be green).
	glClearBufferfv(GL_COLOR, 0, green);

	// Draw our triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Swap the buffers so that what we drew will appear on the screen.
	glfwSwapBuffers(window);
	glfwPollEvents();

	glReadPixels(80, 80, 1, 1, GL_RGB, GL_FLOAT, out);

	gpu = out[1];

	delete[] out;

	return gpu;

}




