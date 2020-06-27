#include "../features.hpp"

void aimbot::run_aimbot(c_usercmd* cmd)
{
	float closest = FLT_MAX;
	int hitbox = 0;

	if (!variables::aim_enable)
		return;

	if (variables::aim_head)
		hitbox = 0;
	else if (variables::aim_body)
		hitbox = 6;

	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); i++)
	{
		player_t* player = (player_t*)interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player());
		player_t* e = (player_t*)interfaces::entity_list->get_client_entity(i);

		if (!e)
			continue;

		if (e->team() == player->team())
			continue;

		if (e == csgo::local_player)
			continue;

		if (e->health() <= 0)
			continue;

		if (e->health() <= variables::aim_health_hitbox_override)
			hitbox = 6;

		vec3_t ed = aimbot::normalize(aimbot::calc_angle(player->get_eye_pos(), e->get_hitbox_position(hitbox)));
		if ((cmd->viewangles - ed).Length2D() <= closest)
			closest = (cmd->viewangles - ed).Length2D();

		if (!e->dormant() && e->is_alive() && e->is_player() && (cmd->viewangles - ed).Length2D() == closest)
		{
			if (GetAsyncKeyState(VK_LBUTTON) && (cmd->viewangles - ed).Length2D() <= variables::aim_fov)
				cmd->viewangles = ed;
		}
	}
}

vec3_t aimbot::calc_angle(const vec3_t& vecSource, const vec3_t& vecDestination)
{
	vec3_t qAngles;
	vec3_t delta = vec3_t((vecSource[0] - vecDestination[0]), (vecSource[1] - vecDestination[1]), (vecSource[2] - vecDestination[2]));
	float hyp = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
	qAngles[0] = (float)(atan(delta[2] / hyp) * (180.0f / M_PI));
	qAngles[1] = (float)(atan(delta[1] / delta[0]) * (180.0f / M_PI));
	qAngles[2] = 0.f;
	if (delta[0] >= 0.f)
		qAngles[1] += 180.f;

	return qAngles;
}

vec3_t aimbot::normalize(vec3_t angle)
{
	if (angle.x > 89.f)
		angle.x = 89.f;

	if (angle.x < -89.f)
		angle.x = -89.f;

	if (angle.y > 180)
		angle.y = 180;

	if (angle.y < -180)
		angle.y = -180;

	angle.z = 0;

	return angle;
}