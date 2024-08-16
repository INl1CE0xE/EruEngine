#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer/external/stb_image.h" // external lib

#include "external/glm/glm.hpp" //glm GL Math
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

#include <iostream>
#include "Renderer/shader.h"
#include "Renderer/texture.h"

GLfloat vertices[]{
    // точки               цвета              позиция текстуры
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f
};
GLint indices[]{
     0, 1, 3, // первый треугольник
     1, 2, 3  // второй треугольник
};

//GLfloat colors[]{
//     1.0f, 0.0f, 0.0f,
//     0.0f, 1.0f, 0.0f,
//     0.0f, 0.0f, 1.0f
//};

const int windowSizeY = 600;
const int windowSizeX = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //func for resize window
void processInput(GLFWwindow* window);

float mixValue = 0.0f;
float obj_pos = 0.0f;

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

    Texture 
        texture1("F:/EruEngine/src/Resource/wooden_container.jpg", GL_REPEAT, GL_LINEAR, GL_RGB), 
        texture2("F:/EruEngine/src/Resource/awesomeface.png", GL_REPEAT, GL_LINEAR, GL_RGBA);
 
        //Тут используются два VBO для точек и для цвета, чтобы от точек переливался цвет на треугольнике
    GLuint VAO = 0; // Vertex Array Object
    GLuint VBO = 0; // Vertex Buffer Object
    GLuint EBO = 0; // Elements Buffer Object

    glGenVertexArrays(1, &VAO); // Генерация массива для буферов
    glGenBuffers(1, &VBO); // Генерация буфера точек
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // Связывание массива объектов

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязка буфера точек к функции массива точек
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr); // 0
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // glViewport func with resize
    Shader shader("F:/EruEngine/src/Renderer/vshader.vs","F:/EruEngine/src/Renderer/fshader.fs");
    float timevalue;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // это полигон мод, нужно раскомментировать 
   
    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "texture2"), 1);

    glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::translate(trans, glm::vec3(0.3f, 0.3f, 0.0f)); // Это для перестановки (перемещения)
    //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 0.0f)); // Это для поворота вокруг заданной оси
    trans = glm::scale(trans, glm::vec3(0.4, 0.4, 1.0)); // Это уменьшает или увеличивает

    while (!glfwWindowShouldClose(window)) { //Rendering
        processInput(window); // Key Input

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1.texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.texture);

        //glUniform1f(glGetUniformLocation(shader.ID, "mixValue"), mixValue);
        shader.setfloat("mixValue", mixValue); // для рисования 2 текстуры
        //shader.setfloat("xset", obj_pos);
        trans = glm::rotate(trans, obj_pos, glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(mixValue, 0.0f, 0.0f));
        shader.setmat4("transform", trans);
        
        shader.use();
        timevalue = glfwGetTime();
        //shader.setfloat("xset", timevalue); // Чтобы смещать фигуру по оси x 
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue += 0.001f;
        if (mixValue > 1.0f) mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue -= 0.001f;
        if (mixValue < -1.0f) mixValue = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        obj_pos += 0.001f;
        if (obj_pos > 1.0f) obj_pos = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        obj_pos -= 0.001f;
        if (obj_pos < -1.0f) obj_pos = -1.0f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}