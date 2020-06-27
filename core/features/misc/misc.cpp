#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) 
{//note: let user choose the key
	if (GetAsyncKeyState('V') || variables::bhop_max < 1)
		variables::bhop_count = 0;
	if (!variables::bhop_enable || rand() % 100 > variables::bhop_hitchance)
		return;
	if (variables::bhop_count > variables::bhop_max) {
		return;
	}

	static bool last_jumped = false, should_fake = false;

	if (!last_jumped && should_fake) {
		should_fake = false;
		variables::bhop_count += 1;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump) {
		if (csgo::local_player->flags() & fl_onground) {
			last_jumped = true;
			should_fake = true;
		}
		else {
			cmd->buttons &= ~in_jump;
			last_jumped = false;
		}
	}
	else {
		last_jumped = false;
		should_fake = false;
	}
};

void misc::movement::auto_strafe(c_usercmd* cmd) 
{

}
