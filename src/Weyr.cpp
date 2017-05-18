#include <stdio.h>
#include <iostream>
#include <vector>
#include <lodepng/lodepng.h>
// glm additional header to generate transformation matrices directly.
#include <glm/gtc/matrix_transform.hpp>

#include "helpers/ProgramUtilities.h"
#include "helpers/MeshUtilities.h"

#include "Weyr.h"

Weyr::Weyr(){}

Weyr::~Weyr(){}

void Weyr::init(const std::string& objectPath, const std::string& texturePath){
	
	_time = 0.0;
	
	// Load the shaders
	//_programDepthId = createGLProgram("ressources/shaders/object_depth.vert","ressources/shaders/object_depth.frag");
	_programId = createGLProgram("ressources/shaders/weyr.vert","ressources/shaders/weyr.frag");
	
	// Load geometry.
	mesh_t mesh;
	loadObj("ressources/primitive.obj",mesh,Indexed);
	centerAndUnitMesh(mesh);
	computeTangentsAndBinormals(mesh);
	
	// Load geometry.
	mesh_t object;
	loadObj(objectPath,object,Indexed);
	centerAndUnitMesh(object);

	_primitiveCount = mesh.indices.size();
	_objectCount = object.positions.size();
	
	// Create an array buffer to host the geometry data.
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Upload the data to the Array buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.positions.size() * 3, &(mesh.positions[0]), GL_STATIC_DRAW);

	GLuint vbo_nor = 0;
	glGenBuffers(1, &vbo_nor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_nor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.normals.size() * 3, &(mesh.normals[0]), GL_STATIC_DRAW);

	GLuint vbo_uv = 0;
	glGenBuffers(1, &vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.texcoords.size() * 2, &(mesh.texcoords[0]), GL_STATIC_DRAW);

	GLuint vbo_tan = 0;
	glGenBuffers(1, &vbo_tan);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tan);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.tangents.size() * 3, &(mesh.tangents[0]), GL_STATIC_DRAW);

	GLuint vbo_binor = 0;
	glGenBuffers(1, &vbo_binor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_binor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.binormals.size() * 3, &(mesh.binormals[0]), GL_STATIC_DRAW);
	
	
	GLuint vbo_object = 0;
	glGenBuffers(1, &vbo_object);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * object.positions.size() * 3, &(object.positions[0]), GL_STATIC_DRAW);
	
	GLuint vbo_object_nor = 0;
	glGenBuffers(1, &vbo_object_nor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_object_nor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * object.normals.size() * 3, &(object.positions[0]), GL_STATIC_DRAW);

	GLuint vbo_object_uv = 0;
	glGenBuffers(1, &vbo_object_uv);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_object_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * object.texcoords.size() * 2, &(object.texcoords[0]), GL_STATIC_DRAW);
	
	// Generate a vertex array (useful when we add other attributes to the geometry).
	_vao = 0;
	glGenVertexArrays (1, &_vao);
	glBindVertexArray(_vao);
	// The first attribute will be the vertices positions.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(0, 0);

	// The second attribute will be the normals.
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_nor);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(1, 0);

	// The third attribute will be the uvs.
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(2, 0);

	// The fourth attribute will be the tangents.
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_tan);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(3, 0);

	// The fifth attribute will be the binormals.
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_binor);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(4, 0);
	
	// The sixth attribute will be the object vertices positions.
	glEnableVertexAttribArray(5);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_object);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(5, 1);
	
	// The sixth attribute will be the object vertices normals.
	glEnableVertexAttribArray(6);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_object_nor);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(6, 1);
	
	// The sixth attribute will be the object vertices normals.
	glEnableVertexAttribArray(7);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_object_uv);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribDivisor(7, 1);

	// We load the indices data
	glGenBuffers(1, &_ebo);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &(mesh.indices[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	// Get a binding point for the light in Uniform buffer.
	_lightUniformId = glGetUniformBlockIndex(_programId, "Light");
	
	// Load and upload the textures.
	_texColor = loadTexture("ressources/primitive_texture_color.png", _programId, 0,  "textureColor", true);
	
	_texNormal = loadTexture("ressources/primitive_texture_normal.png", _programId, 1, "textureNormal");
	
	_texEffects = loadTexture("ressources/primitive_texture_ao_specular_reflection.png", _programId, 2, "textureEffects");
	
	
	if(texturePath != ""){
		_texObject = loadTexture(texturePath, _programId, 3, "textureObject",true);
		glUseProgram(_programId);
		GLuint useT = glGetUniformLocation(_programId, "useTexture");
		glUniform1i(useT,1);
	} else {
		_texObject = 0;
		glUseProgram(_programId);
		GLuint useT = glGetUniformLocation(_programId, "useTexture");
		glUniform1i(useT,0);
	}
	checkGLError();
	
	_primitiveScale = 0.03f;
	
}


void Weyr::draw(float elapsed, const glm::mat4& view, const glm::mat4& projection, const size_t pingpong){
	
	_time += elapsed;
	
	glm::mat4 model = glm::rotate(glm::mat4(1.0f),float(_time),glm::vec3(0.0f,1.0f,0.0f));
		// Combine the three matrices.
	glm::mat4 V = view * model;
	glm::mat4 VP = projection * V;

	
	
	// Select the program (and shaders).
	glUseProgram(_programId);

	// Select the right sub-uniform buffer to use for the light.
	glUniformBlockBinding(_programId, _lightUniformId, pingpong);
	
	// Upload the MVP matrix.
	GLuint mvpID  = glGetUniformLocation(_programId, "vp");
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &VP[0][0]);
	// Upload the MV matrix.
	GLuint mvID  = glGetUniformLocation(_programId, "view");
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &V[0][0]);
	
	GLuint scaleId  = glGetUniformLocation(_programId, "scale");
	glUniform1f(scaleId, _primitiveScale);
	
	// Bind the textures.
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texColor);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texEffects);
	
	if(_texObject>0){
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, _texObject);
	}
	
	// Select the geometry.
	glBindVertexArray(_vao);
	// Draw!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glDrawElementsInstanced(GL_TRIANGLES, _primitiveCount, GL_UNSIGNED_INT, (void*)0, _objectCount-2);
	
	glBindVertexArray(0);
	glUseProgram(0);
	
	
}


void Weyr::clean(){
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_texColor);
	glDeleteTextures(1, &_texNormal);
	glDeleteTextures(1, &_texEffects);
	glDeleteTextures(1, &_texObject);
	//glDeleteTextures(1, &_texCubeMap);
	//glDeleteTextures(1, &_texCubeMapSmall);
	glDeleteProgram(_programId);
}


