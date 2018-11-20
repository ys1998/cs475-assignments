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

#include "primitive.hpp"
#include "tool.hpp"

/**
 * \brief Everything in mydraw is a part of this namespace.
 */
namespace mydraw
{
	//! Define more brush enumerations here
	enum class brush_type_t {point, eraser_point, smooth, eraser_smooth};

	/**
	 * \brief An abstract base brush class
	 */
	class brush_t : public tool_t
	{
		protected:
			//! Brush type
			brush_type_t brush_type;

		public:
			virtual ~brush_t() { }
			brush_t(brush_type_t t) : tool_t(tool_type_t::brush), brush_type(t) { }

			virtual void action (const primitive_t &pt, canvas_t &canvas) = 0;
			virtual void release (canvas_t &canvas) { }

			const brush_type_t get_brush_type(void) const {return brush_type;}
	};

	/**
	 * \brief A point brush - each brush action is made up of points of the current size in the current draw color.
	 * 
	 * A point brush of size 1 is implemented by default. Extend to implement other point sizes.
	 *
	 */
	class point_brush_t: public brush_t
	{
		public:
			point_brush_t() : brush_t(brush_type_t::point) { size = 1; }
			~point_brush_t() { }

			void action (const primitive_t &pt, canvas_t &canvas);
	};

	/**
	 * \brief A point eraser - each brush action is made up of points of the current size in the current background color.
	 * 
	 * An eraser point brush of size 1 is implemented by default. Extend to implement other point sizes.
	 *
	 */
	class eraser_point_brush_t: public brush_t
	{
		public:
			eraser_point_brush_t() : brush_t(brush_type_t::eraser_point) { size = 1; }
			~eraser_point_brush_t() { }

			void action (const primitive_t &pt, canvas_t &canvas);
	};

	/**
	 * \brief A smooth brush - each brush action is made up of points of the current size in the current draw color.
	 * 
	 * The smooth brush stores the last point and paints along the action between last point and current point
	 *
	 */
	class smooth_brush_t: public brush_t
	{
		private:
			primitive_t *last_pt;
		public:
			smooth_brush_t() : brush_t(brush_type_t::smooth) { last_pt = nullptr; size = 1; }
			~smooth_brush_t() { delete last_pt; }

			void action (const primitive_t &pt, canvas_t &canvas);
			void release (canvas_t &canvas);
	};

	/**
	 * \brief A smooth eraser - each brush action is made up of points of the current size in the current background color.
	 * 
	 * The smooth eraser stores the last point and deletes along the action between last point and current point
	 *
	 */
	class eraser_smooth_brush_t: public brush_t
	{
		private:
			primitive_t *last_pt;
		public:
			eraser_smooth_brush_t() : brush_t(brush_type_t::eraser_smooth) { last_pt = nullptr; size = 1; }
			~eraser_smooth_brush_t() { delete last_pt; }

			void action (const primitive_t &pt, canvas_t &canvas);
			void release (canvas_t &canvas);
	};

}
