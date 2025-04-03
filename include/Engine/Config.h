#pragma once

#include <Engine/EditorUI.h>
#include <string>

enum class Mode {
	Editor,
	Game
};

class Config {
public:
	Mode m_mode = Mode::Game;

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

