#pragma once

#include "Atlas/Renderer/Shader.hpp"
#include <unordered_map>
#include <tuple>
#include <vector>

using GLenum = unsigned int;
using GLint = int;

namespace Atlas {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
    virtual ~OpenGLShader();

    void Bind() const override;
    void Unbind() const override;

    const std::string& GetName() const override {
        return m_Name;
    }

    void SetInt(const std::string& name, const int& value) override;
    void SetIntArray(const std::string& name, int* values, uint32_t count) override;
    void SetFloat(const std::string& name, const float& value) override;
    void SetFloat2(const std::string& name, const glm::vec2& value) override;
    void SetFloat3(const std::string& name, const glm::vec3& value) override;
    void SetFloat4(const std::string& name, const glm::vec4& value) override;
    void SetMat3(const std::string& name, const glm::mat3& value) override;
    void SetMat4(const std::string& name, const glm::mat4& value) override;

    void UploadUniformInt(const std::string& name, const int& value);
    void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
    void UploadUniformFloat(const std::string& name, const float& value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    std::tuple<GLint, GLint> CompileGLSLShaders(const std::string& vertexSource, const std::string& fragmentSource);
    void CreateProgrammer(GLint vertexShader, GLint fragmentShader);
    int GetUniformLocation(const std::string& name);
private:
    unsigned int m_RendererID;
    std::string m_Name;

    std::unordered_map<std::string, unsigned int> m_UniformLocation;
};

} // namespace Atlas
