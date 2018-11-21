#include <stdio.h>
#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include "characters.hpp"
#include "furniture/furniture.hpp"
#include "animation.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.hpp"

/***********************************************************************************************
 Global variables for rendering/loading
***********************************************************************************************/

/* Shader program related variables */
GLuint shaderProgram;
GLuint vPosition, vColor, vNormal, texCoord, uModelViewMatrix, uNormalMatrix, ViewMatrix, applyTexture;
GLuint lampLighting, wallLighting, lampLighted=1, wallLighted=1;

GLuint isLighted=1;
/* Vertex transformation matrices */
glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 lookat_matrix;
glm::mat4 view_matrix;

/* Camera transformation matrices */
glm::mat4 c_rotation_matrix;
/* Camera position and rotation parameters */
GLfloat c_xpos = 0.0, c_ypos = 300.0, c_zpos = 50.0;
GLfloat c_up_x = 0.0, c_up_y = 0.0, c_up_z = 1.0;
GLfloat c_xrot = 0.0, c_yrot = 0.0, c_zrot = 0.0;
double c_near = 2.0, c_far = 1000.0;

/* Global matrix stack */
std::stack<glm::mat4> MatrixStack;

/* Model information */
unsigned int N_CHARACTERS = 3;             		 // number of characters
std::vector<HNode*> roots;                       // stores the root node of each character
std::vector<HNode*> furniture;
std::vector<HNode*> joint_nodes[3];   // stores the joint nodes of each character
int current_node = 0;                            // index of currently selected node
int current_joint = 0;                           // index of currently selected joint

/* Variables for animation */
int FPS = 60;
std::string FILENAME = "keyframes.txt";
std::string CAMERA_FILENAME = "camera_path.txt";
bool show_camera_trajectory = false, save_snaps = false;
AnimationMode mode = INSPECT;
PlaybackState state = STOP;
CameraPointsBuffer cpb;

/***********************************************************************************************/

void initBuffersGL(void){
  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("vshader.glsl");
  std::string fragment_shader_file("fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram(shaderProgram);

  // get attributes from the shader program
  vPosition = glGetAttribLocation(shaderProgram, "vPosition");
  vColor = glGetAttribLocation(shaderProgram, "vColor"); 
  vNormal = glGetAttribLocation(shaderProgram, "vNormal"); 
  texCoord = glGetAttribLocation(shaderProgram, "texCoord"); 
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
  ViewMatrix = glGetUniformLocation(shaderProgram, "ViewMatrix");
  uNormalMatrix = glGetUniformLocation(shaderProgram, "uNormalMatrix");
  applyTexture = glGetUniformLocation(shaderProgram, "applyTexture");
  lampLighting = glGetUniformLocation(shaderProgram, "lampLighting");
  wallLighting = glGetUniformLocation(shaderProgram, "wallLighting");

  // create the characters
  roots.push_back(musicbox::create_character(joint_nodes[0]));
  roots.push_back(panda::create_character(joint_nodes[1]));
  roots.push_back(mantis::create_character(joint_nodes[2]));

  // setup characters on the stage
  roots[0]->change_parameters(glm::vec3(0.0, -15.0, 0.0), glm::vec3(0.0));
  roots[1]->change_parameters(glm::vec3(-3.0, -15.0, 13.0), glm::vec3(0.0));
  roots[2]->change_parameters(glm::vec3(7.0, -15.0, 8.0), glm::vec3(0.0));

  furniture.push_back(room::create_structure());
  furniture.push_back(bed::create_structure(furniture[0]));
  furniture.push_back(stool::create_structure(furniture[0]));
  furniture.push_back(cabinet::create_structure(furniture[0]));
  furniture.push_back(wall_light::create_structure(furniture[0]));
  furniture.push_back(painting::create_structure(furniture[0]));
  furniture.push_back(lamp::create_structure(furniture[0]));
  furniture.push_back(wall::create_structure(furniture[0]));
  furniture.push_back(door::create_structure(furniture[0]));

  cpb.init_buffers();
}

void renderGL(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear matrix stack
	std::stack<glm::mat4>().swap(MatrixStack);

	// create the lookat and up vectors for the camera
	c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
	c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
	c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

	glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * c_rotation_matrix;
	glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * c_rotation_matrix;
	// create the lookat matrix
	lookat_matrix = glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));

	// create the projection matrix
	projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, c_near, c_far);

	view_matrix = projection_matrix * lookat_matrix;
	MatrixStack.push(view_matrix);

	// draw charcters to screen
	for(size_t i=0; i < N_CHARACTERS; ++i){
		roots[i]->render_tree();
	}

	glUniform1i(lampLighting, lampLighted);
	glUniform1i(wallLighting, wallLighted);

	// draw charcters to screen
	for(unsigned int i=0; i < N_CHARACTERS; ++i){
		roots[i]->render_tree();
	}

	for(size_t i=0; i < furniture.size(); ++i)
		furniture[i]->render_tree();

	if(mode == CAMERA){
		cpb.render_points();
		if(show_camera_trajectory)
			cpb.render_path();
	}
}

