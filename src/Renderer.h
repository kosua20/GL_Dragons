#ifndef Renderer_h
#define Renderer_h
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "helpers/MeshUtilities.h"

#include "Framebuffer.h"
#include "camera/Camera.h"
#include "Weyr.h"
#include "Skybox.h"
#include "ScreenQuad.h"
#include "Light.h"



struct Material
{
	glm::vec4 Ka;
	glm::vec4 Kd;
	glm::vec4 Ks;
};


class Renderer {

public:

	Renderer();

	~Renderer();

	/// Init function
	void init(int width, int height, const std::string& objectPath, const std::string& texturePath);

	/// Draw function
	void draw();

	void physics(float elapsedTime);

	/// Clean function
	void clean();

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);

	/// Handle mouse inputs
	void buttonPressed(int button, int action, double x, double y);

	void mousePosition(int x, int y, bool leftPress, bool rightPress);


private:
	
	float _timer;

	GLuint _ubo;
	
	Camera _camera;

	Light _light;
	
	Weyr _weyr;
	Skybox _skybox;
	
	Framebuffer _sceneFramebuffer;
	ScreenQuad _finalScreen;
	
	size_t _pingpong;
	GLuint _padding;
	
};

#endif
