#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

const char* vertex_shader =
"#version 460 core\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460 core\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0);"
"}";

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

    int success; // лог для проверки шейдеров на компиляцию
    char infolog[512];

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Вертекс шейдер
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "Error::Shader:Vertex:compilation_failed\n" << infolog<<std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Фрагментный шейдер
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        std::cout << "Error::Shader:Fragment:compilation_failed\n" << infolog << std::endl;
    }

    GLuint shaderProgram = glCreateProgram(); // Линкование шейдеров в одну шейдерную программу
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
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

    while (!glfwWindowShouldClose(window)) { //Rendering
        processInput(window); // Key Input

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
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