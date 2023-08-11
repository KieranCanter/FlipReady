#include "pch.h"
#include "FlipReady.h"
#include "bakkesmod\wrappers\includes.h"
#include "bakkesmod/wrappers/Engine/WorldInfoWrapper.h"
#include "utils/parser.h"


BAKKESMOD_PLUGIN(FlipReady, "Flip ready indicator", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FlipReady::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("FlipReady loaded");

	cvarManager->registerCvar("flipready_enabled", "1", "1 = enable | 0 = disable", true, true, 0, true, 1);
	cvarManager->registerCvar("flipready_color_fliptext", "#00FF00FF", "Change \"Flip\" text color with hexcode.", true);
	cvarManager->registerCvar("flipready_color_nofliptext", "#FF0000FF", "Change \"No Flip\" text color with hexcode.", true);
	cvarManager->registerCvar("flipready_color_gaugebar", "#00FF00FF", "Change gauge bar color with hexcode.", true, true);
	cvarManager->registerCvar("flipready_fontsize", "20", "Change fontsize (1-100).", true, true, 1, true, 100);
	cvarManager->registerCvar("flipready_barlen", "20", "Change gauge bar length [1-100].", true, true, 1, true, 100);
	cvarManager->registerCvar("flipready_barheight", "5", "Change gauge bar height [1-25].", true, true, 1, true, 25);
	cvarManager->registerCvar("flipready_keepbarratio", "1", "1 = maintain bar ratio | 0 = neglect bar ratio.", true, true, 0, true, 1);
	cvarManager->registerCvar("flipready_positionx", "middle", "Change horizontal position [eft|middle|right].", true);
	cvarManager->registerCvar("flipready_positiony", "top", "Change vertical position [top|middle|bottom].", true);
	
	// Don't save value if it isn't a valid position
	// Cannot have both positions equal middle
	cvarManager->getCvar("flipready_positionx").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "left" && cvar.getStringValue() != "middle" && cvar.getStringValue() != "right") {
			cvar.setValue(oldval);
			LOG("Please enter [left|middle|right]");
		}
		
		if (cvar.getStringValue() == "middle" && cvarManager->getCvar("flipready_positiony").getStringValue() == "middle") {
			cvar.setValue(oldval);
			LOG("Horizontal position not saved. Both positions can't be middle.");
		}
	});

	cvarManager->getCvar("flipready_positiony").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "top" && cvar.getStringValue() != "middle" && cvar.getStringValue() != "bottom") {
			cvar.setValue(oldval);
			LOG("Please enter [top|middle|bottom]");
		}
		if (cvar.getStringValue() == "middle" && cvarManager->getCvar("flipready_positionx").getStringValue() == "middle") {
			cvar.setValue(oldval);
			LOG("Vertical position not saved. Both positions can't be middle.");
		}
	});

	gameWrapper->RegisterDrawable(std::bind(&FlipReady::Render, this, std::placeholders::_1));

}

void FlipReady::onUnload() 
{
	LOG("FlipReady unloaded");
}

void FlipReady::Render(CanvasWrapper canvas)
{
	// Start Pre-Logic 
	bool isEnabled = cvarManager->getCvar("flipready_enabled").getBoolValue();
	
	if (!isEnabled) {
		return;
	}

	if (!gameWrapper->IsInGame()) {
		return;
	}

	auto game = gameWrapper->GetGameEventAsServer();
	
	if (game.GetCars().Count() == 0) {
		return;
	}

	// TODO LOAD IN CVARS AND SET VALUES

	// End Pre-Logic

	// REPLACE BELOW VALUES WITH CVARS AND FIX THE FUCKING SETTINGS
	float realTime = game.GetWorldInfo().GetTimeSeconds();  // realtime of server
	static float timer = 0;
	float deltaTime = 1.5 + (timer - realTime);				// difference between realtime and time at moment of jumping
	auto car = game.GetCars().Get(0);						// user car object

	Vector2 screen = canvas.GetSize();
	float fontSize = (float)screen.X / (float)1000;
	
	canvas.SetColor(255, 255, 255, 255);
	canvas.SetPosition(Vector2{ int(screen.X * .8), int(screen.Y * .1) });

	std::string flip_str;
	unsigned long jumped = car.GetbJumped();				// 1 if car jumped and is still in air, 0 otherwise
	
	if (!car.HasFlip()) {									// car does not have accesible flip
		flip_str = "NO FLIP";
		canvas.SetColor(200, 0, 0, 255);					// set color red
		canvas.DrawString(flip_str, 2 * fontSize, 2 * fontSize);
	}
	else if (jumped) {										// car jumped and is still in air
		canvas.SetColor(0, 255, 0, 255);					// set color green

		// indicator gauge is drawn and filled, depletes to empty over 1.5 seconds
		canvas.DrawBox(Vector2{ int(screen.X * .1), int(screen.Y * .05) });
		canvas.FillBox(Vector2{ int(screen.X * .1 * (deltaTime/1.5)), int(screen.Y * .05) });
	}
	else if (car.HasFlip()) {								// car has flip but has not jumped
		flip_str = "FLIP";
		canvas.SetColor(0, 255, 0, 255);					// set color green
		canvas.DrawString(flip_str, 2 * fontSize, 2 * fontSize);
		timer = realTime;									// constantly update timer to realtime while car hasn't jumped
	}
}

void ShowColors(FRStyle* ref) {

}

void ShowSizes(FRStyle* ref) {

}