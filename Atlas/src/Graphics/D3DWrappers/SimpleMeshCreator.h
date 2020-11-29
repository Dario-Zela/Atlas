#pragma once
#include "pch.h"
#include <DirectXMath.h>
#include "Graphics/D3DWrappers/Buffers.h"

namespace Atlas
{
	//A wrapper over a list of indexed triangles, used to get a shape
	class IndexedTriangleList
	{
	public:
		//Constructor, takes a vector of verticies and a vector of indicies
		IndexedTriangleList(std::vector<DirectX::XMFLOAT3> verteciesIn, std::vector<unsigned short> indeciesIn)
			:m_Vertecies(std::move(verteciesIn)), m_Indecies(std::move(indeciesIn))
		{
			AT_CORE_ASSERT(m_Vertecies.size() > 2, "Too few vertecies");
			AT_CORE_ASSERT(m_Indecies.size() % 3 == 0, "The objects rappresented are not triangles");
		}

		//Allows you to transfom all of the verticies by a matrix
		void Transform(DirectX::XMMATRIX transform)
		{
			for (auto& vertex : m_Vertecies)
			{
				const DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&vertex);
				DirectX::XMStoreFloat3(&vertex, DirectX::XMVector3Transform(position, transform));
			}
		}
		
		//Getters for the data
		std::vector<DirectX::XMFLOAT3> GetVertecies() { return m_Vertecies; }
		std::vector<unsigned short> GetIndecies() { return m_Indecies; }
		
