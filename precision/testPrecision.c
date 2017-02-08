#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


static const char * vs_source[] =
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


// The error callback will just print any errors that GLFW hits.
void error_callback(int error, const char* description) {
  fputs(description, stderr);
}

// The key callback generally handles any input, but we'll just use the ESC key in this example.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

int
main(int argc, char **argv)
{
	// Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
	if (!glfwInit()) {
	  fprintf(stderr, "Failed initialize GLFW.");
	  exit(EXIT_FAILURE);
	}

	// Set the error callback, as mentioned above.
	glfwSetErrorCallback(error_callback);

	// Set up OpenGL options.
	// Use OpenGL verion 4.1,
	//glfwWindowHint(GLFW_STENCIL_BITS, 8);
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
	  fprintf(stderr, "Failed to create GLFW window.");
	  glfwTerminate();
	  exit(EXIT_FAILURE);
	}

	// Use the window as the current context (everything that's drawn will be place in this window).
	glfwMakeContextCurrent(window);

	// Set the keyboard callback so that when we press ESC, it knows what to do.
	glfwSetKeyCallback(window, key_callback);

	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));

	// Makes sure all extensions will be exposed in GLEW and initialize GLEW.
	glewExperimental = GL_TRUE;
	glewInit();

	// Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
	// Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
	
	

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	//change arithmetic operation here
	static const char * fs_source[] =
	{
	    "#version 410 core                                                 \n"
	    "                                                                  \n"
	    "out vec4 color;                                                   \n"
	    "                                                                  \n"
	    "void main(void)                                                   \n"
	    "{                                                                 \n"
	    "    color = vec4(0.0, sin(7.0), 1.0, 1.0);                        \n"
	    "}                                                                 \n"
	};

	// This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
	GLuint program = glCreateProgram();

	// We create a shader with our fragment shader source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");

	}



	// Link shaders
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	// Check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
	}

	
	GLuint vao;
	// Generate vertex arrays for our program. More explanation on this will come in the future.
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// We'll specify that we want to use this program that we've attached the shaders to.
	glUseProgram(program);

	GLfloat* out = malloc(sizeof(GLfloat*) * 4);

	// As long as our window remains open (ESC is not pressed), we'll continue to render things.
	//while(!glfwWindowShouldClose(window))
	//{
	  // Set up our green background color
	  static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f};
	  // Clear the entire buffer with our green color (sets the background to be green).
	  glClearBufferfv(GL_COLOR, 0, green);

	  // Draw our triangles
	  glDrawArrays(GL_TRIANGLES, 0, 3);



	  // Swap the buffers so that what we drew will appear on the screen.
	  glfwSwapBuffers(window);
	  glfwPollEvents();

	//}

	glReadPixels(75, 75, 1, 1, GL_RGB, GL_FLOAT, out);



	printf("out(gpu): %f \t out(cpu): %f\n", out[1], sin(7));


}


