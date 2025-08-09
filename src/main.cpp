#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/shaderUtils.cpp"
#include "const/shaderSources.cpp"
#include <cmath>

using namespace std;

// A set of coordinates for vertices of a triangle
GLfloat vertices[] =
{
    -0.5f,
    -0.5f * float(sqrt(3)) / 3,
    0.0f,
    0.5f,
    -0.5f * float(sqrt(3)) / 3,
    0.0f,
    0.0f,
    0.5f * float(sqrt(3)) * 2 / 3,
    0.0f,
};

GLuint indices[] =
{
    0, 1, 2
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void setupWindowHints() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* createWindow(uint16_t width, uint16_t height, const char* title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        exit(1);
    }

    return window;
}

int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    setupWindowHints();
    GLFWwindow *window = createWindow(800,600,"Noise generator");
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint shaderProgram = glCreateProgram();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    setupShader(shaderProgram, vertexShader, 1, vertexShaderSource);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    setupShader(shaderProgram, fragmentShader, 1, fragmentShaderSource);

    glLinkProgram(shaderProgram);

    // Delete now useless shaders, as they are bind to our program already
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // NOTE: VAO must be ALWAYS used *BEFORE* VBO in ALL commands
    GLuint VAO; // Vertex Array Object
    GLuint VBO; // Shader Buffer Object
    GLuint EBO; // Index Buffer Object

    // Generate buffers with only 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Make VAO the current vertex array object by binding it
    glBindVertexArray(VAO);
    // Bind the VBO specifying that it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Add vertices into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draws the triangle from EBO
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Swaps front and back buffers of the window
        glfwSwapBuffers(window);

        // Takes care of all GLFW events
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
