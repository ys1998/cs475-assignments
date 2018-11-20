/*
    This file is part of the mydraw.

    mydraw is a simple, interactive drawing program written using OpenGL. 
    
    This code base is meant to serve as a starter code for an assignment in a 
    Computer Graphics course.

    Copyright (c) 2018 by Parag Chaudhuri

    mydraw is free software; you can redistribute it and/or modify it under the 
    terms of the MIT License.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "gl_framework.hpp"
#include "color.hpp"
#include <iostream>

namespace csX75
{
  //! Initialize GL State
  void init_gl(mydraw::canvas_t* mycanvas)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    
    //Enable depth testing
    glDisable(GL_DEPTH_TEST);

		//Enable blending
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint tex;
    glGenTextures( 1, &tex);
    
    glBindTexture( GL_TEXTURE_2D, tex );
    mycanvas->set_canvas_texture(tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mycanvas->get_width(), mycanvas->get_height(), 0, GL_RGBA, GL_FLOAT, mycanvas->get_store());
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    // Load shaders and use the resulting shader program
    std::string vertex_shader_file("shaders/vshader.glsl");
    std::string fragment_shader_file("shaders/fshader.glsl");

    std::vector<GLuint> shader_list;
    shader_list.push_back(csX75::load_shader(GL_VERTEX_SHADER, vertex_shader_file));
    shader_list.push_back(csX75::load_shader(GL_FRAGMENT_SHADER, fragment_shader_file));

    GLuint shaderProgram = csX75::create_program(shader_list);
    glUseProgram( shaderProgram );
    mycanvas->set_canvas_sp(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "screen_texture"), 0);

    //! Vertex attributes for a quad that fills the entire screen in normalized device coordinates.
    float screen_quad_vertices[] = 
     { 
          // positions   // texCoords
          -1.0f,  1.0f,  0.0f, 1.0f,
          -1.0f, -1.0f,  0.0f, 0.0f,
           1.0f, -1.0f,  1.0f, 0.0f,

          -1.0f,  1.0f,  0.0f, 1.0f,
           1.0f, -1.0f,  1.0f, 0.0f,
           1.0f,  1.0f,  1.0f, 1.0f
      };

    unsigned int screen_quad_vao, screen_quad_vbo;  

    glGenVertexArrays(1, &screen_quad_vao);
    glGenBuffers(1, &screen_quad_vbo);
    glBindVertexArray(screen_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, screen_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screen_quad_vertices), &screen_quad_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    mycanvas->set_canvas_vao(screen_quad_vao);  
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
	//!Clear specialised callbacks and contexts
	void clear(mydraw::canvas_t *canvas)
	{
		glfwSetCursorPosCallback(glfwGetCurrentContext(), NULL);
    canvas->get_context()->current_primitive->reset();
		canvas->get_context()->click_and_drag_conscious = false;
	}

  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
	//!Reset the brushes to point types
	void reset_tools(mydraw::canvas_t *canvas)
	{
		if(canvas->get_context()->current_brush->get_brush_type() != mydraw::brush_type_t::point){
			delete canvas->get_context()->current_brush;
			canvas->get_context()->current_brush = new mydraw::point_brush_t;
		}
		if(canvas->get_context()->current_eraser->get_brush_type() != mydraw::brush_type_t::eraser_point){
			delete canvas->get_context()->current_eraser;
			canvas->get_context()->current_eraser = new mydraw::eraser_point_brush_t;
		}

		//Reset tool to point brush
		canvas->get_context()->active_tool = canvas->get_context()->current_brush;
	}

  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    mydraw::canvas_t* mycanvas = reinterpret_cast<mydraw::canvas_t*>(glfwGetWindowUserPointer(window));

    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
      mycanvas->save();
      std::cerr<<"Saving canvas."<<std::endl;
    }
    //!Change drawing mode to eraser
    else if (key == GLFW_KEY_E && action == GLFW_PRESS){
			clear(mycanvas);
			
			if(mycanvas->get_context()->current_primitive->get_primitive_mode() != mydraw::primitive_mode_t::point
				|| mycanvas->get_context()->active_tool->get_tool_type() != mydraw::tool_type_t::brush) {
				mycanvas->get_context()->active_tool = mycanvas->get_context()->current_eraser;
				std::cout<<"Switched to \'eraser\'\n";
			}
			else
				switch(((mydraw::brush_t*)mycanvas->get_context()->active_tool)->get_brush_type()){
					case mydraw::brush_type_t::point:
					case mydraw::brush_type_t::smooth:
      			mycanvas->get_context()->active_tool = mycanvas->get_context()->current_eraser;
      			
						std::cout<<"Switched to \'eraser\'\n";
						break;

					case mydraw::brush_type_t::eraser_point:
						delete mycanvas->get_context()->current_eraser;
						mycanvas->get_context()->current_eraser = new mydraw::eraser_smooth_brush_t;
						mycanvas->get_context()->active_tool = mycanvas->get_context()->current_eraser;
						mycanvas->get_context()->click_and_drag_conscious = true;
      			
						std::cout<<"Changed eraser mode to 'smooth'\n";
						break;
						
					case mydraw::brush_type_t::eraser_smooth:
						delete mycanvas->get_context()->current_eraser;
						mycanvas->get_context()->current_eraser = new mydraw::eraser_point_brush_t;
						mycanvas->get_context()->active_tool = mycanvas->get_context()->current_eraser;
						
						std::cout<<"Changed eraser mode to 'point'\n";
						break;
				}
    }

    //!Change drawing mode to brush
    else if (key == GLFW_KEY_B && action == GLFW_PRESS){
			clear(mycanvas);
			
			if(mycanvas->get_context()->current_primitive->get_primitive_mode() != mydraw::primitive_mode_t::point
				|| mycanvas->get_context()->active_tool->get_tool_type() != mydraw::tool_type_t::brush){
				mycanvas->get_context()->active_tool = mycanvas->get_context()->current_brush;
				std::cout<<"Switched to \'brush\'\n";
			}
			else
				switch(((mydraw::brush_t*)mycanvas->get_context()->active_tool)->get_brush_type()){
					case mydraw::brush_type_t::eraser_point:
					case mydraw::brush_type_t::eraser_smooth:
      			mycanvas->get_context()->active_tool = mycanvas->get_context()->current_brush;
      			
						std::cout<<"Switched to \'brush\'\n";
						break;

					case mydraw::brush_type_t::point:
						delete mycanvas->get_context()->current_brush;
						mycanvas->get_context()->current_brush = new mydraw::smooth_brush_t;
						mycanvas->get_context()->active_tool = mycanvas->get_context()->current_brush;
						mycanvas->get_context()->click_and_drag_conscious = true;
      			
						std::cout<<"Changed brush mode to 'smooth'\n";
						break;
						
					case mydraw::brush_type_t::smooth:
						delete mycanvas->get_context()->current_brush;
						mycanvas->get_context()->current_brush = new mydraw::point_brush_t;
						mycanvas->get_context()->active_tool = mycanvas->get_context()->current_brush;
						
						std::cout<<"Changed brush mode to 'point'\n";
						break;
				}
    }
    //!Change drawing mode to fill
    else if (key == GLFW_KEY_F && action == GLFW_PRESS){
      mycanvas->get_context()->active_tool = mycanvas->get_context()->current_fill;
			clear(mycanvas);
      std::cout<<"Switched to \'fill\'\n";
    }
    //!Increment size of drawing tool
    else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS){
      mycanvas->get_context()->active_tool->increase_size();
      std::cout<<"Increased brush size to "<<mycanvas->get_context()->active_tool->get_size()<<std::endl;
    }
    //!Decrement size of drawing tool
    else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
      mycanvas->get_context()->active_tool->decrease_size();
      std::cout<<"Decreased brush size to "<<mycanvas->get_context()->active_tool->get_size()<<std::endl;
    }
    //!Change the brush color
    else if (key == GLFW_KEY_C && action == GLFW_PRESS){
      std::cout<<"Enter new brush color [RGB] : ";
      mydraw::color_t new_color;
      std::cin>>new_color.r>>new_color.g>>new_color.b>>new_color.a;
      mycanvas->get_context()->brush_color = new_color;
    }
    //!Change the eraser color
    else if (key == GLFW_KEY_D && action == GLFW_PRESS){
      std::cout<<"Enter new eraser color [RGB] : ";
      mydraw::color_t new_color;
      std::cin>>new_color.r>>new_color.g>>new_color.b>>new_color.a;
      mycanvas->get_context()->bg_color = new_color;
    }
    //!Cycle through the primitive modes
    else if(key == GLFW_KEY_P && action == GLFW_PRESS){
			clear(mycanvas);
			reset_tools(mycanvas);
			mydraw::primitive_mode_t pm = mycanvas->get_context()->current_primitive->get_primitive_mode();
			delete mycanvas->get_context()->current_primitive;

      switch(pm){
        case mydraw::primitive_mode_t::point:
          std::cout<<"Switched to primitive mode \'line\'\n";
          mycanvas->get_context()->current_primitive = new mydraw::line_t;
          break;
        case mydraw::primitive_mode_t::line:
          std::cout<<"Switched to primitive mode \'triangle\'\n";
          mycanvas->get_context()->current_primitive = new mydraw::triangle_t;
          break;
        case mydraw::primitive_mode_t::triangle:
          std::cout<<"Switched to primitive mode \'point\'\n";
          mycanvas->get_context()->current_primitive = new mydraw::point_t;
          break;
      }
    }
  }  

  //!GLFW cursor position callback
	void pos_callback(GLFWwindow* window, double xpos, double ypos)
  {
    mydraw::canvas_t* mycanvas = reinterpret_cast<mydraw::canvas_t*>(glfwGetWindowUserPointer(window));
		mycanvas->get_context()->current_primitive->set_point(xpos, mycanvas->get_height()-ypos);
    mycanvas->get_context()->active_tool->action(*mycanvas->get_context()->current_primitive, *mycanvas);
  }
  
  //!GLFW mouse callback
  void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    mydraw::canvas_t* mycanvas = reinterpret_cast<mydraw::canvas_t*>(glfwGetWindowUserPointer(window));
    ypos = mycanvas->get_height()-ypos;
    
		switch(button)
    {
      case GLFW_MOUSE_BUTTON_LEFT:
				if (action == GLFW_PRESS) {
					// Set callback
					if(mycanvas->get_context()->click_and_drag_conscious) {
						glfwSetCursorPosCallback(glfwGetCurrentContext(), pos_callback);
					}
					
					// Draw a point
    			mycanvas->get_context()->current_primitive->set_point(xpos, ypos);
					mycanvas->get_context()->active_tool->action(*mycanvas->get_context()->current_primitive, *mycanvas);
				} else if(action == GLFW_RELEASE) {
					if(mycanvas->get_context()->click_and_drag_conscious){
						// Remove callback
						glfwSetCursorPosCallback(glfwGetCurrentContext(), NULL);
						mycanvas->get_context()->active_tool->release(*mycanvas);
					}
				}
        break;
      default:
        break;
    }
  }

  void display_callback(mydraw::canvas_t* mycanvas)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram( mycanvas->get_canvas_sp() );

    glBindVertexArray(mycanvas->get_canvas_vao());

    glBindTexture(GL_TEXTURE_2D, mycanvas->get_canvas_texture());

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mycanvas->get_width(), mycanvas->get_height(), GL_RGBA, GL_FLOAT, mycanvas->get_store());

    glDrawArrays(GL_TRIANGLES, 0, 6);

  }

  int render_gl(int argc, char** argv, mydraw::canvas_t* mycanvas)
  {
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
    window = glfwCreateWindow(mycanvas->get_width(), mycanvas->get_height(), "MyDraw Canvas", NULL, NULL);
    if (!window)
      {
        glfwTerminate();
        return -1;
      }
    
    //! Make the window's context current 
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    //Turn this on to get Shader based OpenGL
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
      {
        //Problem: glewInit failed, something is seriously wrong.
        std::cerr<<"GLEW Init Failed : %s"<<std::endl;
      }

    //Print and see what context got enabled
    std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
    std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
    std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
    std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl<<std::endl;

    //Keyboard Callback
    glfwSetKeyCallback(window, key_callback);
    //Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Ensure we can capture the escape key being pressed below
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetWindowUserPointer(window, (void *)mycanvas);
    //Initialize GL state
    init_gl(mycanvas);
    //initBuffersGL();

    // Loop until the user closes the window
    while (glfwWindowShouldClose(window) == 0)
      {
        // Render here
        display_callback(mycanvas);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        
        // Poll for and process events
        glfwPollEvents();
      }
    
    glfwTerminate();
    return 0;
  }


}

