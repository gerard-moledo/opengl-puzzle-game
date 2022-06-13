#include "system.hpp"

#include <fstream>

namespace System
{
    GLFWwindow* window = nullptr;
    std::unordered_map<std::string, GLuint> programMap;
    
    void Initialize()
    {
        glfwInit();

        window = glfwCreateWindow(1280, 720, "Puzzle", NULL, NULL);

        glfwSetWindowPos(window, 640, 320);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        programMap["floor"] = CreateShader("floor");
        programMap["cube"] = CreateShader("cube");
    }

    GLuint CreateShader(std::string filename)
    {
        std::string vertexSource {};
        std::string fragmentSource {};

        ReadShaderFromFile(vertexSource, filename + std::string(".vs"));
        ReadShaderFromFile(fragmentSource, filename + std::string(".fs"));

        const char* vertexString = vertexSource.c_str();
        const char* fragmentString = fragmentSource.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexString, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentString, NULL);
        glCompileShader(fragmentShader);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLchar info[200];
        glGetProgramInfoLog(program, 200, nullptr, info);
        printf("%s", info);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    void ReadShaderFromFile(std::string& shader, std::string file)
    {
        shader.clear();

        std::ifstream filestream {};
        filestream.open(file);

        std::string line {};
        while (std::getline(filestream, line))
        {
            shader.append(line + "\n");
        }

        filestream.close();
    }

    void Quit()
    {
        glfwDestroyWindow(window);

        glfwTerminate();
    }
}