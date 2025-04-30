#include "OpenGLShader.hpp"

#include <fstream>
#include <glad/glad.h>

#include "OpenGLShader.hpp"
#include "atlaspch.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Atlas {

// TODO: Need to be simplify
OpenGLShader::OpenGLShader(const std::string& filepath) {
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);

    auto [vertexShader, fragmentShader] =
        CompileGLSLShaders(shaderSources[GL_VERTEX_SHADER], shaderSources[GL_FRAGMENT_SHADER]);

    CreateProgrammer(vertexShader, fragmentShader);

    // Always detach shaders after a successful link.
    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot   = filepath.rfind('.');
    auto count     = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name         = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_Name(name) {
    auto [vertexShader, fragmentShader] = CompileGLSLShaders(vertexSrc, fragmentSrc);
    CreateProgrammer(vertexShader, fragmentShader);
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const {
    glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, const int& value) {
    UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
    UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string& name, const float& value) {
    UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
    UploadUniformFloat2(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value) {
    UploadUniformMat3(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
    UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string& name, const int& value) {
    glUniform1i(GetUniformLocation(name), value);
}
void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
    glUniform1iv(GetUniformLocation(name), count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value) {
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    std::string result;
    std::ifstream readin(filepath, std::ios::in | std::ios::binary);
    if (readin) {
        readin.seekg(0, std::ios::end);
        size_t size = readin.tellg();
        if (size != -1) {
            result.resize(size);
            readin.seekg(0, std::ios::beg);
            readin.read(&result[0], size);
        } else {
            AT_CORE_ERROR("Could not read from file '{0}'", filepath);
        }
    } else {
        AT_CORE_ERROR("Could not open file '{0}'", filepath);
    }
    return result;
}
namespace Utils {

static GLenum ShaderTypeFromString(const std::string& type) {
    if (type == "vertex") {
        return GL_VERTEX_SHADER;
    }
    if (type == "fragment" || type == "pixel") {
        return GL_FRAGMENT_SHADER;
    }
    AT_CORE_ASSERT(false, "Unknown shader type!");
    return 0;
}

} // namespace Utils
std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken  = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos             = source.find(typeToken, 0); // Start of shader type declaration line
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
        AT_CORE_ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin     = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
        std::string type = source.substr(begin, eol - begin);
        AT_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos =
            source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
        AT_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

        shaderSources[Utils::ShaderTypeFromString(type)] =
            (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

std::tuple<GLint, GLint> OpenGLShader::CompileGLSLShaders(
    const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        AT_CORE_ERROR("{0}", infoLog.data());
        AT_ASSERT(false, "Vertex Shader compilation failure!");
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        AT_CORE_ERROR("{0}", infoLog.data());
        AT_ASSERT(false, "Fragment Shader compilation failure!");
    }
    return {vertexShader, fragmentShader};
}

void OpenGLShader::CreateProgrammer(GLint vertexShader, GLint fragmentShader) {
    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_RendererID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    // Link our program
    glLinkProgram(m_RendererID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*) &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(m_RendererID);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        AT_CORE_ERROR("{0}", infoLog.data());
        AT_ASSERT(false, "Shader link failure!");
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);
}

int OpenGLShader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocation.find(name) != m_UniformLocation.end()) {
        return m_UniformLocation[name];
    }
    int location            = glGetUniformLocation(m_RendererID, name.c_str());
    m_UniformLocation[name] = location;
    return location;
}

} // namespace Atlas
