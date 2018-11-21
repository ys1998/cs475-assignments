/*
	Declaration of functions for camera and character animation.
*/

#include "animation.hpp"

extern unsigned int N_CHARACTERS;
extern std::vector<HNode*> joint_nodes[];
extern std::vector<HNode*> roots;
extern GLuint lampLighted, wallLighted;

extern std::string FILENAME;

Keyframe::Keyframe(){
	values.clear(); type.clear();
}

int Keyframe::write_frame_to_file(void){
	std::ofstream fout(FILENAME, std::ofstream::out | std::ofstream::app);
	if(fout.is_open()){
		for(size_t i=0; i<values.size(); ++i){
			fout << type[i] << " " << values[i] << " ";
		}
		fout << std::endl;
		fout.close();
		return 0;
	}else{
		std::cout << "Error in opening file " << FILENAME <<".\n";
		return -1;
	}
}

int Keyframe::read_frame_from_file(int line_no){
	std::ifstream fin(FILENAME, std::ifstream::in);
	if(fin.is_open()){
		int i=0;
		std::string str;
		while(i <= line_no && !fin.eof()){
			std::getline(fin, str);
			i++;
		}
		if(i <= line_no || str == "\n" || str == "")
			return -1;
		values.clear(); type.clear();
		std::istringstream iss(str);
		while(iss.peek() != EOF && iss.peek() != '\n'){
			int t; float v;
			iss >> t; type.push_back(t);
			iss >> v; values.push_back(v);
			iss >> std::ws; // eat away whitespace
		}
		fin.close();
		return 0;
	}else{
		std::cout << "Error in opening file " << FILENAME << ".\n";
		return -1;
	}
}

void Keyframe::build_frame_from_state(void){
	values.clear(); type.clear();
	// add root coordinates (both translation and rotation parameters)
	for(size_t i=0; i<N_CHARACTERS; ++i){
		std::vector<float> params = roots[i]->get_parameters();
		std::vector<float> t(6, 1.0);
		values.reserve(values.size() + 6);
		values.insert(values.end(), params.begin(), params.end());
		type.reserve(type.size() + 6);
		type.insert(type.end(), t.begin(), t.end());
	}
	// add joint angles (only rotation parameters)
	for(size_t i=0; i<N_CHARACTERS; ++i){
		for(size_t j=0; j<joint_nodes[i].size(); ++j){
			std::vector<float> params = joint_nodes[i][j]->get_parameters();
			std::vector<float> t(3, 1.0);
			values.reserve(values.size() + 3);
			values.insert(values.end(), params.begin()+3, params.end());
			type.reserve(type.size() + 3);
			type.insert(type.end(), t.begin(), t.end());
		}
	}
	// add light sources
	values.push_back(wallLighted);
	type.push_back(0);
	values.push_back(lampLighted);
	type.push_back(0);
}

std::vector<float> interpolate_keyframes(Keyframe &k1, Keyframe &k2, float t){
	std::vector<float> res(0);
	for(size_t i=0; i<k1.values.size() && i<k2.values.size(); ++i){
		if(k1.type[i] == 0.0 && k2.type[i] == 0.0){
			// binary valued quantity
			if(t < 0.5)
				res.push_back(k1.values[i]);
			else
				res.push_back(k2.values[i]);
		}else if(k1.type[i] == 1.0 && k2.type[i] == 1.0){
			// linear interpolation
			res.push_back(k1.values[i]*(1-t) + k2.values[i]*t);
		}else{
			// empty vector in case of error
			return std::vector<float>(0);
		}
	}
	return res;
}

void update_state(const std::vector<float> &state){
	// update root coordinates (both translation and rotation parameters)
	for(size_t i=0; i<N_CHARACTERS; ++i){
		roots[i]->change_parameters(
			glm::vec3(state[6*i], state[6*i+1], state[6*i+2]),
			glm::vec3(state[6*i+3], state[6*i+4], state[6*i+5])
		);
	}
	// update joint angles (only rotation parameters)
	size_t offset = 6*N_CHARACTERS;
	for(size_t i=0; i<N_CHARACTERS; ++i){
		for(size_t j=0; j<joint_nodes[i].size(); ++j){
			std::vector<float> params = joint_nodes[i][j]->get_parameters();
			joint_nodes[i][j]->change_parameters(
				glm::vec3(params[0], params[1], params[2]),
				glm::vec3(state[offset + 3*j], state[offset + 3*j + 1], state[offset + 3*j + 2])
			);
		}
		offset += 3*joint_nodes[i].size();
	}
	// update light sources
	wallLighted = state[offset];
	lampLighted = state[offset+1];
}