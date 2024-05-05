#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct FRStyle {
	LinearColor color_fliptext;
	LinearColor color_nofliptext;
	LinearColor color_gaugebar;
	float		font_size;
	float		bar_len;
	float		bar_height;
	std::string decay_dir;
	int position_x;
	int position_y;
};

extern int displayComponent;

class FlipReady: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	std::shared_ptr<bool> enabled;

	//Boilerplate
	void onLoad() override;
	void onUnload() override; // Uncomment and implement if you need a unload method
	void Render(CanvasWrapper canvas);

public:
	void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window

	void ShowSave(FRStyle* ref);
	void ShowColors(FRStyle* ref);
	void ShowSizes(FRStyle* ref);
	void ShowLocation(FRStyle* ref);
};
/*
class FlipReadySettings : public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase {
	void RenderSettings() override;
}; */
