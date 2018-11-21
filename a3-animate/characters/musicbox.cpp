#include "musicbox.hpp"
#include <glm/gtx/string_cast.hpp>

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
        std::vector<glm::vec3> n;
        std::vector<glm::vec2> t;

        // define box geometry
        make_texquad(v, c, n,
            glm::vec4(-17.0, -15.0, 0.0, 1.0),
            glm::vec4(-17.0, -15.0, 25.0, 1.0),
            glm::vec4(-17.0, 15.0, 25.0, 1.0),
            glm::vec4(-17.0, 15.0, 0.0, 1.0),
            silver, light_silver, light_silver, silver, t
        );

        make_texquad(v, c, n,
            glm::vec4(-17.0, -15.0, 25.0, 1.0),
            glm::vec4(-17.0, -15.0, 0.0, 1.0),
            glm::vec4(17.0, -15.0, 0.0, 1.0),
            glm::vec4(17.0, -15.0, 25.0, 1.0),
            silver, light_silver, light_silver, silver, t
        );

        make_texquad(v, c, n,
            glm::vec4(17.0, -15.0, 25.0, 1.0),
            glm::vec4(17.0, -15.0, 0.0, 1.0),
            glm::vec4(17.0, 15.0, 0.0, 1.0),
            glm::vec4(17.0, 15.0, 25.0, 1.0),
            silver, light_silver, light_silver, silver, t
        );

        make_texquad(v, c, n,
            glm::vec4(-17.0, 15.0, 0.0, 1.0),
            glm::vec4(-17.0, 15.0, 25.0, 1.0),
            glm::vec4(17.0, 15.0, 25.0, 1.0),
            glm::vec4(17.0, 15.0, 0.0, 1.0),
            silver, light_silver, light_silver, silver, t
        );

        make_texquad(v, c, n,
            glm::vec4(17.0, -15.0, 0.0, 1.0),
            glm::vec4(-17.0, -15.0, 0.0, 1.0),
            glm::vec4(-17.0, 15.0, 0.0, 1.0),
            glm::vec4(17.0, 15.0, 0.0, 1.0),
            silver, silver, silver, silver, t
        );

        make_frustum(v, c, n, 13.0, 13.0, 0.0, 5.0, silver, light_silver);
        make_frustum(v, c, n, 13.0, 0.0, 5.0, 5.0, light_silver, very_light_silver);

        Rigid_Node* box = new Rigid_Node(root, v, c, n);
        box->set_name("Music box");
        box->set_texspace(t);
        box->set_texture("images/box.bmp", 512, 512);

        Hinge_Joint* hinge = new Hinge_Joint(box, glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)), light_silver);
        hinge->change_parameters(glm::vec3(0.0, -15.0, 25.0), glm::vec3(0.0, 90.0, 0.0));
        hinge->set_name("Hinge");
        joints.push_back(hinge);

        v.clear(); c.clear(); n.clear(); t.clear();
        make_texquad(v, c, n,
            glm::vec4(0.0, 0.0, -17.0, 1.0),
            glm::vec4(0.0, 0.0, 17.0, 1.0),
            glm::vec4(0.0, 30.0, 17.0, 1.0),
            glm::vec4(0.0, 30.0, -17.0, 1.0),
            silver, silver, silver, silver, t
        );
        Rigid_Node* lid = new Rigid_Node(hinge, v, c, n);
        lid->set_name("Lid");
        lid->set_texspace(t);
        lid->set_texture("images/box.bmp", 512, 512);
        
        return root;
    }
};