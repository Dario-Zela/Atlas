#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/D3DWrappers/Buffers.h"
#include <DirectXMath.h>
#include "Graphics/D3DWrappers/Texture.h"
#include "Graphics/Drawable.h"

namespace Atlas
{
	struct ModelDrawSettings
	{
		uint proprietiesFlags = 0;
		uint textureFlags = 0;
		std::string vertexShaderPath;
		std::string pixelShaderPath;
		bool addBlending = false;
		bool reSet = false;
		bool addMipMapping = false;
		bool addAnisotropicFiltering = false;
		uint maxAnisotropy = 0;
		DirectX::XMMATRIX viewMatrix;
	};

	enum class MeshTextureFlags
	{
		NONE = 1 << 0,
		DIFFUSE = 1 << 1,
		SPECULAR = 1 << 2,
		AMBIENT = 1 << 3,
		EMISSIVE = 1 << 4,
		HEIGHT = 1 << 5,
		NORMALS = 1 << 6,
		SHININESS = 1 << 7,
		OPACITY = 1 << 8,
		DISPLACEMENT = 1 << 9,
		LIGHTMAP = 1 << 10,
		REFLECTION = 1 << 11,
		BASE_COLOR = 1 << 12,
		NORMAL_CAMERA = 1 << 13,
		EMISSION_COLOR = 1 << 14,
		METALNESS = 1 << 15,
		DIFFUSE_ROUGHNESS = 1 << 16,
		AMBIENT_OCCLUSION = 1 << 17,
		UNKNOWN = 1 << 18
	};

	inline MeshTextureFlags operator|(MeshTextureFlags a, MeshTextureFlags b) { return (MeshTextureFlags)((uint)a | (uint)b); }

	inline uint operator|=(uint a, MeshTextureFlags b) { return (a | (uint)b); }

	#define MAX_MESH_TEXTURE_FLAGS_SIZE 19

	enum class MeshProprietiesFlags
	{
		NONE = 1 << 0,
		NORMAL_COORDINATES = 1 << 1,
		TEXTURE_COORDINATES = 1 << 2,
		COLORS = 1 << 3,
		TANGENTS = 1 << 4,
		BITANGENTS = 1 << 5,
		COLOR_DIFFUSE = 1 << 6,
		COLOR_SPECULAR = 1 << 7,
		COLOR_AMBIENT = 1 << 8,
		COLOR_EMISSIVE = 1 << 9,
		COLOR_TRANSPARENT = 1 << 10,
		WIREFRAME = 1 << 11,
		TWOSIDED = 1 << 12,
		SHADING_MODEL = 1 << 13,
		BLEND_FUNC = 1 << 14,
		OPACITY = 1 << 15,
		SHININESS = 1 << 16,
		SHININESS_STRENGTH = 1 << 17,
		REFRACTI = 1 << 18,
	};

	inline MeshProprietiesFlags operator|(MeshProprietiesFlags a, MeshProprietiesFlags b) { return (MeshProprietiesFlags)((uint)a | (uint)b); }

	inline uint operator|=(uint a, MeshProprietiesFlags b) { return (a | (uint)b); }

	class Mesh : Drawable
	{
	public:
		Mesh(aiMesh* mesh, aiMaterial** materials, std::filesystem::path path);

		void Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings);
		DirectX::XMMATRIX GetTransformXM() override 
		{ 
			return m_Transform;
		}

	private:
		std::shared_ptr<Texture> GetTexture(aiTextureType textureType, int slot, ModelDrawSettings& settings);

		std::vector<DirectX::XMFLOAT3> m_VertexPositions;
		std::vector<DirectX::XMFLOAT2> m_NormalCoordinates;
		std::vector<DirectX::XMFLOAT2> m_TextureCoordinates;
		std::vector<DirectX::XMFLOAT4> m_Colors;
		std::vector<DirectX::XMFLOAT3> m_Tangents;
		std::vector<DirectX::XMFLOAT3> m_Bitangents;
		std::string m_Name;
		aiMaterial* m_Material;
		std::string m_Filepath;

		DirectX::XMMATRIX m_Transform;

		std::vector<unsigned short> m_Indecies;

		bool m_Set;
 	};

	class Node
	{
	public:
		Node(const std::string& name, std::vector<Mesh*> meshes, const DirectX::XMMATRIX& tranform);
		void SetAppliedTranform(const DirectX::XMMATRIX& tranform);
		DirectX::XMFLOAT4X4& GetAppliedTranfrom();
		void AddChild(std::unique_ptr<Node> child) { m_Children.push_back(std::move(child)); }
		void Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings);

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

		void Draw(ModelDrawSettings& settings, DirectX::XMMATRIX transform);
		std::unique_ptr<Node> ParseNode(aiNode* node);
	private:
		DirectX::XMMATRIX m_Camera;
		std::unique_ptr<Node> m_RootNode;
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		Assimp::Importer m_Importer;
	};
}