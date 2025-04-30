#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Atlas/Log.hpp"
#include "Atlas/Renderer/Shader.hpp"
#include "Atlas/Renderer/Texture.hpp"
#include <glm/glm.hpp>

namespace Atlas {

    enum class MaterialFlag : uint32_t {
        None      = Bit(0),
        DepthTest = Bit(1),
        Blend     = Bit(2),
        TwoSided  = Bit(3),
        Lines     = Bit(4),
    };

    class Material {
        enum class UniformType : int32_t { TEX2D = -2, TEXCUBE, INT, FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4 };
        struct UniformStorage {
            UniformStorage() = default;
            const UniformStorage& operator=(const UniformStorage& other) {
                Type  = other.Type;
                value = other.value;
                return (*this);
            }
            ~UniformStorage() {}

            UniformType Type;
            union {
                int i;
                float f;
                glm::vec2 v2;
                glm::vec3 v3;
                glm::vec4 v4;
                std::byte* data;
            } value;
        };


    public:
        Material(Ref<Shader>& shader, const std::string& name = "") : m_Shader(shader), m_Name(name) {
            SetFlag(MaterialFlag::Blend);
            SetFlag(MaterialFlag::DepthTest);
        }
        ~Material() {
            for (auto& uniform : m_UniformStorage) {
                if (uniform.second.Type >= UniformType::MAT3) {
                    delete uniform.second.value.data;
                }
            }
        }

