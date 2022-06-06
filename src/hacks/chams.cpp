#include "chams.h"

// access global variables
#include "../core/globals.h"

// access interfaces
#include "../core/interfaces.h"
#include "../config/config.hpp"
#include "../core/hooks.h"



void hacks::RunChams(
	void* results,
	const CDrawModelInfo& info,
	CMatrix3x4* bones,
	float* flexWeights,
	float* flexDelayedWeights,
	const CVector& modelOrigin,
	const std::int32_t flags) noexcept
{
	if (config_system.item.bChams && interfaces::engine->IsInGame())
	{
		CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();
		if (entity && entity->IsPlayer() && entity->GetTeam() != globals::localPlayer->GetTeam())
		{
			static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

			interfaces::studioRender->SetAlphaModulation(1.0f);

			material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(config_system.item.fHidden);
			interfaces::studioRender->ForcedMaterialOverride(material);
			hooks::DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
			interfaces::studioRender->SetColorModulation(config_system.item.fVisible);
			interfaces::studioRender->ForcedMaterialOverride(material);
			hooks::DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			return interfaces::studioRender->ForcedMaterialOverride(nullptr);
		}
	}
	hooks::DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}