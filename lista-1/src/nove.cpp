#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glhelper/glhelper.hpp>

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

constexpr glh::shape make_hat();
constexpr glh::shape make_head();
constexpr glh::shape make_face();

constexpr GLfloat PIXEL = 0.08f;

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
        glh::compile_shader(&glh::shader::basic_fragment_uniform, GL_FRAGMENT_SHADER)
    });
    glUseProgram(shader_program);

    // geometry
    glh::shape hat = make_hat();
    GLuint hat_vao = glh::create_vao(hat);

    glh::shape head = make_head();
    GLuint head_vao = glh::create_vao(head);

    glh::shape face = make_face();
    GLuint face_vao = glh::create_vao(face);

    GLuint uniform_location = glGetUniformLocation(shader_program, "uniform_color");

    glClearColor(0.69f, 0.69f, 0.69f, 1.0f);
    glLineWidth(2);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);


        glUniform3f(uniform_location, 0.94f, 0.23f, 0.22f);
        glBindVertexArray(hat_vao);
        glDrawElements(GL_TRIANGLES, hat.indices.size(), GL_UNSIGNED_INT, 0);

        glUniform3f(uniform_location, 1.0f, 0.8f, 0.4f);
        glBindVertexArray(head_vao);
        glDrawElements(GL_TRIANGLES, head.indices.size(), GL_UNSIGNED_INT, 0);

        glUniform3f(uniform_location, 0.6f, 0.41f, 0.16f);
        glBindVertexArray(face_vao);
        glDrawElements(GL_TRIANGLES, face.indices.size(), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up

    glfwTerminate();

    return 0;
}

constexpr glh::shape make_head() {
    glh::shape one = glh::shapes::make_rectangle(4 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(one, -1 * PIXEL, 1 * PIXEL);

    glh::shape two = glh::shapes::make_rectangle(9 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(two, -4 * PIXEL, 0.0f);

    glh::shape three = glh::shapes::make_rectangle(10 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(three, -4 * PIXEL, -1 * PIXEL);

    glh::shape four = glh::shapes::make_rectangle(4 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(four, -3 * PIXEL, -2 * PIXEL);

    glh::shape five = glh::shapes::make_rectangle(7 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(five, -3 * PIXEL, -3 * PIXEL);

    return glh::shapes::group({one, two, three, four, five});
}

constexpr glh::shape make_hat() {
    glh::shape top = glh::shapes::make_rectangle(5 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(top, -3 * PIXEL, 3 * PIXEL);

    glh::shape bottom = glh::shapes::make_rectangle(9 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(bottom, -4 * PIXEL, 2 * PIXEL);

    return glh::shapes::group({top, bottom});
}

constexpr glh::shape make_face() {
    glh::shape eye = glh::shapes::make_rectangle(1 * PIXEL, 2 * PIXEL);
    glh::shapes::translate(eye, 1 * PIXEL, 0.0f);

    glh::shape mustache1 = glh::shapes::make_rectangle(1 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(mustache1, 2 * PIXEL, -1 * PIXEL);

    glh::shape mustache2 = glh::shapes::make_rectangle(4 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(mustache2, 1 * PIXEL, -2 * PIXEL);

    glh::shape hair1 = glh::shapes::make_rectangle(3 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(hair1, -4 * PIXEL, 1 * PIXEL);

    glh::shape hair2 = glh::shapes::make_rectangle(1 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(hair2, -3 * PIXEL, 0.0f);

    glh::shape hair3 = glh::shapes::make_rectangle(2 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(hair3, -3 * PIXEL, -1 * PIXEL);

    glh::shape hair4 = glh::shapes::make_rectangle(1 * PIXEL, 3 * PIXEL);
    glh::shapes::translate(hair4, -5 * PIXEL, -2 * PIXEL);

    glh::shape hair5 = glh::shapes::make_rectangle(1 * PIXEL, 1 * PIXEL);
    glh::shapes::translate(hair5, -4 * PIXEL, -2 * PIXEL);

    return glh::shapes::group({eye, mustache1, mustache2, hair1, hair2, hair3, hair4, hair5});
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}