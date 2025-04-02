#pragma once

#include <EditorUI.h>
#include <string>

class Config {
public:
	bool m_editorMode = false;

	std::unique_ptr<EditorUI> m_editorUI;


	static Config& Instance() {
		static Config instance;
		return instance;
	}

	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

private:
	Config() = default;
};
