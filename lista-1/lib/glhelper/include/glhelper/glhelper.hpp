#include <algorithm>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <numbers>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glh {

// helper struct
struct shape {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};

// constants
const GLint DEFAULT_WIDTH = 1280;
const GLint DEFAULT_HEIGHT = 720;

// terminates the program
void terminate();

// GLFW functions
void glfw_error_callback(int code, const char* desc);
void glfw_frambuffer_size_callback(GLFWwindow* window, int width, int height);
void glfw_frambuffer_size_callback_square(GLFWwindow* window, int width, int height);

GLFWwindow* create_window(std::string title, GLint width = DEFAULT_WIDTH, GLint height = DEFAULT_HEIGHT);

// OpenGL functions
GLuint compile_shader(const GLchar* const* shader_source, GLenum type);
GLuint create_shader_program(std::initializer_list<GLuint> shaders, bool delete_shaders = true);

GLuint create_vao(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
GLuint create_vao(std::vector<GLfloat>& vertices);
GLuint create_vao(shape& shape);

// shape functions
namespace shapes {

constexpr shape make_triangle(GLfloat radius, GLfloat x = 0, GLfloat y = 0) {
    std::vector<GLfloat> vertices(6);

    GLfloat slice = 2 * std::numbers::pi / 3;

    vertices[0] = radius * std::cos(0 * slice) + x;
    vertices[1] = radius * std::sin(0 * slice) + y;
    vertices[2] = radius * std::cos(1 * slice) + x;
    vertices[3] = radius * std::sin(1 * slice) + y;
    vertices[4] = radius * std::cos(2 * slice) + x;
    vertices[5] = radius * std::sin(2 * slice) + y;

    return {vertices, {0, 1, 2}};
}

constexpr shape make_rectangle(GLfloat width, GLfloat height) {
    std::vector<GLfloat> vertices{
        0.0f,  0.0f,   // bottom left
        0.0f,  height, // top left
        width, height, // top right
        width, 0.0f    // bottom right
    };

    std::vector<GLuint> indices{
        0, 1, 3, 1, 2, 3
    };
    
    return {vertices, indices};
}

constexpr shape make_polygon(GLfloat radius, GLuint sides, GLfloat x = 0, GLfloat y = 0) {
    GLfloat angle = 0.0f;
    GLfloat slice = 2 * std::numbers::pi / sides;

    std::vector<GLfloat> vertices(sides * 2 + 2);
    std::vector<GLuint> indices(sides * 3);

    for (GLuint i = 0; i < sides; ++i, angle += slice) {
        vertices[2 * i]     = radius * std::cos(angle) + x;
        vertices[2 * i + 1] = radius * std::sin(angle) + y;

        indices[i * 3]     = i;
        indices[i * 3 + 1] = (i + 1) % sides;
        indices[i * 3 + 2] = sides;
    }

    *(vertices.rbegin() + 1) = x;
    *vertices.rbegin() = y;

    return {vertices, indices};
}

constexpr shape make_star(GLfloat radius, GLuint points, GLfloat x = 0, GLfloat y = 0) {
    GLfloat angle = 0.0f;
    GLfloat slice = 2 * std::numbers::pi / (points *= 2);

    std::vector<GLfloat> star_vertices(points * 2 + 2);
    std::vector<GLuint> star_indices(points * 3);

    for (GLuint i = 0; i < points; ++i, angle += slice) {
        star_vertices[2 * i]     = (i % 2 == 0 ? 1 : 0.5) * radius * std::cos(angle) + x;
        star_vertices[2 * i + 1] = (i % 2 == 0 ? 1 : 0.5) * radius * std::sin(angle) + y;

        star_indices[i * 3]     = i % points;
        star_indices[i * 3 + 1] = (i + 1) % points;
        star_indices[i * 3 + 2] = points * 2 + 1;
    }

    *(star_vertices.rbegin() + 1) = x;
    *star_vertices.rbegin() = y;
    
    return {star_vertices, star_indices};
}

constexpr std::vector<GLfloat> make_spiral(GLfloat radius, GLuint loops, GLfloat x = 0, GLfloat y = 0) {
    const GLuint SIDES = 64;
    
    GLfloat angle = 0.0f;
    GLfloat slice = 2 * std::numbers::pi / SIDES;
    GLfloat radius_step = radius / SIDES / loops;
    radius = 0;

    std::vector<GLfloat> vertices(SIDES * loops * 2);

    for (GLuint i = 0; i < loops; ++i) {
        for (GLuint j = 0; j < SIDES; ++j, angle += slice, radius += radius_step) {
            vertices[i * SIDES * 2 + j * 2]     = radius * std::cos(angle) + x;
            vertices[i * SIDES * 2 + j * 2 + 1] = radius * std::sin(angle) + y;
        }
    }

    return vertices;
}

constexpr void translate(shape& shape, GLfloat delta_x, GLfloat delta_y) {
    std::vector<GLfloat>& v = shape.vertices;
    for (auto it = v.begin(), it2 = v.begin() + 1; it != v.end(); it += 2, it2 += 2) {
        *it += delta_x;
        *it2 += delta_y;
    }
}

constexpr void rotate(shape& shape, GLfloat angle, GLfloat center_x, GLfloat center_y) {
    angle = angle * std::numbers::pi / 180.0f;
    GLfloat cos = std::cos(angle);
    GLfloat sin = std::sin(angle);

    // auto end = shape.vertices.begin() + shape.vertices.size() - 2;
    for (auto it = shape.vertices.begin(); it != shape.vertices.end(); it += 2) {
        GLfloat x = *it - center_x;
        GLfloat y = *(it + 1) - center_y;

        *it = x * cos - y * sin + center_x;
        *(it + 1) = y * cos + x * sin + center_y;
    }
}

constexpr void rotate(shape& shape, GLfloat angle) {
    return rotate(shape, angle, *(shape.vertices.rbegin() + 1), shape.vertices.back());
}

constexpr shape group(std::initializer_list<std::reference_wrapper<shape>> shapes) {
    shape result {shapes.begin()->get().vertices, shapes.begin()->get().indices};
    
    GLuint offset = shapes.begin()->get().vertices.size() / 2;
    for (auto it = shapes.begin() + 1; it != shapes.end(); ++it) {
        std::for_each(it->get().indices.begin(), it->get().indices.end(), [offset](GLuint& idx) {
            idx += offset;
        });
        offset += it->get().vertices.size() / 2;

        result.vertices.insert(result.vertices.end(), it->get().vertices.begin(), it->get().vertices.end());
        result.indices.insert(result.indices.end(), it->get().indices.begin(), it->get().indices.end());
    }

    return result;
}

}

// shader sources
namespace shader {

const GLchar* const basic_vertex = R"(
    #version 330 core

    layout (location = 0) in vec2 pos;

    void main() {
        gl_Position = vec4(pos.xy, 0.0f, 1.0f);
    }
)";

const GLchar* const basic_fragment = R"(
    #version 330 core
    
    out vec4 color;
    
    void main() {
        color = vec4(1.0f, 0.84f, 0.1f, 1.0f);
    }
)";

const GLchar* const basic_vertex_color = R"(
    #version 330 core

    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec3 color;

    out vec3 vertex_color;

    void main() {
        gl_Position = vec4(pos, 0.0f, 1.0f);
        vertex_color = color;
    }
)";

const GLchar* const basic_fragment_color = R"(
    #version 330 core
    
    in vec3 vertex_color;

    out vec4 color;
    
    void main() {
        color = vec4(vertex_color, 1.0f);
    }
)";

const GLchar* const basic_fragment_uniform = R"(
    #version 330 core

    out vec4 color;

    uniform vec3 uniform_color;
    
    void main() {
        color = vec4(uniform_color, 1.0f);
    }
)";

}

}