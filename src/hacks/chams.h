#pragma once
#include <stdexcept>

class CDrawModelInfo;
class CMatrix3x4;
class CVector;
namespace hacks
{
	void RunChams(
		void* results,
		const CDrawModelInfo& info,
		CMatrix3x4* bones,
		float* flexWeights,
		float* flexDelayedWeights,
		const CVector& modelOrigin,
		const std::int32_t flags
	) noexcept;
}