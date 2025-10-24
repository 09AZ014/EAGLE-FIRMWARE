#pragma once

#include <vector>

class IModule {
public:
	virtual ~IModule() = default;
	virtual const char* getName() const = 0;
	virtual void setup() = 0;
	virtual void loop() = 0;
};

class ModuleManager {
public:
	void registerModule(IModule* module) {
		modules.push_back(module);
	}

	void setupAll() {
		for (IModule* module : modules) {
			module->setup();
		}
	}

	void loopAll() {
		for (IModule* module : modules) {
			module->loop();
		}
	}

private:
	std::vector<IModule*> modules;
};


