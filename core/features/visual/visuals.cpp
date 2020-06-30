#include "../features.hpp"

void visuals::visual_main()
{
	auto player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!variables::visual_enable)
		return;

	if (variables::visual_force_crosshair && interfaces::engine->is_in_game())
		visuals::visuals_subfunctions::force_crosshair();
	if (variables::visual_spread_crosshair &&interfaces::engine->is_in_game())
		visuals::visuals_subfunctions::innaccuracy_crosshair();

	if (variables::visual_amb_light_red && interfaces::engine->is_in_game())
		interfaces::engine->execute_cmd("mat_ambient_light_r 100");
	else 
		interfaces::engine->execute_cmd("mat_ambient_light_r 0");
	if (variables::visual_amb_light_green && interfaces::engine->is_in_game())
		interfaces::engine->execute_cmd("mat_ambient_light_g 100");
	else 
		interfaces::engine->execute_cmd("mat_ambient_light_g 0");
	if (variables::visual_amb_light_blue && interfaces::engine->is_in_game())
		interfaces::engine->execute_cmd("mat_ambient_light_b 100");
	else
		interfaces::engine->execute_cmd("mat_ambient_light_b 0");

	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); i++)
	{
		auto e = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!e)
			continue;

		if (e->team() == player->team())
			continue;

		if (e == csgo::local_player)
			continue;

		if (e->health() <= 0)
			continue;

		visuals::visuals_subfunctions::esp(e);
	}
}

void visuals::visuals_subfunctions::esp(player_t* ent)
{
	int green = 0;
	int red = 0;
	ent->health() <= 75 ? red = 255 : green = 255;
	player_info_t player_info;
	interfaces::engine->get_player_info(ent->index(), &player_info);
	
	box b_box;
	if (!visuals::get_playerbox(ent, b_box))
		return;

	if (variables::visual_esp_box) {
		render::draw_rect(b_box.x, b_box.y, b_box.w, b_box.h, color(red, green, 0, 255));
		render::draw_rect(b_box.x - 1, b_box.y - 1, b_box.w + 2, b_box.h + 2, color(red, 0, 0, 255));
	}
	if (variables::visual_name) {
		render::draw_text_string(b_box.x - 20, b_box.y + 20, render::fonts::watermark_font, player_info.name, true, color(red, green, 0, 255));
	}
	if (variables::visual_weapon) {
		auto weapon = ent->active_weapon();
		if (!weapon)
			return;

		std::string weapon_name = weapon->get_weapon_data()->m_szWeaponName;
		render::draw_text_string(b_box.x + (b_box.w / 2), b_box.h + b_box.y + 2, render::fonts::watermark_font, weapon_name, true, color(red, green, 0, 255));
	}
	if (variables::visual_skeleton) {
		//later
	}
}

void visuals::visuals_subfunctions::force_crosshair()
{
	int x, y;
	interfaces::engine->get_screen_size(x, y);
	x = x / 2;
	y = y / 2;
	render::draw_line(x - 5, y - 5, x + 5, y + 5, color::red(255));
	render::draw_line(x + 5, y - 5, x - 5, y + 5, color::red(255));
}

void visuals::visuals_subfunctions::innaccuracy_crosshair()
{
	int x, y;
	interfaces::engine->get_screen_size(x, y);
	x = x / 2;
	y = y / 2;
	if(csgo::local_player->is_moving())
		render::draw_circle(x, y, 50, 100, color::red(255));
}

bool visuals::get_playerbox(entity_t* ent, box& in) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	origin = ent->get_absolute_origin();
	min = ent->collideable()->mins() + origin;
	max = ent->collideable()->maxs() + origin;

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	if (!interfaces::debug_overlay->world_to_screen(points[3], flb) || !interfaces::debug_overlay->world_to_screen(points[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points[0], blb) || !interfaces::debug_overlay->world_to_screen(points[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points[2], frb) || !interfaces::debug_overlay->world_to_screen(points[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points[6], blt) || !interfaces::debug_overlay->world_to_screen(points[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	in.x = (int)left;
	in.y = (int)top;
	in.w = int(right - left);
	in.h = int(bottom - top);

	return true;
}