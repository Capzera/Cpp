#include "register_types.h"
#include "card_stats.h"
#include "chess_piece.h"
#include "chess_board.h"
#include "damage_text.h"
#include "wave_manager.h"
#include "battle_manager.h"
#include "save_manager.h"
#include "player_controller.h"
#include "projectile.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
using namespace godot;
void initialize_chess_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
	ClassDB::register_class<CardStats>();
	ClassDB::register_class<ChessPiece>();
	ClassDB::register_class<ChessBoard>();
	ClassDB::register_class<DamageText>();
	ClassDB::register_class<WaveManager>();
	ClassDB::register_class<BattleManager>();
	ClassDB::register_class<PlayerController>();
	ClassDB::register_class<SaveManager>();
	ClassDB::register_class<Projectile>();
}

void uninitialize_chess_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// 这是给 Godot 调用的全局入口函数
	GDExtensionBool GDE_EXPORT chess_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_chess_module);
		init_obj.register_terminator(uninitialize_chess_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
		return init_obj.init();
	}
}
