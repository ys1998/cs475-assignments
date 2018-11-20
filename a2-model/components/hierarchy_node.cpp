/*
	Member function definitions for hierarchy node.
*/

#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

// global shader variables
extern unsigned int vPosition, vColor, uModelViewMatrix;

/**********************************
				HNode functions
***********************************/

HNode::HNode(HNode *parent, const glm::mat4 &shape_transformation)
	: _shape_transformation(shape_transformation), _rotation(glm::mat4()),
	  _translation(glm::mat4()), _parameters{glm::vec3(), glm::vec3()} {
	_parent = parent;
	if(parent != nullptr)
		_parent->add_child(this);
}
		

void HNode::init_buffers() {
	int t = sizeof(glm::vec4);
	// initialize vao and vbo of the object
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	// allocate space for buffers
	glBufferData(GL_ARRAY_BUFFER, t*(_vertices.size() + _colors.size()), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, t*_vertices.size(), &_vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, t*_vertices.size(), t*_colors.size(), &_colors[0]);

	// setup the vertex array as per the shader
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(t*_vertices.size()));

	// set flag to true
	isBufferInitialised = true;
}

void HNode::add_child(HNode* child){
	_children.push_back(child);
}

void HNode::change_parameters(const glm::vec3 &translate, const glm::vec3 &rotate) {	
	_parameters[0] = translate;
	_translation = glm::translate(glm::mat4(), translate);

	_parameters[1] = rotate;
	_rotation = glm::rotate(glm::mat4(), glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	_rotation = glm::rotate(_rotation, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	_rotation = glm::rotate(_rotation, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void HNode::render_node(){
	if(!isBufferInitialised)	init_buffers();
	// multiply shape to transformation matrices
	MatrixStack.push(MatrixStack.top() * _shape_transformation);
	
	// pass result to shader
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(MatrixStack.top()));
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());

	// remove top element of stack
	MatrixStack.pop();
}

void HNode::render_tree(){
	// push matrices to stack for transformations
	MatrixStack.push(MatrixStack.top() * _translation);
	MatrixStack.push(MatrixStack.top() * _rotation);
	
	// draw current node to screen
	render_node();
	
	// draw child nodes
	for(size_t i = 0; i < _children.size(); i++) {
		_children[i]->render_tree();
	}
	
	// remove pushed matrices
	MatrixStack.pop();
	MatrixStack.pop();
}

HNode::~HNode() {
	for(size_t i = _children.size(); i > 0; i--)	delete _children[i-1];
	_children.clear();
	_vertices.clear();
	_colors.clear();
	_parent = nullptr;
}

HNode& HNode::translate(const glm::vec3 &delta) {
	_parameters[0].x += delta.x;
	_parameters[0].y += delta.y;
	_parameters[0].z += delta.z;
	_translation = glm::translate(_translation, delta);

	// std::cout << "New translation parameters (" << get_name() << ") are "
	// 	<< _parameters[0].x << ", " << _parameters[0].y << ", " << _parameters[0].z << "\n";
	return *this;
}

HNode& HNode::rotate(const glm::vec3 &delta) {
	_parameters[1].x += delta.x;
	_parameters[1].y += delta.y;
	_parameters[1].z += delta.z;
	_rotation = glm::rotate(_rotation, glm::radians(delta.x), glm::vec3(1.0f, 0.0f, 0.0f));
	_rotation = glm::rotate(_rotation, glm::radians(delta.y), glm::vec3(0.0f, 1.0f, 0.0f));
	_rotation = glm::rotate(_rotation, glm::radians(delta.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	// std::cout << "New rotation parameters (" << get_name() << ") are "
	// 	<< _parameters[1].x << ", " << _parameters[1].y << ", " << _parameters[1].z << "\n";
	return *this;
}

/**********************************
				Joint functions
***********************************/

Joint::Joint(HNode* h, const bool (&r)[3], const glm::mat4 &s)
	: HNode(h, s), _rotation_axis{r[0],r[1],r[2]} { }

Joint& Joint::rotate(const glm::vec3 &rotate) {
	glm::vec3 rotation_parameters;
	rotation_parameters.x = _rotation_axis[0] ? rotate.x : 0.0f;
	rotation_parameters.y = _rotation_axis[1] ? rotate.y : 0.0f;
	rotation_parameters.z = _rotation_axis[2] ? rotate.z : 0.0f;
	
	HNode::rotate(rotation_parameters);
	
	return *this;
}

/**********************************
			Hinge Joint functions
***********************************/

Hinge_Joint::Hinge_Joint(
    HNode* parent,															// parent node
    const glm::mat4 &shape_transformation,											// shape matrix
    const glm::vec4 &color,											// color of joint node
    const bool (&axis)[3]
) : Joint(parent, axis, shape_transformation){
    // initialize the vertices and colors array
    make_sphere(_vertices, _colors, 1.0, color);
}

/**********************************
	Ball and Socket Joint functions
***********************************/

Ball_And_Socket_Joint::Ball_And_Socket_Joint(
    HNode* parent,                                            // parent node
    const glm::mat4 &shape_transformation,                                   // shape matrix
    const glm::vec4 &color                                    // color of joint node
) : Joint(parent, {true, true, true}, shape_transformation) {
    // initialize the vertices and colors array
    make_sphere(_vertices, _colors, 1.0, color);
}

/**********************************
				Rigid part functions
***********************************/

Rigid_Node::Rigid_Node(
	HNode* parent,											// parent of this node
	const std::vector<glm::vec4> &vertices,					// list of vertices
	const std::vector<glm::vec4> &colors,						// list of colors
	const glm::mat4 &shape_transformation									// shape matrix
) : HNode(parent, shape_transformation) {
	_vertices = vertices;
	_colors = colors;
}

/*************************************************************************************************/