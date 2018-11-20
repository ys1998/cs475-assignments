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

/**
 * \brief Everything in mydraw is a part of this namespace.
 */
namespace mydraw
{
	//! Forward Declaration to prevent a circular dependency
	class canvas_t;

	enum class tool_type_t {brush, fill};

	/**
	 * \brief An abstract base tool class
	 */
	class tool_t
	{
		protected:
			//! Brush size
			int size;

			const tool_type_t tool_type;

		public:
			virtual ~tool_t() { }
			tool_t(tool_type_t t) : tool_type(t) { };

			virtual void action (const primitive_t &pt, canvas_t &canvas) = 0;
			virtual void release (canvas_t &canvas) { }

			const int get_size(void) const { return size; }
			void increase_size() { size++; }
			void decrease_size() { size = (size > 1) ? size-1 : size;}

			const tool_type_t get_tool_type() const { return tool_type;}
	};
}
