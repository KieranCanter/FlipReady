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
	"top",
	"middle"
};

const float lineupBars = 130.0f;
const float buttonSize = 100.0f;

void FlipReady::RenderSettings() {


	static FRStyle frstyle= FRStyle{
		cvarManager->getCvar("flipready_color_fliptext").getColorValue(),
		cvarManager->getCvar("flipready_color_nofliptext").getColorValue(),
		cvarManager->getCvar("flipready_color_gaugebar").getColorValue(),
		cvarManager->getCvar("flipready_fontsize").getFloatValue(),
		cvarManager->getCvar("flipready_barlen").getFloatValue(),
		cvarManager->getCvar("flipready_barheight").getFloatValue(),
		cvarManager->getCvar("flipready_positionx").getStringValue(),
		cvarManager->getCvar("flipready_positiony").getStringValue(),
	};

	ImGui::TextUnformatted("Actively being updated with new options and aesthetics.");

	ImGui::NewLine();

	// Enable toggle
	CVarWrapper enableCvar = cvarManager->getCvar("flipready_enabled");
	static bool enabled = enableCvar.getBoolValue();
	if (ImGui::Checkbox("Enable Plugin", &enabled)) {
		enableCvar.setValue(enabled);
	}

	ImGui::NewLine();

	// Colors:
	FlipReady::ShowColors(&frstyle);

	ImGui::NewLine();

	FlipReady::ShowSizes(&frstyle);

	ImGui::NewLine();

	FlipReady::ShowLocation(&frstyle);
}

