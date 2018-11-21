#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_
GLuint LoadTexture( const char * filename, int width, int height );
void FreeTexture( GLuint texture );
#endif 
