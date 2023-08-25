#include "pch.h"
#include "FlipReady.h"
#include "bakkesmod\wrappers\includes.h"
#include "bakkesmod/wrappers/Engine/WorldInfoWrapper.h"
#include "utils/parser.h"


BAKKESMOD_PLUGIN(FlipReady, "Flip ready indicator", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
int displayComponent;

void FlipReady::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("FlipReady loaded");

	gameWrapper->LoadToastTexture("fr_logo", gameWrapper->GetDataFolder() / "fr_logo_short_square.png");

	// *** Initialize CVars *** //
	
	// Enable
	cvarManager->registerCvar("flipready_enabled", "1", "1 = enable | 0 = disable", true, true, 0, true, 1);
	
	// Colors
	cvarManager->registerCvar("flipready_color_fliptext", "#00FF00FF", "Change \"Flip\" text color with hexcode.", true);
	cvarManager->registerCvar("flipready_color_nofliptext", "#FF0000FF", "Change \"No Flip\" text color with hexcode.", true);
	cvarManager->registerCvar("flipready_color_gaugebar", "#00FF00FF", "Change gauge bar color with hexcode.", true, true);
	
	// Sizes
	cvarManager->registerCvar("flipready_fontsize", "20", "Change fontsize (1-100).", true, true, 1, true, 100);
	cvarManager->registerCvar("flipready_barlen", "20", "Change gauge bar length [1-100].", true, true, 1, true, 100);
	cvarManager->registerCvar("flipready_barheight", "5", "Change gauge bar height [1-25].", true, true, 1, true, 25);
	
	// Gauge Bar Options
	cvarManager->registerCvar("flipready_decaydir", "left", "Change direction of gauge bar decay [left|right]", true);
	
	// Positioning
	cvarManager->registerCvar("flipready_positionx", "middle", "Change horizontal position [left|middle|right].", true);
	cvarManager->registerCvar("flipready_positiony", "top", "Change vertical position [top|middle|bottom].", true);

	// *** End Initialize Cvars *** //

	// Don't save value if it isn't a valid position
	// Cannot have both positions equal middle
	cvarManager->getCvar("flipready_positionx").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "left" && cvar.getStringValue() != "middle" && cvar.getStringValue() != "right") {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter one of [left|middle|right]");
		}
		
		if (cvar.getStringValue() == "middle" && cvarManager->getCvar("flipready_positiony").getStringValue() == "middle") {
			cvar.setValue(oldval);
			LOG("Horizontal position not saved. Both positions can't be middle.");
		}
	});
	cvarManager->getCvar("flipready_positiony").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "top" && cvar.getStringValue() != "middle" && cvar.getStringValue() != "bottom") {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter one of [top|middle|bottom]");
		}
		if (cvar.getStringValue() == "middle" && cvarManager->getCvar("flipready_positionx").getStringValue() == "middle") {
			cvar.setValue(oldval);
			LOG("Vertical position not saved. Both positions can't be middle.");
		}
	});

	// Ensure valid inputs for decay direction
	cvarManager->getCvar("flipready_decaydir").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "left" && cvar.getStringValue() != "right") {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter one of [left|right]");
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

	LinearColor colorFlipText = cvarManager->getCvar("flipready_color_fliptext").getColorValue();
	LinearColor colorNoFlipText = cvarManager->getCvar("flipready_color_nofliptext").getColorValue();
	LinearColor colorGaugeBar = cvarManager->getCvar("flipready_color_gaugebar").getColorValue();
	float fontSize = cvarManager->getCvar("flipready_fontsize").getFloatValue();
	float barLen = 10 * cvarManager->getCvar("flipready_barlen").getFloatValue();
	float barHeight = 10 * cvarManager->getCvar("flipready_barheight").getFloatValue();
	std::string posStrX = cvarManager->getCvar("flipready_positionx").getStringValue();
	std::string posStrY = cvarManager->getCvar("flipready_positiony").getStringValue();
	std::string decayDir = cvarManager->getCvar("flipready_decaydir").getStringValue();


	// End Pre-Logic

	// REPLACE BELOW VALUES WITH CVARS AND FIX THE FUCKING SETTINGS
	float realTime = game.GetWorldInfo().GetTimeSeconds();  // realtime of server
	static float timer = 0;
	float deltaTime = 1.5 + (timer - realTime);				// decay down from 1.5 -> 0
	auto car = game.GetCars().Get(0);						// user car object

	Vector2 screen = canvas.GetSize();
	float exactFontSize = (float)screen.X * 0.0001 * fontSize;

	// Positioning
	float posX = 0;
	float posY = 0;

	if (posStrX == "left")
		posX = screen.X * 0.05;
	else if (posStrX == "right") {
		if (posStrY == "bottom")
			posX = screen.X * 0.8;
		else
			posX = screen.X * 0.95;
	}
	else
		posX = screen.X * 0.5;

	if (posStrY == "middle")
		posY = screen.Y * 0.5;
	else if (posStrY == "bottom")
		posY = screen.Y * 0.9;
	else
		posY = screen.Y * 0.1;

	canvas.SetColor(255, 255, 255, 255);
	canvas.SetPosition(Vector2{ int(posX), int(posY) });


	// CORE FUNCTIONALITY
	std::string flip_str;
	bool hasFlip = car.HasFlip();
	unsigned long jumped = car.GetbJumped();
	
	if ((!car.HasFlip() && displayComponent != 3 && displayComponent != 1) || displayComponent == 2) {									
		flip_str = "NO FLIP";

		// Positioning
		if (posStrX == "middle")
			posX -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5;
		else if (posStrX == "right")
			posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) - (barLen * 0.5);
		else
			posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) + (barLen * 0.5);

		if (posStrY == "middle")
			posY -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5;
		else if (posStrY == "bottom")
			posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) - (barHeight * 0.5);
		else
			posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) + (barHeight * 0.5);

		canvas.SetColor(colorNoFlipText);				
		canvas.SetPosition(Vector2{ int(posX), int(posY) });
		canvas.DrawString(flip_str, exactFontSize, exactFontSize);
	}
	else if ((jumped && displayComponent != 1) || displayComponent == 3) {									
		
		// Positioning
		if (posStrX == "middle")
			posX -= barLen * 0.5;
		else if (posStrX == "right")
			posX -= barLen;

		if (posStrY == "middle")
			posY -= barHeight * 0.5;
		else if (posStrY == "bottom")
			posY -= barHeight;

		canvas.SetColor(colorGaugeBar);					

		// indicator gauge is drawn and filled, depletes to empty over 1.5 seconds

		canvas.SetPosition(Vector2{ int(posX), int(posY) });
		canvas.DrawBox(Vector2{ int(barLen), int(barHeight) });
		
		if (displayComponent != 3) {
			if (decayDir == "left") {
				canvas.FillBox(Vector2{ int(barLen * (deltaTime / 1.5)), int(barHeight) });
			}
			else {
				posX += barLen * ((realTime - timer) / 1.5);
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen * (deltaTime / 1.5)), int(barHeight) });
			}
		}
		else {
			if (decayDir == "left") {
				canvas.FillBox(Vector2{ int(barLen / 2), int(barHeight) });
			}
			else {
				posX += barLen / 2;
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen / 2), int(barHeight) });
			}
		}
	}
	else if (car.HasFlip() || displayComponent == 1) {								
		flip_str = "FLIP";
		
		// Positioning
		if (posStrX == "middle")
			posX -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5;
		else if (posStrX == "right")
			posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) - (barLen * 0.5);
		else
			posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) + (barLen * 0.5);

		if (posStrY == "middle")
			posY -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5;
		else if (posStrY == "bottom")
			posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) - (barHeight * 0.5);
		else
			posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) + (barHeight * 0.5);

		canvas.SetColor(colorFlipText);					
		canvas.SetPosition(Vector2{ int(posX), int(posY) });
		canvas.DrawString(flip_str, exactFontSize, exactFontSize);
		timer = realTime;									
	}
}

void ShowSave(FRStyle* ref) {}

void ShowColors(FRStyle* ref) {}

void ShowSizes(FRStyle* ref) {}

void ShowLocation(FRStyle* ref) {}