#include "../features.hpp"

void misc::movement::anti_aimbot_run(c_usercmd* cmd)
{
	if (cmd->buttons & in_attack || variables::anti_aim_enable)
		return;

	cmd->viewangles.y = 89.f;// pitch

	if (csgo::sending_packet)
		cmd->viewangles.x += 178.9f;
	else 
		cmd->viewangles.x = cmd->viewangles.x + 58.f ;
}