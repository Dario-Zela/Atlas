#pragma once

namespace Atlas
{
	struct SliderData;

	//A class that constructs a GUI
	class GUI
	{
	public:
		//Initialises the gui with a name
		void Init(std::string windowName = "GUI");
		
		//Broadcasts messages from the gui to the sliders
		void Broadcast();

		//Float sliders
		void AddSliderFloat(std::string name, float* val, float min, float max, float scale);
		void AddSliderFloat2(std::string name, float* val, float min, float max, float scale);
		void AddSliderFloat3(std::string name, float* val, float min, float max, float scale);
		void AddSliderFloat4(std::string name, float* val, float min, float max, float scale);

		//Uint sliders
		void AddSliderUint(std::string name, UINT* val, UINT min, UINT max, UINT scale);
		void AddSliderUint2(std::string name, UINT* val, UINT min, UINT max, UINT scale);
		void AddSliderUint3(std::string name, UINT* val, UINT min, UINT max, UINT scale);
		void AddSliderUint4(std::string name, UINT* val, UINT min, UINT max, UINT scale);

		//Init sliders
		void AddSliderInt(std::string name, int* val, int min, int max, int scale);
		void AddSliderInt2(std::string name, int* val, int min, int max, int scale);
		void AddSliderInt3(std::string name, int* val, int min, int max, int scale);
		void AddSliderInt4(std::string name, int* val, int min, int max, int scale);

		//Checkbox
		void AddCheckBox(std::string name, bool* val);

		//Whether or not the gui window is alive
		bool m_Alive;

		//Maps of the elements that are being accessed
		static std::unordered_map<HWND, std::tuple<void*, SliderData>> m_SliderElements;
		static std::unordered_map<HWND, bool*> m_ButtonElements;

	private:
		int m_TimeAfterTest;
		int m_Widgets;
		HWND m_hWnd;
	};
}
