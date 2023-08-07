#include "pch.h"
#include "FlipReady.h"
#include "bakkesmod\wrappers\includes.h"
#include "utils/parser.h"
#include "bakkesmod/wrappers/Engine/WorldInfoWrapper.h"


BAKKESMOD_PLUGIN(FlipReady, "Flip ready indicator", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

float timer = 0;

void FlipReady::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("FlipReady loaded");

	cvarManager->registerCvar("flipready_enabled", "1", "Enable or Disable FlipReady", true, true, 0, true, 1, true);
	gameWrapper->RegisterDrawable(std::bind(&FlipReady::display, this, std::placeholders::_1));

}

void FlipReady::onUnload() 
{
	LOG("FlipReady unloaded");
}

void FlipReady::display(CanvasWrapper canvas)
{
	// Pre-Logic 
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

	float realTime = game.GetWorldInfo().GetTimeSeconds();
	float deltaTime = 1.5 + (timer - realTime);
	auto car = game.GetCars().Get(0);

	Vector2 screen = canvas.GetSize();
	float fontSize = (float)screen.X / (float)1000;
	
	canvas.SetColor(255, 255, 255, 255);
	canvas.SetPosition(Vector2{ int(screen.X * .8), int(screen.Y * .1) });

	std::string flip_str;
	unsigned long jumped = car.GetbJumped();
	
	if (!car.HasFlip()) {
		flip_str = "NO FLIP";
		canvas.SetColor(200, 0, 0, 255);
		canvas.DrawString(flip_str, 2 * fontSize, 2 * fontSize);
	}
	else if (jumped) {
		canvas.SetColor(0, 255, 0, 255);
		canvas.DrawBox(Vector2{ int(screen.X * .1), int(screen.Y * .05) });
		canvas.FillBox(Vector2{ int(screen.X * .1 * (deltaTime/1.5)), int(screen.Y * .05) });
	}
	else if (car.HasFlip()) {
		flip_str = "FLIP";
		canvas.SetColor(0, 255, 0, 255);
		canvas.DrawString(flip_str, 2 * fontSize, 2 * fontSize);
		timer = realTime;
	}
}