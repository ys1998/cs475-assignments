#include "furniture.hpp"
#include "texture.hpp"
#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace lamp {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_structure(HNode* parent) {
        // list of vertices, colors
        std::vector<glm::vec4> v, c;
        std::vector<glm::vec3> n;

        // define box geometry
        make_frustum(v, c, n,
            9.0, 3.0, 20.0, 30.0,
            glm::vec4(1.0,1.0,1.0,1.0), glm::vec4(1.0,1.0,1.0,1.0)
        );        
        for(int i=0; i<n.size(); i++)   n[i] = glm::vec3(-n[i].x, -n[i].y, -n[i].z);

        make_frustum(v,c,n,
            2.0,2.0,0.0,20.0,glm::vec4(),glm::vec4());

        Rigid_Node* root = new Rigid_Node(parent, v, c, n);
        root->change_parameters(glm::vec3(-70.0, 60.0, 0.0), glm::vec3());
        root->set_name("Lamp");
        return root;
    }
}