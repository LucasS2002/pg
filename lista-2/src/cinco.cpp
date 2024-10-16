#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glhelper/glhelper.hpp>

#include <shaders.hpp>

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

    // shader program
    GLuint shader_program = glh::create_shader_program({
        glh::compile_shader(&shaders::vertex, GL_VERTEX_SHADER),
        glh::compile_shader(&shaders::fragment, GL_FRAGMENT_SHADER)
    });
    glUseProgram(shader_program);

    // geometry
    glh::shape triangle1 = glh::shapes::make_triangle(0.25f, -0.5, -0.5);
    glh::shape triangle2 = glh::shapes::make_triangle(0.25f, -0.5,  0.5);
    glh::shape triangle3 = glh::shapes::make_triangle(0.25f,  0.5,  0.5);
    glh::shape triangle4 = glh::shapes::make_triangle(0.25f,  0.5, -0.5);

    GLuint vao1 = glh::create_vao(triangle1);
    GLuint vao2 = glh::create_vao(triangle2);
    GLuint vao3 = glh::create_vao(triangle3);
    GLuint vao4 = glh::create_vao(triangle4);

    // projection
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao1);
        glDrawElements(GL_TRIANGLES, triangle1.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);

        glBindVertexArray(vao2);
        glDrawElements(GL_TRIANGLES, triangle2.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);

        glBindVertexArray(vao3);
        glDrawElements(GL_TRIANGLES, triangle3.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);

        glBindVertexArray(vao4);
        glDrawElements(GL_TRIANGLES, triangle4.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}