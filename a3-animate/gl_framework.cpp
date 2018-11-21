#include "gl_framework.hpp"
#include "hierarchy_node.hpp"
#include "animation.hpp"
#include <iomanip>
#include <iostream>

extern GLfloat c_xrot, c_yrot, c_zrot;
extern GLfloat c_xpos, c_ypos, c_zpos;
extern GLfloat c_up_x, c_up_y, c_up_z;
extern GLuint lampLighted, wallLighted;
extern double c_near, c_far;
extern int current_joint, current_node;
extern std::vector<HNode*> joint_nodes[];
extern std::vector<HNode*> roots;

extern AnimationMode mode;
extern CameraPointsBuffer cpb;
extern PlaybackState state;
extern bool show_camera_trajectory, save_snaps;

namespace csX75 {

	//! Initialize GL State
	void initGL(void) {
		//Set framebuffer clear color
		glClearColor(0.0f, 0.549f, 0.7843f, 1.0f);
		//Set depth buffer furthest depth
		glClearDepth(1.0);
		//Set depth test to less-than
		glDepthFunc(GL_LESS);
		//Enable depth testing
		glEnable(GL_DEPTH_TEST);
	}
	
	//!GLFW Error Callback
	void error_callback(int error, const char* description) {
		std::cerr<<description<<std::endl;
	}
	
