#include "pch.h"
#include "Node.h"
#include "Graphics/AssimpWrapper/Mesh.h"

namespace Atlas
{
	Node::Node(const std::string& name, std::vector<Mesh*> meshes, const DirectX::XMMATRIX& tranform)
		: m_Name(name), m_Meshes(std::move(meshes))
	{
		//Store the original transforms
		DirectX::XMStoreFloat4x4(&m_Transform, tranform);
		DirectX::XMStoreFloat4x4(&m_AppliedTransform, DirectX::XMMatrixIdentity());
	}

	void Node::SetAppliedTranform(const DirectX::XMMATRIX& tranform)
	{
		//Sets the applied tranform
		DirectX::XMStoreFloat4x4(&m_AppliedTransform, tranform);
	}

	DirectX::XMFLOAT4X4& Node::GetAppliedTranfrom()
	{
		return m_AppliedTransform;
	}

	void Node::Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings)
	{
		//Calculate the accumulated treansform
		DirectX::XMMATRIX transform = DirectX::XMLoadFloat4x4(&m_Transform) * DirectX::XMLoadFloat4x4(&m_AppliedTransform) * accumulatedTransform;

		//Draw each mesh
		for (auto& mesh : m_Meshes)
		{
			mesh->Draw(transform, settings);
		}
		//Then draw each child
		for (auto& child : m_Children)
		{
			child->Draw(transform, settings);
		}
	}
}
