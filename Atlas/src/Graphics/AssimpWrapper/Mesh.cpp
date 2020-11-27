#include "pch.h"
#include "Mesh.h"

#include "Graphics/D3DWrappers/Vertex.h"
#include "Graphics/D3DWrappers/InputLayout.h"
#include "Graphics/D3DWrappers/Texture.h"
#include "Graphics/D3DWrappers/TransformationConstantBuffer.h"

#include <filesystem>

namespace Atlas
{
	struct ProprietiesData
	{
		std::tuple<std::string, uint, int> Data[6] =
		{
			{ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT, sizeof(DirectX::XMFLOAT3)},
			{"NORMALS", DXGI_FORMAT_R32G32_FLOAT, sizeof(DirectX::XMFLOAT2)},
			{"TEXTURE_COORDS", DXGI_FORMAT_R32G32_FLOAT, sizeof(DirectX::XMFLOAT2)},
			{"COLORS", DXGI_FORMAT_R32G32B32A32_FLOAT, sizeof(DirectX::XMFLOAT4)},
			{"TANGENTS", DXGI_FORMAT_R32G32B32_FLOAT, sizeof(DirectX::XMFLOAT3)},
			{"BITANGENTS", DXGI_FORMAT_R32G32B32_FLOAT, sizeof(DirectX::XMFLOAT3)}
		};
	};

	static ProprietiesData s_ProprietiesData;

