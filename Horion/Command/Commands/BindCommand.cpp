#include "BindCommand.h"

#include "../../Module/ModuleManager.h"
#include "../../Utils/Utils.h"
#include <algorithm>

BindCommand::BindCommand() : IMCCommand("bind", "Binds modules to specific keys", "<module> <key>") {
	registerAlias("b");
}

bool BindCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1);
	assertTrue(moduleName.size() > 0);

	auto modOpt = moduleMgr->getModuleByName(moduleName);
	if (!modOpt.has_value()) {
		clientMessageF("%sCould not find module with name: %s", RED, moduleName.c_str());
		return true;
	}
	auto mod = modOpt.value();

	if (args->size() >= 3) {
		std::string key = args->at(2);

		assertTrue(key.size() > 0);

		if (key.size() > 1) {
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			const char* needle = key.c_str();

			if (key == "none") {
				mod->setKeybind(0x0);
				clientMessageF("%sSuccessfully unbound %s!", GREEN, mod->getModuleName());
				return true;
			}

			for (int i = 0; i < 190; i++) {
				const char* haystack = Utils::getKeybindName(i);
				size_t len = strlen(needle) + 1;
				char* haystackLowercase = new char[len];
				for (int j = 0; j < len; j++)
					haystackLowercase[j] = tolower(haystack[j]);

				if (strcmp(needle, haystackLowercase) == 0) {
					mod->setKeybind(i);
					clientMessageF("%sThe keybind of %s is now '%s'", GREEN, mod->getModuleName(), haystack);
					delete[] haystackLowercase;
					return true;
				}
				delete[] haystackLowercase;
			}
			clientMessageF("%sInvalid key!", RED);
			return true;
		}

		int keyCode = (int)key.at(0);
		if (keyCode >= 0x61 && keyCode <= 0x7A)  // Convert key to lower case
			keyCode -= 0x20;

		if (keyCode >= 0x30 && keyCode <= 0x5A) {
			auto modOpt = moduleMgr->getModuleByName(moduleName);
			if (!modOpt.has_value()) {
