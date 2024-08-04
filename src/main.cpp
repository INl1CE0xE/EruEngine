#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer/external/stb_image.h" // external lib

#include <iostream>
#include "Renderer/shader.h"

GLfloat vertices[]{
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f
};
GLint indices[]{
     0, 1, 3, // ������ �����������
     1, 2, 3  // ������ �����������
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

    //texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("F:/EruEngine/src/Resource/wooden_container.jpg", &width, &height, &nrChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ��������� ������ ��������� �������� GL_REPEAT (����������� ����� ���������)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // ��������� ���������� ���������� ��������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    //end texture

        //��� ������������ ��� VBO ��� ����� � ��� �����, ����� �� ����� ����������� ���� �� ������������
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    glGenVertexArrays(1, &VAO); // ��������� ������� ��� �������
    glGenBuffers(1, &VBO); // ��������� ������ �����
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); // ���������� ������� ��������

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // �������� ������ ����� � ������� ������� �����
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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ��� ������� ���, ����� ����������������� 
    while (!glfwWindowShouldClose(window)) { //Rendering
        processInput(window); // Key Input

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();
        timevalue = glfwGetTime();
        //shader.setfloat("xset", timevalue); // ����� ������� ������ �� ��� x 
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}