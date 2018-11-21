/*
    Definitions of functions for building components from vertices.
*/

#include "parts.hpp"
#include <glm/trigonometric.hpp>
#include <glm/detail/func_exponential.hpp>
#include <glm/glm.hpp>
#include <iostream>

#define PI 3.14159265358

// Makes triangle with specified vertices
void make_tri(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c,         // coordinates of new vertices
    const glm::vec4 &c_a, const glm::vec4 &c_b, const glm::vec4 &c_c    // colors of new vertices
) {
    vertices.push_back(a); colors.push_back(c_a);
    vertices.push_back(b); colors.push_back(c_b);
    vertices.push_back(c); colors.push_back(c_c);
    glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(b-a), glm::vec3(c-b))); 
    
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
}

// Makes a quadrilateral with specified vertices in cyclic order
void make_quad(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const glm::vec4 &a, const glm::vec4 &b,                             // coordinates of new vertices
    const glm::vec4 &c, const glm::vec4 &d,                             // in cyclic order
    const glm::vec4 &c_a, const glm::vec4 &c_b,                         // colors of new vertices 
    const glm::vec4 &c_c, const glm::vec4 &c_d    
) {
   make_tri(vertices, colors, normals, a, b, c, c_a, c_b, c_c);
   make_tri(vertices, colors, normals, c, d, a, c_c, c_d, c_a);
}

void make_texquad(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const glm::vec4 &a, const glm::vec4 &b,                             // coordinates of new vertices
    const glm::vec4 &c, const glm::vec4 &d,                             // in cyclic order
    const glm::vec4 &c_a, const glm::vec4 &c_b,                         // colors of new vertices 
    const glm::vec4 &c_c, const glm::vec4 &c_d,
    std::vector<glm::vec2> &texspace    
) {
   make_tri(vertices, colors, normals, a, b, c, c_a, c_b, c_c);
   make_tri(vertices, colors, normals, c, d, a, c_c, c_d, c_a);
   texspace.push_back(glm::vec2(0.0,0.0));
   texspace.push_back(glm::vec2(1.0,0.0));
   texspace.push_back(glm::vec2(1.0,1.0));
   texspace.push_back(glm::vec2(1.0,1.0));
   texspace.push_back(glm::vec2(0.0,1.0));
   texspace.push_back(glm::vec2(0.0,0.0));
}

// Makes frustum of cone with lower surface on z=hl with radius rl
// and upper surface on z=hu with radius ru
void make_frustum(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const float &rl, const float &ru,                                   // lower and upper radii
    const float &hl, const float &hu,                                   // lower and upper heights
    const glm::vec4 &c_l, const glm::vec4 &c_u                          // colors of lower and upper edge
) {
    const float step = 6;
    int init = vertices.size();
    for(float angle=0; angle<360; angle+=step) {
        std::vector<glm::vec3> buff_normals;
        make_quad(
            vertices, colors, buff_normals,
            glm::vec4(rl*glm::cos(glm::radians(angle)), rl*glm::sin(glm::radians(angle)), hl, 1.0),
            glm::vec4(rl*glm::cos(glm::radians(angle+step)), rl*glm::sin(glm::radians(angle+step)), hl, 1.0),
            glm::vec4(ru*glm::cos(glm::radians(angle+step)), ru*glm::sin(glm::radians(angle+step)), hu, 1.0),
            glm::vec4(ru*glm::cos(glm::radians(angle)), ru*glm::sin(glm::radians(angle)), hu, 1.0),
            c_l, c_l, c_u, c_u
        );
    }

    glm::vec3 norm = glm::normalize(glm::vec3(hu-hl, 0.0, rl-ru));

    for(size_t i=init; i<vertices.size(); i++){
        glm::vec3 dir_vec = glm::normalize(glm::vec3(vertices[i].x, vertices[i].y, 0.0));
        glm::mat3 rot = glm::mat3(dir_vec.x, dir_vec.y, 0, -dir_vec.y, dir_vec.x, 0, 0, 0, 1);
        normals.push_back(rot*norm);
    }
}

// Makes a tesselleted sphere centered at origin with radius r
void make_sphere(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const float &r, const glm::vec4 &c                                  // radius and color of sphere
) {
    int steps = 40;
    int init = vertices.size();
    for(int i=0; i<steps; ++i){
        std::vector<glm::vec3> buff_normals;
        float h = r - (i*r*2.0/steps);
        float ru = glm::sqrt(r*r-h*h);
        float rl = glm::sqrt(r*r-(h-2.0*r/steps)*(h-2.0*r/steps));
        make_frustum(vertices, colors, buff_normals, rl, ru, h-2.0*r/steps, h, c, c);
    }
    
    for(size_t i=init; i<vertices.size(); i++){
        normals.push_back(glm::vec3(vertices[i]));
    }
}

// Makes a tesselleted hemisphere centered at origin with radius r
// body is present in z>0 space
void make_hemisphere(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const float &r, const glm::vec4 &c                                  // radius and color of sphere
) {
    int steps = 20;
    int init = vertices.size();
    std::vector<glm::vec3> buff_normals;
    for(int i=0; i<steps; ++i){
        float h = r - (i*r/steps);
        float ru = glm::sqrt(r*r-h*h);
        float rl = glm::sqrt(r*r-(h-r/steps)*(h-r/steps));
        make_frustum(vertices, colors, buff_normals, rl, ru, h-r/steps, h, c, c);
    }
    // close the surface below
    make_frustum(vertices, colors, buff_normals, r, 0, 0, 0, c, c);

    for(size_t i=init; i<vertices.size(); i++){
        normals.push_back(glm::vec3(vertices[i]));
    }
    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));
}

void make_egg(
    std::vector<glm::vec4> &vertices,                                   // list of vertices
    std::vector<glm::vec4> &colors,                                     // list of vertex colors
    std::vector<glm::vec3> &normals,                                    // list of normals
    const float &a, const float &b, const float &l,
    const glm::vec4 &c                                                  // color of egg
) {
    const float step = 10;
    int init = vertices.size();
    for(float angle = 0; angle < 360; angle +=step) {
        std::vector<glm::vec3> buff_normals;
        float l_buff = a + b * glm::cos(glm::radians(angle));
        float u_buff = a + b * glm::cos(glm::radians(angle + step));
        float lower_height = (l + l_buff) * glm::cos(glm::radians(angle));
        float lower_radius = l_buff * glm::sin(glm::radians(angle));
        float upper_height = (l + u_buff) * glm::cos(glm::radians(angle + step));
        float upper_radius = u_buff * glm::sin(glm::radians(angle + step));
        make_frustum(vertices, colors, buff_normals, lower_radius, upper_radius, lower_height, upper_height, c, c);
    }

    for(size_t i=init; i<vertices.size(); i++){
        normals.push_back(glm::vec3(vertices[i]));
    }
}