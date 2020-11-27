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

		void AddTechnique(Technique& technique);
		void LinkTechniques(RenderGraph& renderGraph);

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