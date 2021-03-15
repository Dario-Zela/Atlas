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
		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopLayer(std::shared_ptr<Layer> layer);
		void PopOverlay(std::shared_ptr<Layer> overlay);

		//Regular and reverse iterators
		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		//Constant regular and reverse iterators
		std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return m_Layers.end(); }
		std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		//The layer storage
		std::vector<std::shared_ptr<Layer>> m_Layers;
		//The index of insertion
		uint m_LayerInsertIndex = 0;
	};
}
