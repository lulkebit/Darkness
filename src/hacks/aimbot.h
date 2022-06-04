#pragma once

class CUserCmd;
namespace hacks
{
	inline bool aimbotToggle = false;
	inline bool pSilent = false;
	inline float fov = 5.f;
	inline float smoothing = 0.5f;
	void RunAimbot(CUserCmd* cmd) noexcept;
}