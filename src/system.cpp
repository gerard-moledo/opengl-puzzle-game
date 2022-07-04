#include "system.hpp"

#include <fstream>

#include "stb_image.h"

namespace System
{
    int width = 800;
    int height = 450;
    bool initialized = false;
    
    GLFWwindow* window = nullptr;
    std::unordered_map<std::string, GLuint> programMap;

    Render renderFloor;
    Render renderCube;
    Render renderText;
    
    std::vector<LevelInfo> levels;
    
    void Initialize()
    {
        glfwInit();

        window = glfwCreateWindow(width, height, "Puzzle", NULL, NULL);

        glfwSetWindowPos(window, 640, 320);

        glfwMakeContextCurrent(window);

        gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);

        programMap.emplace(std::string("texture"), CreateShader("texture"));
        programMap.emplace(std::string("cube"), CreateShader("cube"));

        renderFloor = Render(ExtractBufferDataFromFile("floor"), programMap["texture"]);
        renderCube = Render(ExtractBufferDataFromFile("cube"), programMap["cube"]);
        renderText = Render(ExtractBufferDataFromFile("text"), programMap["texture"]);

        GenerateFloorTexture(renderFloor);
        GenerateGlyphTextures(renderText);
        
        ExtractLevelDataFromFile(levels);
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

    void GenerateFloorTexture(Render& render)
    {
        std::string textureName = "Tile.png";
        
        int width, height, channelCount;
        unsigned char* data = stbi_load(("Assets/" + textureName).c_str(), &width, &height, &channelCount, 0);
        if (data)
        {
            glGenTextures(1, &render.texture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, render.texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }
    
    void GenerateGlyphTextures(Render& render)
    {
        std::string textureName = "Font.bmp";

        int width, height, channelCount;
        unsigned char* data = stbi_load(("Assets/" + textureName).c_str(), &width, &height, &channelCount, 0);
        if (data)
        {
            render.subTextures.resize(64);
            glGenTextures(64, render.subTextures.data());
            glActiveTexture(GL_TEXTURE0);
            
            for (int cell = 0; cell < 64; cell++)
            {
                int xOffset = (cell % 8) * 32;
                int yOffset = (cell / 8) * 32;

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                
                glBindTexture(GL_TEXTURE_2D, render.subTextures[cell]);
                glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, 32, 32, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        stbi_image_free(data);
    }
    
    BufferInfo ExtractBufferDataFromFile(std::string type)
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

    void ExtractLevelDataFromFile(std::vector<LevelInfo>& levels)
    {
        std::ifstream filestream {};
        filestream.open("Assets/levels.data");

        int level;
        int row, rows, column, columns;
        char cell;
        
        while (filestream.peek() != EOF)
        {
            filestream >> level;

            LevelInfo info;
            
            row = 0, rows = 0;
            column = 0, columns = 0;
            while (filestream >> cell)
            {
                if (cell == '-') break;

                Vector2i location { column, row};

                switch (cell)
                {
                    case '@':
                        info.playerStart = location;
                        break;
                    case '#':
                        info.blockData.emplace_back(std::pair<BlockType, Vector2i>(BlockType::wall, location));
                        break;
                    case 'B':
                        info.blockData.emplace_back(std::pair<BlockType, Vector2i>(BlockType::block, location));
                        break;
                    case 'o':
                        info.blockData.emplace_back(std::pair<BlockType, Vector2i>(BlockType::goal, location));
                        break;
                }

                column++;

                filestream.get();
                
                if (filestream.peek() == '\n' || filestream.peek() == '\r')
                {
                    row++;
                    if (column > columns) columns = column;
                    column = 0;
                }
            }
            rows = row;
            
            info.size = Vector2i { columns, rows };
            info.playerStart = LevelToWorld(info.playerStart, info.size);
            for (auto& data : info.blockData)
            {
                data.second = LevelToWorld(data.second, info.size);
            }

            levels.emplace_back(info);
            
            filestream.ignore(50, '\n');
        }
    }
    
    void Quit()
    {
        glfwDestroyWindow(window);

        glfwTerminate();
    }
}