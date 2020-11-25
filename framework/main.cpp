#include "main.h"

c_base_weapon* c_base_player::get_active_weapon() {
	HANDLE weapon_handle = *reinterpret_cast<HANDLE*>((uintptr_t)this + 0xDB8);
	return (c_base_weapon*)interfaces::entitylist->get_client_entity_from_handle(weapon_handle);
}

const char* c_base_player::get_model_name() {
	return interfaces::model_info->get_model_name(this->get_model());
}

int c_base_player::register_glow_object(colour_t &colour, bool render_when_occluded, bool render_when_unoccluded)
{
	using do_thing = int(__thiscall*)(glow_manager_t*, c_base_player*, vec3_t&, float, bool, bool, int);
	static DWORD func = (DWORD)utilities::find_signature(xorstr("client.dll"), xorstr("55 8B EC 51 53 56 8B F1 57 8B 5E 14"));
	static do_thing reg = (do_thing)func;
	vec3_t col = vec3_t(colour.r(), colour.g(), colour.b());
	float alpha = colour.a();

	return reg(interfaces::glow_object, this, col, alpha, render_when_occluded, render_when_unoccluded, -1);
}

bool c_base_player::has_glow_effect()
{
	for (int i = 0; i < interfaces::glow_object->m_glow_object_definitions.Count(); ++i) {
		glow_object_t &object = interfaces::glow_object->m_glow_object_definitions[i];
		c_base_player *ent = (c_base_player*)interfaces::entitylist->get_client_entity_from_handle(object.entity_handle);
		if (ent == this)
			return true;
	}

	return false;
}

bool c_base_weapon::can_shoot(c_base_player* local)
{
	if (!local || local->get_active_weapon() || local->get_condnew() & TFCond_Cloaked || local->get_condnew() & TFCond_Bonked)
		return false;

	auto net_channel = interfaces::engineclient->get_net_channel_info();

	if (!net_channel)
		return false;

	float lerp = interfaces::convar->find_variable(xorstr("cl_interp"))->get_float();
	float latency = (net_channel->get_latency(0) + net_channel->get_latency(1));

	return (get_next_attack() < ((lerp + latency + (float)local->get_tick_base())) * interfaces::globals->interval_per_tick);
}

namespace utilities {
	void utilities::vector_transform(const vec3_t& in, const matrix3x4_t& matrix, vec3_t& out) {
		out = {
			in.dot(vec3_t(matrix[0][0], matrix[0][1], matrix[0][2])) + matrix[0][3],
			in.dot(vec3_t(matrix[1][0], matrix[1][1], matrix[1][2])) + matrix[1][3],
			in.dot(vec3_t(matrix[2][0], matrix[2][1], matrix[2][2])) + matrix[2][3]
		};
	}
	vec3_t utilities::angle_vector(vec3_t meme) {
		auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
		auto cy = cos(meme.y / 180.f * static_cast<float>(PI));
		auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
		auto cp = cos(meme.x / 180.f* static_cast<float>(PI));
		return vec3_t(cp * cy, cp * sy, -sp);
	}

