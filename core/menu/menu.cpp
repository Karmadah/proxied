#include "menu.hpp"

//todo auto elements positioning

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
		render::draw_filled_rect(x, y, w, h, bg);
		render::draw_filled_rect(x, y, w, 30, header_text);
		render::draw_filled_rect(x, y + 30, w, 2, header_line);
		render::draw_text_string(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white());
};

void menu::render() {
	if (!variables::menu::opened)
		return;

	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "PROXIED - FREE");
	
	//menu_framework::group_box(variables::menu::x + 20, variables::menu::y + 0, 100, 260, render::fonts::watermark_font, "tabs", false); {
		menu_framework::tab(variables::menu::x + 250, variables::menu::y + 0, 100, 30, render::fonts::watermark_font, "Aimbot", menu::current_tab, 0, false);
		menu_framework::tab(variables::menu::x + 350, variables::menu::y + 0, 100, 30, render::fonts::watermark_font, "Visuals", menu::current_tab, 1, false);
		menu_framework::tab(variables::menu::x + 450, variables::menu::y + 0, 100, 30, render::fonts::watermark_font, "Misc", menu::current_tab, 2, false);
		menu_framework::tab(variables::menu::x + 550, variables::menu::y + 0, 100, 30, render::fonts::watermark_font, "Config", menu::current_tab, 3, false);
	//}

	switch (current_tab) {
	case 0:
		menu_framework::group_box(variables::menu::x + 100 + variables::menu::group_box_1_x, variables::menu::y + 35 + variables::menu::group_box_1_y, 550, 230, render::fonts::watermark_font, "aimbot", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Aimbot", variables::aim_enable);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 60, 125, render::fonts::watermark_font, "Field of view", variables::aim_fov, 0.f, 30.f);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Recoil", variables::aim_recoil);
		}
		menu_framework::group_box(variables::menu::x + 100 + variables::menu::group_box_2_x, variables::menu::y + 275 + variables::menu::group_box_2_y, 550, 230, render::fonts::watermark_font, "hitboxes", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 285, variables::menu::x + 235, render::fonts::watermark_font, "Head", variables::aim_head);
			menu_framework::check_box(variables::menu::x + 275, variables::menu::y + 285, variables::menu::x + 375, render::fonts::watermark_font, "Body", variables::aim_body);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 315, 125, render::fonts::watermark_font, "Health Based Override          hp", variables::aim_health_hitbox_override, 0.f, 100.f);
		}
		break;
	case 1:
		menu_framework::group_box(variables::menu::x + 100 + variables::menu::group_box_1_x, variables::menu::y + 35 + variables::menu::group_box_1_y, 550, 230, render::fonts::watermark_font, "visuals", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Enable", variables::visual_enable);
			if (variables::visual_enable)
			{
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "Force crosshair", variables::visual_force_crosshair);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, render::fonts::watermark_font, "Spread crosshair", variables::visual_spread_crosshair);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Box esp", variables::visual_esp_box);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, variables::menu::x + 375, render::fonts::watermark_font, "Name esp", variables::visual_name);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 120, variables::menu::x + 375, render::fonts::watermark_font, "Weapon esp", variables::visual_weapon);
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 135, variables::menu::x + 375, render::fonts::watermark_font, "Chams", variables::visual_chams);
			}
		}
		break;
	case 2:
		menu_framework::group_box(variables::menu::x + 100 + variables::menu::group_box_1_x, variables::menu::y + 35 + variables::menu::group_box_1_y, 550, 230, render::fonts::watermark_font, "misc", false); {
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Auto Jump", variables::bhop_enable);
				if (variables::bhop_enable)
				{
					menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "Auto strafe", variables::auto_strafe_enable);
					menu_framework::slider(variables::menu::x + 120, variables::menu::y + 75, 125, render::fonts::watermark_font, "Auto Jump Max - reset [v]", variables::bhop_max, 0, 5);
					menu_framework::slider(variables::menu::x + 120, variables::menu::y + 105, 125, render::fonts::watermark_font, "Auto Jump Hitchance", variables::bhop_hitchance, 0, 100);
				}
		}
		break;
	case 3:
		menu_framework::group_box(variables::menu::x + 100 + variables::menu::group_box_1_x, variables::menu::y + 35 + variables::menu::group_box_1_y, 550, 230, render::fonts::watermark_font, "Settings", false);{
				menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Watermark", variables::watermark_enable);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 60, 125, render::fonts::watermark_font, "Menu Width", variables::menu::w, 600.f, 800.f);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 90, 125, render::fonts::watermark_font, "Menu Height", variables::menu::h, 500.f, 800.f);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 150, 125, render::fonts::watermark_font, "Group Box 1 Position X", variables::menu::group_box_1_x, -50.f, 50.f);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 180, 125, render::fonts::watermark_font, "Group Box 1 Position Y", variables::menu::group_box_1_y, -50.f, 50.f);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 210, 125, render::fonts::watermark_font, "Group Box 2 Position Y", variables::menu::group_box_2_x, -50.f, 50.f);
				menu_framework::slider(variables::menu::x + 120, variables::menu::y + 240, 125, render::fonts::watermark_font, "Group Box 2 Position Y", variables::menu::group_box_2_y, -50.f, 50.f);
		}
		menu_framework::group_box(variables::menu::x + 100 + variables::menu::group_box_2_x, variables::menu::y + 275 + variables::menu::group_box_2_y, 550, 230, render::fonts::watermark_font, "Extra", false); {
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 305, 125, render::fonts::watermark_font, "Chams Red Value", variables::visual_chams_red, 0.f, 255.f);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 335, 125, render::fonts::watermark_font, "Chams Green Value", variables::visual_chams_green, 0.f, 255.f);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 365, 125, render::fonts::watermark_font, "Chams Blue Value", variables::visual_chams_blue, 0.f, 255.f);
		}
		break;
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}