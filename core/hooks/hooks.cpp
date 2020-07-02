#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::frame_stage::fn frame_stage_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;


bool hooks::initialize() {
	auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto scene_end_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	auto frame_stage_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));

	if (MH_Initialize() != MH_OK) {
		throw std::runtime_error("failed to initialize MH_Initialize.");
		return false;
	}

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(scene_end_target, &scene_end::hook, reinterpret_cast<void**>(&scene_end_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize scene_end. (outdated index?)");
		return false;
	}

	if (MH_CreateHook(frame_stage_target, &frame_stage::hook, reinterpret_cast<void**>(&frame_stage_original)) != MH_OK) {
		throw std::runtime_error("failed to initialize frame_stage. (outdated index?)");
		return false;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		throw std::runtime_error("failed to enable hooks.");
		return false;
	}

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

bool __fastcall hooks::create_move::hook(void* ecx, void* edx, int input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);
	misc::movement::auto_strafe(cmd);

	if (variables::aim_recoil) 
		cmd->viewangles -= csgo::local_player->aim_punch_angle() * 2.f;
		

	prediction::start(cmd); {
		aimbot::run_aimbot(cmd);

	} prediction::end();

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		if (variables::watermark_enable) {
			render::draw_filled_rect(10, 10, 80, 15, color::black(255));
			render::draw_text_string(10, 10, render::fonts::watermark_font, "   proxied.cc", false, color::blue(255));
		}
		menu::toggle();
		menu::render();

		visuals::visual_main();

		break;

	case fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::opened);
		interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::scene_end::hook()
{
	//now do ent loop
	for (int i = 1; i <= interfaces::globals->max_clients; i++)
	{
		auto e = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		auto p = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

		//if you read this im gey - karmada#1337
		if (!e || !e->is_alive() || e->dormant())
			continue;

		static i_material* mat_test = nullptr;
		static i_material* mat_Glass = nullptr;
		auto materialTextured = interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL, true, nullptr);
		auto materialGlass = interfaces::material_system->find_material("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_MODEL, true, nullptr);
		materialTextured->increment_reference_count(); // i forgot this and became gey
		mat_test = materialTextured;
		mat_Glass = materialGlass;
		float chams[4] = {variables::visual_chams_red / 255.f, variables::visual_chams_green / 255.f, variables::visual_chams_blue / 255.f, 255.f / 255.f};

		if (variables::visual_chams && e->team() != p->team())
		{
			interfaces::render_view->modulate_color(chams);
			interfaces::render_view->set_blend(1.f);
			if (variables::visual_chamsIgnoreZ)
			{
				mat_test->set_material_var_flag(material_var_ignorez, true);
			}
			else {
				mat_test->set_material_var_flag(material_var_ignorez, false);
			}

			interfaces::model_render->override_material(mat_test);
			e->draw_model(1, 255);
		}

		if (variables::visual_chamsGlass && e->team() != p->team())
		{
			interfaces::render_view->modulate_color(chams);
			interfaces::render_view->set_blend(0.9f);
			if (variables::visual_chamsIgnoreZ)
			{
				mat_test->set_material_var_flag(material_var_ignorez, true);
			}
			else {
				mat_test->set_material_var_flag(material_var_ignorez, false);
			}

			interfaces::model_render->override_material(mat_Glass);
			e->draw_model(1, 255);
		}

		interfaces::model_render->override_material(nullptr);
	}

	return scene_end_original(interfaces::render_view);
}

void __stdcall hooks::frame_stage::hook(client_frame_stage_t frame_stage)
{
	bool sv = false;
	convar* sv_cheats = interfaces::console->get_convar("sv_cheats");
	convar* cl_grenadepreview = interfaces::console->get_convar("cl_grenadepreview");

	if (frame_stage == FRAME_NET_UPDATE_END && interfaces::engine->is_in_game() && interfaces::engine->is_connected())
	{
		//auto p = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		if (sv == false) {
			sv_cheats->set_value(1);
			sv = true;
		}

		//thirdperson
		static bool init = false;
		if (variables::misc_thirdperson)
		{
			if (init)
			{
				interfaces::engine->execute_cmd("thirdperson");
			}
			init = false;
		}
		else
		{
			if (!init)
			{
				interfaces::engine->execute_cmd("firstperson");
			}
			init = true;
		}

		// grenade prediction
		if (variables::visual_grenade_prediction)
			cl_grenadepreview->set_value(1);
		else
			cl_grenadepreview->set_value(0);


	}
	frame_stage_original(interfaces::client, frame_stage);
}

void __fastcall hooks::override_view::hook(void* _this, int edx, interfaces::CViewSetup* vsView) {
	
	convar* weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");



	override_view_original(interfaces::clientmode);
}