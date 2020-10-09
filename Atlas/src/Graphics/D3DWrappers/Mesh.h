#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/D3DWrappers/Buffers.h"
#include <DirectXMath.h>
#include "Graphics/D3DWrappers/Texture.h"

namespace Atlas
{
	class Mesh
	{
	public:
		Mesh(aiMesh* mesh, aiMaterial* materials, std::filesystem::path path);

		std::vector<DirectX::XMFLOAT3> GetVertexPositions() { return m_VertexPositions; }
		std::vector<DirectX::XMFLOAT2> GetNormalCoordinates() { return m_NormalCoordinates; }
		std::vector<DirectX::XMFLOAT2> GetTextureCoordinates() { return m_TextureCoordinates; }
		std::vector<DirectX::XMFLOAT4> GetColors() { return m_Colors; }
		std::vector<DirectX::XMFLOAT3> GetTangents() { return m_Tangents; }
		std::vector<DirectX::XMFLOAT3> GetBitangents() { return m_Bitangents; }
		std::string GetName() { return m_Name; }

		std::shared_ptr<Texture> GetTexture(uint textureType);

	private:
		std::vector<DirectX::XMFLOAT3> m_VertexPositions;
		std::vector<DirectX::XMFLOAT2> m_NormalCoordinates;
		std::vector<DirectX::XMFLOAT2> m_TextureCoordinates;
		std::vector<DirectX::XMFLOAT4> m_Colors;
		std::vector<DirectX::XMFLOAT3> m_Tangents;
		std::vector<DirectX::XMFLOAT3> m_Bitangents;
		std::string m_Name;
		aiMaterial m_Material;
		std::string m_Filepath;

		std::vector<unsigned short> m_Indecies;
 	};

	class Node
	{
	public:
		Node(const std::string& name, std::vector<Mesh*> meshes, const DirectX::XMMATRIX& tranform);
		void SetAppliedTranform(const DirectX::XMMATRIX& tranform);
		DirectX::XMFLOAT4X4& GetAppliedTranfrom();
	private:
		std::string m_Name;
		std::vector<std::unique_ptr<Node>> m_Children;
		std::vector<Mesh*> m_Meshes;
		DirectX::XMFLOAT4X4 m_Transform;
		DirectX::XMFLOAT4X4 m_AppliedTransform;
	};

	class Model
	{
	public:
		Model(std::string path);

	private:
		Node m_RootNode;
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
	};
}