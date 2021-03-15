#pragma once
#include <DirectXMath.h>
#include "Graphics/RenderGraphAPI/Technique.h"

#include "Graphics/AssimpWrapper/CommonClasses.h"

namespace Assimp
{
	class Importer;
}

struct aiNode;

namespace Atlas
{

	class Node;
	class Mesh;

	//A wrapper over the aiScene
	//It represents a scene graph
	class Scene
	{
	public:
		//Constructor, requires the path to the model
		Scene(const std::string& path);

		//Draws the scene with a draw settings and a transform
		void Draw(ModelDrawSettings& settings, DirectX::XMMATRIX& transform);
		//Draws the scene only with draw settings
		void Draw(ModelDrawSettings& settings);

		//A way to apply a transform to a specific node
		//By using it's name
		void ApplyTransform(const std::string& nodeName, DirectX::XMMATRIX& transform);

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
		Assimp::Importer* m_Importer;
	};
}