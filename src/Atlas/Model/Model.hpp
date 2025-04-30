//#pragma once
//
//#include "Atlas/Renderer/Texture.hpp"
//#include "Atlas/Renderer/VertexArray.hpp"
//#include "Atlas/Renderer/Buffer.hpp"
//#include "Atlas/Renderer/Renderer.hpp"
//#include "Atlas/Renderer/Shader.hpp"
//
//#include <assimp/scene.h>
//#include <assimp/Importer.hpp>
//#include <glm/glm.hpp>
//#include <unordered_map>
//
//namespace Atlas {
//
//struct MeshVertex {
//    glm::vec3 Position;
//    glm::vec3 Normal;
//    glm::vec2 TexCoord;
//    glm::vec3 Tangent;
//    glm::vec3 Bitangent;
//};
//
//struct MeshTexture {
//    std::string type;
//    Ref<Texture2D> data;
//    glm::vec2 offset;
//    glm::vec2 scale;
//    float weight;
//};
//
//struct MeshData {
//    std::vector<MeshVertex> vertices;
//    std::vector<uint32_t> indices;
//    std::vector<MeshTexture> textures;
//};
//
//class Mesh {
//public:
//    Mesh() = default;
//    Mesh(const MeshData& meshdata);
//    void Draw(Shader& shader, unsigned int type);
//    const Mesh& operator=(const Mesh& other) {
//        m_VertexArray  = other.m_VertexArray;
//        m_IndexBuffer  = other.m_IndexBuffer;
//        m_VertexBuffer = other.m_VertexBuffer;
//        m_MeshTextures = other.m_MeshTextures;
//        return *this;
//    }
//
//private:
//    Ref<VertexArray> m_VertexArray;
//    Ref<IndexBuffer> m_IndexBuffer;
//    Ref<VertexBuffer> m_VertexBuffer;
//    std::vector<MeshTexture> m_MeshTextures;
//};
//
//class Model {
//public:
//    Model(std::string path);
//
//    const MeshData& GetData() {
//        return m_MeshData;
//    }
//    const glm::vec3& GetCenter() const noexcept {
//        return (m_MinBound + m_MaxBound) * 0.5f;
//    }
//    const glm::vec3& GetSize() const noexcept {
//        return m_MaxBound - m_MinBound;
//    }
//
//    const std::vector<MeshData>& _test_get_data() {
//        return m_Data;
//    }
//
//private:
//    void LoadModel();
//    void processNode(aiNode* node, const aiScene* scene);
//    MeshData processMesh(aiMesh* mesh, const aiScene* scene);
//
//    std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
//    void ConvertToAtlasData();
//    void CalculateMinMaxBound();
//
//private:
//    std::string m_ModelPath;
//    std::vector<MeshData> m_Data;
//    MeshData m_MeshData;
//
//    std::unordered_map<std::string, MeshTexture> m_TexturesLoaded;
//    glm::vec3 m_MinBound = glm::vec3(FLT_MAX);
//    glm::vec3 m_MaxBound = glm::vec3(-FLT_MAX);
//};
//
//
//} // namespace Atlas