	float utilities::distance_point_to_line(vec3_t point, vec3_t origin, vec3_t direction) {
		auto point_direction = point - origin;

		auto temporary_offset = point_direction.dot(direction) / (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		if (temporary_offset < 0.000001f) {
			return FLT_MAX;
		}

		auto perpendicular_point = origin + (direction * temporary_offset);

		return (point - perpendicular_point).length();
	}
	float utilities::get_field_of_view(const vec3_t& angle, const vec3_t& aim_angle) {
		vec3_t delta = aim_angle - angle;

		auto clamp_angle = [&](vec3_t& vector) {
			while (vector[0] > 89) { vector[0] -= 180; }
			while (vector[0] < -89) { vector[0] += 180; }
			while (vector[1] > 180) { vector[1] -= 360; }
			while (vector[1] < -180) { vector[1] += 360; }
			while (vector[2] != 0) { vector[2] = 0; }
		}; clamp_angle(delta);

		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}

	float utilities::fl_clamp(float in, float low, float high) {
		return clamp(in, low, high);
	}

	vec3_t utilities::vectorangles(vec3_t start, vec3_t end) {
		vec3_t delta = end - start;

		float magnitude = sqrtf(delta.x * delta.x + delta.y * delta.y);
		float pitch = atan2f(-delta.z, magnitude) * RADPI;
		float yaw = atan2f(delta.y, delta.x) * RADPI;

		vec3_t angle(pitch, yaw, 0.0f);
		auto clamp_angle = [&](vec3_t& vector) {
			while (vector[0] > 89) { vector[0] -= 180; }
			while (vector[0] < -89) { vector[0] += 180; }
			while (vector[1] > 180) { vector[1] -= 360; }
			while (vector[1] < -180) { vector[1] += 360; }
			while (vector[2] != 0) { vector[2] = 0; }
		};
		clamp_angle(angle);
		return angle;
	}

	vec3_t utilities::vectorangles2(vec3_t start, vec3_t end) {
		auto clamp_angle = [&](vec3_t& vector) {
			while (vector[0] > 89) { vector[0] -= 180; }
			while (vector[0] < -89) { vector[0] += 180; }
			while (vector[1] > 180) { vector[1] -= 360; }
			while (vector[1] < -180) { vector[1] += 360; }
			while (vector[2] != 0) { vector[2] = 0; }
		};
		vec3_t angles;
		vec3_t delta = start - end;
		vectorangles(delta, angles);
		clamp_angle(delta);
		return angles;
	}
	void utilities::vectorangles3(const vec3_t& forward, ang_t& angles, vec3_t* up)
	{
		vec3_t  left;
		float   len, up_z, pitch, yaw, roll;

		// get 2d length.
		len = forward.length2d();

		if (up && len > 0.001f) {
			pitch = RAD2DEG(std::atan2(-forward.z, len));
			yaw = RAD2DEG(std::atan2(forward.y, forward.x));

			// get left direction vector using cross product.
			left = (*up).cross(forward).normalized();

			// calculate up_z.
			up_z = (left.y * forward.x) - (left.x * forward.y);

			// calculate roll.
			roll = RAD2DEG(std::atan2(left.z, up_z));
		}

		else {
			if (len > 0.f) {
				// calculate pitch and yaw.
				pitch = RAD2DEG(std::atan2(-forward.z, len));
				yaw = RAD2DEG(std::atan2(forward.y, forward.x));
				roll = 0.f;
			}

			else {
				pitch = (forward.z > 0.f) ? -90.f : 90.f;
				yaw = 0.f;
				roll = 0.f;
			}
		}

		// set out angles.
		angles = { pitch, yaw, roll };
	}
	float utilities::get_field_of_view(vec3_t angle, vec3_t src, vec3_t dst) {
		vec3_t ang, aim; float mag, u_dot_v;
		ang = calc_angle(src, dst);

		make_vector(angle, aim); make_vector(ang, ang);

		mag = sqrtf(pow(aim[0], 2) + pow(aim[1], 2) + pow(aim[2], 2));
		u_dot_v = aim.dot(ang);

		return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
	}
	float utilities::get_angle_from_point(vec3_t point) {
		static int screen_x = 0;
		static int screen_y = 0;

		if (!screen_y || !screen_x) {
			interfaces::engineclient->get_screen_size(screen_x, screen_y);
		}

		int v_x = (screen_x / 2) - point.x;
		int v_y = (screen_y / 2) - point.y;

		float angle_radians = atan2(v_y, v_x);

		float angle_deg = RAD2DEG(angle_radians);

		return angle_deg;
	}
	vec3_t utilities::get_hitbox_position(c_base_player* entity, int hitbox) {
		DWORD* model = entity->get_model();
		if (!model) return vec3_t();

		studiohdr_t *hdr = interfaces::model_info->get_studio_model(model);
		if (!hdr) return vec3_t();

		matrix3x4_t matrix[128];
		if (!entity->setup_bones(matrix, 128, 0x100, 0))
			return vec3_t();

		int  hitbox_set_index = *(int *)((DWORD)hdr + 0xB0);
		if (!hitbox_set_index) return vec3_t();

		mstudiohitboxset_t* set = (mstudiohitboxset_t *)(((PBYTE)hdr) + hitbox_set_index);

		mstudiobbox_t* box = set->get_new_hitbox(hitbox);
		if (!box) return vec3_t();

		vec3_t center = (box->bbmin + box->bbmax) * 0.5f;

		vec3_t final_hitbox;

		utilities::vector_transform(center, matrix[box->bone], final_hitbox); //now with multipoint

		return final_hitbox;
	}

	vec3_t utilities::get_multipoint_position(c_base_player* entity, int hitbox, int scale)
	{
		DWORD* model = entity->get_model();
		if (!model) return vec3_t();

		studiohdr_t* hdr = interfaces::model_info->get_studio_model(model);
		if (!hdr) return vec3_t();

		matrix3x4_t matrix[128];
		if (!entity->setup_bones(matrix, 128, 0x100, 0))
			return vec3_t();

		int hitbox_set_index = *(int*)((DWORD)hdr + 0xB0);
		if (!hitbox_set_index) return vec3_t();

		mstudiohitboxset_t* set = (mstudiohitboxset_t*)(((PBYTE)hdr) + hitbox_set_index);

		mstudiobbox_t* box = set->get_new_hitbox(hitbox);
		if (!box) return vec3_t();

		vec3_t min = box->bbmin * (float)scale / 100.f;
		vec3_t max = box->bbmax * (float)scale / 100.f;

		vec3_t center = (min + max) * 0.5f;
		vec3_t vhitbox;
		
		//if (!center.validate())
		//	return vec3_t();

		utilities::vector_transform(center, matrix[box->bone], vhitbox);

		if (utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), vhitbox))
		{
			return vhitbox;
		}

