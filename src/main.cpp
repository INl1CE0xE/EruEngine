#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int height = 600;
const int width = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "EruEngine", NULL, NULL); //GLFW window
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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // glViewport func with resize

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}