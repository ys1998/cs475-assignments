#include "panda.hpp"

namespace panda {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_character(std::vector<HNode*> &joints) {
        HNode* root = new HNode(nullptr);
        root->set_name("Panda");
        joints.push_back(root);

        std::vector<glm::vec4> v, c;
        std::vector<glm::vec3> n;
        glm::mat4 s;

        // hip
        make_hemisphere(v, c, n, 1, glm::vec4(0.3137, 0.196, 0.0, 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(4.0, 4.0, 3.0));
        s = glm::rotate(s, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* hip = new Rigid_Node(root, v, c, n, s);

        // waist
        s = glm::scale(glm::mat4(1.0), glm::vec3(3.99, 3.99, 2.0));
        Ball_And_Socket_Joint* waist = new Ball_And_Socket_Joint(hip, s, glm::vec4(1.0));

        // torso
        s = glm::scale(glm::mat4(1.0), glm::vec3(4.0, 4.0, 6.0));
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(1.0));
        Rigid_Node* torso = new Rigid_Node(waist, v, c, n, s);

        // neck joint 1
        s = glm::scale(glm::mat4(1.0), glm::vec3(2.5, 2.5, 2.0));
        Ball_And_Socket_Joint* neck_joint_1 = new Ball_And_Socket_Joint(torso, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        neck_joint_1->change_parameters(glm::vec3(0.0, 0.0, 4.5), glm::vec3(0.0));

        // neck
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 2.51, 2.0, 0.0, 1.5, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        Rigid_Node* neck = new Rigid_Node(neck_joint_1, v, c, n);
        neck->change_parameters(glm::vec3(0.0, 0.0, 0.5), glm::vec3(0.0));

        // neck joint 2
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.99, 1.99, 1.5));
        Ball_And_Socket_Joint* neck_joint_2 = new Ball_And_Socket_Joint(neck, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        neck_joint_2->change_parameters(glm::vec3(0.0, 0.0, 1.5), glm::vec3(0.0));

        // head
        s = glm::scale(glm::mat4(1.0), glm::vec3(2.5, 2.5, 2.0));
        v.clear(); c.clear(); n.clear();
        make_sphere(v, c, n, 1.0, glm::vec4(1.0));
        Rigid_Node* head = new Rigid_Node(neck_joint_2, v, c, n, s);
        head->change_parameters(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0));

        // ears
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(glm::vec3(0.0), 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(0.7, 0.2, 1.3));
        Rigid_Node* left_ear = new Rigid_Node(head, v, c, n, s);
        left_ear->change_parameters(glm::vec3(-1.5, 0.0, 1.0), glm::vec3(0.0));
        Rigid_Node* right_ear = new Rigid_Node(head, v, c, n, s);
        right_ear->change_parameters(glm::vec3(1.5, 0.0, 1.0), glm::vec3(0.0));

        // black eye patches
        v.clear(); c.clear(); n.clear();
        make_sphere(v, c, n, 1.0, glm::vec4(glm::vec3(0.0), 1.0));
        Rigid_Node* left_eye_patch = new Rigid_Node(head, v, c, n);
        left_eye_patch->change_parameters(glm::vec3(-0.7, 1.4, 0.4), glm::vec3(0.0));
        Rigid_Node* right_eye_patch = new Rigid_Node(head, v, c, n);
        right_eye_patch->change_parameters(glm::vec3(0.7, 1.4, 0.4), glm::vec3(0.0));

        // eyes
        v.clear(); c.clear(); n.clear();
        make_sphere(v, c, n, 0.5, glm::vec4(1.0));
        Rigid_Node* left_eye = new Rigid_Node(head, v, c, n);
        left_eye->change_parameters(glm::vec3(-0.8, 1.9, 0.6), glm::vec3(0.0));
        Rigid_Node* right_eye = new Rigid_Node(head, v, c, n);
        right_eye->change_parameters(glm::vec3(0.8, 1.9, 0.6), glm::vec3(0.0));

        v.clear(); c.clear(); n.clear();
        make_sphere(v, c, n, 0.2, glm::vec4(0.0, 0.5, 0.0, 1.0));
        Rigid_Node* left_eye1 = new Rigid_Node(head, v, c, n);
        left_eye1->change_parameters(glm::vec3(-0.8, 2.3, 0.6), glm::vec3(0.0));
        Rigid_Node* right_eye1 = new Rigid_Node(head, v, c, n);
        right_eye1->change_parameters(glm::vec3(0.8, 2.3, 0.6), glm::vec3(0.0));

