#include "Mesh.hpp"

#include <filesystem>

#include <glm/glm.hpp>

namespace Atlas {

    static std::string GetRootPath(const std::string& path) noexcept {
        return std::string(AT_CURRENT_PATH) + path;
    }

    glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix) {
        glm::mat4 result;
        // the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        result[0][0] = matrix.a1;
        result[1][0] = matrix.a2;
        result[2][0] = matrix.a3;
        result[3][0] = matrix.a4;
        result[0][1] = matrix.b1;
        result[1][1] = matrix.b2;
        result[2][1] = matrix.b3;
        result[3][1] = matrix.b4;
        result[0][2] = matrix.c1;
        result[1][2] = matrix.c2;
        result[2][2] = matrix.c3;
        result[3][2] = matrix.c4;
        result[0][3] = matrix.d1;
        result[1][3] = matrix.d2;
        result[2][3] = matrix.d3;
        result[3][3] = matrix.d4;
        return result;
    }
    static const uint32_t s_MeshImportFlags = aiProcess_CalcTangentSpace | // Create binormals/tangents just in case
                                              aiProcess_Triangulate | // Make sure we're triangles
                                              aiProcess_SortByPType | // Split meshes by primitive type
                                              aiProcess_GenNormals | // Make sure we have legit normals
                                              aiProcess_GenUVCoords | // Convert UVs if required
                                              aiProcess_OptimizeMeshes | // Batch draws where possible
                                              aiProcess_ValidateDataStructure; // Validation

    Mesh::Mesh(const std::string& filepath) : m_FilePath(filepath) {
        AT_CORE_INFO("Loading mesh: {0}", filepath.c_str());

        m_Importer = Atlas::CreateScope<Assimp::Importer>();

        const aiScene* scene = m_Importer->ReadFile(filepath, s_MeshImportFlags);

        if (!scene || !scene->HasMeshes()) {
            AT_CORE_ERROR("Failed to load mesh file: {0}", filepath);
        }

        m_Scene = scene;

        m_MeshShader = Shader::Create(GetRootPath("assets/shaders/test_mesh.glsl"));

        uint32_t vertexCount = 0;
        uint32_t indexCount  = 0;
        m_Submeshes.reserve(scene->mNumMeshes);
        for (size_t m = 0; m < scene->mNumMeshes; ++m) {
            aiMesh* mesh = scene->mMeshes[m];

            Submesh& submesh      = m_Submeshes.emplace_back();
            submesh.BaseVertex    = vertexCount;
            submesh.BaseIndex     = indexCount;
            submesh.MaterialIndex = mesh->mMaterialIndex;
            submesh.VertexCount   = mesh->mNumVertices;
            submesh.IndexCount    = mesh->mNumFaces * 3;
            submesh.MeshName      = mesh->mName.C_Str();

            vertexCount += mesh->mNumVertices;
            indexCount += submesh.IndexCount;

            AT_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
            AT_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

            // Vertices
            auto& aabb = submesh.BoundingBox;
            aabb.Min   = {FLT_MAX, FLT_MAX, FLT_MAX};
            aabb.Max   = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
            for (size_t i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;
                vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                vertex.Normal   = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
                aabb.Min.x      = glm::min(vertex.Position.x, aabb.Min.x);
                aabb.Min.y      = glm::min(vertex.Position.y, aabb.Min.y);
                aabb.Min.z      = glm::min(vertex.Position.z, aabb.Min.z);
                aabb.Max.x      = glm::max(vertex.Position.x, aabb.Max.x);
                aabb.Max.y      = glm::max(vertex.Position.y, aabb.Max.y);
                aabb.Max.z      = glm::max(vertex.Position.z, aabb.Max.z);

                if (mesh->HasTangentsAndBitangents()) {
                    vertex.Tangent  = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                    vertex.Binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
                }

                if (mesh->HasTextureCoords(0)) {
                    vertex.Texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
                }

                m_StaticVertices.push_back(vertex);
            }

            // Indices
            for (size_t i = 0; i < mesh->mNumFaces; i++) {
                AT_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
                Index index = {mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]};
                m_Indices.push_back(index);

                m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex],
                    m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
            }
        }

        TraverseNodes(scene->mRootNode);

        // Materials
        if (scene->HasMaterials()) {
            AT_CORE_TRACE("---- Materials - {0} ----", filepath);

            m_Textures.resize(scene->mNumMaterials);
            m_Materials.resize(scene->mNumMaterials);

            Ref<Texture2D> whiteTexture = Texture2D::Create(GetRootPath("assets/textures/white_base.jpeg"));

            for (uint32_t i = 0; i < scene->mNumMaterials; i++) {
                auto aiMaterial     = scene->mMaterials[i];
                auto aiMaterialName = aiMaterial->GetName();

                auto mi        = CreateRef<Material>(m_MeshShader, aiMaterialName.data);
                m_Materials[i] = mi;

                AT_CORE_TRACE("  {0} (Index = {1})", aiMaterialName.data, i);
                aiString aiTexPath;
                uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
                AT_CORE_TRACE("    TextureCount = {0}", textureCount);

                glm::vec3 albedoColor(0.8f);
                aiColor3D aiColor;
                if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS) {
                    albedoColor = {aiColor.r, aiColor.g, aiColor.b};
                }

                mi->Set("u_MaterialUniforms.AlbedoColor", albedoColor);

                float shininess, metalness;
                if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS) {
                    shininess = 80.0f; // Default value
                }

                if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS) {
                    metalness = 0.0f;
                }

                float roughness = 1.0f - glm::sqrt(shininess / 100.0f);
                AT_CORE_TRACE("    COLOR = {0}, {1}, {2}", aiColor.r, aiColor.g, aiColor.b);
                AT_CORE_TRACE("    ROUGHNESS = {0}", roughness);
                AT_CORE_TRACE("    METALNESS = {0}", metalness);
                bool hasAlbedoMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
                bool fallback     = !hasAlbedoMap;
                if (hasAlbedoMap) {
                    // TODO: Temp - this should be handled by Hazel's filesystem
                    std::filesystem::path path = filepath;
                    auto parentPath            = path.parent_path();
                    parentPath /= std::string(aiTexPath.data);
                    std::string texturePath = parentPath.string();
                    AT_CORE_TRACE("    Albedo map path = {0}", texturePath);

                    auto texture = Texture2D::Create(texturePath);
                    if (texture->Loaded()) {
                        m_Textures[i] = texture;
                        mi->Set("u_AlbedoTexture", texture);
                    } else {
                        AT_CORE_ERROR("Could not load texture: {0}", texturePath);
                        fallback = true;
                    }
                }

                if (fallback) {
                    AT_CORE_TRACE("    No albedo map");
                    mi->Set("u_AlbedoTexture", whiteTexture);
                }

                // Normal maps
                mi->Set("u_MaterialUniforms.UseNormalMap", (int) false);
                bool hasNormalMap = aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
                fallback          = !hasNormalMap;
                if (hasNormalMap) {
                    // TODO: Temp - this should be handled by Hazel's filesystem
                    std::filesystem::path path = filepath;
                    auto parentPath            = path.parent_path();
                    parentPath /= std::string(aiTexPath.data);
                    std::string texturePath = parentPath.string();
                    AT_CORE_TRACE("    Normal map path = {0}", texturePath);
                    auto texture = Texture2D::Create(texturePath);
                    if (texture->Loaded()) {
                        m_Textures.push_back(texture);
                        mi->Set("u_NormalTexture", texture);
                        mi->Set("u_MaterialUniforms.UseNormalMap", true);
                    } else {
                        AT_CORE_ERROR("    Could not load texture: {0}", texturePath);
                        fallback = true;
                    }
                }

                if (fallback) {
                    AT_CORE_TRACE("    No normal map");
                    mi->Set("u_NormalTexture", whiteTexture);
                }

                // Roughness map
                bool hasRoughnessMap = aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS;
                fallback             = !hasRoughnessMap;
                if (hasRoughnessMap) {
                    // TODO: Temp - this should be handled by Hazel's filesystem
                    std::filesystem::path path = filepath;
                    auto parentPath            = path.parent_path();
                    parentPath /= std::string(aiTexPath.data);
                    std::string texturePath = parentPath.string();
                    AT_CORE_TRACE("    Roughness map path = {0}", texturePath);
                    auto texture = Texture2D::Create(texturePath);
                    if (texture->Loaded()) {
                        m_Textures.push_back(texture);
                        mi->Set("u_RoughnessTexture", texture);
                    } else {
                        AT_CORE_TRACE("    Could not load texture: {0}", texturePath);
                        fallback = true;
                    }
                }

                if (fallback) {
                    AT_CORE_TRACE("    No roughness map");
                    mi->Set("u_RoughnessTexture", whiteTexture);
                    mi->Set("u_MaterialUniforms.Roughness", roughness);
                }


                bool metalnessTextureFound = false;
                for (uint32_t p = 0; p < aiMaterial->mNumProperties; p++) {
                    auto prop = aiMaterial->mProperties[p];
                    if (prop->mType == aiPTI_String) {
                        uint32_t strLength = *(uint32_t*) prop->mData;
                        std::string str(prop->mData + 4, strLength);

                        std::string key = prop->mKey.data;
                        if (key == "$raw.ReflectionFactor|file") {
                            // TODO: Temp - this should be handled by Hazel's filesystem
                            std::filesystem::path path = filepath;
                            auto parentPath            = path.parent_path();
                            parentPath /= str;
                            std::string texturePath = parentPath.string();
                            AT_CORE_TRACE("    Metalness map path = {0}", texturePath);
                            auto texture = Texture2D::Create(texturePath);
                            if (texture->Loaded()) {
                                metalnessTextureFound = true;
                                m_Textures.push_back(texture);
                                mi->Set("u_MetalnessTexture", texture);
                            } else {
                                AT_CORE_TRACE("    Could not load texture: {0}", texturePath);
                            }
                            break;
                        }
                    }
                }

                fallback = !metalnessTextureFound;
                if (fallback) {
                    AT_CORE_TRACE("    No metalness map");
                    mi->Set("u_MetalnessTexture", whiteTexture);
                    mi->Set("u_MaterialUniforms.Metalness", metalness);
                }
            }
            AT_CORE_TRACE("------------------------");
        } else {
            auto mi = CreateRef<Material>(m_MeshShader, "Atlas-Default");
            // mi->Set("u_MaterialUniforms.AlbedoTexToggle", 0.0f);
            // mi->Set("u_MaterialUniforms.NormalTexToggle", 0.0f);
            // mi->Set("u_MaterialUniforms.MetalnessTexToggle", 0.0f);
            // mi->Set("u_MaterialUniforms.RoughnessTexToggle", 0.0f);
            mi->Set("u_MaterialUniforms.AlbedoColor", glm::vec3(0.8f, 0.1f, 0.3f));
            mi->Set("u_MaterialUniforms.Metalness", 0.0f);
            mi->Set("u_MaterialUniforms.Roughness", 0.8f);
            m_Materials.push_back(mi);
        }
        m_VertexBuffer =
            VertexBuffer::Create((float*) m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
        m_BufferLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float3, "a_Binormal"},
        };
        m_IndexBuffer = IndexBuffer::Create((uint32_t*) m_Indices.data(), m_Indices.size() * sizeof(Index));
        m_VertexBuffer->SetLayout(m_BufferLayout);
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform)
        : m_StaticVertices(vertices), m_Indices(indices) {
        Submesh submesh;
        submesh.BaseVertex = 0;
        submesh.BaseIndex  = 0;
        submesh.IndexCount = indices.size() * 3;
        submesh.Transform  = transform;
        m_Submeshes.push_back(submesh);

        m_VertexBuffer =
            VertexBuffer::Create((float*) m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
        m_IndexBuffer  = IndexBuffer::Create((uint32_t*) m_Indices.data(), m_Indices.size() * sizeof(Index));
        m_BufferLayout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float3, "a_Binormal"},
        };
        m_VertexBuffer->SetLayout(m_BufferLayout);
    }

    Mesh::~Mesh() {}

    static std::string LevelToSpaces(uint32_t level) {
        std::string result = "";
        for (uint32_t i = 0; i < level; i++) {
            result += "--";
        }
        return result;
    }

    void Mesh::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level) {
        glm::mat4 transform = parentTransform * Mat4FromAssimpMat4(node->mTransformation);
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            uint32_t mesh     = node->mMeshes[i];
            auto& submesh     = m_Submeshes[mesh];
            submesh.NodeName  = node->mName.C_Str();
            submesh.Transform = transform;
        }

        // AT_CORE_TRACE("{0} {1}", LevelToSpaces(level), node->mName.C_Str());

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            TraverseNodes(node->mChildren[i], transform, level + 1);
        }
    }
    void Atlas::Mesh::CaulcuateMaxAABB() {
        for (Submesh& submesh : m_Submeshes) {
            AABB& aabb = submesh.BoundingBox;
            m_aabb.Max = glm::max(m_aabb.Max, aabb.Max);
            m_aabb.Min = glm::min(m_aabb.Min, aabb.Min);
        }
    }
} // namespace Atlas