		vec3_t points[]{
		  vec3_t(min.x, min.y, min.z),
		  vec3_t(min.x, max.y, min.z),
		  vec3_t(min.x, min.y, max.z),
		  vec3_t(min.x, max.y, max.z),
		  vec3_t(max.x, min.y, min.z),
		  vec3_t(max.x, max.y, min.z),
		  vec3_t(max.x, min.y, max.z),
		  vec3_t(max.x, max.y, max.z),
		};

		for (auto point : points) {
			utilities::vector_transform(point, matrix[box->bone], vhitbox);
			if (utilities::is_entity_visible(ctx::local_player, entity, ctx::local_player->get_eye_position(), vhitbox))
				return vhitbox;
		}

		return vec3_t();
	}

	bool utilities::has_condition(c_base_player* entity, int condition) {
		if (entity->get_condition() & condition) {
			return true;
		}
		return false;
	}

	void utilities::angle_vectors(const vec3_t &angles, vec3_t* forward) {
		float sp, sy, cp, cy;

		sy = sinf(DEG2RAD(angles[1]));
		cy = cosf(DEG2RAD(angles[1]));

		sp = sinf(DEG2RAD(angles[0]));
		cp = cosf(DEG2RAD(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	void utilities::angle_vectors(const vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up)
	{
		float sr, sp, sy, cr, cp, cy;
		sin_cos(DEG2RAD(angles.x), &sp, &cp);
		sin_cos(DEG2RAD(angles.y), &sy, &cy);
		sin_cos(DEG2RAD(angles.z), &sr, &cr);

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up) {
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	void utilities::make_vector(vec3_t angle, vec3_t& vector) {
		float pitch	= float(angle[0] * PI / 180), yaw = float(angle[1] * PI / 180), tmp = float(cos(pitch));
		vec3_t final_vec = vec3_t(float(-tmp * -cos(yaw)), float(sin(yaw)*tmp), float(-sin(pitch)));
		vector = final_vec;
	}

	void utilities::vector_angle(vec3_t &forward, vec3_t &angles) {
		float yaw, pitch;
		if (forward[1] == 0 && forward[0] == 0) {
			if (forward[2] > 0) {
				pitch = 270;
			}
			else {
				pitch = 90;
			} yaw = 0;
		}
		else {
			yaw = RAD2DEG(atan2f(forward[1], forward[0]));
			if (yaw < 0) { yaw += 360; }

			pitch = RAD2DEG(atan2f(-forward[2], forward.length2d()));
			if (pitch < 0) { pitch += 360; }
		} angles = vec3_t(pitch, yaw, 0);
	}

	vec3_t utilities::calc_angle(vec3_t src, vec3_t dst) {
		vec3_t aim_angles, delta; float hyp; delta = src - dst;
		hyp = sqrtf((delta[0] * delta[0]) + (delta[1] * delta[1]));
		aim_angles[0] = atanf(delta[2] / hyp) * RADPI;
		aim_angles[1] = atanf(delta[1] / delta[0]) * RADPI;
		aim_angles[2] = 0.0f;
		if (delta[0] >= 0.0) aim_angles[1] += 180.0f;
		return aim_angles;
	}

	bool utilities::is_valid_player(c_base_player* entity) {
		if (!entity || entity->is_dead() || entity->is_dormant()) {
			return false;
		}

		if (entity->get_client_class()->class_id != 246) {
			return false;
		}

		if (entity->get_team_num() == ctx::local_player->get_team_num()) {
			return false;
		}

		return true;
	}

	bool utilities::is_projectile(c_base_player* entity, c_base_weapon* weapon) {
		if (!entity || !weapon) {
			return false;
		}

		int weapon_id = weapon->get_item_definition_index();

		if (entity->get_class_num() == TF2_Soldier && weapon->get_slot() == 0 || weapon_id == WPN_RighteousBison) {
			return true;
		}

		if (entity->get_class_num() == TF2_Pyro && (weapon->get_slot() == 0 || weapon_id != WPN_NewShotgun || weapon_id != WPN_PyroShotgun || weapon_id != WPN_ReserveShooter)) {
			return true;
		}

		if (entity->get_class_num() == TF2_Demoman && weapon->get_slot() != 2) {
			return true;
		}
		
		if (entity->get_class_num() == TF2_Engineer && weapon_id == WPN_RescueRanger || weapon_id == WPN_ShortCircut || weapon_id == WPN_Wrangler || weapon_id == WPN_FestiveWrangler || weapon_id == WPN_Pomson) {
			return true;
		}

		if (entity->get_class_num() == TF2_Medic && weapon->get_slot() == 0) {
			return true;
		}

		if (entity->get_class_num() == TF2_Sniper && weapon_id == WPN_Huntsman || weapon_id == WPN_FestiveHuntsman || weapon_id == WPN_CompoundBow || weapon_id == WPN_Jarate || weapon_id == WPN_FestiveJarate) {
			return true;
		}

		if (entity->get_class_num() == TF2_Scout && weapon_id == WPN_Milk || weapon_id == WPN_MutatedMilk) {
			return true;
		}

		return false;
	}

	bool utilities::is_special_fire_type(c_base_player* entity, c_base_weapon* weapon) {
		if (!entity || !weapon) {
			return false;
		}

		int weapon_id = weapon->get_item_definition_index();
		if (weapon_id == WPN_Huntsman || weapon_id == WPN_CompoundBow || weapon_id == WPN_FestiveHuntsman || weapon_id == WPN_LoooseCannon || weapon_id == WPN_DragonsFury || (entity->get_class_num() == TF2_Demoman and weapon->get_slot() == 1))
			return true;
		else
			return false;
	}

	bool utilities::is_entity_visible(c_base_player* local_player, c_base_player* entity, vec3_t vec_start, vec3_t vec_end) {
		trace_t tr; ray_t ray; trace_filter filter;

		filter.skip = local_player;
		ray.init(vec_start, vec_end);
		interfaces::enginetrace->trace_ray(ray, MASK_SHOT_HULL, &filter, &tr);

		return (tr.ent == entity || tr.fraction >= 0.99f);
	}

	bool utilities::is_point_visible(c_base_player* local_player, vec3_t vec_start, vec3_t vec_end) {
		trace_t tr; ray_t ray; trace_filter filter;

		filter.skip = local_player;
		ray.init(vec_start, vec_end);
		interfaces::enginetrace->trace_ray(ray, MASK_SOLID, &filter, &tr);

		return (tr.fraction > 0.9f);
	}


	colour_t utilities::rainbow(float val)
	{
		std::vector<float> out{ 0, 0, 0 };
		out[0] = cosf(val) * .5f + .5f;
		out[1] = cosf(val - 2.f * PI / 3.f) * .5f + .5f;
		out[2] = cosf(val - 4.f * PI / 3.f) * .5f + .5f;

		out[0] *= 255.f;
		out[1] *= 255.f;
		out[2] *= 255.f;

		return colour_t(out[0], out[1], out[2]);
	}

	void utilities::normalize_yaw(float yaw)
	{
		if (yaw > 180) {
			yaw -= (round(yaw / 360) * 360.f);
		}
		else if (yaw < -180) {
			yaw += (round(yaw / 360) * -360.f);
		}
	}

	bool utilities::ray_to_obb(const vec3_t& origin, const vec3_t& direction, const vec3_t& position, const vec3_t& min, const vec3_t& max, const matrix3x4_t &orientation)
	{
		vec3_t p = position - origin;

		vec3_t X(orientation[0][0], orientation[0][1], orientation[0][2]);
		vec3_t Y(orientation[1][0], orientation[1][1], orientation[1][2]);
		vec3_t Z(orientation[2][0], orientation[2][1], orientation[2][2]);

		vec3_t f(
			X.dot(direction),
			Y.dot(direction),
			Z.dot(direction)
		);

		vec3_t e(
			X.dot(p),
			Y.dot(p),
			Z.dot(p)
		);

		float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; ++i) {
			if (float_compare(f[i], 0)))
			{
				if (-e[i] + min[i] > 0 || -e[i] + max[i] < 0) {
					return false;
				}
				f[i] = 0.00001f; // Avoid div by 0!
			}

			t[i * 2 + 0] = (e[i] + max[i]) / f[i]; // tmin[x, y, z]
			t[i * 2 + 1] = (e[i] + min[i]) / f[i]; // tmax[x, y, z]
		}

		float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
		float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));

		// if tmax < 0, ray is intersecting AABB
		// but entire AABB is behing it's origin
		if (tmax < 0) {
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			return false;
		}

		return true;
	}
}

