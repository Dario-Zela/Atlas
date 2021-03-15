#pragma once

namespace Atlas
{
	struct SliderData;

	//A class that constructs a GUI
	class GUI
	{
	public:
		//Initialises the GUI with a name
		void Init(const std::string& windowName = "GUI", uint width = 500, uint height = 400);
		
		//Broadcasts messages from the GUI to the sliders
		void Broadcast();

		//Float sliders
		void AddSliderFloat(const std::string& name, float* val, float min, float max, float scale);
		void AddSliderFloat2(const std::string& name, float* val, float min, float max, float scale);
		void AddSliderFloat3(const std::string& name, float* val, float min, float max, float scale);
		void AddSliderFloat4(const std::string& name, float* val, float min, float max, float scale);

		//Uint sliders
		void AddSliderUint(const std::string& name, uint* val, uint min, uint max, uint scale);
		void AddSliderUint2(const std::string& name, uint* val, uint min, uint max, uint scale);
		void AddSliderUint3(const std::string& name, uint* val, uint min, uint max, uint scale);
		void AddSliderUint4(const std::string& name, uint* val, uint min, uint max, uint scale);

		//Init sliders
		void AddSliderInt(const std::string& name, int* val, int min, int max, int scale);
		void AddSliderInt2(const std::string& name, int* val, int min, int max, int scale);
		void AddSliderInt3(const std::string& name, int* val, int min, int max, int scale);
		void AddSliderInt4(const std::string& name, int* val, int min, int max, int scale);

		//Checkbox
		void AddCheckBox(const std::string& name, bool* val);

		//Whether or not the GUI window is alive
		bool m_Alive;

	private:
		//The Window Procedure
		static LRESULT CALLBACK GUIProc(HWND hwnd, uint msg, WPARAM wparam, LPARAM lparam);

		//Maps of the elements that are being accessed
		static std::unordered_map<HWND, std::tuple<void*, SliderData>> m_SliderElements;
		static std::unordered_map<HWND, bool*> m_ButtonElements;

		int m_TimeAfterTest;
		int m_Widgets;
		HWND m_hWnd;
	};
}
