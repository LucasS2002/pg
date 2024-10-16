#include <glad/glad.h>

namespace shaders {

const GLchar* const vertex = R"(
    #version 330 core

    layout (location = 0) in vec2 pos;

    uniform mat4 projection;

    void main() {
        gl_Position = projection * vec4(pos.xy, 0.0f, 1.0f);
    }
)";

const GLchar* const fragment = R"(
    #version 330 core
    
    out vec4 color;
    
    void main() {
        color = vec4(1.0f, 0.84f, 0.1f, 1.0f);
    }
)";

}