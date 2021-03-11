#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <DirectXMath.h>

#include "Graphics/AssimpWrapper/Mesh.h"
#include "Graphics/AssimpWrapper/Node.h"

#include "Graphics/RenderGraphAPI/Technique.h"

namespace Atlas
{
	//A wrapper over the aiScene
	//It represents a scene graph
	class Scene
	{
	public:
		//Constructor, requires the path to the model
		Scene(std::string path);

		//Draws the scene with a draw settings and a transform
		void Draw(ModelDrawSettings& settings, DirectX::XMMATRIX& transform);
		//Draws the scene only with draw settings
		void Draw(ModelDrawSettings& settings);

		//A way to apply a transform to a specific node
		//By using it's name
		void ApplyTransform(std::string nodeName, DirectX::XMMATRIX& transform);

		//Adds a technique to the entire scene
		void AddTechnique(Technique& technique);
		//Links the entire scene to a render graph
		void LinkTechniques(RenderGraph& renderGraph);

		//Add nodes recursively
		std::unique_ptr<Node> ParseNode(aiNode* node);
	private:
		//The main camera of the scene
		DirectX::XMMATRIX m_Camera;
		//The root node of the scene
		std::unique_ptr<Node> m_RootNode;
		//A owner vector for the meshes
		std::vector<std::unique_ptr<Mesh>> m_Meshes;
		//A reference to the importer
		Assimp::Importer m_Importer;
	};
}