#include "pch.h"
#include "Renderer2D.h"
#include "Graphics/Graphics.h"

#include "Graphics/InputLayout.h"
#include "Graphics/Shaders.h"
#include "Graphics/Buffers.h"
#include "Graphics/ViewPort.h"

#include <fstream>

namespace Atlas
{
	static std::string FontToText(Font font)
	{
		switch (font)
		{
		case Font::Verdana:
			return "Verdana";
		case Font::TimesNewRoman:
			return "TimesNewRoman";
		case Font::FixedDsys:
			return "FixedDsys";
		default:
			AT_CORE_ASSERT(false, "Unknown Font Used");
			return "";
		}
	}

	struct QuadVertex
	{
		dx::XMVECTOR position;
		//dx::XMFLOAT2 texCoord;
		dx::XMVECTOR color;
		//float textIndex;
		//float textureScale;
	};

	struct Character
	{
		char value;
		dx::XMFLOAT2 texCoords[4];
		dx::XMFLOAT2 offset;
		float advance;
		dx::XMFLOAT2 size;
	};

	struct FontCommon
	{
		float lineHeight;
		float base;
	};

	struct CharacterSheet
	{
		Character characters[95];
		FontCommon commons;
	};

	struct Renderer2DStorage
	{
		const unsigned int MaxQuads = 10000;
		const unsigned int MaxVerteces = MaxQuads * 4;
		const unsigned int MaxIndeces= MaxQuads * 6;
		//static const unsigned int MaxTextureSlots = 32;

		VertexShader* vertexShader;
		PixelShader* pixelShader;
		InputLayout* inputLayout;
		IndexBuffer* indexBuffer;

		unsigned int QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		//Texture2D* TextureSlots[MaxTextureSlots];
		//unsigned int TextureIndex = 0;
		
		//Texture2D* FontTex[SUPPORTED_FONTS];
		//CharacterSheet FontCharacters[SUPPORTED_FONTS];
	};

	static Renderer2DStorage sStorage;

	void Renderer2D::Init()
	{
		sStorage.vertexShader = new VertexShader("TestVertex.cso");
		sStorage.pixelShader = new PixelShader("TestPixel.cso");

		sStorage.inputLayout = new InputLayout(
			{
				{ "aPosition", DXGI_FORMAT_R32G32B32A32_FLOAT, 0},
				//{ "aTexCoord", DXGI_FORMAT_R32G32_FLOAT, 0},
				{ "aColor", DXGI_FORMAT_R32G32B32A32_FLOAT, 0},
				//{"aTexIndex", DXGI_FORMAT_R32_FLOAT, 0},
				//{"aTexScale", DXGI_FORMAT_R32_FLOAT, 0}
			}, sStorage.vertexShader->GetBlob());

		sStorage.QuadVertexBufferBase = new QuadVertex[sStorage.MaxVerteces];

		unsigned short* quadIndeces = new unsigned short[sStorage.MaxIndeces];

		unsigned int offset = 0;
		for (unsigned int i = 0; i < sStorage.MaxIndeces;)
		{
			quadIndeces[i++] = offset + 0;
			quadIndeces[i++] = offset + 1;
			quadIndeces[i++] = offset + 2;

			quadIndeces[i++] = offset + 2;
			quadIndeces[i++] = offset + 3;
			quadIndeces[i++] = offset + 0;

			offset += 4;
		}

		sStorage.indexBuffer = new IndexBuffer(quadIndeces, sStorage.MaxIndeces);

		delete[] quadIndeces;

		/*
		int samplers[sStorage.MaxTextureSlots];
		for (int i = 0; i < sStorage.MaxTextureSlots; i++)
			samplers[i] = i;

		sStorage.Shader->SetIntArray("uTexture", samplers, sStorage.MaxTextureSlots);

		for (int i = 0; i < SUPPORTED_FONTS; i++)
		{
			Font font = (Font)i;
			sStorage.FontTex[i] = new OpenGLTexture2D("assets/Fonts/" + FontToText(font) + ".png");
			CompileCharSheet(ReadFontFile(font), &sStorage.FontCharacters[i]);
		}
		*/
	}

