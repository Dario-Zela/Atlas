#include "pch.h"
#include "Scene.h"
#include <filesystem>
#include "Graphics/RenderGraphAPI/RenderGraph.h"

namespace Atlas
{
	Scene::Scene(std::string path)
	{
		//Get the scene with triangles only, and calculating tangents and bitangents when necessary.
		auto scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
		AT_CORE_ASSERT(scene, m_Importer.GetErrorString());	//If the result is invalid, the error is outputted

		//Reserve the space for the meshes
		m_Meshes.reserve(scene->mNumMeshes);

		//Then create them
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			m_Meshes.push_back(std::move(std::make_unique<Mesh>(scene->mMeshes[i], scene->mMaterials, path)));
		}

		//If there is a camera store it
		if (scene->HasCameras())
		{
			aiMatrix4x4 camera;
			scene->mCameras[0]->GetCameraMatrix(camera);
			m_Camera = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&camera));
		}
		//Else, set it to identity
		else
		{
			m_Camera = DirectX::XMMatrixIdentity();
		}
		
		//If the scene has embedded textures, warn the user that they cannot be accessed
		if (scene->HasTextures())
		{
			AT_WARN("The model posseses embedded textures, which cannot be accessed by the library")
		}

		//Then get the root node
		m_RootNode = ParseNode(scene->mRootNode);
	}

	void Scene::Draw(ModelDrawSettings& settings, DirectX::XMMATRIX& transform)
	{
		auto trans = m_Camera * transform;
		//Draw the root node
		m_RootNode->Draw(trans, settings);
	}

	void Scene::Draw(ModelDrawSettings& settings)
	{
		//Draw the root node
		m_RootNode->Draw(m_Camera, settings);
	}

	void Scene::ApplyTransform(std::string nodeName, DirectX::XMMATRIX& transform)
	{
		AT_CORE_ASSERT(m_Nodes.find(nodeName) != m_Nodes.end(), "The selected node doesn't exist")
		m_Nodes[nodeName]->SetAppliedTranform(transform);
	}

	void Scene::AddTechnique(Technique& technique)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->AddTechnique(technique);
		}
	}

	void Scene::LinkTechniques(RenderGraph& renderGraph)
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->LinkTechniques(renderGraph);
		}
	}

	std::unique_ptr<Node> Scene::ParseNode(aiNode* node)
	{
		//Get the transform
		DirectX::XMMATRIX transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(
			reinterpret_cast<DirectX::XMFLOAT4X4*>(&node->mTransformation)));

		//Add a reference to the meshes that the node possessed
		std::vector<Mesh*> nodeMeshes;
		nodeMeshes.reserve(node->mNumMeshes);
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			auto meshIdx = node->mMeshes[i];
			nodeMeshes.push_back(m_Meshes.at(meshIdx).get());
		}

		//Make the node
		std::unique_ptr<Node> returnNode = std::make_unique<Node>(node->mName.C_Str(), std::move(nodeMeshes), transform);

		m_Nodes[node->mName.C_Str()] = returnNode.get();

		//Recursively add it's children
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			returnNode->AddChild(ParseNode(node->mChildren[i]));
		}

		//Return the node
		return returnNode;
	}
}