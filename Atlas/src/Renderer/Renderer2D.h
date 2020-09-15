#pragma once
#include "Renderer/OrthographicCamera.h"
#include <DirectXMath.h>
//#include "Renderer/Texture.h"

#undef DrawText

namespace Atlas
{
	class ViewPort;

	struct CharacterSheet;

	enum class Font
	{
		Verdana, TimesNewRoman, FixedDsys
	};

	const int SUPPORTED_FONTS = 3;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera, ViewPort& viewPort);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, const dx::XMVECTOR& color, float rotation = 0.0f);
		static void DrawQuad(const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, const dx::XMVECTOR& color, float rotation = 0.0f);
		//static void DrawQuad(const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, Texture2D& texture, const dx::XMVECTOR& shade = dx::XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f), float textureScale = 1.0f, float rotation = 0.0f);
		//static void DrawQuad(const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, Texture2D& texture, const dx::XMVECTOR& shade = dx::XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f), float textureScale = 1.0f, float rotation = 0.0f);

		//static void DrawText(const char* text, const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, Font font, const dx::XMVECTOR& shade = dx::XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f), float rotation = 0.0f);
		//static void DrawText(const char* text, const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, Font font, const dx::XMVECTOR& shade = dx::XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f), float rotation = 0.0f);

		static void DrawLine(const dx::XMFLOAT2& position1, const dx::XMFLOAT2& position2, float thickness, const dx::XMVECTOR& color);
		static void DrawRect(const dx::XMFLOAT2& position1, const dx::XMFLOAT2& position2, float thickness, const dx::XMVECTOR& color);

		//static void DrawTextureTile(const dx::XMFLOAT2& position, const dx::XMFLOAT2& size, Texture2D& texture, unsigned int tilePosition, unsigned int tileSize, const dx::XMVECTOR& shade = dx::XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f));
		//static void DrawTextureTile(const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, Texture2D& texture, unsigned int tilePosition, unsigned int tileSize, const dx::XMVECTOR& shade = dx::XMVECTOR(1.0f, 1.0f, 1.0f, 1.0f));
	private:
		//static void DrawChar(char c, dx::XMMATRIX& transform, const dx::XMFLOAT3& position, const dx::XMFLOAT2& size, const dx::XMVECTOR& shade, float textureIndex, float fontIndex);
		static std::string ReadFontFile(Font font);
		static void CompileCharSheet(const std::string& stringData, CharacterSheet* sheet);
	};
}