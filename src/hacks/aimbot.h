#pragma once

class CUserCmd;
namespace hacks
{
	inline bool alway_on = false;
	inline bool auto_shot = false;
	void RunAimbot(CUserCmd* cmd) noexcept;
	//void Runalwayonaimbot(CUserCmd* cmd) noexcept;
	void autoshot(CUserCmd* cmd) noexcept;
}
