#pragma once
#include <DirectXMath.h>

namespace Atlas
{
	struct ModelDrawSettings;
	class Mesh;

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
}