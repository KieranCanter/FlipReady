#include "pch.h"
#include "FlipReady.h"
#include <sstream>
#include <iostream>

void ShowColors(FRStyle* ref);
void ShowSizes(FRStyle* ref);
void ShowLocation(FRStyle* ref);
std::string linearcolor2hex(LinearColor color);


const FRStyle frstyle_default = FRStyle{
	LinearColor{0, 255, 0, 255},
	LinearColor{255, 0, 0, 255},
	LinearColor{0, 255, 0, 255},
	20.0f,
	20.0f,
	5.0f,
	"left",
	200,
	150,
};

const float lineupBars = 130.0f;
const float buttonSize = 100.0f;
bool inDragMode = false;

void FlipReady::RenderSettings() {


	static FRStyle frstyle= FRStyle{
		cvarManager->getCvar("flipready_color_fliptext").getColorValue(),
		cvarManager->getCvar("flipready_color_nofliptext").getColorValue(),
		cvarManager->getCvar("flipready_color_gaugebar").getColorValue(),
		cvarManager->getCvar("flipready_fontsize").getFloatValue(),
		cvarManager->getCvar("flipready_barlen").getFloatValue(),
		cvarManager->getCvar("flipready_barheight").getFloatValue(),
		cvarManager->getCvar("flipready_decaydir").getStringValue(),
		cvarManager->getCvar("flipready_positionx").getIntValue(),
		cvarManager->getCvar("flipready_positiony").getIntValue(),
	};


	// Enable toggle
	CVarWrapper enableCvar = cvarManager->getCvar("flipready_enabled");
	static bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable Plugin", &enabled)) {
		enableCvar.setValue(enabled);
	}

	if (ImGui::Button("SAVE ALL", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
		frstyle = FRStyle{
			cvarManager->getCvar("flipready_color_fliptext").getColorValue(),
			cvarManager->getCvar("flipready_color_nofliptext").getColorValue(),
			cvarManager->getCvar("flipready_color_gaugebar").getColorValue(),
			cvarManager->getCvar("flipready_fontsize").getFloatValue(),
			cvarManager->getCvar("flipready_barlen").getFloatValue(),
			cvarManager->getCvar("flipready_barheight").getFloatValue(),
			cvarManager->getCvar("flipready_decaydir").getStringValue(),
			cvarManager->getCvar("flipready_positionx").getIntValue(),
			cvarManager->getCvar("flipready_positiony").getIntValue(),
		};
		cvarManager->executeCommand("writeconfig", false);
		gameWrapper->Toast("SETTINGS \nSAVED", "", "fr_logo", 3.5, ToastType_Info);
	}

	if (ImGui::Button("RESET ALL", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
		ImGui::OpenPopup("Reset settings to default?");
	}
	
	int popupPosX = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2;
	int popupPosY = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2;
	ImVec2 center = ImVec2(popupPosX, popupPosY);
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Reset settings to default?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		popupPosX -= ImGui::GetWindowWidth() / 2;
		popupPosY -= ImGui::GetWindowHeight() / 2;
		center = ImVec2(popupPosX, popupPosY);
		ImGui::SetWindowPos(center);

		ImGui::Text("This will reset all settings to default.");
		ImGui::NewLine();
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0))) {
			frstyle = frstyle_default;
			cvarManager->getCvar("flipready_color_fliptext").setValue(frstyle_default.color_fliptext);
			cvarManager->getCvar("flipready_color_nofliptext").setValue(frstyle_default.color_nofliptext);
			cvarManager->getCvar("flipready_color_gaugebar").setValue(frstyle_default.color_gaugebar);
			cvarManager->getCvar("flipready_fontsize").setValue(frstyle_default.font_size);
			cvarManager->getCvar("flipready_barlen").setValue(frstyle_default.bar_len);
			cvarManager->getCvar("flipready_barheight").setValue(frstyle_default.bar_height);
			cvarManager->getCvar("flipready_decaydir").setValue(frstyle_default.decay_dir);
			cvarManager->getCvar("flipready_positiony").setValue(frstyle_default.position_y);
			cvarManager->getCvar("flipready_positionx").setValue(frstyle_default.position_x);

			gameWrapper->Toast("ALL SETTINGS RESET \nTO DEFAULT", "", "fr_logo", 3.5, ToastType_Info);
			cvarManager->executeCommand("writeconfig", false);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::NewLine();

	// Colors:
	FlipReady::ShowColors(&frstyle);

	ImGui::NewLine();

	// Sizes: 
	FlipReady::ShowSizes(&frstyle);

	ImGui::NewLine();

	// Location:
	FlipReady::ShowLocation(&frstyle);
}

