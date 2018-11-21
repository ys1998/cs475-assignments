#include "furniture.hpp"
#include "texture.hpp"
#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace wall {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_structure(HNode *parent) {
        // list of vertices, colors
        std::vector<glm::vec4> v, c;
        std::vector<glm::vec3> n;
        std::vector<glm::vec2> t;

        // define box geometry
        make_texquad(v, c, n,
            glm::vec4(-100.0, 100.0, 100.0, 1.0),
            glm::vec4(-100.0, 100.0, 0.0, 1.0),
            glm::vec4(-80.0, 100.0, 0.0, 1.0),
            glm::vec4(-80.0, 100.0, 100.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            t[i].y = t[i].y * 0.1;
        }

        make_texquad(v, c, n,
            glm::vec4(-80.0, 100.0, 100.0, 1.0),
            glm::vec4(-80.0, 100.0, 80.0, 1.0),
            glm::vec4(-40.0, 100.0, 80.0, 1.0),
            glm::vec4(-40.0, 100.0, 100.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            t[i].y = t[i].y * 0.2 + 0.1;
            t[i].x = t[i].x * 0.2;
        }

        make_texquad(v, c, n,
            glm::vec4(-40.0, 100.0, 100.0, 1.0),
            glm::vec4(-40.0, 100.0, 0.0, 1.0),
            glm::vec4(0.0, 100.0, 0.0, 1.0),
            glm::vec4(0.0, 100.0, 100.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            t[i].y = t[i].y * 0.2 + 0.3;
        }

        make_texquad(v, c, n,
            glm::vec4(0.0, 100.0, 100.0, 1.0),
            glm::vec4(0.0, 100.0, 60.0, 1.0),
            glm::vec4(40.0, 100.0, 60.0, 1.0),
            glm::vec4(40.0, 100.0, 100.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            t[i].y = t[i].y * 0.2 + 0.5;
            t[i].x = t[i].x * 0.4;
        }
        
        make_texquad(v, c, n,
            glm::vec4(0.0, 100.0, 40.0, 1.0),
            glm::vec4(0.0, 100.0, 0.0, 1.0),
            glm::vec4(40.0, 100.0, 0.0, 1.0),
            glm::vec4(40.0, 100.0, 40.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            t[i].y = t[i].y * 0.2 + 0.5;
            t[i].x = t[i].x * 0.4 + 0.6;
        }

        make_texquad(v, c, n,
            glm::vec4(40.0, 100.0, 100.0, 1.0),
            glm::vec4(40.0, 100.0, 0.0, 1.0),
            glm::vec4(100.0, 100.0, 0.0, 1.0),
            glm::vec4(100.0, 100.0, 100.0, 1.0),
            glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), t
        );
        for(int i=t.size()-6; i<t.size(); i++){
            t[i].y = t[i].y * 0.3 + 0.7;
        }

        Rigid_Node* root = new Rigid_Node(parent, v, c, n);
        root->change_parameters(glm::vec3(0.0, 0.0, 0.0), glm::vec3());
        root->set_name("Front wall");
        root->set_texspace(t);
        root->set_texture("images/brick.bmp", 500, 333);
        
        return root;
    }
}