        // nose
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(glm::vec3(0.0), 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(0.7, 1.0, 0.5));
        s = glm::rotate(s, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* nose = new Rigid_Node(head, v, c, n, s);
        nose->change_parameters(glm::vec3(0.0, 1.7, 0.0), glm::vec3(0.0));

        // mouth
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(0.5, 0.0, 0.0, 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.0, 1.0, 0.3));
        s = glm::rotate(s, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* mouth = new Rigid_Node(head, v, c, n, s);
        mouth->change_parameters(glm::vec3(0.0, 1.7, -0.6), glm::vec3(0.0));

        // left shoulder
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.49, 1.49, 1.49));
        Ball_And_Socket_Joint* left_shoulder = new Ball_And_Socket_Joint(torso, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        left_shoulder->change_parameters(glm::vec3(-2.5, 0.0, 3.5), glm::vec3(0.0, -45.0, 0.0));

        // left upper arm
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 1.2, 1.4, 0.0, 2.5, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        Rigid_Node* left_upper = new Rigid_Node(left_shoulder, v, c, n);
        left_upper->change_parameters(glm::vec3(-0.2, 0.0, 0.1), glm::vec3(0.0, -60.0, 0.0));

        // left elbow
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.39));
        Hinge_Joint* left_elbow = new Hinge_Joint(left_upper, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        left_elbow->change_parameters(glm::vec3(0.0, 0.0, 2.49), glm::vec3(0.0, -90.0, 90.0));

        // left lower arm
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 1.4, 1.0, 0.0, 4.0, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        Rigid_Node* left_lower = new Rigid_Node(left_elbow, v, c, n);
        left_lower->change_parameters(glm::vec3(0.0), glm::vec3(0.0, 90.0, 0.0));

        // left wrist
        Ball_And_Socket_Joint* left_wrist = new Ball_And_Socket_Joint(left_lower, glm::mat4(1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        left_wrist->change_parameters(glm::vec3(0.0, 0.0, 4.0));

        // left hand
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(glm::vec3(0.0), 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.2, 1.0, 1.0));
        s = glm::rotate(s, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        Rigid_Node* left_hand = new Rigid_Node(left_wrist, v, c, n, s);
        left_hand->change_parameters(glm::vec3(0.3, 0.0, 1.0), glm::vec3(0.0));

        // right shoulder
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.49, 1.49, 1.49));
        Ball_And_Socket_Joint* right_shoulder = new Ball_And_Socket_Joint(torso, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        right_shoulder->change_parameters(glm::vec3(2.5, 0.0, 3.5), glm::vec3(0.0, 45.0, 0.0));

        // right upper arm
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 1.2, 1.4, 0.0, 2.5, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        Rigid_Node* right_upper = new Rigid_Node(right_shoulder, v, c, n);
        right_upper->change_parameters(glm::vec3(0.2, 0.0, 0.1), glm::vec3(0.0, 60.0, 0.0));

        // right elbow
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.39));
        Hinge_Joint* right_elbow = new Hinge_Joint(right_upper, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        right_elbow->change_parameters(glm::vec3(0.0, 0.0, 2.49), glm::vec3(0.0, -90.0, 90.0));

        // right lower arm
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 1.4, 1.0, 0.0, 4.0, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        Rigid_Node* right_lower = new Rigid_Node(right_elbow, v, c, n);
        right_lower->change_parameters(glm::vec3(0.0), glm::vec3(0.0, 90.0, 0.0));

        // right wrist
        Ball_And_Socket_Joint* right_wrist = new Ball_And_Socket_Joint(right_lower, glm::mat4(1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        right_wrist->change_parameters(glm::vec3(0.0, 0.0, 4.0));

        // right hand
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(glm::vec3(0.0), 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.2, 1.0, 1.0));
        s = glm::rotate(s, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        Rigid_Node* right_hand = new Rigid_Node(right_wrist, v, c, n, s);
        right_hand->change_parameters(glm::vec3(-0.3, 0.0, 1.0), glm::vec3(0.0));

        // left hip
        s = glm::scale(glm::mat4(1.0), glm::vec3(2.0, 2.0, 2.0));
        Ball_And_Socket_Joint* left_hip = new Ball_And_Socket_Joint(hip, s, glm::vec4(0.3137, 0.196, 0.0, 1.0));
        left_hip->change_parameters(glm::vec3(-1.8, 0.0, -2.0), glm::vec3(0.0));

        // left thigh
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 2.01, 1.5, 0.0, 2.25, glm::vec4(0.3137, 0.196, 0.0, 1.0), glm::vec4(0.5882, 0.3529, 0.0, 1.0));
        Rigid_Node* left_thigh = new Rigid_Node(left_hip, v, c, n);
        left_thigh->change_parameters(glm::vec3(0.0), glm::vec3(0.0, -155.0, 0.0));

        // left knee
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.49));
        Hinge_Joint* left_knee = new Hinge_Joint(left_thigh, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        left_knee->change_parameters(glm::vec3(0.0, 0.0, 2.25), glm::vec3(0.0, 90.0, 90.0));

        // left lower leg
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 1.5, 1.0, 0.0, 2.0, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        s = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* left_lower_leg = new Rigid_Node(left_knee, v, c, n, s);
        left_lower_leg->change_parameters(glm::vec3(0.0), glm::vec3(0.0, -90.0, 0.0));

        // left ankle
        s = glm::scale(glm::mat4(1.0), glm::vec3(0.9, 0.9, 0.9));
        Ball_And_Socket_Joint* left_ankle = new Ball_And_Socket_Joint(left_lower_leg, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        left_ankle->change_parameters(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, 22.0));

        // left foot
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(0.3137, 0.196, 0.0, 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.0, 1.25, 1.7));
        s = glm::rotate(s, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* left_foot = new Rigid_Node(left_ankle, v, c, n, s);
        left_foot->change_parameters(glm::vec3(0.0, 1.0, -1.0), glm::vec3(0.0));

        // right hip
        s = glm::scale(glm::mat4(1.0), glm::vec3(2.0, 2.0, 2.0));
        Ball_And_Socket_Joint* right_hip = new Ball_And_Socket_Joint(hip, s, glm::vec4(0.3137, 0.196, 0.0, 1.0));
        right_hip->change_parameters(glm::vec3(1.8, 0.0, -2.0), glm::vec3(0.0));

        // right thigh
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 2.01, 1.5, 0.0, 2.25, glm::vec4(0.3137, 0.196, 0.0, 1.0), glm::vec4(0.5882, 0.3529, 0.0, 1.0));
        Rigid_Node* right_thigh = new Rigid_Node(right_hip, v, c, n);
        right_thigh->change_parameters(glm::vec3(0.0), glm::vec3(0.0, 155.0, 0.0));

        // right knee
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.49));
        Hinge_Joint* right_knee = new Hinge_Joint(right_thigh, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        right_knee->change_parameters(glm::vec3(0.0, 0.0, 2.25), glm::vec3(0.0, 90.0, 90.0));

        // right lower leg
        v.clear(); c.clear(); n.clear();
        make_frustum(v, c, n, 1.5, 1.0, 0.0, 2.0, glm::vec4(0.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
        s = glm::rotate(glm::mat4(1.0), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* right_lower_leg = new Rigid_Node(right_knee, v, c, n, s);
        right_lower_leg->change_parameters(glm::vec3(0.0), glm::vec3(0.0, -90.0, 0.0));
        
        // right ankle
        s = glm::scale(glm::mat4(1.0), glm::vec3(0.9, 0.9, 0.9));
        Ball_And_Socket_Joint* right_ankle = new Ball_And_Socket_Joint(right_lower_leg, s, glm::vec4(0.0, 0.0, 0.0, 1.0));
        right_ankle->change_parameters(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -22.0));

        // right foot
        v.clear(); c.clear(); n.clear();
        make_hemisphere(v, c, n, 1.0, glm::vec4(0.3137, 0.196, 0.0, 1.0));
        s = glm::scale(glm::mat4(1.0), glm::vec3(1.0, 1.25, 1.7));
        s = glm::rotate(s, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        Rigid_Node* right_foot = new Rigid_Node(right_ankle, v, c, n, s);
        right_foot->change_parameters(glm::vec3(0.0, 1.0, -1.0), glm::vec3(0.0));

        // add joints to vector
        waist->set_name("Waist"); joints.push_back(waist);
        neck_joint_1->set_name("Lower neck joint"); joints.push_back(neck_joint_1);
        neck_joint_2->set_name("Upper neck joint"); joints.push_back(neck_joint_2);
        left_shoulder->set_name("Left shoulder"); joints.push_back(left_shoulder);
        right_shoulder->set_name("Right shoulder"); joints.push_back(right_shoulder);
        left_elbow->set_name("Left elbow"); joints.push_back(left_elbow);
        right_elbow->set_name("Right elbow"); joints.push_back(right_elbow);
        left_wrist->set_name("Left wrist"); joints.push_back(left_wrist);
        right_wrist->set_name("Right wrist"); joints.push_back(right_wrist);
        left_hip->set_name("Left hip"); joints.push_back(left_hip);
        right_hip->set_name("Right hip"); joints.push_back(right_hip);
        left_knee->set_name("Left knee"); joints.push_back(left_knee);
        right_knee->set_name("Right knee"); joints.push_back(right_knee);
        left_ankle->set_name("Left ankle"); joints.push_back(left_ankle);
        right_ankle->set_name("Right ankle"); joints.push_back(right_ankle);

        // name the root node
        hip->set_name("Hip");
        return root;
    }
};