	Mesh::Mesh(aiMesh* mesh, aiMaterial** materials, std::filesystem::path path)
	{
		//Reserve the space for the vertex positions
		m_VertexPositions.reserve(mesh->mNumVertices);
		bool attributes[] = { mesh->HasTextureCoords(0), mesh->HasNormals(), mesh->HasVertexColors(0), mesh->HasTangentsAndBitangents() };

		//If they are present, reserve space for normals, colors, tangents and bitangents
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

		//Add the data
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

		//Save the material and filepath if it is present
		if (mesh->mMaterialIndex >= 0)
		{
			m_Material = materials[mesh->mMaterialIndex];
			m_Filepath = path.parent_path().string() + "\\";
		}

		//Save the name
		m_Name = mesh->mName.C_Str();

		//Add the indicies
		m_Indecies.reserve((size_t)mesh->mNumFaces * 3);

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
		//If the mesh has already been drawn
		//And the settings do not make it need to be redrawn
		//Only update the transform and draw the shape
		if (m_Set && !settings.reSet)
		{
			m_Transform = accumulatedTransform;
		}
		else
		{
			//Else clear the bindables
			ClearBindables();

			//Create the vertex and a reference to needed
			//settings
			Vertex vertex;
			bool proprieties[5];

			vertex.AddAttribute(s_ProprietiesData.Data[0]);

			//Iterate over every vertex element
			for (uint i = 1; i < 6; i++)
			{
				proprieties[i - 1] = false;

				//And add the appropriate data
				if ((settings.proprietiesFlags & (1 << i)) != 0)
				{
					vertex.AddAttribute(s_ProprietiesData.Data[i]);
					proprieties[i - 1] = true;
				}
			}

			{
				//Create a vector for the data
				//If the data is selected, it is added to the vertex
				for (int i = 0; i < m_VertexPositions.size(); i++)
				{
					std::vector<float> data;
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

					vertex.PushVertexData(data.data());
				}

				//And then a vertex buffer is created and added
				AddBindable(vertex.GetVertexBuffer(m_Name));
			}

			//Add a stride for the constant buffer and storage for the data
			uint strideCBuff = 0;
			std::vector<float> cbuffData;

			//Iterate over every subdata propriety
			for (uint i = 5; i < MAX_MESH_FLAGS_SIZE; i++)
			{
				//If the propriety has been activated
				if ((settings.proprietiesFlags & (1 << i)) != 0)
				{
					switch (i)
					{
						//Get the data and push it into the vector
						//Update the stride
					case 6:
					{
						aiColor3D data;
						m_Material->Get(AI_MATKEY_COLOR_DIFFUSE, data);
						if (data.IsBlack()) AT_WARN("The diffuse color of {0} is black, which is the default color, meaning that the value may be missing", m_Name)

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

						cbuffData.push_back((float)data);
						strideCBuff += sizeof(int);
					}
					break;
					case 12:
					{
						int data;
						m_Material->Get(AI_MATKEY_TWOSIDED, data);

						cbuffData.push_back((float)data);
						strideCBuff += sizeof(int);
					}
					break;
					case 13:
					{
						int data;
						m_Material->Get(AI_MATKEY_SHADING_MODEL, data);

						cbuffData.push_back((float)data);
						strideCBuff += sizeof(int);
					}
					break;
					case 14:
					{
						float data;
						m_Material->Get(AI_MATKEY_OPACITY, data);

						cbuffData.push_back((float)data);
						strideCBuff += sizeof(float);
					}
					break;
					case 15:
					{
						float data;
						m_Material->Get(AI_MATKEY_SHININESS, data);

						cbuffData.push_back(data);
						strideCBuff += sizeof(float);
					}
					break;
					case 16:
					{
						float data;
						m_Material->Get(AI_MATKEY_SHININESS_STRENGTH, data);

						cbuffData.push_back(data);
						strideCBuff += sizeof(float);
					}
					break;
					case 17:
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

			//If the stide is not 0 add a vertex constant buffer
			if (strideCBuff != 0) AddBindable(VertexConstantBuffer::Create(cbuffData.data(), strideCBuff));

			//The index buffer is created
			AddBindable(IndexBuffer::Create(m_Indecies.data(), (uint)m_Indecies.size() * sizeof(unsigned short), m_Name));

			//Checks that the textures are added propperly
			AT_CORE_ASSERT(!(settings.textureFlags > 0 && !proprieties[1]), "Tried to request textures without adding texture coordinates");
			AT_CORE_ASSERT(!(settings.textureFlags == 0 && proprieties[1]), "Tried to request textures without specifying what textures");

			//Add ther respective textures
			if (proprieties[1] && settings.textureFlags > 0)
			{
				//The slot is incremented to allow for more then one texture at a time
				int slot = 0;
				for (int i = 0; i < MAX_MESH_FLAGS_SIZE; i++)
				{
					if ((settings.textureFlags & 1 << i) == 1 << i)
					{
						//Texture and sampler are bound
						AddBindable(GetTexture((aiTextureType)i, slot, settings));
						slot++;
					}
				}
			}

			//If the view matrix is available, it is added to the transformation constant buffer
			if (DirectX::XMMatrixIsNaN(settings.viewMatrix))
			{
				AddBindable(TransformationConstantBuffer::Create(*this, DirectX::XMMatrixIdentity()));
			}
			else
			{
				AddBindable(TransformationConstantBuffer::Create(*this, settings.viewMatrix));
			}

			//Make the set flag to true
			m_Set = true;
		}

		//Submit the data
		Drawable::Submit();
	}

	std::shared_ptr<Texture> Mesh::GetTexture(aiTextureType textureType, int slot, ModelDrawSettings& settings)
	{
		aiString path;

		//If the material has the appropriate texture, it is added
		if (m_Material->GetTexture(textureType, 0, &path) == aiReturn::aiReturn_SUCCESS)
		{
			return std::move(Texture::Create(m_Filepath + path.C_Str(), settings.addMipMapping, slot));
		}
		//Else the user is warned and a white texture is used
		else
		{
			AT_CRITICAL("The Mesh " + m_Name + " doesn't have the selected texture type:" + std::to_string((uint)textureType) + "So it has been replaced with a blank texture to avoid failure");
			uint data = 0xffffffff;
			return std::move(Texture::Create(1, 1, &data, slot));
		}
	}
}