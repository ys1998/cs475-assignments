/*
    Definitions of functions for building components from vertices.
*/

#include "parts.hpp"
#include <glm/trigonometric.hpp>
#include <glm/detail/func_exponential.hpp>

// Makes triangle with specified vertices
void make_tri(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c,         // coordinates of new vertices
    const glm::vec4 &c_a, const glm::vec4 &c_b, const glm::vec4 &c_c    // colors of new vertices
) {
    vertices.push_back(a); colors.push_back(c_a);
    vertices.push_back(b); colors.push_back(c_b);
    vertices.push_back(c); colors.push_back(c_c);
}

// Makes a quadrilateral with specified vertices in cyclic order
void make_quad(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    const glm::vec4 &a, const glm::vec4 &b,                             // coordinates of new vertices
    const glm::vec4 &c, const glm::vec4 &d,                             // in cyclic order
    const glm::vec4 &c_a, const glm::vec4 &c_b,                         // colors of new vertices 
    const glm::vec4 &c_c, const glm::vec4 &c_d    
) {
   make_tri(vertices, colors, a, b, c, c_a, c_b, c_c);
   make_tri(vertices, colors, a, c, d, c_a, c_c, c_d); 
}

// Makes frustum of cone with lower surface on z=hl with radius rl
// and upper surface on z=hu with radius ru
void make_frustum(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    const float &rl, const float &ru,                                   // lower and upper radii
    const float &hl, const float &hu,                                   // lower and upper heights
    const glm::vec4 &c_l, const glm::vec4 &c_u                          // colors of lower and upper edge
) {
    const float step = 6;
    for(float angle=0; angle<360; angle+=step) {
        make_quad(
            vertices, colors,
            glm::vec4(rl*glm::cos(glm::radians(angle)), rl*glm::sin(glm::radians(angle)), hl, 1.0),
            glm::vec4(rl*glm::cos(glm::radians(angle+step)), rl*glm::sin(glm::radians(angle+step)), hl, 1.0),
            glm::vec4(ru*glm::cos(glm::radians(angle+step)), ru*glm::sin(glm::radians(angle+step)), hu, 1.0),
            glm::vec4(ru*glm::cos(glm::radians(angle)), ru*glm::sin(glm::radians(angle)), hu, 1.0),
            c_l, c_l, c_u, c_u
        );
    }
}

// Makes a tesselleted sphere centered at origin with radius r
void make_sphere(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    const float &r, const glm::vec4 &c                                  // radius and color of sphere
) {
    int steps = 40;
    for(int i=0; i<steps; ++i){
        float h = r - (i*r*2.0/steps);
        float ru = glm::sqrt(r*r-h*h);
        float rl = glm::sqrt(r*r-(h-2.0*r/steps)*(h-2.0*r/steps));
        make_frustum(vertices, colors, rl, ru, h-2.0*r/steps, h, c, c);
    }
}


// Makes a tesselleted hemisphere centered at origin with radius r
// body is present in z>0 space
void make_hemisphere(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    const float &r, const glm::vec4 &c                                  // radius and color of sphere
) {
    int steps = 20;
    for(int i=0; i<steps; ++i){
        float h = r - (i*r/steps);
        float ru = glm::sqrt(r*r-h*h);
        float rl = glm::sqrt(r*r-(h-r/steps)*(h-r/steps));
        make_frustum(vertices, colors, rl, ru, h-r/steps, h, c, c);
    }
    // close the surface below
    make_frustum(vertices, colors, r, 0, 0, 0, c, c);
}

void make_egg(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    const float &a, const float &b, const float &l,
    const glm::vec4 &c                                                  // color of egg
) {
    const float step = 10;
    for(float angle = 0; angle < 360; angle +=step) {
        float l_buff = a + b * glm::cos(glm::radians(angle));
        float u_buff = a + b * glm::cos(glm::radians(angle + step));
        float lower_height = (l + l_buff) * glm::cos(glm::radians(angle));
        float lower_radius = l_buff * glm::sin(glm::radians(angle));
        float upper_height = (l + u_buff) * glm::cos(glm::radians(angle + step));
        float upper_radius = u_buff * glm::sin(glm::radians(angle + step));
        make_frustum(vertices, colors, lower_radius, upper_radius, lower_height, upper_height, c, c);
    }
}