void FlipReady::ShowColors(FRStyle* ref) {
	if (ImGui::BeginTabBar("##colors_tab", ImGuiTabBarFlags_NoTooltip)) {
		if (ImGui::BeginTabItem("Colors (Crtl+click to enter values)")) {
			ImGuiColorEditFlags color_flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreview
				| ImGuiColorEditFlags_AlphaBar;

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
			if (colorFlipText != frstyle_default.color_fliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorFlipTextCvar.setValue(frstyle_default.color_fliptext);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: #00FF00FF");
			}
			if (colorFlipText != ref->color_fliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorFlipTextCvar.setValue(ref->color_fliptext);
				}
				if (ImGui::IsItemHovered()) {
					std::string prev = linearcolor2hex(ref->color_fliptext);
					ImGui::SetTooltip(("Revert back to: #" + prev).c_str());
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
			if (colorNoFlipText != frstyle_default.color_nofliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorNoFlipTextCvar.setValue(frstyle_default.color_nofliptext);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: #FF0000FF");
			}
			if (colorNoFlipText != ref->color_nofliptext) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorNoFlipTextCvar.setValue(ref->color_nofliptext);
				}
				if (ImGui::IsItemHovered()) {
					std::string prev = linearcolor2hex(ref->color_nofliptext);
					ImGui::SetTooltip(("Revert back to: #" + prev).c_str());
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
			if (colorGaugeBar != frstyle_default.color_gaugebar) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorGaugeBarCvar.setValue(frstyle_default.color_gaugebar);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: #00FF00FF");
			}
			if (colorGaugeBar != ref->color_gaugebar) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					colorGaugeBarCvar.setValue(ref->color_gaugebar);
				}
				if (ImGui::IsItemHovered()) {
					std::string prev = linearcolor2hex(ref->color_gaugebar);
					ImGui::SetTooltip(("Revert back to: #" + prev).c_str());
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

			// Keep Bar Ratio:
			CVarWrapper barLenCvar = cvarManager->getCvar("flipready_barlen");
			float barLen = barLenCvar.getFloatValue();
			CVarWrapper barHeightCvar = cvarManager->getCvar("flipready_barheight");
			float barHeight = barHeightCvar.getFloatValue();
			float barRatio = barLen / barHeight;

			CVarWrapper keepBarRatioCvar = cvarManager->getCvar("flipready_keepbarratio");
			static bool keepBarRatio = keepBarRatioCvar.getBoolValue();
			if (ImGui::Checkbox("Maintain Gauge Bar Ratio", &keepBarRatio)) {
				keepBarRatioCvar.setValue(keepBarRatio);
			}

			if (barRatio != 4.0f) {
				ImGui::SameLine(0.0f, 15.0f);
				if (ImGui::Button("RESET RATIO", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barRatio = 4.0f;
					barLenCvar.setValue(barHeight * barRatio);
					keepBarRatio = true;
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: Length:Height = 4:1");
			}

			// Font Size:
			ImGui::PushID("fontsize");
			ImGui::TextUnformatted("Font Size:");

			ImGui::SameLine(lineupBars, 0.0f);
			CVarWrapper fontSizeCvar = cvarManager->getCvar("flipready_fontsize");
			float fontSize = fontSizeCvar.getFloatValue();
			if (ImGui::SliderFloat("##FontSize", &fontSize, 1.0f, 100.0f, "%.1f", 1.0f)) {
				fontSizeCvar.setValue(fontSize);
			}
			if (fontSize != frstyle_default.font_size) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					fontSizeCvar.setValue(frstyle_default.font_size);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: 20.0");
			}
			if (fontSize != ref->font_size) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					fontSizeCvar.setValue(ref->font_size);
				}
				if (ImGui::IsItemHovered()) {
					std::stringstream prev;
					prev << std::setprecision(1) << std::fixed << "Revert back to: " << ref->font_size;

					std::string out = prev.str();
					ImGui::SetTooltip(out.c_str());
				}
			}
			ImGui::PopID();

			// Bar Length:
			ImGui::PushID("barlen");
			ImGui::TextUnformatted("Gauge Bar Length:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderFloat("##BarLen", &barLen, 1.0f, 100.0f, "%.1f", 1.0f)) {
				if (keepBarRatio) {
					if (barLen >= barRatio) {
						barLenCvar.setValue(barLen);
						barHeightCvar.setValue(barLen * (1/barRatio));
					}
					else {
						barLenCvar.setValue(barRatio);
						barHeightCvar.setValue(1);
					}
				}
				else {
					barLenCvar.setValue(barLen);
				}
			}
			if (barLen != frstyle_default.bar_len) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barLenCvar.setValue(frstyle_default.bar_len);
					if (keepBarRatio)
						barHeightCvar.setValue(frstyle_default.bar_height);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: 20.0");
			}
			if (barLen != ref->bar_len) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barLenCvar.setValue(ref->bar_len);
					if (keepBarRatio) {
						barHeightCvar.setValue(ref->bar_len * (1/barRatio));
					}
				}
				if (ImGui::IsItemHovered()) {
					std::stringstream prev;
					prev << std::setprecision(1) << std::fixed << "Revert back to: " << ref->bar_len;
					if (keepBarRatio) {
						prev << " (also height -> " << ref->bar_height << ")";
					}

					std::string out = prev.str();
					ImGui::SetTooltip(out.c_str());
				}
			}
			ImGui::PopID();

			// Bar Height:
			ImGui::PushID("barheight");
			ImGui::TextUnformatted("Gauge Bar Height:");

			ImGui::SameLine(lineupBars, 0.0f);
			if (ImGui::SliderFloat("##BarHeight", &barHeight, 1.0f, 25.0f, "%.1f", 1.0f)) {
				barHeightCvar.setValue(barHeight);
				if (keepBarRatio) {
					barLenCvar.setValue(barHeight * barRatio);
				}
			}
			if (barHeight != frstyle_default.bar_height) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barHeightCvar.setValue(frstyle_default.bar_height);
					if (keepBarRatio)
						barLenCvar.setValue(frstyle_default.bar_len);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: 5.0");
			}
			if (barHeight != ref->bar_height) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barHeightCvar.setValue(ref->bar_height);
					if (keepBarRatio) {
						barLenCvar.setValue(ref->bar_height * barRatio);
					}
				}
				if (ImGui::IsItemHovered()) {
					std::stringstream prev;
					prev << std::setprecision(1) << std::fixed << "Revert back to: " << ref->bar_height;
					if (keepBarRatio) {
						prev << " (also length -> " << ref->bar_len << ")";
					}

					std::string out = prev.str();
					ImGui::SetTooltip(out.c_str());
				}
			}
			ImGui::PopID();

			ImGui::PushID("bardecay");
			static int dummy2 = 0;

			ImGui::TextUnformatted("Gauge Bar Decay Direction:");
			ImGui::SameLine(lineupBars + 50.0f, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2{ 0.5, 0.5 });

			if (ImGui::Selectable("LEFT", dummy2 == 0, ImGuiSelectableFlags_None, ImVec2(100, ImGui::GetFrameHeight())))
				dummy2 = 0;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Gauge bar decays to the left");

			ImGui::SameLine(0.0f, 10.0f);

			if (ImGui::Selectable("RIGHT", dummy2 == 1, ImGuiSelectableFlags_None, ImVec2(100, ImGui::GetFrameHeight())))
				dummy2 = 1;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Gauge bar decays to the right");

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

			CVarWrapper posXCvar = cvarManager->getCvar("flipready_positionx");
			std::string posX = posXCvar.getStringValue();
			CVarWrapper posYCvar = cvarManager->getCvar("flipready_positiony");
			std::string posY = cvarManager->getCvar("flipready_positiony").getStringValue();

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