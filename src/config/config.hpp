#pragma once

#include <filesystem>

class c_config final {
public:
	void run(const char*) noexcept;
	void load(size_t) noexcept;
	void save(size_t) const noexcept;
	void add(const char*) noexcept;
	void remove(size_t) noexcept;
	void rename(size_t, const char*) noexcept;
	void reset() noexcept;

	constexpr auto& get_configs() noexcept {
		return configs;
	}

	struct {
		bool bAimbot{ false };
		float fFov{ 5.0f };
		float fSmooth{ 0.5f };
		bool pSilent{ false };
		bool bBhop{ false };
	} item;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern c_config config_system;