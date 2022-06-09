#include "renderer.hpp"

#include <unordered_map>
#include <fstream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

Render::Render(BufferInfo info, GLuint p)
{
    glGenVertexArrays(1, &vao);    
    glBindVertexArray(vao);

    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, info.arrayBufferData.size() * sizeof(float), info.arrayBufferData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.elementBufferData.size() * sizeof(unsigned int), info.elementBufferData.data(), GL_STATIC_DRAW);

    for (BufferLayout layout : info.layouts)
    {
        glVertexAttribPointer(
            layout.location, layout.size,
            GL_FLOAT, GL_FALSE,
            layout.stride, (void*) layout.offset
        );
        glEnableVertexAttribArray(layout.location);
    }
    
    glBindVertexArray(0);

    
    program = p;
    transformLocation = glGetUniformLocation(program, "transform");
    colorLocation = glGetUniformLocation(program, "color");
    textureLocation = glGetUniformLocation(program, "textureSampler");
}

//===========================================
// RENDERER
//===========================================

void Renderer::AddRender(std::string type)
{
    BufferInfo data = ExtractDataFromFile(type);
    GLuint program = CreateShader(type);
    Render render = Render(data, program);

    renders.emplace_back(render);
}

void Renderer::Draw(glm::mat4 transform, glm::mat4 model)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (Render& render : renders)
    {
        glBindVertexArray(render.vao);
        glUseProgram(render.program);

        glm::mat4 renderTransform = transform * model;
        glUniformMatrix4fv(render.transformLocation, 1, GL_FALSE, glm::value_ptr(transform * model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glUseProgram(0);
        glBindVertexArray(0);
    }
}

BufferInfo Renderer::ExtractDataFromFile(std::string type)
{
    BufferInfo data;
    
    std::ifstream filestream {};
    filestream.open(type + ".data");

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
            while (filestream.peek() != '\n')
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
            
            filestream.clear();
            filestream.get();

            while (filestream >> arrayBufferDatum)
            {
                data.arrayBufferData.emplace_back(arrayBufferDatum);
            }
            filestream.clear();
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

    return data;
}

GLuint Renderer::CreateShader(std::string filename)
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

void Renderer::ReadShaderFromFile(std::string& shader, std::string file)
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