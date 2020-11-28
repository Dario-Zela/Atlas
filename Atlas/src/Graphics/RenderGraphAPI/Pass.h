#pragma once
#include "Graphics/RenderGraphAPI/Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"
#include "Graphics/Graphics.h"

namespace Atlas
{
	class RenderTarget;
	class DepthStencilBuffer;
	class Bindable;
	class RenderGraph;

	class Pass
	{
		friend RenderGraph;
	public:
		Pass(std::string name);
		virtual void Execute(wrl::ComPtr<ID3D11DeviceContext> context) = 0;
		virtual void ExecuteImmidiate() = 0;
		std::string& GetName() { return m_Name; }

		std::vector<std::unique_ptr<Sink>>& GetSinks() { return m_Sinks; }
		Source& GetSource(std::string& registeredName);
		Sink& GetSink(std::string& registeredName);

		void AddBindable(std::shared_ptr<Bindable> bindable);
		void BindAll(wrl::ComPtr<ID3D11DeviceContext> context);
		void BindAll();

		void SetSinkLink(std::string registeredName, std::string target);
		void Finalise();

		virtual void Reset() { }
		int GetLevel() { return m_Level; }

	protected:
		void RegisterSink(std::unique_ptr<Sink> sink);
		void RegisterSource(std::unique_ptr<Source> source);

		std::shared_ptr<RenderTarget> m_RenderTarget;
		std::shared_ptr<DepthStencilBuffer> m_DepthBuffer;

	private:
		std::vector<std::unique_ptr<Sink>> m_Sinks;
		std::vector<std::unique_ptr<Source>> m_Sources;
		std::vector<std::shared_ptr<Bindable>> m_Bindables;
		std::string m_Name;
		int m_Level;
	};
}