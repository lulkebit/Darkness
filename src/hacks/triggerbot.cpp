#include "triggerbot.h"

// access global variables
#include "../core/globals.h"

// access interfaces
#include "../core/interfaces.h"
#include "../config/config.hpp"
#include <windows.h>
#include <WinUser.h>


void hacks::RunTriggerbot(CUserCmd* cmd) noexcept
{
	if (config_system.item.bTriggerbot)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			if (!globals::localPlayer || !globals::localPlayer->IsAlive())
				return;

			CVector eyePosition;
			globals::localPlayer->GetEyePosition(eyePosition);

			CVector aimPunch;
			globals::localPlayer->GetAimPunch(aimPunch);

			const CVector dst = eyePosition + CVector{ cmd->viewAngles + aimPunch }.ToVector() * 1000.f;

			CTrace trace;
			interfaces::engineTrace->TraceRay({ eyePosition, dst }, 0x46004009, globals::localPlayer, trace);

			if (!trace.entity || !trace.entity->IsPlayer())
				return;

			if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
				return;

			cmd->buttons |= CUserCmd::IN_ATTACK;

			return;
		}
	}
}
