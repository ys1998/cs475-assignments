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

#include "primitive.hpp"
#include <glm/vec2.hpp>

namespace mydraw {
	//! Function declarations for point_t
	void point_t::set_point(int x, int y) {
		pt.x = x; pt.y = y;
	}
	void point_t::set_point(glm::vec2 p) {
		set_point(p.x, p.y);
	}
	glm::vec2 point_t::get_point(int pos=0) const{
		if(pos > 0)	return glm::vec2(-1, -1);
		return glm::vec2(pt.x, pt.y);
	}
	void point_t::reset(void) {
		pt.x = -1; pt.y = -1;
	}

	//! Function declarations for line_t
	void line_t::set_point(int x, int y){
		pt[1].x = pt[0].x; pt[0].x = x;
		pt[1].y = pt[0].y; pt[0].y = y;
	}
	void line_t::set_point(glm::vec2 p) {
		set_point(p.x, p.y);
	}
	glm::vec2 line_t::get_point(int pos) const{
		if(pos == 0){
			return glm::vec2(pt[0].x, pt[0].y);
		}else if(pos == 1){
			return glm::vec2(pt[1].x, pt[1].y);
		}else{
			return glm::vec2(-1,-1);
		}
	}
	void line_t::reset(void) {
		pt[0].x = -1; pt[0].y = -1;
		pt[1].x = -1; pt[1].y = -1;
	}

	//! Function declarations form triangle_t
	void triangle_t::set_point(int x, int y){
		pt[2].x = pt[1].x; pt[1].x = pt[0].x; pt[0].x = x;
		pt[2].y = pt[1].y; pt[1].y = pt[0].y; pt[0].y = y;
	}
	void triangle_t::set_point(glm::vec2 p) {
		set_point(p.x, p.y);
	}
	glm::vec2 triangle_t::get_point(int pos) const{
		if(pos == 0){
			return glm::vec2(pt[0].x, pt[0].y);
		}else if(pos == 1){
			return glm::vec2(pt[1].x, pt[1].y);
		}else if(pos == 2){
			return glm::vec2(pt[2].x, pt[2].y);
		}else{
			return glm::vec2(-1,-1);
		}
	}
	void triangle_t::reset(void) {
		pt[0].x = -1; pt[0].y = -1;
		pt[1].x = -1; pt[1].y = -1;
		pt[2].x = -1; pt[2].y = -1;
	}
}
