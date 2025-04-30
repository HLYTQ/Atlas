#pragma once

#include "Atlas/Core.hpp"
#include "Atlas/Renderer/Buffer.hpp"
#include "Atlas/Renderer/Material.hpp"
#include "Atlas/Renderer/Renderer3D.hpp"
#include "Atlas/Renderer/RendererAPI.hpp"
#include "Atlas/Renderer/Texture.hpp"
#include "Atlas/Renderer/VertexArray.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstdint>
#include <string>
#include <unordered_map>

#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/LogStream.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Atlas {

    struct AABB {
        glm::vec3 Min, Max;

        AABB() : Min(0.0f), Max(0.0f) {}

        AABB(const glm::vec3& min, const glm::vec3& max) : Min(min), Max(max) {}
    };

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 Texcoord;
        glm::vec3 Tangent;
        glm::vec3 Binormal;
    };

    struct Triangle {
        Vertex V0, V1, V2;

        Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) : V0(v0), V1(v1), V2(v2) {}
    };


    struct Index {
        uint32_t V1, V2, V3;
    };

    struct Submesh {
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;
        uint32_t VertexCount;

        glm::mat4 Transform{1.0f};
        AABB BoundingBox;

        std::string NodeName;
        std::string MeshName;
    };

    class Mesh {
    public:
        Mesh(const std::string& filepath);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
        ~Mesh();

        std::vector<Submesh>& GetSubmeshes() noexcept {
            return m_Submeshes;
        }
        const std::vector<Submesh>& GetSubmeshes() const noexcept {
            return m_Submeshes;
        }
        Ref<Shader> GetMeshShader() noexcept {
            return m_MeshShader;
        }
        std::vector<Ref<Material>>& GetMaterials() noexcept {
            return m_Materials;
        }
        const std::vector<Ref<Material>>& GetMaterials() const noexcept {
            return m_Materials;
        }
        const std::vector<Ref<Texture2D>>& GetTextures() const noexcept {
            return m_Textures;
        }
        const std::string& GetFilePath() const noexcept {
            return m_FilePath;
        }
        Ref<VertexBuffer> GetVertexBuffer() {
            return m_VertexBuffer;
        }
        Ref<IndexBuffer> GetIndexBuffer() {
            return m_IndexBuffer;
        }

        const glm::vec3& GetCenter() const noexcept {
            return (m_aabb.Min + m_aabb.Max) * 0.5f;
        }
        const glm::vec3& GetSize() const noexcept {
            return m_aabb.Max - m_aabb.Min;
        }

    private:
        void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);
        void CaulcuateMaxAABB();

    private:
        std::vector<Submesh> m_Submeshes;

        Scope<Assimp::Importer> m_Importer;
        glm::mat4 m_InverseTransform;

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        BufferLayout m_BufferLayout;

        std::vector<Vertex> m_StaticVertices;
        std::vector<Index> m_Indices;

        Ref<Shader> m_MeshShader;
        std::vector<Ref<Texture2D>> m_Textures;
        std::vector<Ref<Texture2D>> m_NormalMaps;
        std::vector<Ref<Material>> m_Materials;


        std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

        const aiScene* m_Scene;
        std::string m_FilePath;

        AABB m_aabb = AABB(glm::vec3(FLT_MAX), glm::vec3(-FLT_MAX));

        friend class Renderer3D;
        friend class RendererAPI;
    };

} // namespace Atlas
