#ifndef Weyr_h
#define Weyr_h
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>



class Weyr {

public:

	Weyr();

	~Weyr();

	/// Init function
	void init(const std::string& objectPath, const std::string& texturePath);

	/// Draw function
	void draw(float elapsed, const glm::mat4& view, const glm::mat4& projection, const size_t pingpong);
	
	/// Clean function
	void clean();

	
	float _primitiveScale;
private:
	
	GLuint _programId;
	GLuint _vao;
	GLuint _ebo;
	GLuint _texColor;
	GLuint _texNormal;
	GLuint _texEffects;
	GLuint _texObject;
	GLuint _lightUniformId;
	
	size_t _primitiveCount;
	size_t _objectCount;
	double _time;
	glm::mat4 _lightMVP;
	
};

#endif
