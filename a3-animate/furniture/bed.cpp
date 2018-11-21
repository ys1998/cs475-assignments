#include "furniture.hpp"
#include "texture.hpp"
#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace bed {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_structure(HNode *parent) {
        // list of vertices, colors
        std::vector<glm::vec4> v, c;
        std::vector<glm::vec3> n;
        std::vector<glm::vec2> t;

        // define box geometry
        make_texquad(v, c, n,
            glm::vec4(-27.0, -35.0, 0.0, 1.0),
            glm::vec4(-27.0, -35.0, 10.0, 1.0),
            glm::vec4(-27.0, 35.0, 10.0, 1.0),
            glm::vec4(-27.0, 35.0, 0.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );

        make_texquad(v, c, n,
            glm::vec4(-27.0, -35.0, 10.0, 1.0),
            glm::vec4(-27.0, -35.0, 0.0, 1.0),
            glm::vec4(27.0, -35.0, 0.0, 1.0),
            glm::vec4(27.0, -35.0, 10.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );

        make_texquad(v, c, n,
            glm::vec4(27.0, -35.0, 10.0, 1.0),
            glm::vec4(27.0, -35.0, 0.0, 1.0),
            glm::vec4(27.0, 35.0, 0.0, 1.0),
            glm::vec4(27.0, 35.0, 10.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );

        make_texquad(v, c, n,
            glm::vec4(-27.0, 35.0, 0.0, 1.0),
            glm::vec4(-27.0, 35.0, 10.0, 1.0),
            glm::vec4(27.0, 35.0, 10.0, 1.0),
            glm::vec4(27.0, 35.0, 0.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );

        make_texquad(v, c, n,
            glm::vec4(-27.0, -35.0, 10.0, 1.0),
            glm::vec4(27.0, -35.0, 10.0, 1.0),
            glm::vec4(27.0, 35.0, 10.0, 1.0),
            glm::vec4(-27.0, 35.0, 10.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );

        // make_texquad(v, c, n,
        //     glm::vec4(-27.0, 0.0, 0.0, 1.0),
        //     glm::vec4(27.0, 0.0, 0.0, 1.0),
        //     glm::vec4(27.0, 35.0, 0.0, 1.0),
        //     glm::vec4(-27.0, 35.0, 0.0, 1.0),
        //     glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        // );

        t.clear();

        t.push_back(glm::vec2(0.7,0.7)); t.push_back(glm::vec2(1.0,0.7)); t.push_back(glm::vec2(1.0,1.0));
	   	t.push_back(glm::vec2(1.0,1.0)); t.push_back(glm::vec2(0.7,1.0)); t.push_back(glm::vec2(0.7,0.7));
	   	t.push_back(glm::vec2(0.7,0.7)); t.push_back(glm::vec2(1.0,0.7)); t.push_back(glm::vec2(1.0,1.0));
	   	t.push_back(glm::vec2(1.0,1.0)); t.push_back(glm::vec2(0.7,1.0)); t.push_back(glm::vec2(0.7,0.7));
	   	t.push_back(glm::vec2(0.7,0.7)); t.push_back(glm::vec2(1.0,0.7)); t.push_back(glm::vec2(1.0,1.0));
	   	t.push_back(glm::vec2(1.0,1.0)); t.push_back(glm::vec2(0.7,1.0)); t.push_back(glm::vec2(0.7,0.7));
	   	t.push_back(glm::vec2(0.7,0.7)); t.push_back(glm::vec2(1.0,0.7)); t.push_back(glm::vec2(1.0,1.0));
	   	t.push_back(glm::vec2(1.0,1.0)); t.push_back(glm::vec2(0.7,1.0)); t.push_back(glm::vec2(0.7,0.7));
	   	t.push_back(glm::vec2(0.7,0.7)); t.push_back(glm::vec2(1.0,0.7)); t.push_back(glm::vec2(1.0,1.0));
	   	t.push_back(glm::vec2(1.0,1.0)); t.push_back(glm::vec2(0.7,1.0)); t.push_back(glm::vec2(0.7,0.7));

        Rigid_Node* root = new Rigid_Node(parent, v, c, n);
        root->change_parameters(glm::vec3(70.0, -50.0, 0.0), glm::vec3());
        root->set_name("Bed");
        root->set_texspace(t);
        root->set_texture("images/all.bmp", 512, 512);
        
        return root;
    }
}