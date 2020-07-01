#pragma once

namespace variables {

	//rage
	inline bool aim_enable = false;
	inline float aim_fov = 0.f;
	inline bool aim_recoil = false;
	inline bool aim_head = false;
	inline bool aim_body = false;
	inline float aim_health_hitbox_override = 0.f;
	inline float aim_max_misses = 0.f;
	inline float aim_hitchance = 0.f;

	//visual
	inline bool visual_enable = false;
	inline bool visual_esp_box = false;
	inline bool visual_skeleton = false;
	inline bool visual_force_crosshair = false;
	inline bool visual_spread_crosshair = false;
	inline bool visual_name = false;
	inline bool visual_chams = false;
	inline bool visual_chamsGlass = false;
	inline bool visual_chamsEnable = false;
	inline bool visual_chamsIgnoreZ = false;
	inline bool visual_weapon = false;
	inline bool visual_amb_light_red = false;
	inline bool visual_amb_light_green = false;
	inline bool visual_amb_light_blue = false;
	inline bool visual_grenade_prediction = false;

	//misc
	inline bool bhop_enable = false;
	inline bool auto_strafe_enable = false;
	inline float bhop_max = 0;
	inline float bhop_hitchance = 100;
	inline bool misc_thirdperson = false;

	//cfg
	inline bool watermark_enable = false;

	//internals
	inline int bhop_count = 0;
	inline int bhop_timer = 0;
	inline float visual_chams_red = 0.f;
	inline float visual_chams_green = 0.f;
	inline float visual_chams_blue = 0.f;
	namespace menu {
		inline bool opened = true;
		inline int x = 140, y = 140;
		inline float w = 670, h = 520;
		inline float group_box_1_x = 0, group_box_1_y = 0, group_box_2_x, group_box_2_y;
	}
}