        void Set(const std::string& name, int value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::INT;
                tmp.value.i            = value;
                m_UniformStorage[name] = tmp;
            } else {
                m_UniformStorage[name].value.i = value;
            }
        }
        void Set(const std::string& name, float value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::FLOAT;
                tmp.value.f            = value;
                m_UniformStorage[name] = tmp;
            } else {
                m_UniformStorage[name].value.f = value;
            }
        }
        void Set(const std::string& name, const glm::vec2& value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::VEC2;
                tmp.value.v2           = value;
                m_UniformStorage[name] = tmp;
            } else {
                m_UniformStorage[name].value.v2 = value;
            }
        }
        void Set(const std::string& name, const glm::vec3& value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::VEC3;
                tmp.value.v3           = value;
                m_UniformStorage[name] = tmp;
            } else {
                m_UniformStorage[name].value.v3 = value;
            }
        }
        void Set(const std::string& name, const glm::vec4& value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::VEC4;
                tmp.value.v4           = value;
                m_UniformStorage[name] = tmp;
            } else {
                m_UniformStorage[name].value.v4 = value;
            }
        }
        void Set(const std::string& name, const glm::mat3& value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::MAT3;
                tmp.value.data         = (std::byte*) new glm::mat3(value);
                m_UniformStorage[name] = tmp;
            } else {
                *(glm::mat3*) m_UniformStorage[name].value.data = value;
            }
        }
        void Set(const std::string& name, const glm::mat4& value) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                tmp.Type               = UniformType::MAT4;
                tmp.value.data         = (std::byte*) new glm::mat4(value);
                m_UniformStorage[name] = tmp;
            } else {
                *(glm::mat4*) m_UniformStorage[name].value.data = value;
            }
        }
        void Set(const std::string& name, const Ref<Texture2D>& texture2D) {
            if (m_UniformStorage.find(name) == m_UniformStorage.end()) {
                UniformStorage tmp;
                uint32_t slot          = m_TexSlotAllocator.GetSlot();
                tmp.Type               = UniformType::TEX2D;
                tmp.value.i            = (int) slot;
                m_Texture2Ds[slot]     = texture2D;
                m_UniformStorage[name] = tmp;
            } else {
                m_Texture2Ds[m_UniformStorage[name].value.i] = texture2D;
            }
        }

        float& GetFloat(const std::string& name) {
            return static_cast<float>(m_UniformStorage[name].value.f);
        }
        int& GetInt(const std::string& name) {
            return static_cast<int>(m_UniformStorage[name].value.i);
        }
        glm::vec2& GetVec2(const std::string& name) {
            return static_cast<glm::vec2>(m_UniformStorage[name].value.v2);
        }
        glm::vec3& GetVec3(const std::string& name) {
            return static_cast<glm::vec3>(m_UniformStorage[name].value.v3);
        }
        glm::vec4& GetVec4(const std::string& name) {
            return static_cast<glm::vec4>(m_UniformStorage[name].value.v4);
        }
        glm::mat3& GetMat3(const std::string& name) {
            return *(glm::mat3*) m_UniformStorage[name].value.data;
        }
        glm::mat4& GetMat4(const std::string& name) {
            return *((glm::mat4*) m_UniformStorage[name].value.data);
        }
        Ref<Texture2D>& GetTexture2D(const std::string& name) {
            if (m_UniformStorage[name].Type == UniformType::TEX2D) {
                return m_Texture2Ds[m_UniformStorage[name].value.i];
            }
        }

        const Ref<Shader>& GetShader() const noexcept {
            return m_Shader;
        }
        const std::string& GetName() const noexcept {
            return m_Name;
        }
        const std::unordered_map<std::string, UniformStorage>& GetUniformStorage() const noexcept {
            return m_UniformStorage;
        }

        void UpdateForRendering() {
            m_Shader->Bind();
            for (auto& [name, uniform] : m_UniformStorage) {
                switch (uniform.Type) {
                case UniformType::TEX2D:
                case UniformType::TEXCUBE:
                    break;
                case UniformType::INT:
                    {
                        int value = m_UniformStorage[name].value.i;
                        m_Shader->SetInt(name, value);
                        break;
                    }
                case UniformType::FLOAT:
                    {
                        float value = m_UniformStorage[name].value.f;
                        m_Shader->SetFloat(name, value);
                        break;
                    }
                case UniformType::VEC2:
                    {
                        const glm::vec2& value = m_UniformStorage[name].value.v2;
                        m_Shader->SetFloat2(name, value);
                        break;
                    }
                case UniformType::VEC3:
                    {
                        const glm::vec3& value = m_UniformStorage[name].value.v3;
                        m_Shader->SetFloat3(name, value);
                        break;
                    }
                case UniformType::VEC4:
                    {
                        const glm::vec4& value = m_UniformStorage[name].value.v4;
                        m_Shader->SetFloat4(name, value);
                        break;
                    }
                case UniformType::MAT3:
                    {
                        const glm::mat3& value = *(glm::mat3*) (m_UniformStorage[name].value.data);
                        m_Shader->SetMat3(name, value);
                        break;
                    }
                case UniformType::MAT4:
                    {
                        const glm::mat4& value = *(glm::mat4*) (m_UniformStorage[name].value.data);
                        m_Shader->SetMat4(name, value);
                        break;
                    }
                default:
                    AT_CORE_ASSERT(false, "No Correct Uniform Type!");
                    break;
                }
            }

            for (auto& [slot, texture2D] : m_Texture2Ds) {
                if (texture2D) {
                    texture2D->Bind(slot);
                }
            }
        }

        const uint32_t GetFlags() const noexcept {
            return m_MaterialFlags;
        }
        const bool GetFlag(MaterialFlag flag) const noexcept {
            return (uint32_t) flag & m_MaterialFlags;
        }
        void SetFlag(MaterialFlag flag, bool value = true) {
            if (value) {
                m_MaterialFlags |= (uint32_t) flag;
            } else {
                m_MaterialFlags &= ~(uint32_t) flag;
            }
        }

    private:
        std::string m_Name;
        Ref<Shader>& m_Shader;
        std::unordered_map<std::string, UniformStorage> m_UniformStorage;
        std::map<uint32_t, Ref<Texture2D>> m_Texture2Ds;
        TextureSlotAlocator m_TexSlotAllocator;
        uint32_t m_MaterialFlags = 0;
    };

} // namespace Atlas
