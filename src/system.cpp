#include "system.hpp"

#include <fstream>

namespace System
{
    int width = 800;
    int height = 450;
    bool initialized = false;
    
    GLFWwindow* window = nullptr;
    std::unordered_map<std::string, GLuint> programMap;

    Render renderFloor;
    Render renderCube;
    
    void Initialize()
    {
        glfwInit();

        window = glfwCreateWindow(width, height, "Puzzle", NULL, NULL);

        glfwSetWindowPos(window, 640, 320);

        glfwMakeContextCurrent(window);

        gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

        programMap.emplace(std::string("floor"), CreateShader("floor"));
        programMap.emplace(std::string("cube"), CreateShader("cube"));

        renderFloor = Render(ExtractDataFromFile("floor"), programMap["floor"]);
        renderCube = Render(ExtractDataFromFile("cube"), programMap["cube"]);
    }

    GLuint CreateShader(std::string filename)
    {
        std::string vertexSource {};
        std::string fragmentSource {};

        ReadShaderFromFile(vertexSource, "Assets/" + filename + ".vs");
        ReadShaderFromFile(fragmentSource, "Assets/" + filename + ".fs");

        const GLchar* vertexString = vertexSource.c_str();
        const GLchar* fragmentString = fragmentSource.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexString, NULL);
        glCompileShader(vertexShader);
        GLint obj;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &obj);
        GLchar vertInfo[1000];
        glGetShaderInfoLog(vertexShader, 1000, nullptr, vertInfo);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentString, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &obj);
        GLchar fragInfo[1000];
        glGetShaderInfoLog(fragmentShader, 1000, nullptr, fragInfo);
        
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        
        GLchar info[2200];
        glGetProgramInfoLog(program, 2200, nullptr, info);

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

    BufferInfo ExtractDataFromFile(std::string type)
    {
        BufferInfo data;

        std::fstream filestream {};
        filestream.open("Assets/" + type + ".data");
        
        std::string bufferType;
        std::vector<int> layoutSizes;
        int size;
        float arrayBufferDatum;
        unsigned int elementBufferDatum;
        
        while (filestream.peek() != EOF)
         {
             filestream >> bufferType;

             if (bufferType[0] == 'A')
             {
                 int totalSize = 0;
                 while (filestream.peek() != '\r' && filestream.peek() != '\n')
                {
                    filestream >> size;
                    layoutSizes.emplace_back(size);
                    totalSize += size;
                 }

                int offset = 0;
                for (int sizeIndex = 0; sizeIndex < layoutSizes.size(); sizeIndex++)
                {
                    BufferLayout layout;
                    layout.location = sizeIndex;
                    layout.size = layoutSizes[sizeIndex];
                    layout.stride = sizeof(float) * totalSize;
                    layout.offset = sizeof(float) * offset;
                    data.layouts.emplace_back(layout);

                    offset += layout.size;
                }
                filestream.get();

                while (filestream >> arrayBufferDatum)
                {
                    data.arrayBufferData.emplace_back(arrayBufferDatum);
                }
                filestream.clear();
                filestream.putback('E');
             }
            
             if (bufferType[0] == 'E')
             {
                 while (filestream >> elementBufferDatum)
                 {
                     data.elementBufferData.emplace_back(elementBufferDatum);
                 }
                 filestream.clear();
             }
        }

        filestream.close();
        
        return data;
    }
    
    void Quit()
    {
        glfwDestroyWindow(window);

        glfwTerminate();
    }
}