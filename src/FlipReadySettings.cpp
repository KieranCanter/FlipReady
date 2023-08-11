#include "pch.h"
#include "FlipReady.h"
#include <sstream>
#include <iostream>

void ShowColors(FRStyle* ref);
void ShowSizes(FRStyle* ref);
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
}

void FlipReady::ShowColors(FRStyle* ref) {
	if (ImGui::BeginTabBar("##colors_tab", ImGuiTabBarFlags_None)) {
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
	if (ImGui::BeginTabBar("##sizes_tab", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Sizes (Ctrl+click to enter values)")) {

			// Keep Bar Ratio:
			CVarWrapper barLenCvar = cvarManager->getCvar("flipready_barlen");
			float barLen = barLenCvar.getFloatValue();
			CVarWrapper barHeightCvar = cvarManager->getCvar("flipready_barheight");
			float barHeight = barHeightCvar.getFloatValue();
			float ratio = barLen / barHeight;

			CVarWrapper keepRatioCvar = cvarManager->getCvar("flipready_keepbarratio");
			static bool keepRatio = keepRatioCvar.getBoolValue();
			if (ImGui::Checkbox("Maintain Gauge Bar Ratio", &keepRatio)) {
				keepRatioCvar.setValue(keepRatio);
			}

			if (ratio != 4.0f) {
				ImGui::SameLine(0.0f, 15.0f);
				if (ImGui::Button("RESET RATIO", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					ratio = 4.0f;
					barLenCvar.setValue(barHeight * ratio);
					keepRatio = true;
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
				if (keepRatio) {
					if (barLen >= ratio) {
						barLenCvar.setValue(barLen);
						barHeightCvar.setValue(barLen * (1/ratio));
					}
					else {
						barLenCvar.setValue(ratio);
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
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: 20.0");
			}
			if (barLen != ref->bar_len) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barLenCvar.setValue(ref->bar_len);
					if (keepRatio) {
						barHeightCvar.setValue(ref->bar_len * (1/ratio));
					}
				}
				if (ImGui::IsItemHovered()) {
					std::stringstream prev;
					prev << std::setprecision(1) << std::fixed << "Revert back to: " << ref->bar_len;
					if (keepRatio) {
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
				if (keepRatio) {
					barLenCvar.setValue(barHeight * ratio);
				}
			}
			if (barHeight != frstyle_default.bar_height) {
				ImGui::SameLine(0.0f, 1.0f);
				if (ImGui::Button("RESET DEFAULT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barHeightCvar.setValue(frstyle_default.bar_height);
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Default: 5.0");
			}
			if (barHeight != ref->bar_height) {
				ImGui::SameLine(0.0f, 2.0f);
				if (ImGui::Button("REVERT", ImVec2(buttonSize, ImGui::GetFrameHeight()))) {
					barHeightCvar.setValue(ref->bar_height);
					if (keepRatio) {
						barLenCvar.setValue(ref->bar_height * ratio);
					}
				}
				if (ImGui::IsItemHovered()) {
					std::stringstream prev;
					prev << std::setprecision(1) << std::fixed << "Revert back to: " << ref->bar_height;
					if (keepRatio) {
						prev << " (also length -> " << ref->bar_len << ")";
					}

					std::string out = prev.str();
					ImGui::SetTooltip(out.c_str());
				}
			}
			ImGui::PopID();

			


			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

std::string linearcolor2hex(LinearColor color) {
	char res[16];
	snprintf(res, sizeof res, "%02X%02X%02X%02X", int(color.R), int(color.G), int(color.B), int(color.A));
	return std::string(res);
}