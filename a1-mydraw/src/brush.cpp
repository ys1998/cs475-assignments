/*
    This file is part of the mydraw.

    mydraradius is a simple, interactive drawing program written using OpenGL. 
    
    This code base is meant to serve as a starter code for an assignment in a 
    Computer Graphics course.

    Copyright (c) 2018 by Parag Chaudhuri

    mydraradius is free software; you can redistribute it and/or modify it under the 
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

#include "brush.hpp"
#include "canvas.hpp"

namespace mydraw {
	// Draw or erase a circle given its centre and radius along with the canvas and the function for drawing/erasing
	void draw_circle(glm::vec2 pt, unsigned int radius, canvas_t *canvas, void (canvas_t::*draw)(unsigned, unsigned))
	 {
		unsigned int x = pt.x, y = pt.y;
		unsigned int width = canvas->get_width(), height = canvas->get_height();

		for(int i = -1 * (int)radius; i <= (int)radius; ++i)
			for(int j = -1 * (int)radius; j <= (int)radius; ++j)
				if((int)x+i >= 0 && (int)x+i < (int)width && (int)y+j >= 0 && (int)y+j < (int)height && i*i + j*j <= (int)(radius*radius))
					(canvas->*draw)((int)x+i, (int)y+j);

	}

	// Draw a line of circles
	// Uses Bresenham's algorithm for determining the pixels of the centres of the circles
	void draw_line_of_circles(glm::vec2 pt, glm::vec2 old_pt, unsigned radius, canvas_t *canvas, void (canvas_t::*draw)(unsigned, unsigned))
	 {
		unsigned x = pt.x, y = pt.y, oldx = old_pt.x, oldy = old_pt.y;
		float eps = 0, del;
		unsigned x1, x2, y1, y2, delta_var, inc_var, ul, *var_x, *var_y;
		int inc;

		if(x > oldx) {
			x1 = oldx; y1 = oldy;
			x2 = x; y2 = y;
		} else {
			x1 = x; y1 = y;
			x2 = oldx; y2 = oldy;
		}
		del = ((float)y2-(float)y1)/((float)x2-(float)x1);

		if(del > 1) {
			inc = 1; del = 1/del;
			inc_var = y1; ul = y2;
			delta_var = x1;
			var_x = &delta_var;
			var_y = &inc_var;
		} else if(del > 0) {
			inc = 1;
			inc_var = x1; ul = x2;
			delta_var = y1;
			var_x = &inc_var;
			var_y = &delta_var;
		} else if(del > -1) {
			inc = -1; del = -del;
			inc_var = x1; ul = x2;
			delta_var = y1;
			var_x = &inc_var;
			var_y = &delta_var;
		} else {
			inc = -1; del = -1/del;
			inc_var = y2; ul = y1;
			delta_var = x2;
			var_x = &delta_var;
			var_y = &inc_var;
		}
		
		for(; inc_var < ul; inc_var++) {
			glm::vec2 buff_pt = {*var_x, *var_y};
			if(eps + del > 0.5) {
				delta_var = delta_var + inc;
				eps = eps + del - 1;
				draw_circle(buff_pt, radius, canvas, draw);
			} else {
				draw_circle(buff_pt, radius, canvas, draw);
				eps = eps + del;
			}
		}
	}

	// Point brush actions
	void point_brush_t::action (const primitive_t &pt, canvas_t &canvas)
	 {
		if(pt.get_primitive_mode() == primitive_mode_t::point) {
			draw_circle(pt.get_point(), get_size(), &canvas, &canvas_t::set_pixel);
		} else if(pt.get_primitive_mode() == primitive_mode_t::line) {
			glm::vec2 p1 = pt.get_point(0);
			if(p1.x == -1 && p1.y == -1) {
				// line cannot be created
			} else {
				glm::vec2 p2 = pt.get_point(1);
				if(p2.x == -1 && p2.y == -1) {
					// only one point exists
					draw_circle(p1, get_size(), &canvas, &canvas_t::set_pixel);
				} else {
					draw_line_of_circles(p1, p2, get_size(), &canvas, &canvas_t::set_pixel);
				}
			}
		} else if(pt.get_primitive_mode() == primitive_mode_t::triangle) {
			glm::vec2 p1 = pt.get_point(0);
			if(p1.x == -1 && p1.y == -1) {
				// triangle cannot be created
			} else {
				glm::vec2 p2 = pt.get_point(1);
				if(p2.x == -1 && p2.y == -1) {
					// only one point exists
					draw_circle(p1, get_size(), &canvas, &canvas_t::set_pixel);
				} else {
					glm::vec2 p3 = pt.get_point(2);
					if(p3.x == -1 && p3.y == -1) {
						// only two points exist
						draw_circle(p1, get_size(), &canvas, &canvas_t::set_pixel);
						draw_circle(p2, get_size(), &canvas, &canvas_t::set_pixel);
					} else {
						// triangle can be drawn
						draw_line_of_circles(p1, p2, get_size(), &canvas, &canvas_t::set_pixel);
						draw_line_of_circles(p2, p3, get_size(), &canvas, &canvas_t::set_pixel);
						draw_line_of_circles(p3, p1, get_size(), &canvas, &canvas_t::set_pixel);
					}
				}
			}
		}
	}
	
	// Eraser brush actions
	void eraser_point_brush_t::action (const primitive_t &pt, canvas_t &canvas)
	 {
		if(pt.get_primitive_mode() == primitive_mode_t::point) {
			draw_circle(pt.get_point(), get_size(), &canvas, &canvas_t::erase_pixel);
		} else if(pt.get_primitive_mode() == primitive_mode_t::line) {
			glm::vec2 p1 = pt.get_point(0);
			if(p1.x == -1 && p1.y == -1) {
				// line cannot be created
			} else {
				glm::vec2 p2 = pt.get_point(1);
				if(p2.x == -1 && p2.y == -1) {
					// only one point exists
					draw_circle(p1, get_size(), &canvas, &canvas_t::erase_pixel);
				} else {
					draw_line_of_circles(p1, p2, get_size(), &canvas, &canvas_t::erase_pixel);
				}
			}
		} else if(pt.get_primitive_mode() == primitive_mode_t::triangle) {
			glm::vec2 p1 = pt.get_point(0);
			if(p1.x == -1 && p1.y == -1) {
				// triangle cannot be created
			} else {
				glm::vec2 p2 = pt.get_point(1);
				if(p2.x == -1 && p2.y == -1) {
					// only one point exists
					draw_circle(p1, get_size(), &canvas, &canvas_t::erase_pixel);
				} else {
					glm::vec2 p3 = pt.get_point(2);
					if(p3.x == -1 && p3.y == -1) {
						// only two points exist
						draw_circle(p1, get_size(), &canvas, &canvas_t::erase_pixel);
						draw_circle(p2, get_size(), &canvas, &canvas_t::erase_pixel);
					} else {
						// triangle can be drawn
						draw_line_of_circles(p1, p2, get_size(), &canvas, &canvas_t::erase_pixel);
						draw_line_of_circles(p2, p3, get_size(), &canvas, &canvas_t::erase_pixel);
						draw_line_of_circles(p3, p1, get_size(), &canvas, &canvas_t::erase_pixel);
					}
				}
			}
		}
	}

	// Smooth brush actions
	void smooth_brush_t::action (const primitive_t &pt, canvas_t &canvas)
	 {
		if(pt.get_primitive_mode() == primitive_mode_t::point) {
			if(last_pt == nullptr) {
				last_pt = new point_t;
				draw_circle(pt.get_point(), get_size(), &canvas, &canvas_t::set_pixel);
			} else {
				draw_line_of_circles(pt.get_point(), last_pt->get_point(), get_size(), &canvas, &canvas_t::set_pixel);
			}

			last_pt->set_point(pt.get_point());
		}
	}

	void smooth_brush_t::release (canvas_t &canvas)
	 {
		delete last_pt;
		last_pt = nullptr;
	}
	
	// Smooth eraser brush actions
	void eraser_smooth_brush_t::action (const primitive_t &pt, canvas_t &canvas)
	 {
		if(pt.get_primitive_mode() == primitive_mode_t::point) {
			if(last_pt == nullptr) {
				last_pt = new point_t;
				draw_circle(pt.get_point(), get_size(), &canvas, &canvas_t::erase_pixel);
			} else {
				draw_line_of_circles(pt.get_point(), last_pt->get_point(), get_size(), &canvas, &canvas_t::erase_pixel);
			}

			last_pt->set_point(pt.get_point());
		}
	}
	
	void eraser_smooth_brush_t::release (canvas_t &canvas)
	 {
		delete last_pt;
		last_pt = nullptr;
	}
}
