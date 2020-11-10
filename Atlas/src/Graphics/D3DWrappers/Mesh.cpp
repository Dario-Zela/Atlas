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

		if (attributes[0])
			m_TextureCoordinates.reserve(mesh->mNumVertices);

		if (attributes[1])
			m_NormalCoordinates.reserve(mesh->mNumVertices);

		if (attributes[2])
			m_Colors.reserve(mesh->mNumVertices);

		if (attributes[3])
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
			m_Material = materials[mesh->mMaterialIndex];
			m_Filepath = path.parent_path().string() + "\\";
		}

		m_Name = mesh->mName.C_Str();

		m_Indecies.reserve((uint)mesh->mNumFaces * (uint)3);

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
			ClearBindables();

			std::vector<InputElement> elements;
			elements.push_back({ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT });

			uint strideVertex = sizeof(DirectX::XMFLOAT3);
			uint strideCBuff = 0;
			std::vector<float> cbuffData;
			bool proprieties[5];

			for (uint i = 1; i < 19; i++)
			{
				if (i < 6)
				{
					proprieties[i - 1] = false;
				}

				if ((settings.proprietiesFlags & (1 << i)) != 0)
				{
					switch (i)
					{
						case 1:
						{
							elements.push_back({ "NORMALS", DXGI_FORMAT_R32G32_FLOAT });
							proprieties[0] = true;
							strideVertex += sizeof(DirectX::XMFLOAT2);
						}
							break;
						case 2:
						{
							elements.push_back({ "TEXTURE_COORDS", DXGI_FORMAT_R32G32_FLOAT });
							proprieties[1] = true;
							strideVertex += sizeof(DirectX::XMFLOAT2);
						}
							break;
						case 3:
						{
							elements.push_back({ "COLORS", DXGI_FORMAT_R32G32B32A32_FLOAT });
							proprieties[2] = true;
							strideVertex += sizeof(DirectX::XMFLOAT4);
						}
							break;
						case 4:
						{
							elements.push_back({ "TANGENTS", DXGI_FORMAT_R32G32B32_FLOAT });
							proprieties[3] = true;
							strideVertex += sizeof(DirectX::XMFLOAT3);
						}
							break;
						case 5:
						{
							elements.push_back({ "BITANGENTS", DXGI_FORMAT_R32G32B32_FLOAT });
							proprieties[4] = true;
							strideVertex += sizeof(DirectX::XMFLOAT3);
						}
							break;
						case 6:
						{
							aiColor3D data;
							m_Material->Get(AI_MATKEY_COLOR_DIFFUSE, data);
							if(data.IsBlack()) AT_WARN("The diffuse color of {0} is black, which is the default color, meaning that the value may be missing", m_Name)

							cbuffData.push_back(data.r);
							cbuffData.push_back(data.g);
							cbuffData.push_back(data.b);
							
							strideCBuff += sizeof(DirectX::XMFLOAT3);
						}
							break;
						case 7:
						{
							aiColor3D data;
							m_Material->Get(AI_MATKEY_COLOR_SPECULAR, data);
							if (data.IsBlack()) AT_WARN("The specular color of {0} is black, which is the default color, meaning that the value may be missing", m_Name)

							cbuffData.push_back(data.r);
							cbuffData.push_back(data.g);
							cbuffData.push_back(data.b);

							strideCBuff += sizeof(DirectX::XMFLOAT3);
						}
							break;
						case 8:
						{
							aiColor3D data;
							m_Material->Get(AI_MATKEY_COLOR_AMBIENT, data);
							if (data.IsBlack()) AT_WARN("The ambient color of {0} is black, which is the default color, meaning that the value may be missing", m_Name)

							cbuffData.push_back(data.r);
							cbuffData.push_back(data.g);
							cbuffData.push_back(data.b);

							strideCBuff += sizeof(DirectX::XMFLOAT3);
						}
						break;
						case 9:
						{
							aiColor3D data;
							m_Material->Get(AI_MATKEY_COLOR_EMISSIVE, data);
							if (data.IsBlack()) AT_WARN("The emmissive color of {0} is black, which is the default color, meaning that the value may be missing", m_Name)

							cbuffData.push_back(data.r);
							cbuffData.push_back(data.g);
							cbuffData.push_back(data.b);

							strideCBuff += sizeof(DirectX::XMFLOAT3);
						}
							break;
						case 10:
						{
							aiColor3D data;
							m_Material->Get(AI_MATKEY_COLOR_TRANSPARENT, data);
							if (data.IsBlack()) AT_WARN("The transparency color of {0} is black, which is the default color, meaning that the value may be missing", m_Name)

							cbuffData.push_back(data.r);
							cbuffData.push_back(data.g);
							cbuffData.push_back(data.b);

							strideCBuff += sizeof(DirectX::XMFLOAT3);
						}
							break;
						case 11:
						{
							int data;
							m_Material->Get(AI_MATKEY_ENABLE_WIREFRAME, data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(int);
						}
							break;
						case 12:
						{
							int data;
							m_Material->Get(AI_MATKEY_TWOSIDED, data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(int);
						}
							break;
						case 13:
						{
							int data;
							m_Material->Get(AI_MATKEY_SHADING_MODEL, data);
							
							cbuffData.push_back(data);
							strideCBuff += sizeof(int);
						}
							break;
						case 14:
						{
							int data;
							m_Material->Get(AI_MATKEY_BLEND_FUNC, data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(int);
						}
							break;
						case 15:
						{
							float data;
							m_Material->Get(AI_MATKEY_OPACITY, data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(float);
						}
							break;
						case 16:
						{
							float data;
							m_Material->Get(AI_MATKEY_SHININESS, data);
							AT_TRACE("{0}", data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(float);
						}
						break;
						case 17:
						{
							float data;
							m_Material->Get(AI_MATKEY_SHININESS_STRENGTH, data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(float);
						}
							break;
						case 18:
						{
							float data;
							m_Material->Get(AI_MATKEY_REFRACTI, data);

							cbuffData.push_back(data);
							strideCBuff += sizeof(float);
						}
							break;
					}
				}
			}

			if (strideCBuff != 0) AddBindable(VertexConstantBuffer::Create(cbuffData.data(), strideCBuff));

			{
				std::vector<float> data;
				for (int i = 0; i < m_VertexPositions.size(); i++)
				{
					data.push_back(m_VertexPositions[i].x);
					data.push_back(m_VertexPositions[i].y);
					data.push_back(m_VertexPositions[i].z);

					if (proprieties[0])
					{
						data.push_back(m_NormalCoordinates[i].x);
						data.push_back(m_NormalCoordinates[i].y);
					}

					if (proprieties[1])
					{
						data.push_back(m_TextureCoordinates[i].x);
						data.push_back(m_TextureCoordinates[i].y);
					}

					if (proprieties[2])
					{
						data.push_back(m_Colors[i].x);
						data.push_back(m_Colors[i].y);
						data.push_back(m_Colors[i].z);
						data.push_back(m_Colors[i].w);
					}

					if (proprieties[3])
					{
						data.push_back(m_Tangents[i].x);
						data.push_back(m_Tangents[i].y);
						data.push_back(m_Tangents[i].z);
					}

					if (proprieties[4])
					{
						data.push_back(m_Bitangents[i].x);
						data.push_back(m_Bitangents[i].y);
						data.push_back(m_Bitangents[i].z);
					}
				}

				AddBindable(VertexBuffer::Create(data.data(), (uint)data.size() * sizeof(float), strideVertex, m_Name));
			}

			AddBindable(IndexBuffer::Create(m_Indecies.data(), (uint)m_Indecies.size() * sizeof(unsigned short), m_Name));

			AT_CORE_ASSERT(!(settings.textureFlags > 0 && !proprieties[1]), "Tried to request textures without adding texture coordinates");
			AT_CORE_ASSERT(!(settings.textureFlags == 0 && proprieties[1]), "Tried to request textures without specifying what textures");

			if (proprieties[1] && settings.textureFlags > 0)
			{
				int slot = 0;
				for (int i = 0; i < MAX_MESH_TEXTURE_FLAGS_SIZE; i++)
				{
					//std::shared_ptr<Texture> texture = GetTexture((aiTextureType)i, slot);
					if ((settings.textureFlags & 1 << i) == 1 << i)
					{
						AddBindable(GetTexture((aiTextureType)i, slot, settings));
						AddBindable(Sampler::Create(settings.addMipMapping, settings.addAnisotropicFiltering, settings.maxAnisotropy, slot));
						slot++;
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

			AddBindable(InputLayout::Create(elements, blob));

			Graphics::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			Graphics::SetRenderTarget();

			if (DirectX::XMMatrixIsNaN(settings.viewMatrix))
			{
				AddBindable(TransformationConstantBuffer::Create(*this, DirectX::XMMatrixIdentity()));
			}
			else
			{
				AddBindable(TransformationConstantBuffer::Create(*this, settings.viewMatrix));
			}

			m_Set = true;
		}

		Drawable::Draw();
	}

	std::shared_ptr<Texture> Mesh::GetTexture(aiTextureType textureType, int slot, ModelDrawSettings& settings)
	{
		aiString path;

		if (m_Material->GetTexture(textureType,0, &path) == aiReturn::aiReturn_SUCCESS)
		{
			return std::move(Texture::Create(m_Filepath + path.C_Str(), settings.addMipMapping, slot));
		}
		else
		{
			AT_CRITICAL("The Mesh " + m_Name + " doesn't have the selected texture type:" + std::to_string((uint)textureType) + "So it has been replaced with a blank texture to avoid failure");
			uint data = 0xffffffff;
			return std::move(Texture::Create(1, 1, &data, slot));
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
		}
	}

	Model::Model(std::string path)
	{
		auto scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
		AT_ASSERT(scene, m_Importer.GetErrorString());

		m_Meshes.reserve(scene->mNumMeshes);

		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			m_Meshes.push_back(std::move(std::make_unique<Mesh>(scene->mMeshes[i], scene->mMaterials, path)));
		}

		if (scene->HasCameras())
		{
			aiMatrix4x4 camera;
			scene->mCameras[0]->GetCameraMatrix(camera);
			m_Camera = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&camera));
		}
		else
		{
			m_Camera = DirectX::XMMatrixIdentity();
		}
		
		if (scene->HasTextures())
		{
			AT_WARN("The model posseses embedded textures, which cannot be accessed by the library")
		}

		m_RootNode = ParseNode(scene->mRootNode);
	}

	void Model::Draw(ModelDrawSettings& settings, DirectX::XMMATRIX transform)
	{
		auto trans = m_Camera * transform;
		m_RootNode->Draw(trans, settings);
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