	void Renderer2D::Shutdown()
	{
		/*
		for (int i = 0; i < SUPPORTED_FONTS; i++)
		{
			if(sStorage.FontTex[i])
				delete sStorage.FontTex[i];
		}
		*/

		delete sStorage.indexBuffer;
		delete sStorage.inputLayout;
		delete sStorage.pixelShader;
		delete sStorage.vertexShader;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera, ViewPort& viewPort)
	{
		sStorage.pixelShader->Bind();
		sStorage.vertexShader->Bind();
		ConstantBuffer cb((void*)&camera.GetViewProjectionMatrix(), sizeof(dx::XMMATRIX));
		cb.Bind();

		sStorage.indexBuffer->Bind(0);
		sStorage.inputLayout->Bind();

		Graphics::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Graphics::SetRenderTarget();

		viewPort.Bind();

		//sStorage.TextureIndex = 0;
		sStorage.QuadIndexCount = 0;
		sStorage.QuadVertexBufferPtr = sStorage.QuadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		unsigned int data = (unsigned int)((unsigned char*)sStorage.QuadVertexBufferPtr - (unsigned char*)sStorage.QuadVertexBufferBase);
		VertexBuffer vertexBuffer(sStorage.QuadVertexBufferBase, data, sizeof(QuadVertex));
		vertexBuffer.Bind(sizeof(QuadVertex), 0);

		/*
		for (unsigned int i = 0; i < sStorage.TextureIndex; i++)
		{
			sStorage.TextureSlots[i]->Bind(i);
		}
		*/

		Graphics::DrawIndexed(sStorage.indexBuffer->GetCount());
	}

	void Renderer2D::Flush()
	{
		EndScene();
		//sStorage.TextureIndex = 0;
		sStorage.QuadIndexCount = 0;
		sStorage.QuadVertexBufferPtr = sStorage.QuadVertexBufferBase;
	}

	void Renderer2D::DrawQuad(const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, const dx::XMVECTOR& color, float rotation)
	{
		DrawQuad(dx::XMFLOAT3(position.x, position.y, 0.0f), size, color, rotation);
	}

	void Renderer2D::DrawQuad(const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, const dx::XMVECTOR& color, float rotation)
	{
		/*
		if (sStorage.TextureIndex == sStorage.MaxTextureSlots || sStorage.QuadIndexCount == sStorage.MaxIndeces)
		{
			Flush();
		}
		*/

		dx::XMMATRIX transform = dx::XMMatrixScaling(size.x, size.y, 1.0f);
		if (rotation != 0.0f)
			transform *= dx::XMMatrixRotationZ(dx::XMConvertToRadians(rotation));
		transform *= dx::XMMatrixTranslation(position.x , position.y, position.z);

		dx::XMMATRIX positions = dx::XMMATRIX(-0.5f, -0.5f, 0.0f, 1.0f,
										 0.5f, -0.5f, 0.0f, 1.0f,
										 0.5f,  0.5f, 0.0f, 1.0f,
										-0.5f,  0.5f, 0.0f, 1.0f);
		
		positions = transform * positions;

		sStorage.QuadVertexBufferPtr->position = positions.r[0];
		//sStorage.QuadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		sStorage.QuadVertexBufferPtr->color = color;
		//sStorage.QuadVertexBufferPtr->textIndex = -1.0f;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[1];
		//sStorage.QuadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		sStorage.QuadVertexBufferPtr->color = color;
		//sStorage.QuadVertexBufferPtr->textIndex = -1.0f;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[2];
		//sStorage.QuadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		sStorage.QuadVertexBufferPtr->color = color;
		//sStorage.QuadVertexBufferPtr->textIndex = -1.0f;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[3];
		//sStorage.QuadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		sStorage.QuadVertexBufferPtr->color = color;
		//sStorage.QuadVertexBufferPtr->textIndex = -1.0f;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadIndexCount += 6;
	}

