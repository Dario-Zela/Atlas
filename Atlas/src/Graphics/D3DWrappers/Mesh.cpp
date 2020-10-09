#include "pch.h"
#include "Mesh.h"
#include <filesystem>

namespace Atlas
{
	Mesh::Mesh(aiMesh* mesh, aiMaterial* materials, std::filesystem::path path)
	{
		m_VertexPositions.reserve(mesh->mNumVertices);
		bool attributes[] = { mesh->HasTextureCoords(0), mesh->HasNormals(), mesh->HasVertexColors(0), mesh->HasTangentsAndBitangents() };

		if(attributes[0])
			m_TextureCoordinates.reserve(mesh->mNumVertices);

		if(attributes[1])
			m_NormalCoordinates.reserve(mesh->mNumVertices);

		if(attributes[2])
			m_Colors.reserve(mesh->mNumVertices);

		if(attributes[3])
		{
			m_Tangents.reserve(mesh->mNumVertices);
			m_Bitangents.reserve(mesh->mNumVertices);
		}

		for (uint i = 0; i < mesh->mNumVertices; i++)
		{
			m_VertexPositions.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mVertices[i]));
			if (attributes[0])
				m_TextureCoordinates.push_back(*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh->mTextureCoords[0][i]));

			if (attributes[1])
				m_NormalCoordinates.push_back(*reinterpret_cast<DirectX::XMFLOAT2*>(&mesh->mNormals[i]));

			if (attributes[2])
				m_Colors.push_back(*reinterpret_cast<DirectX::XMFLOAT4*>(&mesh->mColors[0][i]));

			if (attributes[3])
			{
				m_Tangents.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mTangents[i]));
				m_Bitangents.push_back(*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mBitangents[i]));
			}
		}

		if (mesh->mMaterialIndex >= 0) 
		{
			auto material = materials[mesh->mMaterialIndex];
			m_Filepath = path.parent_path().string() + "\\";
		}

		m_Name = mesh->mName.C_Str();

		m_Indecies.reserve(mesh->mNumFaces * (int)3);

		for (uint i = 0; i < mesh->mNumFaces; i++)
		{
			const auto& face = mesh->mFaces[i];
			m_Indecies.push_back(face.mIndices[0]);
			m_Indecies.push_back(face.mIndices[1]);
			m_Indecies.push_back(face.mIndices[2]);
		}
	}

	std::shared_ptr<Texture> Mesh::GetTexture(uint textureType)
	{
		aiString path;
		if (m_Material.GetTexture((aiTextureType)textureType, 0, &path) == aiReturn::aiReturn_SUCCESS)
		{
			return std::move(Texture::Create(m_Filepath + path.C_Str()));
		}
		else
		{
			return nullptr;
		}
	}

	Node::Node(const std::string& name, std::vector<Mesh*> meshes, const DirectX::XMMATRIX& tranform)
		: m_Name(name), m_Meshes(std::move(meshes))
	{
		DirectX::XMStoreFloat4x4(&m_Transform, tranform);
		DirectX::XMStoreFloat4x4(&m_AppliedTransform, DirectX::XMMatrixIdentity());
	}

	void Node::SetAppliedTranform(const DirectX::XMMATRIX& tranform)
	{
		DirectX::XMStoreFloat4x4(&m_AppliedTransform, tranform);
	}

	DirectX::XMFLOAT4X4& Node::GetAppliedTranfrom()
	{
		return m_AppliedTransform;
	}

	Model::Model(std::string path)
	{
		Assimp::Importer imp;
		const auto model = imp.ReadFile(R"(D:\uploads_files_1839214_helicopter.obj)", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		for (int i = 0; i < model->mNumMeshes; i++)
		{
			m_Meshes.push_back(model->mMeshes[i]);
		}
	}
}