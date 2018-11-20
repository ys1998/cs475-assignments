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

/***********************************************************************************************
 Global variables for rendering/loading
***********************************************************************************************/

/* Shader program related variables */
GLuint shaderProgram;
GLuint vPosition, vColor, uModelViewMatrix;

/* Vertex transformation matrices */
glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 lookat_matrix;
glm::mat4 view_matrix;

/* Camera transformation matrices */
glm::mat4 c_rotation_matrix;
/* Camera position and rotation parameters */
GLfloat c_xpos = 30.0, c_ypos = 0.0, c_zpos = 0.0;
GLfloat c_up_x = 0.0, c_up_y = 0.0, c_up_z = 1.0;
GLfloat c_xrot = 0.0, c_yrot = 0.0, c_zrot = 0.0;
double c_near = 0.0;

/* Global matrix stack */
std::stack<glm::mat4> MatrixStack;

/* Enable/disable perspective view */
bool enable_perspective = false;

/* Model information */
const int N_CHARACTERS = 3;                      // number of characters
std::vector<HNode*> roots;                       // stores the root node of each character
std::vector<HNode*> joint_nodes[N_CHARACTERS];   // stores the joint nodes of each character
int current_node = 0;                            // index of currently selected node
int current_joint = 0;                           // index of currently selected joint

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
  uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");

  // create the characters
  roots.push_back(musicbox::create_character(joint_nodes[0]));
  roots.push_back(panda::create_character(joint_nodes[1]));
  roots.push_back(mantis::create_character(joint_nodes[2]));

  // setup characters on the stage
  roots[0]->change_parameters(glm::vec3(3.0, -15.0, 0.0), glm::vec3(0.0));
  roots[1]->change_parameters(glm::vec3(0.0, 15.0, 5.0), glm::vec3(0.0));
  roots[2]->change_parameters(glm::vec3(10.0, 15.0, 5.0), glm::vec3(0.0));
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
  if(enable_perspective) {
		projection_matrix = glm::perspective(90.0f, 1.0f, 1.0f, 50.0f);
  } else {
    double edge_len = c_near - 40.0 < 0 ? c_near - 40.0 : 0;
    projection_matrix = glm::ortho(edge_len, -edge_len, edge_len, -edge_len, edge_len, 40.0);
  }

  view_matrix = projection_matrix * lookat_matrix;

  MatrixStack.push(view_matrix);

  // draw charcters to screen
  for(int i=0; i < N_CHARACTERS; ++i){
    roots[i]->render_tree();
  }
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
  window = glfwCreateWindow(512, 512, "Assignment 2 : Hierarchical Modelling", NULL, NULL);
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

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0) {
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
  }
  
  glfwTerminate();

  // free memory
  for(size_t i = roots.size(); i > 0; i--)  delete roots[i-1];
  roots.clear();

  return 0;
}