	/*
	void Renderer2D::DrawQuad(const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, Texture2D& texture, const dx::XMVECTOR& shade, float textureScale, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, shade, textureScale, rotation);
	}

	void Renderer2D::DrawQuad(const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, Texture2D& texture, const dx::XMVECTOR& shade, float textureScale, float rotation)
	{
		if (sStorage.TextureIndex == sStorage.MaxTextureSlots || sStorage.QuadIndexCount == sStorage.MaxQuads)
		{
			Flush();
		}

		dx::XMMATRIX transform = dx::XMMatrixScaling(size.x, size.y, 1.0f);
		if (rotation != 0.0f)
			transform *= dx::XMMatrixRotationZ(dx::XMConvertToRadians(rotation));
		transform *= dx::XMMatrixTranslation(position.x , position.y, position.z);

		dx::XMMATRIX positions = dx::XMMATRIX(-0.5f, -0.5f, 0.0f, 1.0f,
										 0.5f, -0.5f, 0.0f, 1.0f,
										 0.5f,  0.5f, 0.0f, 1.0f,
										-0.5f,  0.5f, 0.0f, 1.0f);

		positions = transform * positions;

		float textureIndex = -1.0f;

		for (unsigned int i = 0; i < sStorage.TextureIndex; i++)
		{
			if (*sStorage.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.0f)
		{
			textureIndex = (float)sStorage.TextureIndex;
			sStorage.TextureSlots[(int)sStorage.TextureIndex] = &texture;
			sStorage.TextureIndex++;
		}

		sStorage.QuadVertexBufferPtr->position = positions.r[0];
		//sStorage.QuadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = textureScale;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[1];
		//sStorage.QuadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = textureScale;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[2];
		//sStorage.QuadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = textureScale;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[3];
		//sStorage.QuadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = textureScale;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadIndexCount += 6;
	}

	void Renderer2D::DrawText(const char* text, const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, Font font, const dx::XMVECTOR& shade, float rotation)
	{
		DrawText(text, { position.x, position.y, 0.0f }, size, font, shade, rotation);
	}

	void Renderer2D::DrawText(const char* text, const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, Font font, const dx::XMVECTOR& shade, float rotation)
	{
		if (sStorage.TextureIndex == sStorage.MaxTextureSlots || sStorage.QuadIndexCount == sStorage.MaxQuads)
		{
			Flush();
		}

		dx::XMMATRIX transform = dx::XMMATRIX(1.0f);

		float textureIndex = -1.0f;
		float fontIndex = (float)(int)font;

		for (unsigned int i = 0; i < sStorage.TextureIndex; i++)
		{
			if (sStorage.TextureSlots[i] == sStorage.FontTex[(int)fontIndex])
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.0f)
		{
			textureIndex = (float)sStorage.TextureIndex;
			sStorage.TextureSlots[(int)sStorage.TextureIndex] = sStorage.FontTex[(int)fontIndex];
			sStorage.TextureIndex++;
		}

		if (rotation != 0.0f)
			transform = glm::rotate(transform, glm::radians(rotation), dx::XMFLOAT3(0, 0, 1));

		transform = glm::translate(transform, dx::XMFLOAT3(0.0f, sStorage.FontCharacters[(int)fontIndex].commons.base * size.y, 0.0f));
		dx::XMMATRIX lineCarrier(transform);

		for (int i = 0; i < std::string(text).length(); i++)
		{
			if (text[i] == '\n')
			{
				lineCarrier = glm::translate(lineCarrier, dx::XMFLOAT3(0.0f, sStorage.FontCharacters[(int)fontIndex].commons.lineHeight * size.y, 0.0f));
				transform = lineCarrier;
				continue;
			}

			DrawChar(text[i], transform, position, size, shade, textureIndex, fontIndex);
		}
	}
	*/
	void Renderer2D::DrawLine(const dx::XMFLOAT2& position1, const dx::XMFLOAT2& position2, float thickness, const dx::XMVECTOR& color)
	{
		float dx = position2.x - position1.x;
		float dy = position2.y - position1.y;
		float rotation = dx == 0 ? 90 : dx::XMConvertToRadians(std::atan(dy / dx));
		float lenght = std::sqrt(dx * dx + dy * dy);

		DrawQuad(dx::XMFLOAT2(dx/2.0f + position1.x, dy/2.0f + position1.y), dx::XMFLOAT2(lenght, thickness), color, rotation);
	}

	void Renderer2D::DrawRect(const dx::XMFLOAT2& position1, const dx::XMFLOAT2& position2, float thickness, const dx::XMVECTOR& color)
	{
		dx::XMFLOAT2 offset = dx::XMFLOAT2(thickness / 2.0f, 0.0f);
		DrawLine(dx::XMFLOAT2(position1.x - offset.x, position1.y - offset.y), dx::XMFLOAT2(position2.x + offset.x, position1.y + offset.y), thickness, color);
		DrawLine(position1, dx::XMFLOAT2(position1.x, position2.y), thickness, color);
		DrawLine(dx::XMFLOAT2(position2.x - offset.x, position2.y - offset.y), dx::XMFLOAT2(position1.x- +offset.x, position2.y - +offset.y), thickness, color);
		DrawLine(position2, dx::XMFLOAT2(position2.x, position1.y), thickness, color);
	}

