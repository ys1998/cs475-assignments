#include "gl_framework.hpp"
#include "hierarchy_node.hpp"
#include <iomanip>
#include <iostream>

extern GLfloat c_xrot, c_yrot, c_zrot;
extern GLfloat c_xpos, c_ypos, c_zpos;
extern double c_near;
extern int current_joint, current_node;
extern std::vector<HNode*> joint_nodes[];
extern std::vector<HNode*> roots;
extern bool enable_perspective;

int space;

namespace csX75
{
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
    if(action == GLFW_PRESS || action == GLFW_REPEAT){
      switch(key){
        // Close window
        case GLFW_KEY_ESCAPE:
          glfwSetWindowShouldClose(window, GL_TRUE); break;

        // Toggle perspective mode
        case GLFW_KEY_P:
          enable_perspective = !enable_perspective; break;

        // Change camera settings
        case GLFW_KEY_RIGHT:
          if(GLFW_MOD_SHIFT & mods) c_zpos += 3;
          else                      c_zrot += 3;
          break;
        case GLFW_KEY_LEFT:
          if(GLFW_MOD_SHIFT & mods) c_zpos -= 3;
          else                      c_zrot -= 3;
          break;
        case GLFW_KEY_UP:
          if(GLFW_MOD_SHIFT & mods) c_ypos -= 3;
          else                      c_yrot -= 3;
          break;
        case GLFW_KEY_DOWN:
          if(GLFW_MOD_SHIFT & mods) c_ypos += 3;
          else                      c_yrot += 3;
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
            joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, 0.0f, 5.0f));
          else
            joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, 0.0f, 5.0f));
          break;
        case GLFW_KEY_S:
          if(GLFW_MOD_SHIFT & mods)
            joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, 0.0f, -5.0f));
          else
            joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, 0.0f, -5.0f));
          break;
        case GLFW_KEY_D:
          if(GLFW_MOD_SHIFT & mods)
            joint_nodes[current_node][current_joint]->translate(glm::vec3(5.0f, 0.0f, 0.0f));
          else
            joint_nodes[current_node][current_joint]->rotate(glm::vec3(5.0f, 0.0f, 0.0f));
          break;
        case GLFW_KEY_A:
          if(GLFW_MOD_SHIFT & mods)
            joint_nodes[current_node][current_joint]->translate(glm::vec3(-5.0f, 0.0f, 0.0f));
          else
            joint_nodes[current_node][current_joint]->rotate(glm::vec3(-5.0f, 0.0f, 0.0f));
          break;
        case GLFW_KEY_E:
          if(GLFW_MOD_SHIFT & mods)
            joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, 5.0f, 0.0f));
          else
            joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, 5.0f, 0.0f));
          break;
        case GLFW_KEY_Q:
          if(GLFW_MOD_SHIFT & mods)
            joint_nodes[current_node][current_joint]->translate(glm::vec3(0.0f, -5.0f, 0.0f));
          else
            joint_nodes[current_node][current_joint]->rotate(glm::vec3(0.0f, -5.0f, 0.0f));
          break;

        // Zoom in/out
        case GLFW_KEY_EQUAL:
          c_near += 1;
          break;
        case GLFW_KEY_MINUS:
          c_near -= 1;
          break;

        // Display help
        case GLFW_KEY_H:
          space = 27;
          std::cout << "Controls:\n"
            << std::setw(space) << std::left << "H" << ": Display this help\n"
            << std::setw(space) << "P" << ": Change perspective mode\n"
            << std::setw(space) << "R" << ": Select character\n"
            << std::setw(space) << "J" << ": Select joint\n"
            << std::setw(space) << "Up/Down/Left/Right" << ": Rotate camera about +X/-X/+Y/-Y axes\n"
            << std::setw(space) << "Scroll left/right" << ": Rotate camera about -Z/+Z axis\n"
            << std::setw(space) << "Shift + Up/Down/Left/Right" << ": Translate camera about +X/-X/+Y/-Y axes\n"
            << std::setw(space) << "Scroll up/down" << ": Translate camera about +Z/-Z axis\n"
            << std::setw(space) << "W/S/D/A/E/Q" << ": Rotate selected joint about +Z/-Z/+X/-X/+Y/-Y axes\n"
            << std::setw(space) << "Shift + W/S/D/A/E/Q" << ": Translate selected joint about +Z/-Z/+X/-X/+Y/-Y axes\n"
            << std::setw(space) << "+/-" << ": Zoom the camera in/out\n";
          break;

        default:
          if(key >= GLFW_KEY_LEFT_SHIFT && key <= GLFW_KEY_RIGHT_SUPER) break;
          std::cout<<"Invalid key. Press H for help.\n";
      }
    } 
  }

  //!GLFW scroll callback
  void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double abs_xoff = xoffset < 0 ? -xoffset : xoffset;
    double abs_yoff = yoffset < 0 ? -yoffset : yoffset;
    if(abs_xoff > abs_yoff)
      c_xrot += xoffset;
    else
      c_xpos += yoffset;
  }
};  
  


