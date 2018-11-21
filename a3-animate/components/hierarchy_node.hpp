/*
	Class that represents a node in the hierarchy tree.
*/

#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET(offset) ((GLvoid*) (offset))

extern std::stack<glm::mat4> MatrixStack;

/*************************************
 						Base class
**************************************/

class HNode {
	protected:
	std::vector<glm::vec4> _vertices;
	std::vector<glm::vec4> _colors;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec2> _texspace;

	std::string _name;

	GLuint _vao, _vbo, _tex;

	// transformation matrices
	glm::mat4 _shape_transformation;				// building shape from primitive (includes scaling, shearing)
	glm::mat4 _rotation;
	glm::mat4 _translation;

	glm::vec3 _parameters[2];
	
	std::vector<HNode*> _children;
	HNode* _parent;
	
	bool isBufferInitialised = false, isTextured = false;
	void add_child(HNode*);

	public:
	HNode(HNode* p = nullptr, const glm::mat4 & = glm::mat4());
	virtual ~HNode();
	void init_buffers();

	void set_name(const std::string &s) { _name = s; };
	std::string get_name() {return _name;}

	void set_texture( const char * filename, int width, int height );
	void remove_texture();
	void set_texspace(std::vector<glm::vec2> &);

	// Reset translation and rotation parameters
	void change_parameters(const glm::vec3 & = glm::vec3(0.0), const glm::vec3 & = glm::vec3(0.0)); 

	// get translation and rotation parameters
	std::vector<float> get_parameters(void);

	// draw node/tree to the screen
	void render_node();
	void render_tree();

	// methods to add translation and rotation
	virtual HNode& translate(const glm::vec3 &);
	virtual HNode& rotate(const glm::vec3 &);
};

/*************************************
 					Derived classes
**************************************/

// Node with restricted degrees of freedom
class Joint : public HNode {
	protected:
	// Axes along which rotation is prohibited
	// true - allowed, false - prohibited
	const bool _rotation_axis[3];

	public:
	Joint(HNode*, const bool (&)[3], const glm::mat4 & = glm::mat4());

	virtual Joint& translate(const glm::vec3 &) { return *this; };
	virtual Joint& rotate(const glm::vec3 &);
};

// Unit sphere that can rotate only about the z-axis
// Orientation and shape can be altered using shape matrix
class Hinge_Joint : public Joint {
	public:
	Hinge_Joint(HNode*, const glm::mat4 & = glm::mat4(), const glm::vec4 & = glm::vec4(1.0), const bool (&)[3] = {false, false, true});
};

// Unit sphere that can rotate about any of x,y or z-axis
// Orientation and shape can be altered using shape matrix
class Ball_And_Socket_Joint : public Joint {
	public:
	Ball_And_Socket_Joint(HNode*, const glm::mat4 & = glm::mat4(), const glm::vec4 & = glm::vec4(1.0));
};

// General class for all rigid components (dof = 0)
class Rigid_Node : public HNode {
	public:
	Rigid_Node(HNode*, const std::vector<glm::vec4>&, const std::vector<glm::vec4>&, const std::vector<glm::vec3> &, const glm::mat4 & = glm::mat4());

	virtual Rigid_Node& translate(const glm::vec3 &) { return *this; };
	virtual Rigid_Node& rotate(const glm::vec3 &) { return *this; };
};

/*************************************************************************************************/

#endif
