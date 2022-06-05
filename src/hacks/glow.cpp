#include "glow.h"

// access global variables
#include "../core/globals.h"

// access interfaces
#include "../core/interfaces.h"
#include "../config/config.hpp"

void hacks::RunGlow() noexcept
{
	if (config_system.item.bGlow)
	{
		for (int i = 0; i < interfaces::glow->glowObjects.size; i++)
		{
			IGlowManager::CGlowObject& glowObject = interfaces::glow->glowObjects[i];

			if (glowObject.IsUnused())
				continue;

			if (!glowObject.entity)
				continue;

			switch (glowObject.entity->GetClientClass()->classID)
			{
			case CClientClass::CCSPlayer:
				if (!glowObject.entity->IsAlive())
					break;

				if (config_system.item.bEnemyGlow)
				{
					if (glowObject.entity->GetTeam() != globals::localPlayer->GetTeam())
						glowObject.SetColor(config_system.item.fEnemyGlow[0], config_system.item.fEnemyGlow[1], config_system.item.fEnemyGlow[2], config_system.item.fEnemyGlow[3]);
				}

				if (config_system.item.bTeamGlow)
				{
					if (glowObject.entity->GetTeam() == globals::localPlayer->GetTeam())
						glowObject.SetColor(config_system.item.fTeamGlow[0], config_system.item.fTeamGlow[1], config_system.item.fTeamGlow[2], config_system.item.fTeamGlow[3]);
				}
				break;

			case CClientClass::CC4:
				if (config_system.item.bC4Glow)
					glowObject.SetColor(config_system.item.fC4Glow[0], config_system.item.fC4Glow[1], config_system.item.fC4Glow[2], config_system.item.fC4Glow[3]);
				break;

			case CClientClass::CChicken:
				if (config_system.item.bChickenGlow)
					glowObject.SetColor(config_system.item.fChickenGlow[0], config_system.item.fChickenGlow[1], config_system.item.fChickenGlow[2], config_system.item.fChickenGlow[3]);
				break;

			default:
				break;
			}
		}
	}
}