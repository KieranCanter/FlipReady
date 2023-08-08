#include "pch.h"
#include "FlipReady.h"
#include "bakkesmod\wrappers\includes.h"
#include "utils/parser.h"
#include "bakkesmod/wrappers/Engine/WorldInfoWrapper.h"


BAKKESMOD_PLUGIN(FlipReady, "Flip ready indicator", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

float timer = 0; // global var to act as starting time at the moment of car jump

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
	// End Pre-Logic

	float realTime = game.GetWorldInfo().GetTimeSeconds();  // realtime of server
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