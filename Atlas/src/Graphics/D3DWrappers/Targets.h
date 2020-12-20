#pragma once
#include "Core\Core.h"

namespace Atlas
{
	enum class TargetShader
	{
		VertexShader = 1 << 0,
		PixelShader = 1 << 1,
		DomainShader = 1 << 2,
		HullShader = 1 << 3,
		GeometryShader = 1 << 4,
		ComputeShader = 1 << 5
	};

	#define MAX_TARGETS 6

	//////////////////////////////////////////////////////////////////
	//Overrided operators to simplify adding proprieties

	inline TargetShader operator|(TargetShader a, TargetShader b) { return (TargetShader)((uint)a | (uint)b); }

	inline uint operator|=(uint a, TargetShader b) { return (a | (uint)b); }
}