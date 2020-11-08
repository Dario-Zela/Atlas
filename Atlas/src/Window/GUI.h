#pragma once

namespace Atlas
{
	struct SliderData;

	class GUI
	{
	public:
		void Init(std::string windowName = "GUI");
		void Broadcast();
		void AddSliderFloat(std::string name, float* val, float min, float max, float scale);
		void AddSliderFloat2(std::string name, float* val, float min, float max, float scale);
		void AddSliderFloat3(std::string name, float* val, float min, float max, float scale);
		void AddSliderFloat4(std::string name, float* val, float min, float max, float scale);

		void AddSliderUint(std::string name, UINT* val, UINT min, UINT max, UINT scale);
		void AddSliderUint2(std::string name, UINT* val, UINT min, UINT max, UINT scale);
		void AddSliderUint3(std::string name, UINT* val, UINT min, UINT max, UINT scale);
		void AddSliderUint4(std::string name, UINT* val, UINT min, UINT max, UINT scale);

		void AddSliderInt(std::string name, int* val, int min, int max, int scale);
		void AddSliderInt2(std::string name, int* val, int min, int max, int scale);
		void AddSliderInt3(std::string name, int* val, int min, int max, int scale);
		void AddSliderInt4(std::string name, int* val, int min, int max, int scale);

		void AddCheck(std::string name, bool* val);

		bool m_Alive;

		static std::unordered_map<HWND, std::tuple<void*, SliderData>> m_SliderElements;
		static std::unordered_map<HWND, bool*> m_ButtonElements;
		int m_TimeAfterTest;
		int m_Widgets;
	private:
		HWND m_hWnd;
	};
}
