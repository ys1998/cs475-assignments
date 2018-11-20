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

#include "fill.hpp"
#include "primitive.hpp"
#include "canvas.hpp"
#include <queue>
#include <iostream>

namespace mydraw{
	bool floodfill_t::equicolor(const color_t &x, const color_t &y){
		return (x.r==y.r) && (x.g==y.g) && (x.b==y.b) && (x.a==y.a);
	}

	bool inside(const unsigned int &x, const unsigned int &y, const canvas_t &cv){
		unsigned int width = cv.get_width();
		unsigned int height = cv.get_height();
		return x<width && y<height;
	}

	void floodfill_t::action(const primitive_t &pt, canvas_t &canvas){
		unsigned x = pt.get_point().x, y = pt.get_point().y;
		// store original color
		color_t c = canvas.get_pixel(x,y);
		
		bool filled[canvas.get_width()][canvas.get_height()];
		
		for(unsigned int i=0;i<canvas.get_width(); ++i){
			for(unsigned int j=0; j<canvas.get_height(); ++j){
				filled[i][j] = false;
			}
		}
		
		filled[x][y] = true;
		
		std::queue<unsigned int> qx, qy;
		qx.push(x);
		qy.push(y);

		unsigned int new_x, new_y;

		while(!qx.empty()){
			new_x = qx.front();
			new_y = qy.front();
			canvas.set_pixel(new_x, new_y);
			
			// insert neighbors into the queue
			new_y++;
			if(inside(new_x, new_y, canvas) && !filled[new_x][new_y] && equicolor(canvas.get_pixel(new_x, new_y), c)){
				qx.push(new_x); qy.push(new_y);
				filled[new_x][new_y] = true;
			}

			new_y-=2;
			if(inside(new_x, new_y, canvas) && !filled[new_x][new_y] && equicolor(canvas.get_pixel(new_x, new_y), c)){
				qx.push(new_x); qy.push(new_y);
				filled[new_x][new_y] = true;
			}

			new_x++; new_y++;
			if(inside(new_x, new_y, canvas) && !filled[new_x][new_y] && equicolor(canvas.get_pixel(new_x, new_y), c)){
				qx.push(new_x); qy.push(new_y);
				filled[new_x][new_y] = true;
			}

			new_x-=2;
			if(inside(new_x, new_y, canvas) && !filled[new_x][new_y] && equicolor(canvas.get_pixel(new_x, new_y), c)){
				qx.push(new_x); qy.push(new_y);
				filled[new_x][new_y] = true;
			}
			qx.pop(); qy.pop();
		}
	}
}
