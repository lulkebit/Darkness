#include "gui.h"

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../ext/imgui/imgui_impl_win32.h"

#include <stdexcept>
#include <vector>
#include "../ext/imgui/imgui_internal.h"
#include "hacks/misc.h"
#include "core/interfaces.h"
#include "hacks/aimbot.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	// populate window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName, 
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	// create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;

	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;

	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	if (!SetupWindowClass("hackClass001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");

	DestroyWindow();
	DestroyWindowClass();
}

void Colors()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16);
	style.Colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);

	style.Colors[ImGuiCol_Header] = ImColor(30, 30, 30);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(28, 28, 28);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(28, 28, 28);
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
	);

	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	io.IniFilename = NULL;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Framd.ttf", 16.0f);
	// ImGui::StyleColorsDark();
	Colors();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// restore wnd proc
	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

void gui::Render() noexcept
{
	struct tab_data {
		const char* name;
	};

	static std::vector<tab_data> tabs = {
		{ "Legitbot" },
		{ "Visuals" },
		{ "Misc" },
		{ "Config" }
	};

	static int tab = 0;

	// start menu frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowSize({ 800, 500 });
	ImGui::Begin("Darkness", 
		&open,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
		);
	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);
	ImGui::BeginChild("##LeftSide", ImVec2(120, ImGui::GetContentRegionAvail().y), true);
	{
		for (unsigned int i = 0; i < tabs.size(); i++)
		{
			bool selected = (tab == i);
			if (ImGui::Selectable(tabs[i].name, &selected))
				tab = i;

			if (selected)
				ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndChild();

	{
		ImGui::SameLine(0);
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
	}

	ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
	{
		auto& data = tabs[tab];
		if (data.name == "Legitbot")
		{
			ImGui::Checkbox("Aimbot", &hacks::aimbotToggle);
			if (hacks::aimbotToggle)
			{
				ImGui::SliderFloat("FOV", &hacks::fov, 0.1f, 180.0f);
				ImGui::SliderFloat("Smoothing", &hacks::smoothing, 0.1f, 1.0f);
			}
		}
		else if (data.name == "Visuals")
		{

		}
		else if (data.name == "Misc")
		{
			ImGui::Checkbox("Bhop", &hacks::bhopToggle);
		}
		else if (data.name == "Config")
		{
			
		}
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
)
{
	ImGuiIO& io = ::ImGui::GetIO();

	// toggle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gui::open = !gui::open;

	//if (gui::open)
		//SetCursor(io.MouseDrawCursor ? NULL : LoadCursor(NULL, IDC_ARROW));
	//else
		//SetCursor(NULL);

	// pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParameter,
		longParameter
	)) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParameter,
		longParameter
	);
}