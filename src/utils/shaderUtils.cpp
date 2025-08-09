#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void setupShader(GLuint shaderProgram, GLuint shader, uint8_t p, const char* shaderSource) {
    glShaderSource(shader, p, &shaderSource, NULL);
    glCompileShader(shader);
    glAttachShader(shaderProgram, shader);
}
