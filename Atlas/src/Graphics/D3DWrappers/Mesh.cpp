#include "pch.h"
#include "Mesh.h"
#include <filesystem>

#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/D3DWrappers/InputLayout.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/D3DWrappers/ViewPort.h"
#include "Graphics/D3DWrappers/TransformationConstantBuffer.h"
#include "Graphics/D3DWrappers/Sampler.h"

#include "Graphics/D3DWrappers/Blendable.h"

namespace Atlas
{
	Mesh::Mesh(aiMesh* mesh, aiMaterial** materials, std::filesystem::path path)
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
			m_Material = *materials[mesh->mMaterialIndex];
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

	void Mesh::Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings)
	{
		if (m_Set && !settings.reSet)
		{
			m_Transform = accumulatedTransform;
		}
		else 
		{
			//Need to clear Bindables

			bool needsNormals = (settings.proprietiesFlags & (uint)MeshProprietiesFlags::NORMAL_COORDINATES) == (uint)MeshProprietiesFlags::NORMAL_COORDINATES;
			bool needsTextures = (settings.proprietiesFlags & (uint)MeshProprietiesFlags::TEXTURE_COORDINATES) == (uint)MeshProprietiesFlags::TEXTURE_COORDINATES;
			bool needsColors = (settings.proprietiesFlags & (uint)MeshProprietiesFlags::COLORS) == (uint)MeshProprietiesFlags::COLORS;
			bool needsTangents = (settings.proprietiesFlags & (uint)MeshProprietiesFlags::TANGENTS) == (uint)MeshProprietiesFlags::TANGENTS;
			bool needsBitangents = (settings.proprietiesFlags & (uint)MeshProprietiesFlags::BITANGENTS) == (uint)MeshProprietiesFlags::BITANGENTS;

			uint stride = sizeof(DirectX::XMFLOAT3);
			if (needsNormals) stride += sizeof(DirectX::XMFLOAT2);
			if (needsTextures) stride += sizeof(DirectX::XMFLOAT2);
			if (needsColors) stride += sizeof(DirectX::XMFLOAT4);
			if (needsTangents) stride += sizeof(DirectX::XMFLOAT3);
			if (needsBitangents) stride += sizeof(DirectX::XMFLOAT3);

			std::vector<float> data;
			for (int i = 0; i < m_VertexPositions.size(); i++)
			{
				data.push_back(m_VertexPositions[i].x);
				data.push_back(m_VertexPositions[i].y);
				data.push_back(m_VertexPositions[i].z);

				if (needsNormals)
				{
					data.push_back(m_NormalCoordinates[i].x);
					data.push_back(m_NormalCoordinates[i].y);
				}

				if (needsTextures)
				{
					data.push_back(m_TextureCoordinates[i].x);
					data.push_back(m_TextureCoordinates[i].y);
				}

				if (needsColors)
				{
					data.push_back(m_Colors[i].x);
					data.push_back(m_Colors[i].y);
					data.push_back(m_Colors[i].z);
					data.push_back(m_Colors[i].w);
				}

				if (needsTangents)
				{
					data.push_back(m_Tangents[i].x);
					data.push_back(m_Tangents[i].y);
					data.push_back(m_Tangents[i].z);
				}

				if (needsBitangents)
				{
					data.push_back(m_Bitangents[i].x);
					data.push_back(m_Bitangents[i].y);
					data.push_back(m_Bitangents[i].z);
				}
			}

			AddBindable(VertexBuffer::Create(data.data(), (uint)m_VertexPositions.size() * sizeof(float), stride, m_Name));

			AddBindable(IndexBuffer::Create(m_Indecies.data(), (uint)m_Indecies.size() * sizeof(unsigned short), m_Name));

			AT_CORE_ASSERT(!(settings.textureFlags > 0 && !needsTextures), "Tried to request textures without adding texture coordinates");
			AT_CORE_ASSERT(!(settings.textureFlags == 0 && needsTextures), "Tried to request textures without specifying what textures");

			if (needsTextures && settings.textureFlags > 0)
			{
				int slot = 0;
				for (int i = 0; i < MAX_MESH_TEXTURE_FLAGS_SIZE; i++)
				{
					if ((settings.textureFlags & 1 << i) == 1 << i)
					{
						std::shared_ptr<Texture> texture = GetTexture((aiTextureType)i, slot);
						if (texture)
						{
							AddBindable(texture);
							AddBindable(Sampler::Create(slot));
							slot++;
						}
					}
				}
			}

			if (settings.addBlending)
				AddBindable(Blendable::Create(true, 0));

			AT_CORE_ASSERT(!(settings.vertexShaderPath == ""), "No valid vertex shader");
			AT_CORE_ASSERT(!(settings.pixelShaderPath == ""), "No valid pixel shader");

			auto shader = VertexShader::Create(settings.vertexShaderPath);
			auto blob = shader->GetBlob();
			AddBindable(shader);

			AddBindable(PixelShader::Create(settings.pixelShaderPath));

			std::vector<InputElement> elements;
			elements.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });
			if (needsNormals) elements.push_back({ "NORMALS", DXGI_FORMAT_R32G32_FLOAT });				
			if (needsTextures) elements.push_back({ "TEXTURE_COORDS", DXGI_FORMAT_R32G32_FLOAT });			
			if (needsColors) elements.push_back({ "COLORS", DXGI_FORMAT_R32G32B32A32_FLOAT });		
			if (needsTangents) elements.push_back({ "TANGENTS", DXGI_FORMAT_R32G32B32_FLOAT });			
			if (needsBitangents) elements.push_back({ "BITANGENTS", DXGI_FORMAT_R32G32B32_FLOAT });		

			AddBindable(InputLayout::Create(elements, blob));

			Graphics::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			Graphics::SetRenderTarget();

			AddBindable(TransformationConstantBuffer::Create(*this, DirectX::XMMatrixIdentity()));

			m_Set = true;
		}

		Drawable::Draw();
	}

	std::shared_ptr<Texture> Mesh::GetTexture(aiTextureType textureType, int slot)
	{
		aiString path;
		if (m_Material.GetTexture(textureType, 0, &path) == aiReturn::aiReturn_SUCCESS)
		{
			return std::move(Texture::Create(m_Filepath + path.C_Str(), slot));
		}
		else
		{
			AT_ASSERT(false, "The Model doesn't have the selected texture type");
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

	void Node::Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings)
	{
		DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4(&m_Transform) * DirectX::XMLoadFloat4x4(&m_AppliedTransform) * accumulatedTransform;
		for (auto mesh : m_Meshes)
		{
			mesh->Draw(transform, settings);
		}
		for (auto& child : m_Children)
		{
			child->Draw(transform, settings);
			break;
		}
	}

	Model::Model(std::string path)
	{
		Assimp::Importer imp;
		const auto model = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		m_Meshes.reserve(model->mNumMeshes);

		for (uint i = 0; i < model->mNumMeshes; i++)
		{
			m_Meshes.push_back(std::move(std::make_unique<Mesh>(model->mMeshes[i], model->mMaterials, path)));
		}

		m_RootNode = ParseNode(model->mRootNode);
	}

	void Model::Draw(ModelDrawSettings& settings)
	{
		//Test this

		auto identity = DirectX::XMMatrixPerspectiveLH(1, 3.0f / 4.0f, 1, 1000) * DirectX::XMMatrixScaling(10,10,10);
		m_RootNode->Draw(identity, settings);
	}

	std::unique_ptr<Node> Model::ParseNode(aiNode* node)
	{
		DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(
			reinterpret_cast<DirectX::XMFLOAT4X4*>(&node->mTransformation)));

		std::vector<Mesh*> nodeMeshes;
		nodeMeshes.reserve(node->mNumMeshes);
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			auto meshIdx = node->mMeshes[i];
			nodeMeshes.push_back(m_Meshes.at(meshIdx).get());
		}

		std::unique_ptr<Node> returnNode = std::make_unique<Node>(node->mName.C_Str(), std::move(nodeMeshes), transform);

		for (uint i = 0; i < node->mNumChildren; i++)
		{
			returnNode->AddChild(ParseNode(node->mChildren[i]));
		}

		return returnNode;
	}
}