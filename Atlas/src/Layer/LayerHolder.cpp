#include "pch.h"
#include "LayerHolder.h"

namespace Atlas
{
	LayerStack::~LayerStack()
	{
		//Deletes all layers that are remaining in the stack
		for (Layer* layer : m_Layers)
		{
			//It also calls the detach functions
			layer->OnDetach();
			delete layer;
		}
	}

	//Adds a layer
	void LayerStack::PushLayer(Layer* layer)
	{
		//The layer's existance is checked
		AT_CORE_ASSERT(layer, "The layer doesn't exist")
		//This is done to the position of the begging item + the layer index
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;			//The index is incremented
		layer->OnAttach();				//And the attach function is called
	}

	//Adds an overlay
	void LayerStack::PushOverlay(Layer* overlay)
	{
		//The overlay's existance is checked
		AT_CORE_ASSERT(overlay, "The layer doesn't exist")
		//This is at the begging of the stack
		//And as the index is not incremented, the layers will always
		//Be below it
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();			//And the attach function is called
	}

	//Removes a layer
	void LayerStack::PopLayer(Layer* layer)
	{
		//This attempts to find if the layer is in the stack
		//We only need to check from the beginning to the index
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		//If it is not found it will return the end, so it is asserted
		AT_CORE_ASSERT(it != m_Layers.begin() + m_LayerInsertIndex, "The layer wasn't found")

		//If it is found, the layer is detached
		layer->OnDetach();
		//Then erased
		m_Layers.erase(it);
		//And the index is decremented
		m_LayerInsertIndex--;
	}

	//Removes an overlay
	void LayerStack::PopOverlay(Layer* overlay)
	{
		//This attempts to find if the overlay is in the stack
		//We only need to check from index to the end
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		//If it is not found it will return the end, so it is asserted
		AT_CORE_ASSERT(it != m_Layers.end(), "The layer wasn't found")

		//If it is found, the layer is detached
		overlay->OnDetach();
		//Then erased
		m_Layers.erase(it);
	}
}