/**
* netvar_tree - Constructor
*
* Call populate_nodes on every RecvTable under client->GetAllClasses()
*/
void CNetVars::Initialize()
{
	const auto *client_class = interfaces::client->get_all_classes();
	while (client_class != nullptr) {
		const auto class_info = std::make_shared<node>(0);
		auto *recv_table = client_class->table;
		populate_nodes(recv_table, &class_info->nodes);
		nodes.emplace(recv_table->GetName(), class_info);

		client_class = client_class->next_class;
	}
}

/**
* populate_nodes - Populate a node map with brances
* @recv_table:	Table the map corresponds to
* @map:	Map pointer
*
* Add info for every prop in the recv table to the node map. If a prop is a
* datatable itself, initiate a recursive call to create more branches.
*/
void CNetVars::populate_nodes(RecvTable *recv_table, map_type *map)
{
	for (auto i = 0; i < recv_table->GetNumProps(); i++) {
		const auto *prop = recv_table->GetPropTable(i);
		const auto prop_info = std::make_shared<node>(prop->GetOffset());

		if (prop->GetType() == DPT_DataTable)
			populate_nodes(prop->GetDataTable(), &prop_info->nodes);

		map->emplace(prop->GetName(), prop_info);
	}
}

DWORD WINAPI init(LPVOID lpparam) {
	if (setup::ready_to_blast()) {
		setup::setup_interfaces();
		setup::setup_hooks();
	}
	//if (VMProtectIsVirtualMachinePresent() || !VMProtectIsValidImageCRC())
	//	return FALSE;
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstdll, DWORD fdwreason, LPVOID lpvreserved) {
	switch (fdwreason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstdll);
		CreateThread(nullptr, 0, init, hinstdll, 0, nullptr);
		return TRUE;
	case DLL_PROCESS_DETACH:
		FreeLibraryAndExitThread(hinstdll, 0);
		return TRUE;
	default:
		return TRUE;
	}
}