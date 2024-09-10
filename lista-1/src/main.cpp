#include <cstddef>
#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height);
void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode);

GLuint setup_program();
GLuint setup_geometry();

const uint16_t SCREEN_WIDTH = 1280;
const uint16_t SCREEN_HEIGHT = 720;

const GLchar* vertex_shader_source = 
    "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const GLchar* fragment_shader_source = 
    "#version 460 core\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main() {
    // glfw initialization
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // glfw window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW Window\n";
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    
    // glad initialization
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // shader setup
    GLuint shader_program = setup_program();
    glUseProgram(shader_program);

    // geometry setup
    GLuint VAO = setup_geometry();
    glBindVertexArray(VAO);
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
    }

    // clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shader_program);
    
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, GLint key, GLint scancode, GLint action, GLint mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

GLuint setup_program() {
    // error checking variables
    GLint success;
    GLchar infoLog[512];

    // vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    if (vertex_shader == 0) {
        std::cout << "Failed to create vertex shader\n";
        return -1;
    }

    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Vertex shader compilation failed\n" << infoLog << '\n';
        return -1;
    }

    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragment_shader == 0) {
        std::cout << "Failed to create fragment shader\n";
        return -1;
    }

    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Fragment shader compilation failed\n" << infoLog << '\n';
        return -1;
    }

    // shader linking
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, sizeof(infoLog), NULL, infoLog);
        std::cout << "Program linking failed\n" << infoLog << '\n';
        return -1;
    }

    // shader clean up
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

GLuint setup_geometry() {
    // vertex data
    const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glDeleteBuffers(1, &VBO);

    return VAO;
}