#pragma once
#include "pch.h"
#include <DirectXMath.h>
#include "Graphics/D3DWrappers/Buffers.h"

namespace Atlas
{
	struct NormalVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};

	//A wrapper over a list of indexed triangles, used to get a shape
	class IndexedTriangleList
	{
	public:
		//Constructor, takes a vector of vertices and a vector of indices
		IndexedTriangleList(std::vector<DirectX::XMFLOAT3> verteciesIn, std::vector<unsigned short> indeciesIn);

		//Allows you to transform all of the vertices by a matrix
		void Transform(DirectX::XMMATRIX transform);
		
		//Getters for the data
		std::vector<DirectX::XMFLOAT3> GetVertecies();
		std::vector<NormalVertex> GetNormalVertecies();
		std::vector<unsigned short> GetIndecies();
		
	private:

		void SetNormalsIndependentFlat();

		std::vector<DirectX::XMFLOAT3> m_Vertecies;
		std::vector<NormalVertex> m_NormalsVerticies;
		std::vector<unsigned short> m_Indecies;
	};

	//Creates a cube
	struct Cube
	{
		static IndexedTriangleList Make();

		static IndexedTriangleList MakeIndipendentFaces();
	};

	//Creates a plane
	struct Plane
	{
		//This is a tessellated plane
		//This allows for more vertices
		static IndexedTriangleList MakeTessalated(uint divisionsX, uint divisionsY);

		//The default has only 4 vertices
		static IndexedTriangleList Make();
	};

	//Creates a cone
	struct Cone
	{
		static IndexedTriangleList MakeTessalated(uint baseDivisions);

		//This makes all faces independent from each other
		static IndexedTriangleList MakeTessalatedIndipendentFaces(uint baseDivisions);

		//The default is a 24 tessellations cone
		static IndexedTriangleList Make();
	};

	//Creates a prism
	struct Prism
	{
		static IndexedTriangleList MakeTessalated(uint baseDivisions);

		//The default is a 24 tessellations prism
		static IndexedTriangleList Make();
	};

	//Creates a sphere
	struct Sphere
	{
		static IndexedTriangleList MakeTessalated(uint latitudeDivisions, uint longitudeDivisions);

		//The default is a 12 x 24 tessellations sphere
		static IndexedTriangleList Make();
	};

	//Gets a random shape
	static std::tuple<IndexedTriangleList, std::string> GetRandom(std::mt19937& rng)
	{
		switch (rng() % 4)
		{
		case 0: return { Cube::Make(), "Cube" };
		case 1: return { Prism::Make(), "Prism" };
		case 2: return { Cone::Make(), "Cone" };
		case 3: return { Sphere::Make(), "Sphere" };
		}
	}

	//Gets a random shape with independent vertices
	static std::tuple<IndexedTriangleList, std::string> GetRandomIndipendent(std::mt19937& rng)
	{
		switch (rng() % 4)
		{
		case 0: return { Cube::MakeIndipendentFaces(), "Cube" };
		case 1: return { Prism::Make(), "Prism" };
		case 2: return { Cone::MakeTessalatedIndipendentFaces(24), "Cone" };
		case 3: return { Sphere::Make(), "Sphere" };
		}
	}
}