#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Atlas {
class Shader {
public:
    Shader(const char* vertex, const char* fragment);
    ~Shader();
    void Bind() const;
    void Unbind() const;

    int  GetUniformLocation(const std::string& name);
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform1iv(const std::string& name, int count, int* value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4fv(const std::string& name, const glm::mat4& matrix);

protected:
    struct ShaderProgramSource {
        std::string vertexSource;
        std::string fragmentSource;
    };
    ShaderProgramSource ParserShader(const char* vertex, const char* fragment);

    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShaderSource, const ::std::string& fragmentShaderSource);

private:
    unsigned int                         m_shaderProgram;
    std::unordered_map<std::string, int> m_uniformlocationCache;
};
} // namespace Atlas
