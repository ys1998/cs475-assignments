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

#pragma once

#include <glm/vec2.hpp>

namespace mydraw
{
	enum class primitive_mode_t
	{
		point,
		line,
		triangle
	};

	//2D point
	// typedef glm::vec2 point_t;

	//! Base class for primitives
	class primitive_t {
		protected:
			const primitive_mode_t mode;
		public:
			primitive_t(primitive_mode_t t) : mode(t) { }
			virtual ~primitive_t() { }

			const primitive_mode_t get_primitive_mode(void) const { return mode; }

			virtual void set_point(int, int) = 0;
			virtual void set_point(glm::vec2) = 0;

			virtual glm::vec2 get_point(int p=0) const = 0;
			virtual void reset() = 0;
	};

	//! Primitive class for 2D points
	class point_t: public primitive_t {
		private:
			glm::vec2 pt;
		public:
			point_t(): primitive_t(primitive_mode_t::point) {
				pt.x = -1; pt.y = -1;
			}
			
			~point_t() { }
			void set_point(int, int);
			void set_point(glm::vec2);
			void reset(void);
			glm::vec2 get_point(int) const;
	};

	//! Primitive class for 2D lines
	class line_t: public primitive_t {
		private:
			glm::vec2 pt[2];
		public:
			line_t(): primitive_t(primitive_mode_t::line) {
				pt[0].x = -1; pt[0].y = -1;
				pt[1].x = -1; pt[1].y = -1;
			}
			
			~line_t() { };
			void set_point(int, int);
			void set_point(glm::vec2);
			glm::vec2 get_point(int) const;
			void reset(void);
	};

	//! Primitive class for 2D triangles
	class triangle_t: public primitive_t {
		private:
			glm::vec2 pt[3];
		public:
			triangle_t(): primitive_t(primitive_mode_t::triangle) {
				pt[0].x = -1; pt[0].y = -1;
				pt[1].x = -1; pt[1].y = -1;
				pt[2].x = -1; pt[2].y = -1;
			}
			
			~triangle_t() { };
			void set_point(int, int);
			void set_point(glm::vec2);
			glm::vec2 get_point(int) const;
			void reset(void);
	};
}
