#pragma once
#include "Graphics/RenderGraphAPI/Sink.h"
#include "Graphics/RenderGraphAPI/Source.h"

namespace Atlas
{
	class RenderTarget;
	class DepthStencilBuffer;
	class Bindable;

	class Pass
	{
	public:
		Pass(std::string name);
		virtual void Execute() = 0;
		std::string& GetName() { return m_Name; }

		std::vector<std::unique_ptr<Sink>>& GetSinks() { return m_Sinks; }
		Source& GetSource(std::string& registeredName);
		Sink& GetSink(std::string& registeredName);

		void AddBindable(std::shared_ptr<Bindable> bindable);
		void BindAll();

		void SetSinkLink(std::string registeredName, std::string target);
		void Finalise();

		virtual void Reset() { }

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
	};
}