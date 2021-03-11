#pragma once
#include "Layer/Layer.h"

namespace Atlas 
{
	//A representation of a layer stack
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		//The functions add layers and overlays to the stack
		//The difference between an overlay and a layer is that
		//An overlay is added at the beginning of the stack
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		//Regular and reverse iterators
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		//Constant regular and reverse iterators
		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		//The layer storage
		std::vector<Layer*> m_Layers;
		//The index of insertion
		uint m_LayerInsertIndex = 0;
	};
}
