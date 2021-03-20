#pragma once
#include <assimp/scene.h>
#include "Graphics/Drawable.h"
#include "Graphics/AssimpWrapper/CommonClasses.h"

namespace Atlas
{
	class Texture;

	//A wrapper over an aimesh
	class Mesh : public Drawable
	{
	public:
		//Constructor, takes in an aimesh, aimaterial array and the path to the mesh
		Mesh(aiMesh* mesh, aiMaterial** materials, std::filesystem::path path);

		//The draw function that takes the accumulated transforms and the settings
		void Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings);

		//The transform of the transform constant buffer
		DirectX::XMMATRIX GetTransform() override { return m_Transform; }

		//Wraps the add technique method to allow the outside to add the technique to the mesh
		void AddTechniqueWrapper(Technique& technique) { AddTechnique(technique); }
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
		std::vector<unsigned short> m_Indecies;

		//The transform
		DirectX::XMMATRIX m_Transform;

		//If the mesh has previously been constructed
		bool m_Set;
	};
}