	/*
	void Renderer2D::DrawTextureTile(const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, Texture2D& texture, unsigned int tilePosition, unsigned int tileSize, const dx::XMVECTOR& shade)
	{
		DrawTextureTile(dx::XMFLOAT3(position, 0.0f), size, texture, tilePosition, tileSize, shade);
	}

	void Renderer2D::DrawTextureTile(const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, Texture2D& texture, unsigned int tilePosition, unsigned int tileSize, const dx::XMVECTOR& shade)
	{
		if (sStorage.TextureIndex == sStorage.MaxTextureSlots || sStorage.QuadIndexCount == sStorage.MaxQuads)
		{
			Flush();
		}

		dx::XMMATRIX transform = dx::XMMatrixScaling(size.x, size.y, 1.0f);
		transform *= dx::XMMatrixTranslation(position.x , position.y, position.z);;

		dx::XMMATRIX positions = dx::XMMATRIX(-0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f);

		positions = transform * positions;

		float textureIndex = -1.0f;

		for (unsigned int i = 0; i < sStorage.TextureIndex; i++)
		{
			if (*sStorage.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == -1.0f)
		{
			textureIndex = (float)sStorage.TextureIndex;
			sStorage.TextureSlots[(int)sStorage.TextureIndex] = &texture;
			sStorage.TextureIndex++;
		}

		float textureH = (float)tileSize / (float)texture.GetHeight();
		float textureW = (float)tileSize / (float)texture.GetWidth();

		unsigned int xPosition = tilePosition / (texture.GetHeight() / tileSize);
		unsigned int yPosition = tilePosition % (texture.GetHeight() / tileSize);

		sStorage.QuadVertexBufferPtr->position = positions.r[0];
		//sStorage.QuadVertexBufferPtr->texCoord = { textureW * xPosition,textureH * yPosition };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[1];
		//sStorage.QuadVertexBufferPtr->texCoord = { textureW * (xPosition + 1),textureH * yPosition };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[2];
		//sStorage.QuadVertexBufferPtr->texCoord = { textureW * (xPosition + 1),textureH * (yPosition + 1) };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[3];
		//sStorage.QuadVertexBufferPtr->texCoord = { textureW * xPosition,textureH * (yPosition + 1) };
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadIndexCount += 6;
	}

	void Renderer2D::DrawChar(char c, dx::XMMATRIX& transform, const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, const dx::XMVECTOR& shade, float textureIndex, float fontIndex)
	{
		dx::XMMATRIX positions = dx::XMMATRIX(-0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 1.0f);

		Character data = Character();
		auto fontData = sStorage.FontCharacters[(int)fontIndex];

		for (auto font : fontData.characters)
		{
			if (c == font.value)
			{
				data = font;
				break;
			}
		}

		if (data.value != c)
		{
			return;
		}

		transform = glm::translate(transform, dx::XMFLOAT3(data.size.x * size.x * 0.5f, 0.0f, 0.0f));
		dx::XMMATRIX transformation(transform);		
		transformation = glm::translate(transformation, position + dx::XMFLOAT3(data.offset.x * size.x, (-data.offset.y - data.size.y * 0.5f) * size.y, 0.0f));
		transformation = glm::scale(transformation, { size.x * data.size.x, size.y * data.size.y, 1.0f });

		transform = glm::translate(transform, dx::XMFLOAT3(data.advance * size.x - data.size.x * 0.5f * size.x, 0.0f, 0.0f));
		positions = transformation * positions;

		sStorage.QuadVertexBufferPtr->position = positions.r[0];
		//sStorage.QuadVertexBufferPtr->texCoord = data.texCoords[0];
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[1];
		//sStorage.QuadVertexBufferPtr->texCoord = data.texCoords[1];
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[2];
		//sStorage.QuadVertexBufferPtr->texCoord = data.texCoords[2];
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadVertexBufferPtr->position = positions.r[3];
		//sStorage.QuadVertexBufferPtr->texCoord = data.texCoords[3];
		sStorage.QuadVertexBufferPtr->color = shade;
		//sStorage.QuadVertexBufferPtr->textIndex = textureIndex;
		//sStorage.QuadVertexBufferPtr->textureScale = 1.0f;
		sStorage.QuadVertexBufferPtr++;

		sStorage.QuadIndexCount += 6;
	}
	*/
	std::string Renderer2D::ReadFontFile(Font font)
	{
		std::string result;
		std::ifstream in("assets/Fonts/" + FontToText(font) + ".fnt", std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			if (in.tellg() != -1)
			{
				result.resize(in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else
			{
				AT_CORE_ERROR("Couldn't read the font {0}", FontToText(font));
			}
		}
		else
		{
			AT_CORE_ERROR("Couldn't open the font file at {0}", "assets/Fonts/" + FontToText(font) + ".fnt");
		}
		return result;
	}

	void Renderer2D::CompileCharSheet(const std::string& stringData, CharacterSheet* sheet)
	{
		size_t position = 0;
		int chars = 0;

		size_t AttributePosition = stringData.find_first_of(",", position);
		float charSize = (float)std::stoi(stringData.substr(position, AttributePosition));
		position = AttributePosition + 1;

		AttributePosition = stringData.find_first_of(",", position);
		sheet->commons.lineHeight = std::stoi(stringData.substr(position, AttributePosition)) / charSize;
		position = AttributePosition + 1;

		AttributePosition = stringData.find_first_of(",", position);
		sheet->commons.base = std::stoi(stringData.substr(position, AttributePosition)) / charSize;
		position = AttributePosition + 1;

		AttributePosition = stringData.find_first_of(",", position);
		float pageWidth = (float)std::stoi(stringData.substr(position, AttributePosition));
		position = AttributePosition + 1;

		AttributePosition = stringData.find_first_of(",", position);
		float pageHeight = (float)std::stoi(stringData.substr(position, AttributePosition));
		position = AttributePosition + 1;

		sheet->commons.lineHeight /= pageHeight;
		sheet->commons.base /= pageHeight;

		while (position != std::string::npos && chars != 95)
		{
			AttributePosition = stringData.find_first_of(",", position);
			sheet->characters[chars].value = std::stoi(stringData.substr(position, AttributePosition));
			position = AttributePosition + 1;

			AttributePosition = stringData.find_first_of(",", position);
			float x = std::stoi(stringData.substr(position, AttributePosition)) / pageWidth;
			position = AttributePosition + 1;

			AttributePosition = stringData.find_first_of(",", position);
			float y = std::stoi(stringData.substr(position, AttributePosition)) / pageHeight;
			position = AttributePosition + 1;

			AttributePosition = stringData.find_first_of(",", position);
			float width = std::stoi(stringData.substr(position, AttributePosition)) / pageWidth;
			position = AttributePosition + 1;

			AttributePosition = stringData.find_first_of(",", position);
			float height = std::stoi(stringData.substr(position, AttributePosition)) / pageHeight;
			position = AttributePosition + 1;

			sheet->characters[chars].texCoords[0] = dx::XMFLOAT2(x, 1-( y + height));
			sheet->characters[chars].texCoords[1] = dx::XMFLOAT2(x + width, 1 - (y + height));
			sheet->characters[chars].texCoords[2] = dx::XMFLOAT2(x + width, 1 - y);
			sheet->characters[chars].texCoords[3] = dx::XMFLOAT2(x, 1 - y);

			sheet->characters[chars].size = dx::XMFLOAT2(width / charSize, height / charSize);

			AttributePosition = stringData.find_first_of(",", position);
			int xoffset = std::stoi(stringData.substr(position, AttributePosition));
			position = AttributePosition + 1;

			AttributePosition = stringData.find_first_of(",", position);
			int yoffset = std::stoi(stringData.substr(position, AttributePosition));
			position = AttributePosition + 1;

			sheet->characters[chars].offset = dx::XMFLOAT2((xoffset / pageWidth) / charSize, (yoffset / pageHeight) / charSize);

			AttributePosition = stringData.find_first_of(",", position);
			sheet->characters[chars].advance = std::stoi(stringData.substr(position, AttributePosition)) / charSize / pageWidth;
			position = AttributePosition + 1;

			chars++;
		}
	}

}