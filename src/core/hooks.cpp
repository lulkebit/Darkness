#include "hooks.h"

// include minhook for epic hookage
#include "../../ext/minhook/minhook.h"

#include <intrin.h>
#include <stdexcept>
#include "../hacks/misc.h"
#include "../hacks/aimbot.h"

#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_dx9.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../config/config.hpp"

void hooks::Setup() noexcept
{
	MH_Initialize();

	// AllocKeyValuesMemory hook
	MH_CreateHook(
		memory::Get(interfaces::keyValuesSystem, 1),
		&AllocKeyValuesMemory,
		reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)
	);

	// CreateMove hook
	MH_CreateHook(
		memory::Get(interfaces::clientMode, 24),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	);

	MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	);

	MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	);

	MH_EnableHook(MH_ALL_HOOKS);

	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	// restore hooks
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	// uninit minhook
	MH_Uninitialize();
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
	// if function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesEngine) ||
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesClient)) 
		return nullptr;

	// return original
	return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd) noexcept
{
	// make sure this function is being called from CInput::CreateMove
	if (!cmd->commandNumber)
		return CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);
	if (config_system.item.pSilent)
		// this would be done anyway by returning true
		if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
			interfaces::engine->SetViewAngles(cmd->viewAngles);

	// get our local player here
	globals::UpdateLocalPlayer();

	if (globals::localPlayer && globals::localPlayer->IsAlive())
	{
		// example bhop
		hacks::RunBunnyHop(cmd);

		// run aimbot
		hacks::RunAimbot(cmd);

		hacks::Runalwayonaimbot(cmd);
	}

	if (!config_system.item.pSilent)
		// this would be done anyway by returning true
		if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
			interfaces::engine->SetViewAngles(cmd->viewAngles);

	return false;
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	// stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}