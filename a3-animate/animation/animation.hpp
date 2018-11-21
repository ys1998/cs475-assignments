/*
	Function definitions for animation related tasks.
*/

#ifndef _ANIMATION_HPP_
#define _ANIMATION_HPP_

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "parts.hpp"
#include "hierarchy_node.hpp"


enum AnimationMode { INSPECT, CAMERA, RECORD, PLAYBACK };
enum PlaybackState { STOP, INIT, PLAY, PAUSE, RESUME };

/* Camera */

class CameraPointsBuffer {
private:
	GLuint vao, vbo;
	std::vector<glm::vec4> v, c;
	std::vector<glm::vec3> n;
	glm::mat4 umat;

	GLuint p_vao, p_vbo;
	std::vector<glm::vec4> pv, pc;
	
public:
	bool changed;
	std::vector<glm::vec3> points;
	CameraPointsBuffer();
	void init_buffers(void);
	void render_points(void);
	void render_path(void);
	void load_from_file(void);
	void save_to_file(void);
};

glm::vec3 compute_camera_trajectory(std::vector<glm::vec3>, float);

/* Character */

class Keyframe {
public:
	std::vector<float> values;	// values of joint angles, bulb state etc.
	std::vector<int> type;		// type of interpolation: 0 - binary, 1 - linear

	Keyframe();
	int write_frame_to_file(void);
	int read_frame_from_file(int);
	void build_frame_from_state(void);
};

std::vector<float> interpolate_keyframes(Keyframe&, Keyframe&, float);
void update_state(const std::vector<float>&);

#endif