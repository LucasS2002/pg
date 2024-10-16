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

    GLFWwindow* window = glh::create_window("4");
    glfwMakeContextCurrent(window);

    // GLAD init
    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // viewport
    glViewport(glh::DEFAULT_WIDTH / 2, glh::DEFAULT_HEIGHT / 2, glh::DEFAULT_WIDTH / 2, glh::DEFAULT_HEIGHT / 2);

    // shader program
    GLuint shader_program = glh::create_shader_program({
        glh::compile_shader(&shaders::vertex, GL_VERTEX_SHADER),
        glh::compile_shader(&shaders::fragment, GL_FRAGMENT_SHADER)
    });
    glUseProgram(shader_program);

    // geometry
    glh::shape triangle = glh::shapes::make_triangle(0.5f);
    GLuint vao = glh::create_vao(triangle);

    // projection
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, triangle.indices.size(), GL_UNSIGNED_INT, (GLvoid*) 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}