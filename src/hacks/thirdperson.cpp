#include "thirdperson.h"

// access global variables
#include "../core/globals.h"

// access interfaces
#include "../core/interfaces.h"
#include "../config/config.hpp"

void hacks::RunThirdperson() noexcept
{
	if (config_system.item.bThirdperson)
		return;
}
