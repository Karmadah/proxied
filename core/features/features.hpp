#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void auto_strafe(c_usercmd* cmd);
	};
	float NormalizeYaw(float yaw);
}

namespace aimbot {
	void run_aimbot(c_usercmd* cmd);
	vec3_t calc_angle(const vec3_t& vecSource, const vec3_t& vecDestination);
	vec3_t normalize(vec3_t angle);
};

namespace visuals {
	//box 
	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
	void visual_main();
	bool get_playerbox(entity_t* ent, box& in);
	namespace visuals_subfunctions {
		void esp(player_t* ent);
		void force_crosshair();
		void innaccuracy_crosshair();
	};
};