void save_snap_to_file(GLFWwindow* window, int snap_cntr){
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	unsigned char *pixels = new unsigned char[4 * width * height];
	unsigned char *store = new unsigned char[4 * width * height];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void *)pixels);
	for(int x=0; x < width; x++){
		for(int y=0; y < height; y++){
			int index = 0, uindex = 0;
			index = (4*width*y) + (4*x);
			uindex = (4*width*(height-1-y)) + (4*x);
			store[uindex] = pixels[index];
			store[uindex+1] = pixels[index + 1];
			store[uindex+2] = pixels[index + 2];
			store[uindex+3] = pixels[index + 3];
		}
	}
	char filename[128];
	std::snprintf(filename, 128, "snaps/snap_%04d.tga", snap_cntr);
	stbi_write_tga(filename, width, height, 4, (void*)store);
	delete[] pixels;
	delete[] store;
}

int main(int argc, char** argv) {
	//! The pointer to the GLFW window
	GLFWwindow* window;

	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(csX75::error_callback);

	//! Initialize GLFW
	if (!glfwInit())
		return -1;

	//We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//This is for MacOSX - can be omitted otherwise
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	//We don't want the old OpenGL 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	//! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(960, 960, "Assignment 3 : Rendering and Animation", NULL, NULL);
	if (!window) {
			glfwTerminate();
			return -1;
	}
	
	//! Make the window's context current 
	glfwMakeContextCurrent(window);

	//Initialize GLEW
	//Turn this on to get Shader based OpenGL
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
			//Problem: glewInit failed, something is seriously wrong.
			std::cerr<<"GLEW Init Failed : %s"<<std::endl;
	}

	// print and see what context got enabled
	std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
	std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
	std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
	std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

	// Keyboard Callback
	glfwSetKeyCallback(window, csX75::key_callback);
	// Scroll callback
	glfwSetScrollCallback(window, csX75::scroll_callback);
	// Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);
	// Mouse click callback
	glfwSetMouseButtonCallback(window, csX75::mouse_button_callback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Initialize GL state
	csX75::initGL();
	initBuffersGL();

	/* PLAYBACK mode variables */
	double timer = 0.0;
	// Placeholder keyframes
	Keyframe k1, k2;
	// Keyframe counter
	int cntr = 2;
	// Keyframe interpolation parameters
	float t = 0.0, dt = 1.0/(0.5 * FPS);	// 0.5s gap between each keyframe 
	float u = 0.0, du = 1.0/FPS;
	// Whether character animation is possible
	bool character_animation = true;
	// Animation snapshot counter
	int snap_cntr = 0;

	// Loop until the user closes the window
	while (glfwWindowShouldClose(window) == 0) {
			// Render here
			renderGL();

			// Swap front and back buffers
			glfwSwapBuffers(window);
			
			// Poll for and process events
			glfwPollEvents();

			// Control playback sequence
			if(mode == PLAYBACK){
				glm::vec3 new_c_pos;
				glm::mat4 rot;
				switch(state){
					case INIT:
					/* camera animation */
					u = 0.0;
					if(cpb.points.size() > 1) {
						c_xrot = c_yrot = c_zrot = 0.0;
						du = cpb.points.size() > 1 ? 1.0/(FPS * (cpb.points.size()-1)) : 1.0/FPS;
						new_c_pos = compute_camera_trajectory(cpb.points, u); // this is a rotated point
						// rotate in reverse direction to prevent double rotation
						rot = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
						rot = glm::rotate(rot, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
						rot = glm::rotate(rot, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));
						new_c_pos = glm::vec3(glm::vec4(new_c_pos, 1.0) * glm::transpose(rot));
						c_xpos = new_c_pos.x; c_ypos = new_c_pos.y; c_zpos = new_c_pos.z;
					}
					else {
						u = 1.0;
					}

					/* character animation */
					t = 0.0;
					cntr = 2;
					if(k1.read_frame_from_file(0) < 0 || k2.read_frame_from_file(1) < 0)
						character_animation = false;
					else
						update_state(interpolate_keyframes(k1, k2, t));

					/* general */
					snap_cntr = 0;
					timer = glfwGetTime();
					state = PLAY;
					break;
					
					case RESUME:
					timer += glfwGetTime() - timer;
					state = PLAY;
					break;
					
					case PLAY:
					if(glfwGetTime() - timer > 1.0/FPS){
						/* save snapshot to disk */
						if(save_snaps){
							snap_cntr++;
							save_snap_to_file(window, snap_cntr);
						}

						/* camera animation */
						if(u < 1.0){
							u += du;
							new_c_pos = compute_camera_trajectory(cpb.points, u); // this point is already rotated
							// rotate in reverse direction to prevent double rotation
							rot = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
							rot = glm::rotate(rot, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
							rot = glm::rotate(rot, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));
							new_c_pos = glm::vec3(glm::vec4(new_c_pos, 1.0) * glm::transpose(rot));
							c_xpos = new_c_pos.x; c_ypos = new_c_pos.y; c_zpos = new_c_pos.z;
							timer = glfwGetTime();
						}

						/* character animation */
						else if(character_animation){
							if(t <= 1.0){
								t += dt;
								update_state(interpolate_keyframes(k1, k2, t));
								timer = glfwGetTime();
							}else{
								t = 0.0;
								k1 = k2;
								if(k2.read_frame_from_file(cntr++) < 0){
									state = STOP;
								}
							}
						}
						else {
							state = STOP;
						}
					}
					break;
					default: break;
				}
			}
	}
	
	glfwTerminate();

	// free memory
	for(size_t i = roots.size(); i > 0; i--)  delete roots[i-1];
	roots.clear();

	return 0;
}