	//!GLFW framebuffer resize callback
	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		//!Resize the viewport to fit the window size - draw to entire window
		glViewport(0, 0, width, height);
	}
	
	//!GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		float factor = 1.0;
		if(action == GLFW_PRESS || action == GLFW_REPEAT){
			/* General keys */
			if(key == GLFW_KEY_ESCAPE){
				glfwSetWindowShouldClose(window, GL_TRUE);
			}else if(key == GLFW_KEY_M){
				mode = (AnimationMode)((mode + 1) % 4);
				switch(mode){
					case INSPECT: state = PAUSE; std::cout << "Switched to INSPECT mode.\n"; break;
					case CAMERA: std::cout << "Switched to CAMERA mode.\n"; break;
					case RECORD: std::cout << "Switched to RECORD mode.\n"; break;
					case PLAYBACK: std::cout << "Switched to PLAYBACK mode.\n"; break;
				}
			}else if(key == GLFW_KEY_H){
				int space = 27;
				std::cout << "Controls:\n"
					<< "\nGENERAL\n\n"
					<< std::setw(space) << std::left << "H" << ": Display this help\n"
					<< std::setw(space) << "Esc" << ": Close window\n"
					<< std::setw(space) << "M" << ": Switch modes in cyclic manner [INSPECT|CAMERA|RECORD|PLAYBACK]\n"
					<< "\n[ INSPECT ]\n\n"
					<< std::setw(space) << "R" << ": Select character\n"
					<< std::setw(space) << "J" << ": Select joint\n"
					<< std::setw(space) << "Up/Down/Left/Right" << ": Rotate camera about Y[-/+] and Z[-/+] axes\n"
					<< std::setw(space) << "./," << ": Rotate camera about X[-/+] axis\n"
					<< std::setw(space) << "Shift + Up/Down/Left/Right" << ": Translate camera along Y[-/+] and Z[-/+] axes\n"
					<< std::setw(space) << "Shift + ./," << ": Translate camera along X[-/+] axis\n"
					<< std::setw(space) << "W/S/D/A/E/Q" << ": Rotate selected joint about Z[+/-], X[+/-] and Y[+/-] axes\n"
					<< std::setw(space) << "Shift + W/S/D/A/E/Q" << ": Translate selected joint along Z[+/-], X[+/-] and Y[+/-] axes\n"
					<< std::setw(space) << "+/-" << ": Zoom the camera in/out\n"
					<< std::setw(space) << "L" << ": Toggle the wall light source\n"
					<< std::setw(space) << "Shift + L" << ": Toggle the lamp (second light source)\n"
					<< "\n[ CAMERA ]\n\n"
					<< std::setw(space) << "Up/Down/Left/Right" << ": Rotate camera about Y[-/+] and Z[-/+] axes\n"
					<< std::setw(space) << "Scroll left/right" << ": Rotate camera about X[-/+] axis\n"
					<< std::setw(space) << "+/-" << ": Zoom the camera in/out\n"
					<< std::setw(space) << "Scroll up/down" << ": Move the camera in/out\n"
					<< std::setw(space) << "Left click" << ": Mark control point for Bezier curve\n"
					<< std::setw(space) << "Right click" << ": Delete last marked point\n"
					<< std::setw(space) << "S" << ": Save marked points to file\n"
					<< std::setw(space) << "L" << ": Load marked points from file\n"
					<< std::setw(space) << "R" << ": Toggle rendering of Bezier curve for current control points\n"
					<< "\n[ RECORD ]\n\n"
					<< std::setw(space) << "R" << ": Select character\n"
					<< std::setw(space) << "J" << ": Select joint\n"
					<< std::setw(space) << "Up/Down/Left/Right" << ": Rotate camera about Y[-/+] and Z[-/+] axes\n"
					<< std::setw(space) << "./," << ": Rotate camera about X[-/+] axis\n"
					<< std::setw(space) << "Shift + Up/Down/Left/Right" << ": Translate camera along Y[-/+] and Z[-/+] axes\n"
					<< std::setw(space) << "Shift + ./," << ": Translate camera along X[-/+] axis\n"
					<< std::setw(space) << "W/S/D/A/E/Q" << ": Rotate selected joint about Z[+/-], X[+/-] and Y[+/-] axes\n"
					<< std::setw(space) << "Shift + W/S/D/A/E/Q" << ": Translate selected joint along Z[+/-], X[+/-] and Y[+/-] axes\n"
					<< std::setw(space) << "+/-" << ": Zoom the camera in/out\n"
					<< std::setw(space) << "L" << ": Toggle the wall light source\n"
					<< std::setw(space) << "Shift + L" << ": Toggle the lamp (second light source)\n"
					<< std::setw(space) << "C" << ": Save current keyframe (state) to file\n"
					<< "\n[ PLAYBACK ]\n\n"
					<< std::setw(space) << "P" << ": Toggle between playing/pausing the animation\n"
					<< std::setw(space) << "S" << ": Toggle saving animation snaps to disk\n";

			}
			/* Keys for CAMERA mode */
			else if(mode == CAMERA){
				switch(key){
					case GLFW_KEY_RIGHT: c_zrot += 3; break;
					case GLFW_KEY_LEFT: c_zrot -= 3; break;
					case GLFW_KEY_UP: c_yrot -= 3; break;
					case GLFW_KEY_DOWN: c_yrot += 3; break;
					case GLFW_KEY_PERIOD: c_xrot += 3; break;
					case GLFW_KEY_COMMA: c_xrot -= 3; break;
					case GLFW_KEY_EQUAL:
						factor = (1 - 1/glm::distance(glm::vec3(c_xpos, c_ypos, c_zpos), glm::vec3(0.0)));
						c_xpos *= factor;
						c_ypos *= factor;
						c_zpos *= factor;
						c_far -= 1;
						break;
					case GLFW_KEY_MINUS:
						factor = (1 + 1/glm::distance(glm::vec3(c_xpos, c_ypos, c_zpos), glm::vec3(0.0)));
						c_xpos *= factor;
						c_ypos *= factor;
						c_zpos *= factor;
						c_far += 1;
						break;
					case GLFW_KEY_L: 
						cpb.load_from_file(); 
						std::cout << "Loaded control points from file.\n"; 
						break;
					case GLFW_KEY_S: 
						cpb.save_to_file(); 
						std::cout << "Saved control points to file.\n";
						break;
					case GLFW_KEY_R: show_camera_trajectory = !show_camera_trajectory; break;
				}
			}
			/* Keys common to INSPECT or RECORD */
			if(mode == INSPECT || mode == RECORD){
				int scaling_factor = 3;
				switch(key){
					// Change camera settings
					case GLFW_KEY_PERIOD: 
						if(GLFW_MOD_SHIFT & mods) c_xpos += scaling_factor;
						else					  c_xpos += scaling_factor;
						break;
					case GLFW_KEY_COMMA: 
						if(GLFW_MOD_SHIFT & mods) c_xpos -= scaling_factor;
						else					  c_xpos -= scaling_factor;
						break;
					case GLFW_KEY_RIGHT:
						if(GLFW_MOD_SHIFT & mods) c_zpos += scaling_factor;
						else                      c_zrot += scaling_factor;
						break;
					case GLFW_KEY_LEFT:
						if(GLFW_MOD_SHIFT & mods) c_zpos -= scaling_factor;
						else                      c_zrot -= scaling_factor;
						break;
					case GLFW_KEY_UP:
						if(GLFW_MOD_SHIFT & mods) c_ypos -= scaling_factor;
						else                      c_yrot -= scaling_factor;
						break;
					case GLFW_KEY_DOWN:
						if(GLFW_MOD_SHIFT & mods) c_ypos += scaling_factor;
						else                      c_yrot += scaling_factor;
						break;

					// Select character
					case GLFW_KEY_R:
						std::cout << "CHARACTERS\n";
						for(size_t i = 0; i < roots.size(); ++i){
							std::cout << i << "\t" << roots[i]->get_name() << std::endl;
						}
						std::cout << "Select character : ";
						std::cin >> current_node;
						break;

					// Select joint
					case GLFW_KEY_J:
						std::cout << "CHARACTER: " << roots[current_node]->get_name() << "\nJOINTS\n";
						std::cout << "0\tCentroid\n";
						for(size_t i = 1; i < joint_nodes[current_node].size(); ++i){
							std::cout << i << "\t" << joint_nodes[current_node][i]->get_name() << std::endl;
						}
						std::cout << "Select joint : ";
						std::cin >> current_joint;
						break;

					// Change character settings
					case GLFW_KEY_W:
						if(GLFW_MOD_SHIFT & mods)
							joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, 0.0f, 1.0f));
						else
							joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, 0.0f, 1.0f));
						break;
					case GLFW_KEY_S:
						if(GLFW_MOD_SHIFT & mods)
							joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, 0.0f, -1.0f));
						else
							joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, 0.0f, -1.0f));
						break;
					case GLFW_KEY_D:
						if(GLFW_MOD_SHIFT & mods)
							joint_nodes[current_node][current_joint]->translate(glm::vec3(1.0f, 0.0f, 0.0f));
						else
							joint_nodes[current_node][current_joint]->rotate(glm::vec3(1.0f, 0.0f, 0.0f));
						break;
					case GLFW_KEY_A:
						if(GLFW_MOD_SHIFT & mods)
							joint_nodes[current_node][current_joint]->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
						else
							joint_nodes[current_node][current_joint]->rotate(glm::vec3(-1.0f, 0.0f, 0.0f));
						break;
					case GLFW_KEY_E:
						if(GLFW_MOD_SHIFT & mods)
							joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, 1.0f, 0.0f));
						else
							joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, 1.0f, 0.0f));
						break;
					case GLFW_KEY_Q:
						if(GLFW_MOD_SHIFT & mods)
							joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, -1.0f, 0.0f));
						else
							joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, -1.0f, 0.0f));
						break;

					// Zoom in/out
					case GLFW_KEY_EQUAL:
						factor = (1 - 1/glm::distance(glm::vec3(c_xpos, c_ypos, c_zpos), glm::vec3(0.0)));
						c_xpos *= factor;
						c_ypos *= factor;
						c_zpos *= factor;
						c_far -= 1;
						break;
					case GLFW_KEY_MINUS:
						factor = (1 + 1/glm::distance(glm::vec3(c_xpos, c_ypos, c_zpos), glm::vec3(0.0)));
						c_xpos *= factor;
						c_ypos *= factor;
						c_zpos *= factor;
						c_far += 1;
						break;
					case GLFW_KEY_L:
						if(GLFW_MOD_SHIFT & mods)
							lampLighted = lampLighted == 1 ? 0 : 1;
						else
							wallLighted = wallLighted == 1 ? 0 : 1;
						break;
					default:
						break;
				}
			}
			/* Keys for PLAYBACK mode */
			if(mode == PLAYBACK){
				if(key == GLFW_KEY_P){
					switch(state){
						case STOP: state = INIT; break;
						case PLAY: state = PAUSE; break;
						case PAUSE: state = RESUME; break;
						default: break;
					}
				}else if(key == GLFW_KEY_S){
					save_snaps = !save_snaps;
					if(save_snaps)
						std::cout << "Snapshots will be saved to snaps/ folder. Ensure that it is empty.\n";
				}
			}
			/* Keys for RECORD mode */
			if(mode == RECORD){
				if(key == GLFW_KEY_C){
					Keyframe k; k.build_frame_from_state(); k.write_frame_to_file();
					std::cout << "Saved keyframe.\n";
				}
			}
		}
	} 

	//!GLFW scroll callback
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		float factor = 1.0;
		if(mode == CAMERA){
			factor = (1 - yoffset/glm::distance(glm::vec3(c_xpos, c_ypos, c_zpos), glm::vec3(0.0)));
			c_xpos *= factor;
			c_ypos *= factor;
			c_zpos *= factor;
			c_far -= yoffset;
		}
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
		if(mode == CAMERA){
			if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
				float factor = 2;
				double xpos, ypos;
				int width, height;
				glfwGetCursorPos(window, &xpos, &ypos);
				glfwGetWindowSize(window, &width, &height);

				glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
				rot = glm::rotate(rot, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
				rot = glm::rotate(rot, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));
				glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * rot;
				glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * rot;
				glm::vec3 tangent = glm::normalize(glm::cross(glm::vec3(c_up), glm::vec3(c_pos)));

				cpb.points.push_back(
					glm::vec3(c_pos)*(1 - (float)c_near/glm::distance(glm::vec3(c_xpos, c_ypos, c_zpos), glm::vec3(0.0)))
					+ (float)(xpos/width - 0.5)*factor*tangent 
					+ (float)(0.5 - ypos/height)*factor*glm::vec3(c_up));

				cpb.changed = true;

			}else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
				if(!cpb.points.empty()){
					cpb.points.pop_back();
					cpb.changed = true;
				}
			}
		}
	}
};
