#include "hierarchy_node.hpp"
#include <vector>

namespace mantis {
    // Function to create the character
    // returns pointer to root node, and adds all joints to vector
    HNode* create_character(std::vector<HNode*> &);
};