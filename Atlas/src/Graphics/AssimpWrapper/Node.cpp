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
		//Sets the applied transform
		DirectX::XMStoreFloat4x4(&m_AppliedTransform, tranform);
	}

	DirectX::XMFLOAT4X4& Node::GetAppliedTranfrom()
	{
		return m_AppliedTransform;
	}

	bool Node::ApplyTransform(const std::string& nodeName, DirectX::XMMATRIX& transform)
	{
		if (m_Name != nodeName)
		{
			if (m_Children.size() == 0)
				return false;
			else
			{
				for (auto& child : m_Children)
					if (child->ApplyTransform(nodeName, transform))
						return true;
			}
		}
		else
		{
			SetAppliedTranform(transform);
			return true;
		}

		AT_CORE_ASSERT(false, "The selected node doesn't exist")
	}

	void Node::Draw(DirectX::XMMATRIX& accumulatedTransform, ModelDrawSettings& settings)
	{
		//Calculate the accumulated transform
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
