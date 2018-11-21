#include "furniture.hpp"
#include "texture.hpp"
#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace wall_light {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_structure(HNode* parent) {
        // list of vertices, colors
        std::vector<glm::vec4> v, c;
        std::vector<glm::vec3> n;
        std::vector<glm::vec2> t;

        // define box geometry
        make_quad(v, c, n,
            glm::vec4(10.0, -10.0, 90.0, 1.0),
            glm::vec4(-10.0, -10.0, 90.0, 1.0),
            glm::vec4(-10.0, 10.0, 90.0, 1.0),
            glm::vec4(10.0, 10.0, 90.0, 1.0),
            glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0)
        );
        make_frustum(v,c,n,1.0,1.0,90.0,100.0,glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0));

        Rigid_Node* root = new Rigid_Node(parent, v, c, n);
        root->set_name("Wall Light");
        return root;
    }
}