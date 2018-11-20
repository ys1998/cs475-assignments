#include "mantis.hpp"
#include "hierarchy_node.hpp"
#include "parts.hpp"
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace mantis {
    class colors {
    public:
        static const glm::vec4 Light_YellowGreen;
        static const glm::vec4 Dark_YellowGreen;
        static const glm::vec4 Light_BlueGreen;
        static const glm::vec4 Dark_BlueGreen;
        static const glm::vec4 Light_Green;
        static const glm::vec4 Dark_Green;
        static const glm::vec4 White;
        static const glm::vec4 Black;
        static const glm::vec4 Yellow;
    };

    const glm::vec4 colors::Light_YellowGreen = glm::vec4(0.713725490196078, 0.823529411764706, 0.188235294117647, 1.0);
    const glm::vec4 colors::Dark_YellowGreen = glm::vec4(0.580392156862745, 0.670588235294118, 0.168627450980392, 1.0);
    const glm::vec4 colors::Light_BlueGreen = glm::vec4(0.325490196078431, 0.682352941176471, 0.313725490196078, 1.0);
    const glm::vec4 colors::Dark_Green = glm::vec4(0.0, 0.32156862745098, 0.184313725490196, 1.0);    
    const glm::vec4 colors::Light_Green = glm::vec4(0.13921568627451, 0.562745098039216, 0.084313725490196, 1.0);
    const glm::vec4 colors::Dark_BlueGreen = glm::vec4(0.129411764705882, 0.207843137254902, 0.133333333333333, 1.0);
    const glm::vec4 colors::White = glm::vec4(1.0, 1.0, 1.0, 1.0);
    const glm::vec4 colors::Black = glm::vec4(0.0, 0.0, 0.0, 1.0);
    const glm::vec4 colors::Yellow = glm::vec4(1.0, 1.0, 0.0, 1.0);

    HNode* create_character(std::vector<HNode*> &joints) {
        HNode* root = new HNode(nullptr);
        root->set_name("Mantis");
        joints.push_back(root);

        std::vector<glm::vec4> vertices, colors;
        glm::mat4 shape_transform;

        
        /* ----------- Abdomen Begin ----------*/
        make_frustum(vertices, colors, 0.4f, 0.4f, 0, 1.5f, colors::Light_Green, colors::Light_Green);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.75, 0.75, 1.0));
        Rigid_Node* abdomen = new Rigid_Node(root, vertices, colors, shape_transform);
        abdomen->set_name("Abdomen");
        abdomen->change_parameters(glm::vec3(), glm::vec3(0, 45.0f, 0));
        vertices.clear();   colors.clear();
        /* ----------- Abdomen End ----------*/


        /* ----------- Ab-Chest Joint Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.3, 0.3, 0.3));
        Ball_And_Socket_Joint* chest_joint = new Ball_And_Socket_Joint(abdomen, shape_transform, colors::Black);
        chest_joint->change_parameters(glm::vec3(0, 0, 1.7f), glm::vec3(0, -30, 0));
        chest_joint->set_name("Abdomen-Chest joint");
        joints.push_back(chest_joint);
        /* ----------- Ab-Chest Joint End ----------*/


        /* ----------- Chest Begin ----------*/
        make_egg(vertices, colors, 1.25f, -0.25f, -0.75f, colors::Light_Green);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(1.0, 0.5f, 1.0));
        shape_transform = glm::rotate(shape_transform, glm::radians(90.f), glm::vec3(0, -1.0f, 0));
        Rigid_Node* chest = new Rigid_Node(chest_joint, vertices, colors, shape_transform);
        chest->change_parameters(glm::vec3(0, 0, 1.3f));
        chest->set_name("Chest");
        vertices.clear();   colors.clear();
        /* ----------- Chest End ----------*/


        /* ----------- Neck Begin ----------*/
        make_sphere(vertices, colors, 0.3f, colors::Black);
        make_frustum(vertices, colors, 0.4f, 0.4f, 0, 2.0f, colors::Light_Green, colors::Light_Green);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.75, 0.75, 1.0));
        Rigid_Node* neck = new Rigid_Node(chest, vertices, colors, shape_transform);
        neck->change_parameters(glm::vec3(0.05f, 0, 1.4f));
        neck->set_name("Neck");
        vertices.clear();   colors.clear();
        /* ----------- Chest End ----------*/


        /* ----------- Head-Neck Joint Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.3, 0.3, 0.3));
        Ball_And_Socket_Joint* head_joint = new Ball_And_Socket_Joint(neck, shape_transform, colors::Black);
        head_joint->change_parameters(glm::vec3(0, 0, 2.2f));
        head_joint->set_name("Head-Neck joint");
        joints.push_back(head_joint);
        /* ----------- Head-Neck Joint End ----------*/


        /* ----------- Head Begin ----------*/
        make_frustum(vertices, colors, 0.55f, 0.5f, -1.0f, 0.3f, colors::Light_Green, colors::Light_Green);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.8, 1.0, 1.0));
        Rigid_Node* head = new Rigid_Node(head_joint, vertices, colors, shape_transform);
        head->change_parameters(glm::vec3(0.3f, 0, 0.4f), glm::vec3(0, -60, 0));
        head->set_name("Head");
        vertices.clear();   colors.clear();

        make_sphere(vertices, colors, 0.55f, colors::Light_Green);
        Rigid_Node *h1 = new Rigid_Node(head, vertices, colors, shape_transform);
        h1->change_parameters(glm::vec3(0, 0, -1.0f));
        vertices.clear();   colors.clear();
        make_sphere(vertices, colors, 0.5f, colors::Light_Green);
        Rigid_Node *h2 = new Rigid_Node(head, vertices, colors, shape_transform);
        h2->change_parameters(glm::vec3(0, 0, 0.3f));
        vertices.clear();   colors.clear();
        /* ----------- Head End ----------*/


        /* ----------- Eye Begin ----------*/
        make_sphere(vertices, colors, 0.3f, colors::Yellow);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(1.0, 1.3, 1.0));
        Rigid_Node* eye[] = {
            new Rigid_Node(head, vertices, colors, shape_transform),
            new Rigid_Node(head, vertices, colors, shape_transform),
        };
        eye[0]->change_parameters(glm::vec3(0.4, -0.6f, 0.2f));
        eye[1]->change_parameters(glm::vec3(0.4, 0.6f, 0.2f));
        eye[0]->set_name("Left eye");
        eye[1]->set_name("Right eye");
        vertices.clear();   colors.clear();
        /* ----------- Eye End ----------*/

        /* ----------- Eyeball Begin ----------*/
        make_sphere(vertices, colors, 0.2f, colors::Black);
        shape_transform = glm::mat4();
        Rigid_Node* eyeball[] = {
            new Rigid_Node(eye[0], vertices, colors, shape_transform),
            new Rigid_Node(eye[1], vertices, colors, shape_transform),
        };
        eyeball[0]->change_parameters(glm::vec3(0.2f, 0, 0));
        eyeball[1]->change_parameters(glm::vec3(0.2f, 0, 0));
        vertices.clear();   colors.clear();
        /* ----------- Eyeball End ----------*/


        /* ----------- Mustache Begin ----------*/
        make_hemisphere(vertices, colors, 0.25f, colors::Dark_Green);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(1, 1.3, 1));
        Rigid_Node* mustache[] = {
            new Rigid_Node(head, vertices, colors, shape_transform),
            new Rigid_Node(head, vertices, colors, shape_transform),
        };
        mustache[0]->change_parameters(glm::vec3(0.44f, -0.5f, -1.3f));
        mustache[1]->change_parameters(glm::vec3(0.44f, 0.5f, -1.3f));
        mustache[0]->set_name("Left mustache");
        mustache[1]->set_name("Right mustache");
        vertices.clear();   colors.clear();
        /* ----------- Mustache End ----------*/


        /* ----------- Tail Joint Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));
        Ball_And_Socket_Joint* tail_joint = new Ball_And_Socket_Joint(abdomen, shape_transform, colors::Black);
        tail_joint->set_name("Tail joint");
        joints.push_back(tail_joint);
        /* ----------- Tail Joint End ----------*/


        /* ----------- Tail Begin ----------*/
        make_sphere(vertices, colors, 1.0f, colors::Light_Green);
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.6f, 1.0f, 3.0f));
        Rigid_Node* tail = new Rigid_Node(tail_joint, vertices, colors, shape_transform);
        tail->change_parameters(glm::vec3(-2.0f, 0, -2.3f), glm::vec3(0, 45.0f, 0));
        tail->set_name("Tail");
        vertices.clear();   colors.clear();
        /* ----------- Tail End ----------*/


        /* ----------- Leg Upper Joints Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.15, 0.15, 0.15));
        Ball_And_Socket_Joint* leg_upper_joints[] = {
            new Ball_And_Socket_Joint(abdomen, shape_transform, colors::Black),
            new Ball_And_Socket_Joint(abdomen, shape_transform, colors::Black),
            new Ball_And_Socket_Joint(abdomen, shape_transform, colors::Black),
            new Ball_And_Socket_Joint(abdomen, shape_transform, colors::Black),
        };
        leg_upper_joints[0]->change_parameters(glm::vec3(0, -0.2f, 0.5f));
        leg_upper_joints[0]->set_name("Right foreleg upper joint");
        leg_upper_joints[1]->change_parameters(glm::vec3(0, 0.2f, 0.5f));
        leg_upper_joints[1]->set_name("Left foreleg upper joint");
        leg_upper_joints[2]->change_parameters(glm::vec3(0, -0.2f, 0.25f));
        leg_upper_joints[2]->set_name("Right hindleg upper joint");
        leg_upper_joints[3]->change_parameters(glm::vec3(0, 0.2f, 0.25f));
        leg_upper_joints[3]->set_name("Left hindleg upper joint");

        joints.push_back(leg_upper_joints[0]);
        joints.push_back(leg_upper_joints[1]);
        joints.push_back(leg_upper_joints[2]);
        joints.push_back(leg_upper_joints[3]);
        /* ----------- Leg Upper Joints End ----------*/


        /* ----------- Leg upper Begin ----------*/
        make_frustum(vertices, colors, 0.1f, 0.15f, 0, 1.8f, colors::Dark_Green, colors::Dark_Green);
        shape_transform = glm::mat4();
        Rigid_Node* leg_upper[] = {
            new Rigid_Node(leg_upper_joints[0], vertices, colors, shape_transform),
            new Rigid_Node(leg_upper_joints[1], vertices, colors, shape_transform),
            new Rigid_Node(leg_upper_joints[2], vertices, colors, shape_transform),
            new Rigid_Node(leg_upper_joints[3], vertices, colors, shape_transform),
        };
        leg_upper[0]->change_parameters(glm::vec3(), glm::vec3(20, 70, 0));
        leg_upper[1]->change_parameters(glm::vec3(), glm::vec3(-20, 70, 0));
        leg_upper[2]->change_parameters(glm::vec3(), glm::vec3(80, 50, 0));
        leg_upper[3]->change_parameters(glm::vec3(), glm::vec3(-80, 50, 0));
        leg_upper[0]->set_name("Right upper foreleg");
        leg_upper[1]->set_name("Left upper foreleg");
        leg_upper[2]->set_name("Right upper hindleg");
        leg_upper[3]->set_name("Left upper hindleg");
        vertices.clear();   colors.clear();
        /* ----------- Leg upper End ----------*/


        /* ----------- Knee Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));
        Hinge_Joint* knee_joints[] = {
            new Hinge_Joint(leg_upper[0], shape_transform, colors::Black, {false, true, false}),
            new Hinge_Joint(leg_upper[1], shape_transform, colors::Black, {false, true, false}),
            new Hinge_Joint(leg_upper[2], shape_transform, colors::Black, {false, true, false}),
            new Hinge_Joint(leg_upper[3], shape_transform, colors::Black, {false, true, false}),
        };
        knee_joints[0]->change_parameters(glm::vec3(0, 0, 1.8f));
        knee_joints[0]->set_name("Right foreleg knee joint");
        knee_joints[1]->change_parameters(glm::vec3(0, 0, 1.8f));
        knee_joints[1]->set_name("Left foreleg knee joint");
        knee_joints[2]->change_parameters(glm::vec3(0, 0, 1.8f));
        knee_joints[2]->set_name("Right hindleg knee joint");
        knee_joints[3]->change_parameters(glm::vec3(0, 0, 1.8f));
        knee_joints[3]->set_name("Left hindleg knee joint");

        joints.push_back(knee_joints[0]);
        joints.push_back(knee_joints[1]);
        joints.push_back(knee_joints[2]);
        joints.push_back(knee_joints[3]);
        /* ----------- Knee End ----------*/


        /* ----------- Leg lower Begin ----------*/
        make_frustum(vertices, colors, 0.15f, 0.1f, 0, 6.0f, colors::Dark_Green, colors::Dark_Green);
        shape_transform = glm::mat4();
        Rigid_Node* leg_lower[] = {
            new Rigid_Node(knee_joints[0], vertices, colors, shape_transform),
            new Rigid_Node(knee_joints[1], vertices, colors, shape_transform),
            new Rigid_Node(knee_joints[2], vertices, colors, shape_transform),
            new Rigid_Node(knee_joints[3], vertices, colors, shape_transform),
        };
        leg_lower[0]->change_parameters(glm::vec3(), glm::vec3(30, 40, 0));
        leg_lower[1]->change_parameters(glm::vec3(), glm::vec3(-30, 40, 0));
        leg_lower[2]->change_parameters(glm::vec3(), glm::vec3(70, 10, 0));
        leg_lower[3]->change_parameters(glm::vec3(), glm::vec3(-70, 10, 0));
        leg_lower[0]->set_name("Right lower foreleg");
        leg_lower[1]->set_name("Left lower foreleg");
        leg_lower[2]->set_name("Right lower hindleg");
        leg_lower[3]->set_name("Left lower hindleg");
        vertices.clear();   colors.clear();
        /* ----------- Leg lower End ----------*/


        /* ----------- Arm Upper Joints Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));
        Ball_And_Socket_Joint* arm_upper_joints[] = {
            new Ball_And_Socket_Joint(chest, shape_transform, colors::Black),
            new Ball_And_Socket_Joint(chest, shape_transform, colors::Black),
        };
        arm_upper_joints[0]->change_parameters(glm::vec3(0.1f, -0.6f, -0.3f), glm::vec3(69, -41, -23));
        arm_upper_joints[0]->set_name("Right arm upper joint");
        arm_upper_joints[1]->change_parameters(glm::vec3(0.1f, 0.6f, -0.3f), glm::vec3(-69, -41, 23));
        arm_upper_joints[1]->set_name("Left arm upper joint");
        
        joints.push_back(arm_upper_joints[0]);
        joints.push_back(arm_upper_joints[1]);
        /* ----------- Arm Upper Joints End ----------*/


        /* ----------- Arm Upper Begin ----------*/
        make_frustum(vertices, colors, 0.2f, 0.2f, 0, 1.8f, colors::Dark_Green, colors::Dark_Green);
        shape_transform = glm::mat4();
        Rigid_Node* arm_upper[] = {
            new Rigid_Node(arm_upper_joints[0], vertices, colors, shape_transform),
            new Rigid_Node(arm_upper_joints[1], vertices, colors, shape_transform),
        };
        arm_upper[0]->change_parameters(glm::vec3(), glm::vec3(55, 44, 0));
        arm_upper[1]->change_parameters(glm::vec3(), glm::vec3(-55, 44, 0));
        arm_upper[0]->set_name("Right upper arm");
        arm_upper[1]->set_name("Left upper arm");
        vertices.clear();   colors.clear();
        /* ----------- Arm Upper End ----------*/


        /* ----------- Elbow Joints Begin ----------*/
        shape_transform = glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));
        Hinge_Joint* elbow[] = {
            new Hinge_Joint(arm_upper[0], shape_transform, colors::Black),
            new Hinge_Joint(arm_upper[1], shape_transform, colors::Black),
        };
        elbow[0]->change_parameters(glm::vec3(0, 0, 1.8f));
        elbow[0]->set_name("Right elbow joint");
        elbow[1]->change_parameters(glm::vec3(0, 0, 1.8f));
        elbow[1]->set_name("Left elbow joint");
        
        joints.push_back(elbow[0]);
        joints.push_back(elbow[1]);
        /* ----------- Elbow Joints End ----------*/

        
        /* ----------- Forearm Begin ----------*/
        make_egg(vertices, colors, 1.25f, -0.25f, -0.75f, colors::Dark_Green);
        shape_transform = glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
        shape_transform = glm::rotate(shape_transform, glm::radians(180.f), glm::vec3(1.0f, 0, 0));
        shape_transform = glm::scale(shape_transform, glm::vec3(0.2f, 1.5f, 1.0f));
        Rigid_Node* forearm[] = {
            new Rigid_Node(elbow[0], vertices, colors, shape_transform),
            new Rigid_Node(elbow[1], vertices, colors, shape_transform),
        };
        forearm[0]->change_parameters(glm::vec3(1.8f, -0.4f, 0.25f), glm::vec3(0, 0, 80));
        forearm[1]->change_parameters(glm::vec3(1.8f, 0.4f, 0.25f), glm::vec3(0, 0, -80));
        forearm[0]->set_name("Right forearm");
        forearm[1]->set_name("Left forearm");
        vertices.clear();   colors.clear();
        /* ----------- Arm Upper End ----------*/
        return root;
    }
};

