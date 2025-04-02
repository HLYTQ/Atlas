#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

#include <cstdlib>

#include <fstream>
#include <iostream>
#include <sstream>

namespace Atlas {

Shader::Shader(const char* vertex, const char* fragment) {
    auto source     = ParserShader(vertex, fragment);
    m_shaderProgram = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
    glDeleteProgram(m_shaderProgram);
}

void Shader::Bind() const {
    glUseProgram(m_shaderProgram);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_uniformlocationCache.find(name) != m_uniformlocationCache.end()) {
        return m_uniformlocationCache[name];
    }
    int location{};
    location = glGetUniformLocation(m_shaderProgram, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    }
    m_uniformlocationCache[name] = location;
    return location;
}

void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1iv(const std::string& name, int count, int* value) {
    glUniform1iv(GetUniformLocation(name), count, value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {

    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4fv(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

Shader::ShaderProgramSource Shader::ParserShader(const char* vertex, const char* fragment) {
    std::ifstream       vShaderFile;
    std::ifstream       fShaderFile;
    ShaderProgramSource programSource;

    // 保证ifstream对象可以抛出异常：
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertex, std::ios::in);
        fShaderFile.open(fragment, std::ios::in);
        std::stringstream vss;
        std::stringstream fss;

        vss << vShaderFile.rdbuf();
        fss << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        programSource.vertexSource   = vss.str();
        programSource.fragmentSource = fss.str();
    } catch (std::ifstream::failure& e) {
        std::cout << e.what() << std::endl;
    }
    return programSource;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id{};
    id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length{};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* msg = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, msg);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
                  << std::endl;
        std::cout << msg << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    unsigned int shaderProgram{};
    shaderProgram = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(shaderProgram, fs);
    glAttachShader(shaderProgram, vs);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

} // namespace Atlas
