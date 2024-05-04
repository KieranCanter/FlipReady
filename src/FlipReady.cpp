#include "pch.h"
#include "FlipReady.h"
#include "bakkesmod\wrappers\includes.h"
#include "bakkesmod/wrappers/Engine/WorldInfoWrapper.h"
#include "utils/parser.h"

#include <regex>
#include <algorithm>


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
	cvarManager->registerCvar("flipready_barheight", "5", "Change gauge bar height [1-25].", true);
	
	// Gauge Bar Options
	cvarManager->registerCvar("flipready_decaydir", "left", "Change direction of gauge bar decay [left|right].", true);
	
	// Positioning
	cvarManager->registerCvar("flipready_positionx", "0", "Change horizontal position [0-resolution length].", true);
	cvarManager->registerCvar("flipready_positiony", "0", "Change vertical position [0-resolution height].", true);

	// *** End Initialize Cvars *** //


	// *** Input Validation *** //

	// Enable
	cvarManager->getCvar("flipready_enabled").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 0 || cvar.getIntValue() > 1) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter one of [0|1]");
		}
	});

	// Colors
	// "Flip" Text
	cvarManager->getCvar("flipready_color_fliptext").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		// Check for general hexcode match
		std::regex generalPattern("^(\s*)#[a-fA-F0-9]{6}([a-fA-F0-9]{2})?(\s*)$");
		if (std::regex_match(cvar.getStringValue(), generalPattern)) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a valid hexcode value.");
		}

		// If not given alpha values in hexcode, automatically fill in as opaque
		std::regex noAlphaPattern("^(\s*)#[a-fA-F0-9]{6}(\s*)$");
		if (std::regex_match(cvar.getStringValue(), noAlphaPattern)) {
			std::string newVal = cvar.getStringValue() + "FF";
			cvar.setValue(newVal);
		}

		// If lowercase values are given, convert to uppercase
		std::regex lowercasePattern("^(\s*)#[a-f0-9]{8}(\s*)$");
		if (std::regex_match(cvar.getStringValue(), lowercasePattern)) {
			std::string newVal = cvar.getStringValue();
			std::transform(newVal.begin(), newVal.end(), newVal.begin(), ::toupper);
			cvar.setValue(newVal);
		}
	});

	// "No Flip" Text
	cvarManager->getCvar("flipready_color_nofliptext").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		// Check for general hexcode match
		std::regex generalPattern("^(\s*)#[a-fA-F0-9]{6}([a-fA-F0-9]{2})?(\s*)$");
		if (std::regex_match(cvar.getStringValue(), generalPattern)) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a valid hexcode value.");
		}

		// If not given alpha values in hexcode, automatically fill in as opaque
		std::regex noAlphaPattern("^(\s*)#[a-fA-F0-9]{6}(\s*)$");
		if (std::regex_match(cvar.getStringValue(), noAlphaPattern)) {
			std::string newVal = cvar.getStringValue() + "FF";
			cvar.setValue(newVal);
		}

		// If lowercase values are given, convert to uppercase
		std::regex lowercasePattern("^(\s*)#[a-f0-9]{8}(\s*)$");
		if (std::regex_match(cvar.getStringValue(), lowercasePattern)) {
			std::string newVal = cvar.getStringValue();
			std::transform(newVal.begin(), newVal.end(), newVal.begin(), ::toupper);
			cvar.setValue(newVal);
		}
	});

	// "Gauge Bar"
	cvarManager->getCvar("flipready_color_gaugebar").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		// Check for general hexcode match
		std::regex generalPattern("^(\s*)#[a-fA-F0-9]{6}([a-fA-F0-9]{2})?(\s*)$");
		if (std::regex_match(cvar.getStringValue(), generalPattern)) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a valid hexcode value.");
		}

		// If not given alpha values in hexcode, automatically fill in as opaque
		std::regex noAlphaPattern("^(\s*)#[a-fA-F0-9]{6}(\s*)$");
		if (std::regex_match(cvar.getStringValue(), noAlphaPattern)) {
			std::string newVal = cvar.getStringValue() + "FF";
			cvar.setValue(newVal);
		}

		// If lowercase values are given, convert to uppercase
		std::regex lowercasePattern("^(\s*)#[a-f0-9]{8}(\s*)$");
		if (std::regex_match(cvar.getStringValue(), lowercasePattern)) {
			std::string newVal = cvar.getStringValue();
			std::transform(newVal.begin(), newVal.end(), newVal.begin(), ::toupper);
			cvar.setValue(newVal);
		}
	});

	// Sizes
	// Font Size
	cvarManager->getCvar("flipready_fontsize").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 1 || cvar.getIntValue() > 100) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a value between 1 and 100 (inclusive).");
		}
	});
	// Bar Length
	cvarManager->getCvar("flipready_barlen").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 1 || cvar.getIntValue() > 100) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a value between 1 and 100 (inclusive).");
		}
	});
	// Bar Height
	cvarManager->getCvar("flipready_barheight").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 1 || cvar.getIntValue() > 25) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a value between 1 and 25 (inclusive).");
		}
	});

	// Gauge Bar Options
	// Decay Direction
	cvarManager->getCvar("flipready_decaydir").addOnValueChanged([this](std::string oldval, CVarWrapper cvar) {
		if (cvar.getStringValue() != "left" && cvar.getStringValue() != "right") {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter one of [left|right]");
		}
	});

	// Positioning
	// Horizontal Position
	cvarManager->getCvar("flipready_positionx").addOnValueChanged([this, resLen](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 0 || cvar.getIntValue() > resLen) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a value between 0 and {}.", resLen);
		}
	});

	// Vertical Position
	cvarManager->getCvar("flipready_positiony").addOnValueChanged([this, resHei](std::string oldval, CVarWrapper cvar) {
		if (cvar.getIntValue() < 0 || cvar.getIntValue() > resHei) {
			cvar.setValue(oldval);
			LOG("Value not saved. Please enter a value between 0 and {}.", resHei);
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
	float posX = cvarManager->getCvar("flipready_positionx").getFloatValue();
	float posY = cvarManager->getCvar("flipready_positiony").getFloatValue();
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
	
	// Car doesn't have flip
	if ((!car.HasFlip() && displayComponent != 3 && displayComponent != 1) || displayComponent == 2) {
		flip_str = "NO FLIP";

		// Positioning
		posX -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5;
		posY -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5;
		
		/* Alignments for future use?
		Align Left:		posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) + (barLen * 0.5);
		Align Right:	posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) - (barLen * 0.5);
		Align Top:		posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) + (barHeight * 0.5);
		Align Bottom:	posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) - (barHeight * 0.5);
		*/

		canvas.SetColor(colorNoFlipText);
		canvas.SetPosition(Vector2{ int(posX), int(posY) });
		canvas.DrawString(flip_str, exactFontSize, exactFontSize);
	}
	// Car jumped - timer engages
	else if ((jumped && displayComponent != 1) || displayComponent == 3) {

		// Positioning
		posX -= barLen * 0.5;
		posY -= barHeight * 0.5;
		
		/* Alignments for future use?
		Align Left:		posx += barLen;
		Align Right:	posX -= barLen;
		Align Top:		posY += barHeight;	
		Align Bottom:	posY -= barHeight;
		*/

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
	// Car has flip
	else if (car.HasFlip() || displayComponent == 1) {								
		flip_str = "FLIP";
		
		// Positioning
			posX -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5;
			posY -= canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5;
		
		/* Alignments for future use?
			Align Left:		posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) + (barLen * 0.5);
			Align Right:	posX = posX - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).X * 0.5) - (barLen * 0.5);
			Align Top:		posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) + (barHeight * 0.5);
			Align Bottom:	posY = posY - (canvas.GetStringSize(flip_str, exactFontSize, exactFontSize).Y * 0.5) - (barHeight * 0.5);
		*/		

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