#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <ios>
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glhelper/glhelper.hpp>
#include <vector>

namespace glh {

// terminates the program
void terminate() {
    glfwTerminate();
    std::abort();
}

// GLFW functions
void glfw_error_callback(int code, const char* desc) {
    std::cerr << "GLFW encountered an error: 0x" << std::hex << code << std::endl;
    std::cerr << desc << std::endl;

    terminate();
}

void glfw_frambuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_frambuffer_size_callback_square(GLFWwindow* window, int width, int height) {
    int min = std::min(width, height);
    glViewport((min == height) * (width - height) / 2, (min == width) * (height - width) / 2, min, min);
}

GLFWwindow* create_window(std::string title, GLint width, GLint height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return glfwCreateWindow(width, height, title.data(), NULL, NULL); 
}

// OpenGL error checking
GLint _status;

// OpenGL functions
GLuint compile_shader(const GLchar* const* shader_source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, shader_source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &_status);
    if (_status == GL_FALSE) {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, ' ');
        glGetShaderInfoLog(shader, log_length, NULL, log.data());

        std::cerr << "Shader compilation failed.\n" << log << std::endl;
        std::cerr << "Shader:\n" << *shader_source << std::endl;
        terminate();
    }

    return shader;
}

GLuint create_shader_program(std::initializer_list<GLuint> shaders, bool delete_shaders) {    
    GLuint program = glCreateProgram();
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &_status);
    if (_status == GL_FALSE) {
        GLint log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, ' ');
        glGetProgramInfoLog(program, log_length, NULL, log.data());

        std::cerr << "Program linking failed.\n" << log << std::endl;
        terminate();
    }

    if (delete_shaders) {
        for (GLuint shader : shaders) {
            glDeleteShader(shader);
        }
    }

    return program;
}

GLuint create_vao(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    if (!indices.empty()) {
        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), indices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (!indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    return vao;
}

GLuint create_vao(std::vector<GLfloat>& vertices) {
    std::vector<GLuint> dummy{};
    return create_vao(vertices, dummy);
}

GLuint create_vao(shape& shape) {
    return create_vao(shape.vertices, shape.indices);
}

}