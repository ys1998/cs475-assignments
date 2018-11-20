#include "musicbox.hpp"

namespace musicbox {
    HNode* create_character(std::vector<HNode*> &joints) {
        HNode* root = new HNode(nullptr);
        root->set_name("Music Box");
        joints.push_back(root);

        // colors
        glm::vec4 silver = glm::vec4(0.5882, 0.5882, 0.5882, 0.2);
        glm::vec4 light_silver = glm::vec4(0.7843, 0.7843, 0.7843, 0.2);
        glm::vec4 very_light_silver = glm::vec4(0.9, 0.9, 0.9, 0.2);

        // list of vertices, colors
        std::vector<glm::vec4> v, c;

        // define box geometry
        make_quad(v, c, 
            glm::vec4(-17.0, -15.0, 0.0, 1.0),
            glm::vec4(-17.0, -15.0, 25.0, 1.0),
            glm::vec4(-17.0, 15.0, 25.0, 1.0),
            glm::vec4(-17.0, 15.0, 0.0, 1.0),
            silver, light_silver, light_silver, silver
        );

        make_quad(v, c, 
            glm::vec4(-17.0, -15.0, 0.0, 1.0),
            glm::vec4(-17.0, -15.0, 25.0, 1.0),
            glm::vec4(17.0, -15.0, 25.0, 1.0),
            glm::vec4(17.0, -15.0, 0.0, 1.0),
            silver, light_silver, light_silver, silver
        );

        make_quad(v, c, 
            glm::vec4(17.0, -15.0, 0.0, 1.0),
            glm::vec4(17.0, -15.0, 25.0, 1.0),
            glm::vec4(17.0, 15.0, 25.0, 1.0),
            glm::vec4(17.0, 15.0, 0.0, 1.0),
            silver, light_silver, light_silver, silver
        );

        make_quad(v, c, 
            glm::vec4(-17.0, 15.0, 0.0, 1.0),
            glm::vec4(-17.0, 15.0, 25.0, 1.0),
            glm::vec4(17.0, 15.0, 25.0, 1.0),
            glm::vec4(17.0, 15.0, 0.0, 1.0),
            silver, light_silver, light_silver, silver
        );

        make_quad(v, c, 
            glm::vec4(-17.0, -15.0, 0.0, 1.0),
            glm::vec4(17.0, -15.0, 0.0, 1.0),
            glm::vec4(17.0, 15.0, 0.0, 1.0),
            glm::vec4(-17.0, 15.0, 0.0, 1.0),
            silver, silver, silver, silver
        );

        make_frustum(v, c, 13.0, 13.0, 0.0, 5.0, silver, light_silver);
        make_frustum(v, c, 13.0, 0.0, 5.0, 5.0, light_silver, light_silver);

        Rigid_Node* box = new Rigid_Node(root, v, c);

        Hinge_Joint* hinge = new Hinge_Joint(box, glm::mat4(), light_silver);
        hinge->change_parameters(glm::vec3(0.0, -15.0, 25.0), glm::vec3(0.0, 90.0, 0.0));

        v.clear(); c.clear();
        make_quad(v, c, 
            glm::vec4(0.0, 0.0, -17.0, 1.0),
            glm::vec4(0.0, 0.0, 17.0, 1.0),
            glm::vec4(0.0, 30.0, 17.0, 1.0),
            glm::vec4(0.0, 30.0, -17.0, 1.0),
            very_light_silver, very_light_silver, very_light_silver, very_light_silver
        );
        new Rigid_Node(hinge, v, c);
        
        box->set_name("Music box");
        hinge->set_name("Hinge");
        joints.push_back(hinge);
        return root;
    }
};