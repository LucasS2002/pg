#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glhelper/glhelper.hpp>

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLuint shape_count, idx{0};
bool wireframe = false;

constexpr void remove_sides(glh::shape& shape, GLuint count) {
    if (3 * count > shape.indices.size()) return;

    GLuint keep = shape.indices.size() / 3 - count;

    shape.vertices.erase(shape.vertices.begin() + 2 * keep, shape.vertices.begin() + 2 * (keep + count));
    shape.indices.erase(shape.indices.begin() + 3 * keep, shape.indices.end());
    std::for_each(shape.indices.begin(), shape.indices.end(), [keep](GLuint& i) {
        i = std::min(i, keep);
    });
}

int main() {
    // GLFW init
    glfwSetErrorCallback(glh::glfw_error_callback);
    glfwInit();

    GLFWwindow* window = glh::create_window("6 - Use arrow keys to cycle shapes");
    glfwMakeContextCurrent(window);

    // GLAD init
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // viewport
    glh::glfw_frambuffer_size_callback_square(window, glh::DEFAULT_WIDTH, glh::DEFAULT_HEIGHT);

    // callbacks
    glfwSetFramebufferSizeCallback(window, glh::glfw_frambuffer_size_callback_square);
    glfwSetKeyCallback(window, glfw_key_callback);

    // shader program
    GLuint shader_program = glh::create_shader_program({
        glh::compile_shader(&glh::shader::basic_vertex, GL_VERTEX_SHADER),
        glh::compile_shader(&glh::shader::basic_fragment, GL_FRAGMENT_SHADER)
    });
    glUseProgram(shader_program);

    // geometry
    glh::shape circle   = glh::shapes::make_polygon(0.5f, 60);
    glh::shape octagon  = glh::shapes::make_polygon(0.5f, 8);
    glh::shape pentagon = glh::shapes::make_polygon(0.5f, 5);
    glh::shape pacman   = glh::shapes::make_polygon(0.5f, 60);
    remove_sides(pacman, 10);
    glh::shape pizza    = glh::shapes::make_polygon(1.0f, 60, -0.5f, -0.25f);
    remove_sides(pizza, 50);
    glh::shape star     = glh::shapes::make_star(0.5f, 5);

    std::vector<std::pair<GLuint, glh::shape&>> shapes{
        {glh::create_vao(circle), circle},
        {glh::create_vao(octagon), octagon},
        {glh::create_vao(pentagon), pentagon},
        {glh::create_vao(pacman), pacman},
        {glh::create_vao(pizza), pizza},
        {glh::create_vao(star), star}
    };
    shape_count = shapes.size();

    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(shapes[idx].first);
        glDrawElements(GL_TRIANGLES, shapes[idx].second.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteProgram(shader_program);
    std::for_each(shapes.begin(), shapes.end(), [](const auto& s) {
        glDeleteVertexArrays(1, &s.first);
    });

    glfwTerminate();

    return 0;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        wireframe = !wireframe;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        idx = (idx + 1) % shape_count;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        idx = idx == 0 ? shape_count - 1 : idx - 1;
    }
}