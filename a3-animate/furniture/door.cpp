#include "furniture.hpp"
#include "texture.hpp"
#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace door {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_structure(HNode *parent) {
        // list of vertices, colors
        std::vector<glm::vec4> v, c;
        std::vector<glm::vec3> n;
        std::vector<glm::vec2> t;

        // define box geometry
        make_texquad(v, c, n,
            glm::vec4(-80.0, 100.0, 80.0, 1.0),
            glm::vec4(-80.0, 100.0, 0.0, 1.0),
            glm::vec4(-40.0, 100.0, 0.0, 1.0),
            glm::vec4(-40.0, 100.0, 80.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            glm::vec2 b = t[i];
            t[i].x = (1-b.y)*0.8+0.2;
            t[i].y = 1-b.x;
        }

        Rigid_Node* root = new Rigid_Node(parent, v, c, n);
        root->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3());
        root->set_name("Door");
        root->set_texspace(t);
        root->set_texture("images/door.bmp", 150, 290);
        
        return root;
    }
}