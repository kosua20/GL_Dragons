#include <GL/glew.h> // to load OpenGL extensions at runtime
#include <GLFW/glfw3.h> // to set up the OpenGL context and manage window lifecycle and inputs

#include <stdio.h>
#include <iostream>

#include "helpers/ProgramUtilities.h"

#include "Renderer.h"

#define INITIAL_SIZE_WIDTH 800
#define INITIAL_SIZE_HEIGHT 600

/// The shared renderer

Renderer renderer;

/// Help display
void printHelp(){
	std::cout << "GL_Dragons - A dragxels representation" << std::endl;
	std::cout << "Usage: gldragon path/to/obj [path/to/texture.png]" << std::endl;
	std::cout << "Keys : O, P : decrease / increase the size of the dragxels." << std::endl;
}

/// Callbacks

void resize_callback(GLFWwindow* window, int width, int height){
	renderer.resize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	// Handle quitting
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){ 
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	} 
	renderer.keyPressed(key, action);	
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	double x, y;
    glfwGetCursorPos(window, &x, &y);
	renderer.buttonPressed(button, action, x, y);
}


void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
	bool left = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	bool right = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	renderer.mousePosition(xpos,ypos, left, right);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	// Do nothing for now
	// ...
}


/// The main function

int main(int argc, char** argv) {
	
	if(argc < 2){
		std::cerr << "Missing argument." << std::endl;
		printHelp();
		return 1;
	}
	
	std::string objectPath(argv[1]);
	
	if(objectPath == "help"){
		printHelp();
		return 0;
	}
	
	std::string texturePath = "";
	if(argc > 2){
		texturePath = std::string(argv[2]);
	}
	
	
	
	// Initialize glfw, which will create and setup an OpenGL context.
	if (!glfwInit()) {
		std::cerr << "ERROR: could not start GLFW3" << std::endl;
		return 1;
	}
	
	// On OS X, the correct OpenGL profile and version to use have to be explicitely defined.
	#ifdef __APPLE__
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

	// Create a window with a given size. Width and height are macros as we will need them again.
	GLFWwindow* window = glfwCreateWindow(INITIAL_SIZE_WIDTH, INITIAL_SIZE_HEIGHT,"GL_Template", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Bind the OpenGL context and the new window.
	glfwMakeContextCurrent(window);

	// On OS X, GLEW needs the experimental flag, else some extensions won't be loaded.
	#ifdef __APPLE__
	glewExperimental = GL_TRUE;
	#endif
	// Initialize GLEW, for loading modern OpenGL extensions.
	glewInit();
	checkGLError();

	// Create the renderer.
	renderer.init(INITIAL_SIZE_WIDTH,INITIAL_SIZE_HEIGHT, objectPath, texturePath);

	// Setup callbacks for various interactions and inputs.
	glfwSetFramebufferSizeCallback(window, resize_callback);	// Resizing the window
	glfwSetKeyCallback(window,key_callback);					// Pressing a key
	glfwSetMouseButtonCallback(window,mouse_button_callback);	// Clicking the mouse buttons
	glfwSetCursorPosCallback(window,cursor_pos_callback);		// Moving the cursor
	glfwSetScrollCallback(window,scroll_callback);				// Scrolling
	
	// On HiDPI screens, we might have to initially resize the framebuffers size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	renderer.resize(width, height);
	
	// Start the display/interaction loop.
	while (!glfwWindowShouldClose(window)) {

		// Update the content of the window.
		renderer.draw();
		
		//Display the result fo the current rendering loop.
		glfwSwapBuffers(window);

		// Update events (inputs,...).
		glfwPollEvents();
	}

	// Remove the window.
	glfwDestroyWindow(window);
	// Clean other ressources
	renderer.clean();
	// Close GL context and any other GLFW resources.
	glfwTerminate();
	return 0;
}


