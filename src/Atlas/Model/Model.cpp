//#include "Model.hpp"
//
//#include <assimp/postprocess.h>
//#include <filesystem>
//
//#include "atlaspch.hpp"
//
//namespace Atlas {
//
//Model::Model(std::string path) : m_ModelPath(std::move(path)) {
//    LoadModel();
//    // ConvertToAtlasData();
//    CalculateMinMaxBound();
//}
//
//void Model::LoadModel() {
//    Assimp::Importer importer;
//
//    // load FBX model
//    const aiScene* scene =
//        importer.ReadFile(m_ModelPath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        AT_CORE_ERROR("Failed to load model: {}", importer.GetErrorString());
//    }
//
//    processNode(scene->mRootNode, scene);
//}
//
//void Model::processNode(aiNode* node, const aiScene* scene) {
//    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        m_Data.push_back(processMesh(mesh, scene));
//    }
//
//    for (uint32_t i = 0; i < node->mNumChildren; i++) {
//        processNode(node->mChildren[i], scene);
//    }
//}
//
//MeshData Model::processMesh(aiMesh* mesh, const aiScene* scene) {
//    std::vector<MeshVertex> vertices;
//    std::vector<uint32_t> indices;
//    std::vector<MeshTexture> textures;
//
//    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
//        MeshVertex vertex;
//
//        vertex.Position = {
//            mesh->mVertices[i].x, 
//            mesh->mVertices[i].y, 
//            mesh->mVertices[i].z
//        };
//        vertex.Normal   = { 
//            mesh->mNormals[i].x, 
//            mesh->mNormals[i].y, 
//            mesh->mNormals[i].z
//        };
//
//        if (mesh->mTextureCoords[0]) {
//            vertex.TexCoord = { 
//                mesh->mTextureCoords[0][i].x, 
//                mesh->mTextureCoords[0][i].y
//            };
//        } else {
//            vertex.TexCoord = glm::vec2(0.0f, 0.0f);
//        }
//
//        if (mesh->HasTangentsAndBitangents()) {
//            vertex.Tangent   = {
//                mesh->mTangents[i].x, 
//                mesh->mTangents[i].y, 
//                mesh->mTangents[i].z
//            };
//            vertex.Bitangent = {
//                mesh->mBitangents[i].x, 
//                mesh->mBitangents[i].y, 
//                mesh->mBitangents[i].z
//            };
//        }
//
//        vertices.push_back(vertex);
//    }
//    // indices
//    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
//        aiFace face = mesh->mFaces[i];
//
//        for (uint32_t j = 0; j < face.mNumIndices; j++) {
//            indices.push_back(face.mIndices[j]);
//        }
//    }
//    // materials
//    if (scene->HasMaterials()) {
//        AT_CORE_TRACE("---- Materials - {0} ----", m_ModelPath);
//        
//        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//        // 1. diffuse maps
//        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "tex_diffuse", scene);
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//        // 2. specular maps
//        std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "tex_specular", scene);
//        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//        // 2. roughness maps
//        std::vector<MeshTexture> roughnessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "tex_roughness", scene);
//        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
//    }
//
//    return MeshData{vertices, indices, textures};
//}
//
//std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
//    std::vector<MeshTexture> textures;
//    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
//        aiString aiTexPath;
//        mat->GetTexture(type, i, &aiTexPath);
//        auto toFind = m_TexturesLoaded.find(aiTexPath.C_Str());
//
//        if (toFind != m_TexturesLoaded.end()) {
//            textures.push_back(toFind->second);
//        } else { // if texture hasn't been loaded already, load it
//            std::filesystem::path path = m_ModelPath;
//            auto parentPath            = path.parent_path();
//            parentPath /= std::string(aiTexPath.data);
//            std::string texturePath = parentPath.string();
//            AT_CORE_TRACE("    Albedo map path = {0}", texturePath);
//
//            MeshTexture texture;
//            texture.type = typeName;
//            texture.data = Texture2D::Create(texturePath);
//            texture.offset = glm::vec2(0.0f);
//            texture.scale = glm::vec2(1.0f);
//            texture.weight = 1.0f;
//
//            aiUVTransform tex_transform;
//            switch (type) {
//            case aiTextureType_DIFFUSE:
//                if (mat->Get(AI_MATKEY_UVTRANSFORM_DIFFUSE(i), tex_transform) == AI_SUCCESS) {
//                    texture.offset = glm::vec2(tex_transform.mTranslation.x, tex_transform.mTranslation.y);
//                    texture.scale = glm::vec2(tex_transform.mScaling.x, tex_transform.mScaling.y);
//                    texture.weight = 1.0f;
//                }
//            }
//
//            textures.push_back(texture);
//            m_TexturesLoaded[aiTexPath.C_Str()] = texture;
//        }
//    }
//    return textures;
//}
//
//void Model::ConvertToAtlasData() {
//    std::vector<MeshVertex> vertices;
//    std::vector<uint32_t> indices;
//    std::vector<MeshTexture> textures;
//
//    uint32_t count = 0;
//
//    for (size_t i = 0; i < m_Data.size(); i++) {
//        const MeshData& data = m_Data[i];
//        for (MeshVertex vertex : data.vertices) {
//            vertices.push_back(vertex);
//        }
//        for (uint32_t i = 0; i < data.indices.size(); i++) {
//            indices.push_back(i + count);
//        }
//
//        count = static_cast<uint32_t>(vertices.size());
//
//        for (auto unit : m_TexturesLoaded) {
//            textures.push_back(unit.second);
//        }
//    }
//
//    m_MeshData = MeshData{vertices, indices, textures};
//}
//
//void Model::CalculateMinMaxBound() {
//    for (const auto& mesh : m_Data) {
//        for (const auto& vertex : mesh.vertices) {
//            m_MinBound = glm::min(m_MinBound, vertex.Position);
//            m_MaxBound = glm::max(m_MaxBound, vertex.Position);
//        }
//    }
//}
//
//Mesh::Mesh(const MeshData& meshdata) {
//    m_VertexArray = Atlas::VertexArray::Create();
//    m_VertexBuffer = Atlas::VertexBuffer::Create((float*)meshdata.vertices.data(), meshdata.vertices.size() * sizeof(Atlas::MeshVertex));
//
//    m_VertexBuffer->SetLayout({
//        { Atlas::ShaderDataType::Float3, "a_Position" },
//        { Atlas::ShaderDataType::Float3, "a_Normal" },
//        { Atlas::ShaderDataType::Float2, "a_TexCoord" },
//        { Atlas::ShaderDataType::Float3, "a_Tangent" },
//        { Atlas::ShaderDataType::Float3, "a_Bitangent" }
//        });
//
//    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
//    m_IndexBuffer = Atlas::IndexBuffer::Create((uint32_t*)meshdata.indices.data(), meshdata.indices.size());
//    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
//
//    m_MeshTextures.resize(meshdata.textures.size());
//
//    for (int i = 0; i < meshdata.textures.size(); ++i) {
//        m_MeshTextures[i].type = meshdata.textures[i].type;
//        m_MeshTextures[i].data = meshdata.textures[i].data;
//        m_MeshTextures[i].offset = meshdata.textures[i].offset;
//        m_MeshTextures[i].scale = meshdata.textures[i].scale;
//        m_MeshTextures[i].weight = meshdata.textures[i].weight;
//    }
//
//}
//
//void Mesh::Draw(Shader& shader, unsigned int type) {
//    unsigned int diffuseNr = 1;
//    unsigned int normalNr = 1;
//    unsigned int roughnessNr = 1;
//
//    shader.Bind();
//
//    for (int i = 0; i < m_MeshTextures.size(); ++i) {
//        m_MeshTextures[i].data->Bind(i);
//        std::string number;
//        std::string name = m_MeshTextures[i].type;
//        if (name == "tex_diffuse") {
//            number = std::to_string(diffuseNr++);
//        }
//        else if (name == "tex_normal") {
//            number = std::to_string(normalNr++); // transfer unsigned int to string
//        }
//        else if (name == "tex_roughness") {
//            number = std::to_string(roughnessNr++);
//        }
//
//        shader.SetInt(name + number, i);
//        shader.SetFloat2(name + number + "_offset", m_MeshTextures[i].offset);
//        shader.SetFloat2(name + number + "_scale", m_MeshTextures[i].scale);
//        shader.SetFloat(name + number + "_weight", m_MeshTextures[i].weight);
//    }
//    m_VertexArray->Bind();
//    RendererCommand::DrawIndexed(m_VertexArray, type);
//}
//
//} // namespace Atlas
