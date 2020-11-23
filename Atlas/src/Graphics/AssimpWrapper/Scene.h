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
	//Settings that define how a model should be drawn
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

	//The texture creation flags
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

	//The mesh propriety flags
	enum class MeshProprietiesFlags
	{
		NONE = 1 << 0,
		NORMAL_COORDINATES = 1 << 1,
		TEXTURE_COORDINATES = 1 << 2,
		COLORS = 1 << 3,
		TANGENTS = 1 << 4,
		BITANGENTS = 1 << 5,
		COLOR_DIFFUSE = 1 << 6,					//Diffuse color of the material. This is typically scaled by the amount of incoming diffuse light (e.g. using gouraud shading)	
		COLOR_SPECULAR = 1 << 7,				//Specular color of the material. This is typically scaled by the amount of incoming specular light (e.g. using phong shading)	
		COLOR_AMBIENT = 1 << 8,					//Ambient color of the material. This is typically scaled by the amount of ambient light	
		COLOR_EMISSIVE = 1 << 9,				//Emissive color of the material. This is the amount of light emitted by the object. In real time applications it will usually not affect surrounding objects, but raytracing applications may wish to treat emissive objects as light sources.	
		COLOR_TRANSPARENT = 1 << 10,			//Defines the transparent color of the material, this is the color to be multiplied with the color of translucent light to construct the final 'destination color' for a particular position in the screen buffer.
		WIREFRAME = 1 << 11,					//Specifies whether wireframe rendering must be turned on for the material. 0 for false, !0 for true.
		TWOSIDED = 1 << 12,						//Specifies whether meshes using this material must be rendered without backface culling. 0 for false, !0 for true.
		SHADING_MODEL = 1 << 13,				//One of the shadingMode enumerated values. Defines the library shading model to use for (real time) rendering to approximate the original look of the material as closely as possible.
		OPACITY = 1 << 14,						//Defines the opacity of the material in a range between 0-1
		SHININESS = 1 << 15,					//Defines the shininess of a phong-shaded material. This is actually the exponent of the phong specular equation
		SHININESS_STRENGTH = 1 << 16,			//Scales the specular color of the material.
		REFRACTI = 1 << 17,						//Defines the Index Of Refraction for the material. That's not supported by most file formats.
	};

	//////////////////////////////////////////////////////////////////
	//Overrided operators to simplify adding proprieties
	
	inline MeshTextureFlags operator|(MeshTextureFlags a, MeshTextureFlags b) { return (MeshTextureFlags)((uint)a | (uint)b); }

	inline uint operator|=(uint a, MeshTextureFlags b) { return (a | (uint)b); }

	//The maximum number of flags
	#define MAX_MESH_FLAGS_SIZE 19

	inline MeshProprietiesFlags operator|(MeshProprietiesFlags a, MeshProprietiesFlags b) { return (MeshProprietiesFlags)((uint)a | (uint)b); }

	inline uint operator|=(uint a, MeshProprietiesFlags b) { return (a | (uint)b); }

	//////////////////////////////////////////////////////////////////

	//A wrapper over an aimesh
	class Mesh : Drawable
	{
	public:
		//Constructor, takes in an aimesh, aimaterial array and the path to the mesh
		Mesh(aiMesh* mesh, aiMaterial** materials, std::filesystem::path path);

		//The draw function that takes the accumulated tranforms and the settings
		void Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings);
		
		//The transform of the tranform constant buffer
		DirectX::XMMATRIX GetTransformXM() override { return m_Transform; }
	private:
		//A function that gets the texture path from a type, a slot and the settings
		std::shared_ptr<Texture> GetTexture(aiTextureType textureType, int slot, ModelDrawSettings& settings);

		//The data of the mesh
		std::vector<DirectX::XMFLOAT3> m_VertexPositions;
		std::vector<DirectX::XMFLOAT2> m_NormalCoordinates;
		std::vector<DirectX::XMFLOAT2> m_TextureCoordinates;
		std::vector<DirectX::XMFLOAT4> m_Colors;
		std::vector<DirectX::XMFLOAT3> m_Tangents;
		std::vector<DirectX::XMFLOAT3> m_Bitangents;
		std::string m_Name;
		aiMaterial* m_Material;
		std::string m_Filepath;
		
		//The transform
		DirectX::XMMATRIX m_Transform;

		std::vector<unsigned short> m_Indecies;

		//If the mesh has previously been constructed
		bool m_Set;
 	};

	//A wrapper over an aiNode
	//It rappressents a node in a scene graph
	class Node
	{
	public:
		//Constructor, takes in the name of the node, the associated meshes and the tranform
		Node(const std::string& name, std::vector<Mesh*> meshes, const DirectX::XMMATRIX& tranform);

		//Setters and getters on the applied tranform
		void SetAppliedTranform(const DirectX::XMMATRIX& tranform);
		DirectX::XMFLOAT4X4& GetAppliedTranfrom();

		//Adds a child node to node
		void AddChild(std::unique_ptr<Node> child) { m_Children.push_back(std::move(child)); }

		//Draws the meshes associated with the node
		void Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings);
	private:
		std::string m_Name;
		std::vector<std::unique_ptr<Node>> m_Children;
		std::vector<Mesh*> m_Meshes;
		DirectX::XMFLOAT4X4 m_Transform;
		DirectX::XMFLOAT4X4 m_AppliedTransform;
	};

	//A wrapper over the aiScene
	//It rappresents a scene graph
	class Scene
	{
	public:
		//Constructor, requires the path to the model
		Scene(std::string path);

		//Draws the scene with a draw settings and a transform
		void Draw(ModelDrawSettings& settings, DirectX::XMMATRIX& transform);
		//Draws the scene only wirg draw settings
		void Draw(ModelDrawSettings& settings);

		//A way to apply a transform to a specific node
		//By using it's name
		void ApplyTransform(std::string nodeName, DirectX::XMMATRIX& transform);

		//Add nodes recurcively
		std::unique_ptr<Node> ParseNode(aiNode* node);
	private:
		DirectX::XMMATRIX m_Camera;
		std::unique_ptr<Node> m_RootNode;
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		Assimp::Importer m_Importer;
		std::unordered_map<std::string, Node*> m_Nodes;
	};
}