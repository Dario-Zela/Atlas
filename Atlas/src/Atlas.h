//This is what holds all of the header files that will be accessible by the CLI/C++ layer
//They will contain all the classes the user will be able to access

#pragma comment(lib, "d3d11.lib")

#include "pch.h"
#include "Core/Client.h"
#include "Core/Application.h"
#include "Layer/Layer.h"
#include "Core/Input.h"
#include "Graphics/AssimpWrapper/Scene.h"
#include "Core/Camera.h"
#include "Core/TimeStep.h"
#include "Graphics/RenderGraphAPI/RenderGraph.h"
#include "Graphics/RenderGraphAPI/RenderQueuePass.h"
#include "Graphics/RenderGraphAPI/FullScreenPass.h"
#include "Graphics/D3DWrappers/Shaders.h"
#include "Graphics/D3DWrappers/InputLayout.h"
#include "Graphics/D3DWrappers/Topology.h"
#include "Graphics/D3DWrappers/Sampler.h"
#include "Graphics/D3DWrappers/Blendable.h"
#include "Graphics/D3DWrappers/SimpleMeshCreator.h"
#include "Graphics/D3DWrappers/TransformationConstantBuffer.h"
#include "Graphics/D3DWrappers/Texture.h"
#include "Graphics/D3DWrappers/Rasteriser.h"
#include "Graphics/D3DWrappers/Targets.h"
#include "Graphics/D3DWrappers/DepthStencilState.h"
#include "Graphics/D3DWrappers/Buffers.h"
#include "Graphics/D3DWrappers/RenderTarget.h"
#include "Graphics/D3DWrappers/Vertex.h"
#include "Graphics/D3DWrappers/ViewPort.h"