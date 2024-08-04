#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Renderer/shader.h"

GLfloat point[]{
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[]{
     1.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 1.0f
};

const int windowSizeY = 600;
const int windowSizeX = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //func for resize window
void processInput(GLFWwindow* window);

int main()
{
    if (!glfwInit()) {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowSizeX, windowSizeY, "EruEngine", nullptr, nullptr); //GLFW window
    if (window == NULL) {
        std::cout << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //GLAD initialization 
        std::cout << "Failed to initialize GLAD";
        return -1;
    }
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

        //Тут используются два VBO для точек и для цвета, чтобы от точек переливался цвет на треугольнике
    GLuint VBO_points = 0;
    glGenBuffers(1, &VBO_points); // Генерация буфера точек
    glBindBuffer(GL_ARRAY_BUFFER, VBO_points); // Привязка буфера точек к функции массива точек
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    
    GLuint VBO_colors = 0;
    glGenBuffers(1, &VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO); // Генерация массива для буферов
    glBindVertexArray(VAO); // Работа с VAO
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

   
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // glViewport func with resize
    Shader shader("F:/EruEngine/src/Renderer/vshader.vs","F:/EruEngine/src/Renderer/fshader.fs");
    float timevalue;
    while (!glfwWindowShouldClose(window)) { //Rendering
        processInput(window); // Key Input

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        timevalue = glfwGetTime();
        shader.setfloat("xset", timevalue); // Чтобы смещать фигуру по оси x 
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}