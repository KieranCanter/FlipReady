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

	std::string resStr = SettingsWrapper().GetVideoSettings().Resolution;
	std::string resStrLen = resStr.substr(0, resStr.find("x"));
	std::string resStrHei = resStr.substr(resStr.find("x") + 1, resStr.length());
	int resLen = std::stoi(resStrLen);
	int resHei = std::stoi(resStrHei);

	gameWrapper->LoadToastTexture("fr_logo", gameWrapper->GetDataFolder() / "fr_logo_short_square.png");

	// *** Initialize CVars *** //
	
	// Enable
	cvarManager->registerCvar("flipready_enabled", "1", "Disable/enable the plugin [0|1]", true);
	
	// Colors
	cvarManager->registerCvar("flipready_color_fliptext", "#00FF00FF", "Change \"Flip\" text color with hexcode.", true);
	cvarManager->registerCvar("flipready_color_nofliptext", "#FF0000FF", "Change \"No Flip\" text color with hexcode.", true);
	cvarManager->registerCvar("flipready_color_gaugebar", "#00FF00FF", "Change gauge bar color with hexcode.", true, true);
	
	// Sizes
	cvarManager->registerCvar("flipready_fontsize", "20", "Change fontsize [1-100].", true);
	cvarManager->registerCvar("flipready_barlen", "20", "Change gauge bar length [1-100].", true);
	cvarManager->registerCvar("flipready_barheight", "5", "Change gauge bar height [1-100].", true);
	
	// Gauge Bar Options
	cvarManager->registerCvar("flipready_decaydir", "left", "Change direction of gauge bar decay [left|right|down|up|h_collapse|v_collapse].", true);
	
	// Positioning
	cvarManager->registerCvar("flipready_positionx", std::to_string(resLen * 0.1), "Change horizontal position [0-resolution length).", true);
	cvarManager->registerCvar("flipready_positiony", std::to_string(resHei * 0.15), "Change vertical position [0-resolution height).", true);

	// *** End Initialize Cvars *** //


	// *** Input Validation *** //

	// Enable
	cvarManager->getCvar("flipready_enabled").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 0 || cvar.getIntValue() > 1) {
			LOG("Value ({}) not saved. Please enter one of [0|1]", cvar.getIntValue());
			cvar.setValue(oldval);
		}
	});

	// Colors
	// CVarManager does a good enough job validating color inputs
	// Too much regex required here - causes program to crash

	// Sizes
	// Font Size
	cvarManager->getCvar("flipready_fontsize").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 1 || cvar.getIntValue() > 100) {
			LOG("Value ({}) not saved. Please enter a value between [1-100].", cvar.getIntValue());
			cvar.setValue(oldval);
		}
	});
	// Bar Length
	cvarManager->getCvar("flipready_barlen").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 1 || cvar.getIntValue() > 100) {
			LOG("Value ({}) not saved. Please enter a value between [1-100].", cvar.getIntValue());
			cvar.setValue(oldval);
		}
	});
	// Bar Height
	cvarManager->getCvar("flipready_barheight").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 1 || cvar.getIntValue() > 100) {
			LOG("Value ({}) not saved. Please enter a value between [1-25].", cvar.getIntValue());
			cvar.setValue(oldval);
		}
	});

	// Gauge Bar Options
	// Decay Direction
	cvarManager->getCvar("flipready_decaydir").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "left" && cvar.getStringValue() != "right"
			&& cvar.getStringValue() != "down" && cvar.getStringValue() != "up"
			&& cvar.getStringValue() != "h_collapse" && cvar.getStringValue() != "v_collapse") {
			LOG("Value ({}) not saved. Please enter one of [left|right]", cvar.getStringValue());
			cvar.setValue(oldval);
		}
	});

	// Positioning
	// Horizontal Position
	cvarManager->getCvar("flipready_positionx").addOnValueChanged([this, resLen](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 0 || cvar.getIntValue() > resLen) {
			LOG("Value ({}) not saved. Please enter a value between [0-{}).", cvar.getIntValue(), resLen);
			cvar.setValue(oldval);
		}
	});

	// Vertical Position
	cvarManager->getCvar("flipready_positiony").addOnValueChanged([this, resHei](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 0 || cvar.getIntValue() > resHei) {
			LOG("Value ({}) not saved. Please enter a value between [0-{}).", cvar.getIntValue(), resHei);
			cvar.setValue(oldval);
		}
	});
	
	// *** End Input Validation *** //

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

	// Initialize variables with corresponding CVars
	LinearColor colorFlipText = cvarManager->getCvar("flipready_color_fliptext").getColorValue();
	LinearColor colorNoFlipText = cvarManager->getCvar("flipready_color_nofliptext").getColorValue();
	LinearColor colorGaugeBar = cvarManager->getCvar("flipready_color_gaugebar").getColorValue();
	float fontSize = cvarManager->getCvar("flipready_fontsize").getFloatValue();
	float barLen = 10 * cvarManager->getCvar("flipready_barlen").getFloatValue();
	float barHeight = 10 * cvarManager->getCvar("flipready_barheight").getFloatValue();
	int posX = cvarManager->getCvar("flipready_positionx").getFloatValue();
	int posY = cvarManager->getCvar("flipready_positiony").getFloatValue();
	std::string decayDir = cvarManager->getCvar("flipready_decaydir").getStringValue();

	// End Pre-Logic


	float realTime = game.GetWorldInfo().GetTimeSeconds();  // realtime of server
	static float timer = 0;
	float deltaTime = 1.5 + (timer - realTime);				// decay down from 1.5 -> 0
	auto car = game.GetCars().Get(0);						// user car object

	Vector2 screen = canvas.GetSize();
	float exactFontSize = (float)screen.X * 0.0001 * fontSize;

	canvas.SetColor(255, 255, 255, 255);
	canvas.SetPosition(Vector2{ int(posX), int(posY) });


	// CORE FUNCTIONALITY
	std::string flip_str;
	bool hasFlip = car.HasFlip();
	unsigned long jumped = car.GetbJumped();
	
	// Car doesn't have flip - is in air
	if ((!car.HasFlip() && displayComponent != 3 && displayComponent != 1) || displayComponent == 2) {
		flip_str = "NO FLIP";

		// Positioning
		posX -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5;
		posY -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5;

		canvas.SetColor(colorNoFlipText);
		canvas.SetPosition(Vector2{ int(posX), int(posY) });
		canvas.DrawString(flip_str, exactFontSize, exactFontSize);
	}
	// Car jumped - timer engages
	else if ((jumped && displayComponent != 1) || displayComponent == 3) {

		// Positioning
		posX -= barLen * 0.5;
		posY -= barHeight * 0.5;

		canvas.SetColor(colorGaugeBar);					

		// indicator gauge is drawn and filled, depletes to empty over 1.5 seconds

		canvas.SetPosition(Vector2{ int(posX), int(posY) });
		canvas.DrawBox(Vector2{ int(barLen), int(barHeight) });
		
		if (displayComponent != 3) {
			if (decayDir == "left") {
				canvas.FillBox(Vector2{ int(barLen * (deltaTime / 1.5)), int(barHeight) });
			}
			else if (decayDir == "right") {
				posX += barLen * ((realTime - timer) / 1.5);
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen * (deltaTime / 1.5)), int(barHeight) });
			}
			else if (decayDir == "down") {
				posY += barHeight * ((realTime - timer) / 1.5);
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen), int(barHeight * (deltaTime / 1.5)) });
			}
			else if (decayDir == "up") {
				canvas.FillBox(Vector2{ int(barLen), int(barHeight * (deltaTime / 1.5)) });
			}
			else if (decayDir == "h_collapse") {
				posX += barLen * 0.5 * ((realTime - timer) / 1.5);
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen * (deltaTime / 1.5)), int(barHeight) });
			}
			else if (decayDir == "v_collapse") {
				posY += barHeight * 0.5 * ((realTime - timer) / 1.5);
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen), int(barHeight * (deltaTime / 1.5)) });
			}
			else {
				canvas.FillBox(Vector2{ int(barLen * (deltaTime / 1.5)), int(barHeight) });
			}
		}
		else {
			if (decayDir == "left") {
				canvas.FillBox(Vector2{ int(barLen / 2), int(barHeight) });
			}
			else if (decayDir == "right") {
				posX += barLen / 2;
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen / 2), int(barHeight) });
			}
			else if (decayDir == "down") {
				posY += barHeight / 2;
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen), int(barHeight / 2) });
			}
			else if (decayDir == "up") {
				canvas.FillBox(Vector2{ int(barLen), int(barHeight / 2) });
			}
			else if (decayDir == "h_collapse") {
				posX += barLen / 4;
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen / 4), int(barHeight) });
			}
			else if (decayDir == "v_collapse") {
				posY += barHeight / 4;
				canvas.SetPosition(Vector2{ int(posX), int(posY) });
				canvas.FillBox(Vector2{ int(barLen), int(barHeight / 4) });
			}
		}
	}
	// Car has flip
	else if (car.HasFlip() || displayComponent == 1) {								
		flip_str = "FLIP";
		
		// Positioning
			posX -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5;
			posY -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5;

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