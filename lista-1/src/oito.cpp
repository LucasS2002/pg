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

    GLFWwindow* window = glh::create_window("8");
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
        glh::compile_shader(&glh::shader::basic_vertex_color, GL_VERTEX_SHADER),
        glh::compile_shader(&glh::shader::basic_fragment_color, GL_FRAGMENT_SHADER)
    });
    glUseProgram(shader_program);

    // geometry
    std::vector<GLfloat> vertex_data{
         0.0f,  0.6f, 0.99f, 0.03f, 0.0f,
        -0.6f, -0.5f, 0.44f, 0.69f, 0.30f,
         0.6f, -0.3f, 0.36f, 0.62f, 0.82f,
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat), vertex_data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);

    glPointSize(16);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_POINTS, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}