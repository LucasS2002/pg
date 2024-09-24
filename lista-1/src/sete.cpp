#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glhelper/glhelper.hpp>

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    // GLFW init
    glfwSetErrorCallback(glh::glfw_error_callback);
    glfwInit();

    GLFWwindow* window = glh::create_window("7");
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
    std::vector<GLfloat> spiral = glh::shapes::make_spiral(0.75f, 3);
    GLuint vao = glh::create_vao(spiral);
    glBindVertexArray(vao);

    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);
    glLineWidth(2);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_LINE_STRIP, 0, spiral.size() / 2);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}