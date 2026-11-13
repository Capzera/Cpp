#include "battle_manager.h"
#include "wave_manager.h"
#include "chess_piece.h"
#include <godot_cpp/classes/engine.hpp>
using namespace godot;
BattleManager::BattleManager() {}
BattleManager::~BattleManager() {}
void BattleManager::_bind_methods() {
	ADD_SIGNAL(MethodInfo("money_changed", PropertyInfo(Variant::INT, "new_amount")));
	ADD_SIGNAL(MethodInfo("battle_ended", PropertyInfo(Variant::BOOL, "victory")));
	ClassDB::bind_method(D_METHOD("get_money"), &BattleManager::get_money);
	ClassDB::bind_method(D_METHOD("add_money", "amount"), &BattleManager::add_money);
	ClassDB::bind_method(D_METHOD("spend_money", "amount"), &BattleManager::spend_money);
	ClassDB::bind_method(D_METHOD("set_auto_regen", "unlocked"), &BattleManager::set_auto_regen);
	ClassDB::bind_method(D_METHOD("on_king_destroyed"), &BattleManager::on_king_destroyed);
	ClassDB::bind_method(D_METHOD("end_battle", "victory"), &BattleManager::end_battle);
}
void BattleManager::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) return;
	call_deferred("emit_signal", "money_changed", current_money);
}
void BattleManager::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint() || battle_over_) return;
	if (auto_regen_unlocked) {
		regen_timer += delta;
		if (regen_timer >= regen_interval) { regen_timer = 0.0f; add_money(regen_amount); }
	}
	Node* parent = get_parent();
	if (!parent) return;
	// 兼容 WaveManager、WaveManager2（GDScript）和 WaveManagerTutorial
	bool waves_complete = false;
	WaveManager* wm = Object::cast_to<WaveManager>(parent->get_node_or_null("WaveManager"));
	if (wm) {
		waves_complete = wm->is_waves_complete();
	} else {
		// GDScript 波次管理器：查找任意带 is_waves_complete 方法的子节点
		TypedArray<Node> children = parent->get_children();
		for (int i = 0; i < children.size(); ++i) {
			Node* child = Object::cast_to<Node>(children[i]);
			if (child && child->has_method("is_waves_complete")) {
				waves_complete = child->call("is_waves_complete");
				break;
			}
		}
	}
	if (!waves_complete) return;
	TypedArray<Node> siblings = parent->get_children();
	for (int i = 0;i < siblings.size();++i) {
		ChessPiece* p = Object::cast_to<ChessPiece>(siblings[i]);
		if (p && p->get_faction() == FACTION_ENEMY && p->get_current_hp() > 0 && !p->get_is_preview()) return;
	}
	end_battle(true);
}
int BattleManager::get_money() const { return current_money; }
void BattleManager::add_money(int amount) {
	current_money += amount;
	emit_signal("money_changed", current_money);
}
bool BattleManager::spend_money(int amount) {
	if (current_money >= amount) {
		current_money -= amount;
		emit_signal("money_changed", current_money);
		return true;
	}
	return false;
}
void BattleManager::set_auto_regen(bool unlocked) { auto_regen_unlocked = unlocked; }
void BattleManager::on_king_destroyed() { end_battle(false); }
void BattleManager::end_battle(bool victory) {
	if (battle_over_) return;
	battle_over_ = true;
	emit_signal("battle_ended", victory);
}