	private:
		std::vector<DirectX::XMFLOAT3> m_Vertecies;
		std::vector<unsigned short> m_Indecies;
	};

	//Creates a cube
	struct Cube
	{
		static IndexedTriangleList Make()
		{
			constexpr float side = 0.5f;
			std::vector<DirectX::XMFLOAT3> verticies(8);

			verticies[0] = { -side, -side, -side };
			verticies[1] = {  side, -side, -side };
			verticies[2] = { -side,  side, -side };
			verticies[3] = {  side,  side, -side };
			verticies[4] = { -side, -side,  side };
			verticies[5] = {  side, -side,  side };
			verticies[6] = { -side,  side,  side };
			verticies[7] = {  side,  side,  side };

			return { std::move(verticies), {
						0,2,1,	2,3,1,
						1,3,5,	3,7,5,
						2,6,3,	3,6,7,
						4,5,7,	4,7,6,
						0,4,2,	2,4,6,
						0,1,4,	1,5,4 } };
		}

		static IndexedTriangleList MakeIndipendentFaces()
		{
			constexpr float side = 0.5f;
			std::vector<DirectX::XMFLOAT3> verticies(24);

			verticies[0] = { -side, -side, -side };
			verticies[1] = { side, -side, -side };
			verticies[2] = { -side,  side, -side };
			verticies[3] = { side,  side, -side };
			verticies[4] = { -side, -side,  side };
			verticies[5] = { side, -side,  side };
			verticies[6] = { -side,  side,  side };
			verticies[7] = { side,  side,  side };
			verticies[8] = { -side, -side, -side };
			verticies[9] = { -side, side, -side };
			verticies[10] = { -side,  -side, side };
			verticies[11] = { -side,  side, side };
			verticies[12] = { side, -side,  -side };
			verticies[13] = { side, side, -side };
			verticies[14] = { side,  -side,  side };
			verticies[15] = { side,  side,  side };
			verticies[16] = { -side, -side, -side };
			verticies[17] = { side, -side, -side };
			verticies[18] = { -side,  -side, side };
			verticies[19] = { side,  -side, side };
			verticies[20] = { -side, side,  -side };
			verticies[21] = { side, side,  -side };
			verticies[22] = { -side,  side,  side };
			verticies[23] = { side,  side,  side };

			return { std::move(verticies), {
						0,2,1,		2,3,1,
						4,5,7,		4,7,6,
						8,10,9,		10,11,9,
						12,13,15,	12,15,14,
						16,17,18,	18,17,19,
						20,23,21,	20,22,23} };
		}
	};

	//Creates a plane
	struct Plane
	{
		//This is a tesselated plane
		//This allows for more vertecies
		static IndexedTriangleList MakeTessalated(uint divisionsX, uint divisionsY)
		{
			AT_CORE_ASSERT(divisionsX != 0, "Cannot make a plane with 0 tessellation");
			AT_CORE_ASSERT(divisionsY != 0, "Cannot make a plane with 0 tessellation");

			constexpr float width = 2.0f;
			constexpr float height = 2.0f;
			const uint nVerteciesX = divisionsX + 1;
			const uint nVerteciesY = divisionsY + 1;
			std::vector<DirectX::XMFLOAT3> vertecies(nVerteciesX * nVerteciesY);

			{
				const float divisionSizeX = width / (float)divisionsX;
				const float divisionSizeY = height / (float)divisionsY;
				const auto boottomLeftVertex = DirectX::XMVectorSet(-width / 2.0f, -height / 2.0f, 0, 0);

				for (uint y = 0, i = 0; y < nVerteciesY; y++)
				{
					const float yPos = (float)y * divisionSizeY;
					for (uint x = 0; x < nVerteciesX; x++, i++)
					{
						const auto vertex = DirectX::XMVectorAdd(boottomLeftVertex, DirectX::XMVectorSet((float)x * divisionSizeX, yPos, 0, 0));
						DirectX::XMStoreFloat3(&vertecies[i], vertex);
					}
				}
			}

			std::vector<unsigned short> indecies;
			indecies.reserve((size_t)divisionsX * divisionsY * divisionsX * divisionsY * 6);
			{
				const auto getIndex = [nVerteciesX](size_t x, size_t y)
				{
					return (unsigned short)(y * nVerteciesX + x);
				};
				for (size_t y = 0; y < divisionsY; y++)
					for (size_t x = 0; x < divisionsX; x++)
					{
						const unsigned short indexes[] = { getIndex(x,y), getIndex(x + 1,y), getIndex(x,y + 1), getIndex(x + 1,y + 1) };
						indecies.push_back(indexes[0]);
						indecies.push_back(indexes[2]);
						indecies.push_back(indexes[1]);
						indecies.push_back(indexes[1]);
						indecies.push_back(indexes[2]);
						indecies.push_back(indexes[3]);
					}
			}
			return { std::move(vertecies), std::move(indecies) };
		}

		//The default has only 4 vertecies
		static IndexedTriangleList Make()
		{
			return MakeTessalated(1, 1);
		}
	};

	//Creates a cone
	struct Cone
	{
		static IndexedTriangleList MakeTessalated(uint baseDivisions)
		{
			AT_CORE_ASSERT(baseDivisions >= 3, "Cannot make a cone with less then 3 base vertecies");

			const auto base = DirectX::XMVectorSet(1, 0, -1, 0);
			const float longitudeAngle = DirectX::XM_2PI / baseDivisions;
			std::vector<DirectX::XMFLOAT3> vertecies;

			//The base
			for (uint iBase = 0; iBase < baseDivisions; iBase++)
			{
				vertecies.emplace_back();
				auto vertex = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iBase));
				DirectX::XMStoreFloat3(&vertecies.back(), vertex);
			}

			//The centre
			vertecies.emplace_back();
			vertecies.back() = { 0,0,-1 };
			const unsigned short iCenter = (unsigned short)(vertecies.size() - 1);

			//The top
			vertecies.emplace_back();
			vertecies.back() = { 0,0,1 };
			const unsigned short iTop = (unsigned short)(vertecies.size() - 1);

			//Base Indecies
			std::vector<unsigned short> indecies;
			for (unsigned short iBase = 0; iBase < baseDivisions; iBase++)
			{
				indecies.push_back(iCenter);
				indecies.push_back((iBase + 1) % baseDivisions);
				indecies.push_back(iBase);
			}

			//Cone indecies
			for (unsigned short iBase = 0; iBase < baseDivisions; iBase++)
			{
				indecies.push_back(iBase);
				indecies.push_back((iBase + 1) % baseDivisions);
				indecies.push_back(iTop);
			}

			return { std::move(vertecies), std::move(indecies) };
		}

		//This makes all faces indipendent from each other
		static IndexedTriangleList MakeTessalatedIndipendentFaces(uint baseDivisions)
		{
			AT_CORE_ASSERT(baseDivisions >= 3, "Cannot make a cone with less then 3 base vertecies");

			const auto base = DirectX::XMVectorSet(1, 0, -1, 0);
			const float longitudeAngle = DirectX::XM_2PI / baseDivisions;
			std::vector<DirectX::XMFLOAT3> vertecies;

			//The cone
			const unsigned short iCone = (unsigned short)vertecies.size();
			for (uint iBase = 0; iBase < baseDivisions; iBase++)
			{
				const float thetas[] = {
					longitudeAngle * iBase,
					longitudeAngle * (((iBase + 1) == baseDivisions ? 0 : (iBase + 1))) };

				vertecies.emplace_back();
				vertecies.back() = { 0, 0, 1 };
				for (auto theta : thetas) 
				{
					vertecies.emplace_back();
					auto vertex = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(theta));
					DirectX::XMStoreFloat3(&vertecies.back(), vertex);
				}
			}

			//The base
			const auto iBaseCentre = (unsigned short)vertecies.size();
			vertecies.emplace_back();
			vertecies.back() = { 0,0,-1 };
			const unsigned short iBaseEdge = (unsigned short)vertecies.size();
			for (uint iBase = 0; iBase < baseDivisions; iBase++)
			{
				vertecies.emplace_back();
				auto vertex = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iBase));
				DirectX::XMStoreFloat3(&vertecies.back(), vertex);
			}

			//Cone Indecies
			std::vector<unsigned short> indecies;
			for (unsigned short i = 0; i < baseDivisions * 3; i++)
			{
				indecies.push_back(i + iCone);
			}

			//Base indecies
			for (unsigned short iBase = 0; iBase < baseDivisions; iBase++)
			{
				indecies.push_back(iBaseCentre);
				indecies.push_back((iBase + 1) % baseDivisions + iBaseEdge);
				indecies.push_back(iBase + iBaseEdge);
			}

			return { std::move(vertecies), std::move(indecies) };
		}

		//The default is a 24 tesselations cone
		static IndexedTriangleList Make()
		{
			return MakeTessalated(24);
		}
	};

	//Creates a prism
	struct Prism
	{
		static IndexedTriangleList MakeTessalated(uint baseDivisions)
		{
			AT_CORE_ASSERT(baseDivisions >= 3, "Cannot make a cone with less then 3 base vertecies");

			const auto base = DirectX::XMVectorSet(1, 0, -1, 0);
			const auto offset= DirectX::XMVectorSet(0, 0, 2, 0);
			const float longitudeAngle = DirectX::XM_2PI / baseDivisions;
			std::vector<DirectX::XMFLOAT3> vertecies;

			//Near Centre
			vertecies.emplace_back();
			vertecies.back() = { 0,0,-1 };
			const auto iCentreNear = (unsigned short)(vertecies.size() - 1);

			//Far Centre
			vertecies.emplace_back();
			vertecies.back() = { 0,0, 1 };
			const auto iCentreFar = (unsigned short)(vertecies.size() - 1);

			//Base
			for (uint iBase = 0; iBase < baseDivisions; iBase++)
			{
				//Near base
				{
					vertecies.emplace_back();
					auto vertex = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iBase));
					DirectX::XMStoreFloat3(&vertecies.back(), vertex);
				}
				
				//Far base
				{
					vertecies.emplace_back();
					auto vertex = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationZ(longitudeAngle * iBase));
					vertex = DirectX::XMVectorAdd(vertex, offset);
					DirectX::XMStoreFloat3(&vertecies.back(), vertex);
				}
			}

			//Side Indecies
			std::vector<unsigned short> indecies;
			for (unsigned short iBase = 0; iBase < baseDivisions; iBase++)
			{
				const auto i = iBase * 2;
				const auto mod = baseDivisions * 2;
				indecies.push_back(i + 2);
				indecies.push_back((i + 2) % mod + 2);
				indecies.push_back(i + 1 + 2);
				indecies.push_back((i + 2) % mod + 2);
				indecies.push_back((i + 3) % mod + 2);
				indecies.push_back(i + 1 +2);
			}

			//Base Indecies
			for (unsigned short iBase = 0; iBase < baseDivisions; iBase++)
			{
				const auto i = iBase * 2;
				const auto mod = baseDivisions * 2;
				indecies.push_back(i + 2);
				indecies.push_back(iCentreNear);
				indecies.push_back((i + 2) % mod + 2);
				indecies.push_back(iCentreFar);
				indecies.push_back(i + 1 + 2);
				indecies.push_back((i + 3) % mod + 2);
			}

			return { std::move(vertecies), std::move(indecies) };
		}

		//The default is a 24 tesselations prism
		static IndexedTriangleList Make()
		{
			return MakeTessalated(24);
		}
	};

	//Creates a sphere
	struct Sphere
	{
		static IndexedTriangleList MakeTessalated(uint latitudeDivisions, uint longitudeDivisions)
		{
			AT_CORE_ASSERT(latitudeDivisions >= 3, "Cannot make a sphere with less then 3 tessellation");
			AT_CORE_ASSERT(longitudeDivisions >= 3, "Cannot make a sphere with less then 3 tessellation");

			constexpr float radius = 1;
			const auto base = DirectX::XMVectorSet(0, 0, radius, 0);
			const float latitudeAngle = DirectX::XM_PI / (float)latitudeDivisions;
			const float longitudeAngle = DirectX::XM_2PI / (float)longitudeDivisions;

			std::vector<DirectX::XMFLOAT3> vertecies;

			for (uint iLatitude = 1; iLatitude < latitudeDivisions; iLatitude++)
			{
				const auto latitudeBase = DirectX::XMVector3Transform(
					base, DirectX::XMMatrixRotationX(latitudeAngle * iLatitude));

				for (uint iLongitude = 0; iLongitude < longitudeDivisions; iLongitude++)
				{
					vertecies.emplace_back();
					auto vertex = DirectX::XMVector3Transform(latitudeBase, DirectX::XMMatrixRotationZ(longitudeAngle * iLongitude));
					DirectX::XMStoreFloat3(&vertecies.back(), vertex);
				}
			}

			//Add the caps
			const auto iNorthPole = (unsigned short)vertecies.size();
			vertecies.emplace_back();
			DirectX::XMStoreFloat3(&vertecies.back(), base);

			const auto iSouthPole = (unsigned short)vertecies.size();
			vertecies.emplace_back();
			DirectX::XMStoreFloat3(&vertecies.back(), DirectX::XMVectorNegate(base));

			const auto getIndex = [latitudeDivisions, longitudeDivisions](unsigned short iLatitude, unsigned short iLongitude)
			{ return (unsigned short)(iLatitude * longitudeDivisions + iLongitude); };

			std::vector<unsigned short> indecies;;
			for (uint iLatitude = 0; iLatitude < latitudeDivisions - 2; iLatitude++)
			{
				for (uint iLongitude = 0; iLongitude < longitudeDivisions - 1; iLongitude++)
				{
					const unsigned short indexes[] = { getIndex(iLatitude, iLongitude), getIndex(iLatitude + 1,iLongitude), getIndex(iLatitude,iLongitude + 1), getIndex(iLatitude + 1,iLongitude + 1) };
					indecies.push_back(indexes[0]);
					indecies.push_back(indexes[1]);
					indecies.push_back(indexes[2]);
					indecies.push_back(indexes[2]);
					indecies.push_back(indexes[1]);
					indecies.push_back(indexes[3]);
				}

				//Last Band
				indecies.push_back(getIndex(iLatitude, longitudeDivisions - 1));
				indecies.push_back(getIndex(iLatitude + 1, longitudeDivisions - 1));
				indecies.push_back(getIndex(iLatitude, 0));
				indecies.push_back(getIndex(iLatitude, 0));
				indecies.push_back(getIndex(iLatitude + 1, longitudeDivisions - 1));
				indecies.push_back(getIndex(iLatitude + 1, 0));
			}

			//Caps
			for (uint iLongitude = 0; iLongitude < longitudeDivisions - 1; iLongitude++)
			{
				//North Pole
				indecies.push_back(iNorthPole);
				indecies.push_back(getIndex(0, iLongitude));
				indecies.push_back(getIndex(0, iLongitude + 1));
				
				//South Pole
				indecies.push_back(getIndex(latitudeDivisions - 2, iLongitude + 1));
				indecies.push_back(getIndex(latitudeDivisions - 2, iLongitude));
				indecies.push_back(iSouthPole);
			}

			//Wrapping Triangles
			//North Pole
			indecies.push_back(iNorthPole);
			indecies.push_back(getIndex(0, longitudeDivisions - 1));
			indecies.push_back(getIndex(0, 0));

			//South Pole
			indecies.push_back(getIndex(latitudeDivisions - 2, 0));
			indecies.push_back(getIndex(latitudeDivisions - 2, longitudeDivisions - 1));
			indecies.push_back(iSouthPole);

			return { std::move(vertecies), std::move(indecies) };
		}

		//The default is a 12 x 24 tesselations sphere
		static IndexedTriangleList Make()
		{
			return MakeTessalated(12, 24);
		}
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

	//Gets a random shape with indipendent verticies
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