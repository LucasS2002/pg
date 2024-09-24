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

    GLFWwindow* window = glh::create_window("5");
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
    glh::shape triangles_middle;
    {
        glh::shape triangle1 = glh::shapes::make_triangle(0.3f, -0.5f);
        glh::shape triangle2 = glh::shapes::make_triangle(0.3f, 0.5f);
        glh::shapes::rotate(triangle2, 180.0f, 0.5f, 0.0f);
        triangles_middle = glh::shapes::group({triangle1, triangle2});
    }
    glh::shape triangles_top = triangles_middle;
    glh::shapes::translate(triangles_top, 0.0f, 0.6f);
    glh::shape triangles_bottom = triangles_middle;
    glh::shapes::translate(triangles_bottom, 0.0f, -0.6f);

    GLuint top    = glh::create_vao(triangles_top);
    GLuint middle = glh::create_vao(triangles_middle);
    GLuint bottom = glh::create_vao(triangles_bottom);

    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);
    glPointSize(8);
    glLineWidth(2);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(top);
        glDrawElements(GL_POINTS, triangles_top.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);

        glBindVertexArray(middle);
        glDrawElements(GL_TRIANGLES, triangles_middle.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(bottom);
        glDrawElements(GL_TRIANGLES, triangles_bottom.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &top);
    glDeleteVertexArrays(1, &middle);
    glDeleteVertexArrays(1, &bottom);
    glDeleteProgram(shader_program);

    glfwTerminate();

    return 0;
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}