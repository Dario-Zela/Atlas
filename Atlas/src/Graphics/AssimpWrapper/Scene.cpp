#include "pch.h"
#include "Scene.h"
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/RenderGraphAPI/RenderGraph.h"

#include "Graphics/AssimpWrapper/Node.h"
#include "Graphics/AssimpWrapper/Mesh.h"

namespace Atlas
{
	Scene::Scene(const std::string& path)
	{
		//Get the scene with triangles only, and calculating tangents and bi-tangents when necessary.
		auto scene = m_Importer->ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
		AT_CORE_ASSERT(scene, m_Importer->GetErrorString());	//If the result is invalid, the error is outputted

		//Reserve the space for the meshes
		m_Meshes.reserve(scene->mNumMeshes);

		//Then create them
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			AT_CORE_ATTEMPT(m_Meshes.push_back(std::move(std::make_unique<Mesh>(scene->mMeshes[i], scene->mMaterials, path))));
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
			AT_CORE_WARN("The model possesses embedded textures, which cannot be accessed by the library")
		}

		//Then get the root node
		m_RootNode = ParseNode(scene->mRootNode);
	}

	void Scene::Draw(ModelDrawSettings& settings, DirectX::XMMATRIX& transform)
	{
		AT_CORE_ASSERT(!DirectX::XMMatrixIsNaN(transform), "The transform is null")

		auto trans = m_Camera * transform;

		//Draw the root node
		m_RootNode->Draw(trans, settings);
	}

	void Scene::Draw(ModelDrawSettings& settings)
	{
		//Draw the root node
		m_RootNode->Draw(m_Camera, settings);
	}

	void Scene::ApplyTransform(const std::string& nodeName, DirectX::XMMATRIX& transform)
	{
		//Use recursive node traversal to apply the transform
		//If it returns false, it has failed, and an error should be returned
		AT_CORE_ASSERT(m_RootNode->ApplyTransform(nodeName, transform), "The selected node doesn't exist")
	}

	void Scene::AddTechnique(Technique& technique)
	{
		//For each mesh, add the technique
		for (auto& mesh : m_Meshes)
		{
			mesh->AddTechniqueWrapper(technique);
		}
	}

	void Scene::LinkTechniques(RenderGraph& renderGraph)
	{
		//For each mesh, link them to the render graph
		for (auto& mesh : m_Meshes)
		{
			mesh->LinkTechniques(renderGraph);
		}
	}

	std::unique_ptr<Node> Scene::ParseNode(aiNode* node)
	{
		AT_CORE_ASSERT(node, "An error seems to have occurred while reading the library, \nThe nodes are not being recognise, try again or with a different scene")

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

		//Recursively add it's children
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			returnNode->AddChild(ParseNode(node->mChildren[i]));
		}

		//Return the node
		return returnNode;
	}
}