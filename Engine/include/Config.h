#pragma once

#include <string>

class Config {
public:
	bool m_editorMode = false;



	static Config& Instance() {
		static Config instance;
		return instance;
	}

	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

private:
	Config() = default;
};
