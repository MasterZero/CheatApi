#include "global.h"

int main() {

	cheat_api instance;
	instance.test();

	while (true) { //example cheat thread

		auto local_player = cheat_api::driver->read_chain<uintptr_t>(instance.get_module_base("GameAssembly.dll"), {56760616,0xB8,0x0});//GameAssembly::LocalPlayer_c*->LocalPlayer_s->k__BackingField
		if (local_player) {
			//setting plyerflag to "BasePlayer.PlayerFlags IsAdmin = 4"
			cheat_api::driver->write<int>(local_player + 0x690, 4);//BasePlayer::playerFlags

			auto act_uid = cheat_api::driver->read<unsigned int>(local_player + 0x5D8); // BasePlayer::clActiveItem
			if (act_uid) {
				//Getting active held entity ->
				auto act_weap = 0ull;
				auto item_list_vals = cheat_api::driver->read_chain<uintptr_t>(local_player, { 0x6A0,0x28,0x38,0x10 });//BasePlayer::inventory->containerBelt->itemList->vals
				for (auto i = 0; i < 6; i++) {
					auto item = cheat_api::driver->read<uintptr_t>(item_list_vals + 0x20 + (i * 0x8));
					if (act_uid == cheat_api::driver->read<unsigned int>(item + 0x28))//Item::uid
						act_weap = cheat_api::driver->read<uintptr_t>(item + 0xA0);//Item::heldEntity
				}

				//ThickBullets ->
				auto created_proj_list_vals = cheat_api::driver->read_chain<uintptr_t>(act_weap, { 0x370,0x10 });//BaseProjectile::createdProjectiles->vals
				for (auto i = 0; i < 10; i++) {
					auto projectile = cheat_api::driver->read<uintptr_t>(created_proj_list_vals + 0x20 + (i * 0x8));
					if (projectile)
						cheat_api::driver->write<float>(projectile + 0x2C, 1.f);//Projectile::thickness
				}
			}
		}
		auto occlusion_culling_static = cheat_api::driver->read_chain<uintptr_t>(instance.get_module_base("GameAssembly.dll"), { 56755032,0xB8 });
		if (occlusion_culling_static) {
			//enabling debug culling
			cheat_api::driver->write<int>(occlusion_culling_static + 0x94, 1);//OcclusionCulling::_debugShow

			//debug culling filter
			auto debug_settings = cheat_api::driver->read_chain<uintptr_t>(occlusion_culling_static, { 0x80,0x18 });//GameAssembly::OcclusionCulling_c*->OcclusionCulling_s->instance->debugSettings 
			if (debug_settings) 
				cheat_api::driver->write<int>(debug_settings + 0x20, 131072);//OcclusionCulling.DebugSettings::layerFilter
		}
	}


	std::getchar();
}

