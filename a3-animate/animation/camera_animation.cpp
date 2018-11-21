 /*
	Function definitions for camera animation tasks.
*/

#include "animation.hpp"

extern GLuint vPosition, vColor, uModelViewMatrix;
extern glm::mat4 view_matrix;
extern std::string CAMERA_FILENAME;

CameraPointsBuffer::CameraPointsBuffer(){
	points.clear();
	v.clear(); c.clear();
	pv.clear(); pc.clear(); changed = true;
	make_sphere(v, c, n, 0.5, glm::vec4(glm::vec3(0.0), 1.0));
}

void CameraPointsBuffer::init_buffers(void){
	int t = sizeof(glm::vec4);
	// initialize vao and vbo of the object
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// allocate space for buffers
	glBufferData(GL_ARRAY_BUFFER, t*(v.size() + c.size()), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, t*v.size(), &v[0]);
	glBufferSubData(GL_ARRAY_BUFFER, t*v.size(), t*c.size(), &c[0]);

	// setup the vertex array as per the shader
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(t*v.size()));
}

void CameraPointsBuffer::render_points(void){
	glBindVertexArray(vao);
	for(size_t i=0; i<points.size(); ++i){
		umat = view_matrix * glm::translate(glm::mat4(), points[i]);
		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(umat));
		glDrawArrays(GL_TRIANGLES, 0, v.size());
	}
}

void CameraPointsBuffer::save_to_file(void){
	std::ofstream fout(CAMERA_FILENAME, std::ofstream::out | std::ofstream::trunc);
	if(fout.is_open()){
		for(size_t i=0; i<points.size(); ++i){
			fout << points[i].x << " " << points[i].y << " " << points[i].z << std::endl;
		}
		fout.close();
	}else{
		std::cout << "Error in opening file " << CAMERA_FILENAME <<".\n";
	}
}

void CameraPointsBuffer::load_from_file(void){
	std::ifstream fin(CAMERA_FILENAME, std::ifstream::in);
	if(fin.is_open()){
		points.clear();
		std::string str;
		std::getline(fin, str);
		while(!fin.eof()){
			std::istringstream iss(str);
			glm::vec3 temp;
			iss >> temp.x; iss >> temp.y; iss >> temp.z;
			points.push_back(temp);
			std::getline(fin, str);
		}
		changed = true;
		fin.close();
	}else{
		std::cout << "Error in opening file " << CAMERA_FILENAME << ".\n";
	}
}

// De Casteljau's algorithm for finding point on Bezier curve
glm::vec3 compute_camera_trajectory(std::vector<glm::vec3> points, float u){
	std::vector<glm::vec3> temp(points.size() - 1);
	for(size_t i=0; i<points.size()-1; ++i){
		temp[i] = points[i]*(1-u) + points[i+1]*u;
	}
	for(size_t j=1; j < points.size(); ++j){
		for(size_t i=0; i < points.size()-j-1; ++i){
			temp[i] = temp[i]*(1-u) + temp[i+1]*u;		
		}
	}
	return temp[0];
}

void CameraPointsBuffer::render_path(void){
	if (changed && points.size() > 1){
		glDeleteBuffers(1, &p_vbo);
		glDeleteVertexArrays(1, &p_vao);

		// initialize vao and vbo for displaying path
		glGenVertexArrays(1, &p_vao);
		glGenBuffers(1, &p_vbo);
		glBindVertexArray(p_vao);
		glBindBuffer(GL_ARRAY_BUFFER, p_vbo);

		pv.clear(); pc.clear();
		for(float u=0.0; u<= 1.0; u+=0.01){
			pv.push_back(glm::vec4(compute_camera_trajectory(points, u), 1.0));
			pc.push_back(glm::vec4(glm::vec3(0.0), 1.0));
		}

		// allocate space for buffers
		int t = sizeof(glm::vec4);
		glBufferData(GL_ARRAY_BUFFER, t*(pv.size() + pc.size()), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, t*pv.size(), &pv[0]);
		glBufferSubData(GL_ARRAY_BUFFER, t*pv.size(), t*pc.size(), &pc[0]);

		// setup the vertex array as per the shader
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(t*pv.size()));

		glBindVertexArray(p_vao);
		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glDrawArrays(GL_LINE_STRIP, 0, pv.size());

		changed = false;
	} else {
		glBindVertexArray(p_vao);
		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glDrawArrays(GL_LINE_STRIP, 0, pv.size());
	}
}