void FlipReady::ShowSave(FRStyle* ref) {


}


void FlipReady::ShowColors(FRStyle* ref) {
	if (ImGui::BeginTabBar("##colors_tab", ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Colors (Crtl+click to enter values)")) {
			ImGuiColorEditFlags color_flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreview
				| ImGuiColorEditFlags_AlphaBar;

			static int clicked;

			// 'Flip' Text:
			ImGui::PushID("fliptext");
			ImGui::TextUnformatted("\"Flip\" Text:");

			ImGui::SameLine(lineupBars, 0.0f);
			CVarWrapper colorFlipTextCvar = cvarManager->getCvar("flipready_color_fliptext");
			LinearColor colorFlipText = colorFlipTextCvar.getColorValue();
			float colorFTArr[4] = { colorFlipText.R / 255, colorFlipText.G / 255, colorFlipText.B / 255, colorFlipText.A / 255 };
			
			if (ImGui::ColorEdit4("##FlipText", colorFTArr, color_flags)) {
				colorFlipTextCvar.setValue(LinearColor{ colorFTArr[0], colorFTArr[1], colorFTArr[2], colorFTArr[3] } * 255.0f);
			}
			if (ImGui::IsItemActivated())
				clicked = 1;
			if (ImGui::IsItemHovered() || ImGui::IsItemActive() || (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && clicked == 1))
				displayComponent = 1;
			else
				displayComponent = 0;

			if (colorFlipText != frstyle_default.color_fliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorFlipTextCvar.setValue(frstyle_default.color_fliptext);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %s", frstyle_default.color_fliptext);
			}
			if (colorFlipText != ref->color_fliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorFlipTextCvar.setValue(ref->color_fliptext);
				}
				if (ImGui::IsItemHovered()) {
					std::string prev = linearcolor2hex(ref->color_fliptext);
					ImGui::SetTooltip("Revert back to: #%s", linearcolor2hex(ref->color_fliptext));
				}
			}
			ImGui::PopID();

			// 'No Flip' Text:
			ImGui::PushID("nofliptext");
			ImGui::TextUnformatted("\"No Flip\" Text:");

			ImGui::SameLine(lineupBars, 0.0f);
			CVarWrapper colorNoFlipTextCvar = cvarManager->getCvar("flipready_color_nofliptext");
			LinearColor colorNoFlipText = colorNoFlipTextCvar.getColorValue();
			float colorNFTArr[4] = { colorNoFlipText.R / 255, colorNoFlipText.G / 255, colorNoFlipText.B / 255, colorNoFlipText.A / 255 };

			if (ImGui::ColorEdit4("##NoFlipText", colorNFTArr, color_flags)) {
				colorNoFlipTextCvar.setValue(LinearColor{ colorNFTArr[0], colorNFTArr[1], colorNFTArr[2], colorNFTArr[3] } *255.0f);
			}
			if (ImGui::IsItemActivated())
				clicked = 2;
			if (ImGui::IsItemHovered() || ImGui::IsItemActive() || (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && clicked == 2))
				displayComponent = 2;

			if (colorNoFlipText != frstyle_default.color_nofliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorNoFlipTextCvar.setValue(frstyle_default.color_nofliptext);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %s", frstyle_default.color_nofliptext);
			}
			if (colorNoFlipText != ref->color_nofliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorNoFlipTextCvar.setValue(ref->color_nofliptext);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: #%s", linearcolor2hex(ref->color_nofliptext));
				}
			}
			ImGui::PopID();

			// Gauge Bar:
			ImGui::PushID("gaugebar");
			ImGui::TextUnformatted("Gauge Bar:");

			ImGui::SameLine(lineupBars, 0.0f);
			CVarWrapper colorGaugeBarCvar = cvarManager->getCvar("flipready_color_gaugebar");
			LinearColor colorGaugeBar = colorGaugeBarCvar.getColorValue();
			float colorGBArr[4] = { colorGaugeBar.R / 255, colorGaugeBar.G / 255, colorGaugeBar.B / 255, colorGaugeBar.A / 255 };

			if (ImGui::ColorEdit4("##GaugeBar", colorGBArr, color_flags)) {
				colorGaugeBarCvar.setValue(LinearColor{ colorGBArr[0], colorGBArr[1], colorGBArr[2], colorGBArr[3] } * 255.0f);
			}
			if (ImGui::IsItemActivated())
				clicked = 3;
			if (ImGui::IsItemHovered() || ImGui::IsItemActive() || (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && clicked == 3))
				displayComponent = 3;

			if (colorGaugeBar != frstyle_default.color_gaugebar) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorGaugeBarCvar.setValue(frstyle_default.color_gaugebar);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %s", frstyle_default.color_gaugebar);
			}
			if (colorGaugeBar != ref->color_gaugebar) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorGaugeBarCvar.setValue(ref->color_gaugebar);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: #%s", linearcolor2hex(ref->color_gaugebar));
				}
			}
			ImGui::PopID();

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void FlipReady::ShowSizes(FRStyle* ref) {
	if (ImGui::BeginTabBar("##sizes_tab", ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Sizes (Ctrl+click to enter values)")) {
			CVarWrapper displayComponentCvar = cvarManager->getCvar("flipready_displaycomponent");

			// Loading sizes
			CVarWrapper fontSizeCvar = cvarManager->getCvar("flipready_fontsize");
			float fontSize = fontSizeCvar.getFloatValue();
			CVarWrapper barLenCvar = cvarManager->getCvar("flipready_barlen");
			float barLen = barLenCvar.getFloatValue();
			CVarWrapper barHeightCvar = cvarManager->getCvar("flipready_barheight");
			float barHeight = barHeightCvar.getFloatValue();

			// Font Size:
			ImGui::PushID("fontsize");
			ImGui::TextUnformatted("Font Size:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderFloat("##FontSize", &fontSize, 1.0f, 100.0f, "%.1f", 1.0f)) {	
				fontSizeCvar.setValue(fontSize);
			}
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
				displayComponent = 1;

			if (fontSize != frstyle_default.font_size) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					fontSizeCvar.setValue(frstyle_default.font_size);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %.1f", frstyle_default.font_size);
			}
			if (fontSize != ref->font_size) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					fontSizeCvar.setValue(ref->font_size);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: %.1f", ref->font_size);
				}
			}
			ImGui::PopID();

			// Bar Length:
			ImGui::PushID("barlen");
			ImGui::TextUnformatted("Gauge Bar Length:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderFloat("##BarLen", &barLen, 1.0f, 100.0f, "%.1f", 1.0f)) {
					barLenCvar.setValue(barLen);
			}
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
				displayComponent = 3;

			if (barLen != frstyle_default.bar_len) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barLenCvar.setValue(frstyle_default.bar_len);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %.1f", frstyle_default.bar_len);
			}
			if (barLen != ref->bar_len) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barLenCvar.setValue(ref->bar_len);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: %.1f", ref->bar_len);
				}
			}
			ImGui::PopID();

			// Bar Height:
			ImGui::PushID("barheight");
			ImGui::TextUnformatted("Gauge Bar Height:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderFloat("##BarHeight", &barHeight, 1.0f, 25.0f, "%.1f", 1.0f)) {
				barHeightCvar.setValue(barHeight);
			}
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
				displayComponent = 3;

			if (barHeight != frstyle_default.bar_height) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barHeightCvar.setValue(frstyle_default.bar_height);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %.1f", frstyle_default.bar_height);
			}
			if (barHeight != ref->bar_height) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barHeightCvar.setValue(ref->bar_height);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: %.1f", ref->bar_height);
				}
			}
			ImGui::PopID();

			ImGui::PushID("bardecay");
			CVarWrapper decayCvar = cvarManager->getCvar("flipready_decaydir");
			std::string decay = decayCvar.getStringValue();

			// Gauge Bar Decay
			ImGui::TextUnformatted("Gauge Bar Decay Direction:");
			ImGui::SameLine(lineupBars + 50.0f, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2{ 0.5, 0.5 });

			if (ImGui::Selectable("LEFT", decay == "left", ImGuiSelectableFlags_None, ImVec2(100, ImGui::GetFrameHeight())))
				decayCvar.setValue("left");
			if (ImGui::IsItemHovered())
				displayComponent = 3;

			ImGui::SameLine(0.0f, 10.0f);

			if (ImGui::Selectable("RIGHT", decay == "right", ImGuiSelectableFlags_None, ImVec2(100, ImGui::GetFrameHeight())))
				decayCvar.setValue("right");
			if (ImGui::IsItemHovered())
				displayComponent = 3;

			ImGui::PopStyleVar();
			ImGui::PopID();

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void FlipReady::ShowLocation(FRStyle* ref) {
	if (ImGui::BeginTabBar("##location_tab", ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Location")) {
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2{ 0.5, 0.5 });

			// Video resolution length and height
			int resLen = ImGui::GetIO().DisplaySize.x;
			int resHei = ImGui::GetIO().DisplaySize.y;

			CVarWrapper xLocCvar = cvarManager->getCvar("flipready_positionx");
			int xLoc = xLocCvar.getIntValue();
			CVarWrapper yLocCvar = cvarManager->getCvar("flipready_positiony");
			int yLoc = yLocCvar.getIntValue();

			// Drag Mode
			ImGui::Checkbox("Drag Mode", &inDragMode);
			
			if (inDragMode) {
				ImGui::SetMouseCursor(2);
				if (!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseDown(0)) {
					ImVec2 mousePos = ImGui::GetMousePos();
					xLocCvar.setValue(mousePos.x);
					yLocCvar.setValue(mousePos.y);
				}
			}

			// Horizontal Location
			ImGui::PushID("xloc");
			ImGui::TextUnformatted("Horizontal Position:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderInt("##XLoc", &xLoc, 0, resLen, "%d")) {
				xLocCvar.setValue(xLoc);
			}
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
				displayComponent = 1;

			if (xLoc != frstyle_default.position_x) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					xLocCvar.setValue(frstyle_default.position_x);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %d", frstyle_default.position_x);
			}
			if (xLoc != ref->position_x) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					xLocCvar.setValue(ref->position_x);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: %d", ref->position_x);
				}
			}
			ImGui::PopID();

			// Vertical Location
			ImGui::PushID("yloc");
			ImGui::TextUnformatted("Vertical Position:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderInt("##YLoc", &yLoc, 0, resHei, "%d")) {
				yLocCvar.setValue(yLoc);
			}
			if (ImGui::IsItemHovered() || ImGui::IsItemActive())
				displayComponent = 1;

			if (yLoc != frstyle_default.position_y) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					yLocCvar.setValue(frstyle_default.position_y);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: %d", frstyle_default.position_y);
			}
			if (yLoc != ref->position_y) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					yLocCvar.setValue(ref->position_y);
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Revert back to: %d", ref->position_y);
				}
			}
			ImGui::PopID();

			/*
			// Top row
			if (ImGui::Selectable("*##TL", posX == "left" && posY == "top", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posXCvar.setValue("left");
				posYCvar.setValue("top");
			}
			ImGui::SameLine();
			if (ImGui::Selectable("*##TM", posX == "middle" && posY == "top", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posYCvar.setValue("top");
				posXCvar.setValue("middle");
			}
			ImGui::SameLine();
			if (ImGui::Selectable("*##TR", posX == "right" && posY == "top", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posXCvar.setValue("right");
				posYCvar.setValue("top");
			}

			// Middle row
			if (ImGui::Selectable("*##ML", posX == "left" && posY == "middle", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posXCvar.setValue("left");
				posYCvar.setValue("middle");
			}
			ImGui::SameLine();
			ImGui::Selectable(" SELECT\nPOSITION", false, ImGuiSelectableFlags_Disabled, ImVec2(50, 50));
			ImGui::SameLine();
			if (ImGui::Selectable("*##MR", posX == "right" && posY == "middle", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posXCvar.setValue("right");
				posYCvar.setValue("middle");
			}

			// Bottom row
			if (ImGui::Selectable("*##BL", posX == "left" && posY == "bottom", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posXCvar.setValue("left");
				posYCvar.setValue("bottom");
			}
			ImGui::SameLine();
			if (ImGui::Selectable("*##BM", posX == "middle" && posY == "bottom", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posYCvar.setValue("bottom");
				posXCvar.setValue("middle");
			}
			ImGui::SameLine();
			if (ImGui::Selectable("*##BR", posX == "right" && posY == "bottom", ImGuiSelectableFlags_None, ImVec2(50, 50))) {
				posXCvar.setValue("right");
				posYCvar.setValue("bottom");
			}
			*/
			ImGui::PopStyleVar();
		}
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

std::string linearcolor2hex(LinearColor color) {
	char res[16];
	snprintf(res, sizeof res, "%02X%02X%02X%02X", int(color.R), int(color.G), int(color.B), int(color.A));
	return std::string(res);
}