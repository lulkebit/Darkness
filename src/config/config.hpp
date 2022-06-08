#pragma once

#include <filesystem>
#include <string>

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
		// LegitBot
		bool bAimbot{ false };
		int aim_key{ 0 };
		float fFov{ 5.0f };
		float fSmooth{ 0.5f };
		bool pSilent{ false };
		bool bTriggerbot{ false };
		int trigger_key{ 0 };

		// Glow
		bool bGlow{ false };
		bool bEnemyGlow{ false };
		bool bTeamGlow{ false };
		bool bC4Glow{ false };
		bool bChickenGlow{ false };
		float fEnemyGlow[4]{ 1.0f, 0.0f, 0.0f, 1.0f };
		float fTeamGlow[4]{ 0.0f, 0.0f, 1.0f, 1.0f };
		float fC4Glow[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
		float fChickenGlow[4]{ 1.0f, 1.0f, 1.0f, 1.0f };

		// Chams
		bool bChams{ false };
		float fHidden[3]{ 0.0f, 1.0f, 1.0f };
		float fVisible[3]{ 1.0f, 1.0f, 0.0f };
		/*int iMaterialSelected{ 0 };
		const char* sMaterial[13]{ 
			"debug/debugambientcube", 
			"models/inventory_items/trophy_majors/gold", 
			"models/player/ct_fbi/ct_fbi_glass" ,
			"models/gibs/glass/glass",
			"models/inventory_items/trophy_majors/crystal_clear",
			"models/inventory_items/wildfire_gold/wildfire_gold_detail",
			"models/inventory_items/trophy_majors/crystal_blue",
			"models/inventory_items/trophy_majors/velvet",
			"models/inventory_items/cologne_prediction/cologne_prediction_glass",
			"models/inventory_items/dogtags/dogtags_outline",
			"models/inventory_items/dogtags/dogtags_lightray",
			"models/inventory_items/contributor_map_tokens/contributor_charset_color",
			"models/inventory_items/music_kit/darude_01/mp3_detail"
		};*/

		// Misc
		bool bBhop{ false };
		bool bThirdperson{ false };
	} item;

private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};

extern c_config config_system;