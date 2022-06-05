#pragma once

class CUserCmd;
namespace hacks
{
	inline bool aimbotToggle = false;
	inline bool pSilent = false;
	inline float fov = 5.f;
	inline float smoothing = 0.5f;
	inline bool alway_on = false;
	inline bool auto_shot = false;
	void RunAimbot(CUserCmd* cmd) noexcept;
	void Runalwayonaimbot(CUserCmd* cmd) noexcept;
	void autoshot(CUserCmd* cmd) noexcept;
}
