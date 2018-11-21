/*
    Declarations of functions for making parts of objects. 
*/
#ifndef _PARTS
#define _PARTS

#include <vector>
#include <glm/vec4.hpp>

void make_tri(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const glm::vec4 &, const glm::vec4 &, const glm::vec4 &,
    const glm::vec4 &, const glm::vec4 &, const glm::vec4 &
);

void make_quad(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const glm::vec4 &, const glm::vec4 &, const glm::vec4 &, const glm::vec4 &,
    const glm::vec4 &, const glm::vec4 &, const glm::vec4 &, const glm::vec4 &
);

void make_texquad(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const glm::vec4 &, const glm::vec4 &, const glm::vec4 &, const glm::vec4 &,
    const glm::vec4 &, const glm::vec4 &, const glm::vec4 &, const glm::vec4 &,
    std::vector<glm::vec2> &
);

void make_frustum(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const float &, const float &, const float &, const float &,
    const glm::vec4 &, const glm::vec4 &
);

void make_sphere(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const float &, const glm::vec4 &
);

void make_hemisphere(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const float &, const glm::vec4 &
);

void make_egg(
    std::vector<glm::vec4> &, std::vector<glm::vec4> &, std::vector<glm::vec3> &,
    const float &, const float &, const float &, const glm::vec4 &
);
#endif