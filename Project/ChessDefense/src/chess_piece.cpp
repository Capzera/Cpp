#include "chess_piece.h"
#include "damage_text.h"
#include "chess_board.h"
#include "battle_manager.h"
#include "projectile.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
using namespace godot;
ChessPiece::ChessPiece() {}
ChessPiece::~ChessPiece() {}
void ChessPiece::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_stats", "stats"), &ChessPiece::set_stats);
	ClassDB::bind_method(D_METHOD("get_stats"), &ChessPiece::get_stats);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stats", PROPERTY_HINT_RESOURCE_TYPE, "CardStats"), "set_stats", "get_stats");
	ClassDB::bind_method(D_METHOD("set_faction", "faction"), &ChessPiece::set_faction);
	ClassDB::bind_method(D_METHOD("get_faction"), &ChessPiece::get_faction);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "faction", PROPERTY_HINT_ENUM, "Player,Enemy"), "set_faction", "get_faction");
	ClassDB::bind_method(D_METHOD("set_is_king", "is_king"), &ChessPiece::set_is_king);
	ClassDB::bind_method(D_METHOD("get_is_king"), &ChessPiece::get_is_king);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_king"), "set_is_king", "get_is_king");
	ClassDB::bind_method(D_METHOD("set_is_preview", "is_preview"), &ChessPiece::set_is_preview);
	ClassDB::bind_method(D_METHOD("get_is_preview"), &ChessPiece::get_is_preview);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_preview"), "set_is_preview", "get_is_preview");
	ClassDB::bind_method(D_METHOD("set_facing_dir", "dir"), &ChessPiece::set_facing_dir);
	ClassDB::bind_method(D_METHOD("get_facing_dir"), &ChessPiece::get_facing_dir);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "facing_dir"), "set_facing_dir", "get_facing_dir");
	ClassDB::bind_method(D_METHOD("get_current_hp"), &ChessPiece::get_current_hp);
	ClassDB::bind_method(D_METHOD("get_blocked_count"), &ChessPiece::get_blocked_count);
	ClassDB::bind_method(D_METHOD("set_path", "path"), &ChessPiece::set_path);
	ClassDB::bind_method(D_METHOD("take_damage_raw", "amount"), &ChessPiece::take_damage_raw);
}
void ChessPiece::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) return;
	if (stats.is_valid()) { current_hp = stats->get_max_hp(); create_hp_bar(); }
}
void ChessPiece::create_hp_bar() {
	hp_bar = memnew(MeshInstance3D);
	Ref<BoxMesh> box; box.instantiate(); box->set_size(Vector3(1.5f, 0.2f, 0.2f));
	hp_bar->set_mesh(box); hp_bar->set_position(Vector3(0.0f, 2.5f, 0.0f));
	Ref<StandardMaterial3D> mat; mat.instantiate();
	mat->set_albedo(Color(0.0f, 1.0f, 0.0f)); mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	hp_bar->set_surface_override_material(0, mat);
	add_child(hp_bar);
	if (is_preview) hp_bar->set_visible(false);
}
void ChessPiece::update_hp_bar() {
	if (!hp_bar || !stats.is_valid()) return;
	float hp_percent = std::max(0.0f, static_cast<float>(current_hp) / stats->get_max_hp());
	hp_bar->set_scale(Vector3(hp_percent, 1.0f, 1.0f));
	Ref<StandardMaterial3D> mat = hp_bar->get_surface_override_material(0);
	if (mat.is_valid()) {
		if (hp_percent > 0.5f) mat->set_albedo(Color(0.0f, 1.0f, 0.0f));
		else if (hp_percent > 0.2f) mat->set_albedo(Color(1.0f, 1.0f, 0.0f));
		else mat->set_albedo(Color(1.0f, 0.0f, 0.0f));
	}
}
void ChessPiece::set_faction(int p_faction) { faction = static_cast<Faction>(p_faction); } int ChessPiece::get_faction() const { return faction; }
void ChessPiece::set_is_king(bool p_king) { is_king = p_king; } bool ChessPiece::get_is_king() const { return is_king; }
void ChessPiece::set_is_preview(bool p_preview) { is_preview = p_preview; if (hp_bar) hp_bar->set_visible(!p_preview); } bool ChessPiece::get_is_preview() const { return is_preview; }
void ChessPiece::set_facing_dir(int p_dir) {
	facing_dir = p_dir;
	float yaw = 0.0f;
	if (p_dir == 1) yaw = -1.5707963f;
	else if (p_dir == 2) yaw = 3.1415926f;
	else if (p_dir == 3) yaw = 1.5707963f;
	set_rotation(Vector3(0.0f, yaw, 0.0f));
}
int ChessPiece::get_facing_dir() const { return facing_dir; }
void ChessPiece::set_stats(const Ref<CardStats>& p_stats) { stats = p_stats; } Ref<CardStats> ChessPiece::get_stats() const { return stats; }
int ChessPiece::get_current_hp() const { return current_hp; }
int ChessPiece::get_blocked_count() const { return blocked_count_; }
int ChessPiece::max_block() const { return stats.is_valid() ? std::max(0, stats->get_block_count()) : 1; }
void ChessPiece::set_path(const PackedVector2Array& p) {
	path.clear();
	for (int i = 0; i < p.size(); ++i) {
		Vector2 v = p[i];
		path.push_back(Vector2i(static_cast<int>(v.x), static_cast<int>(v.y)));
	}
	path_index = 0;
}
float ChessPiece::get_dynamic_attack_interval() {
	float base_interval = stats->get_attack_interval();
	ChessBoard* board = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (board && board->get_terrain_at_world(get_position()) == TERRAIN_SNOW) return base_interval * 1.5f;
	return base_interval;
}
bool ChessPiece::in_attack_range(ChessPiece* other, ChessBoard* board) {
	if (faction == FACTION_ENEMY) {
		float dist = get_position().distance_to(other->get_position());
		float attack_radius = stats->get_attack_range() * tile_size;
		if (other->get_is_king() && dist <= tile_size * 0.6f) return true;
		if (!other->get_is_king() && dist <= attack_radius + 0.1f) return true;
		return false;
	}
	Vector2i my_grid = board->world_to_grid(get_position());
	Vector2i other_grid = board->world_to_grid(other->get_position());
	if (other_grid == my_grid) return true;
	PackedVector2Array pattern = stats->get_attack_pattern();
	for (int j = 0;j < pattern.size();++j) {
		Vector2 offset = pattern[j];
		Vector2i rot_offset;
		if (facing_dir == 1) { rot_offset.x = (int)offset.x; rot_offset.y = (int)offset.y; }
		else if (facing_dir == 3) { rot_offset.x = -(int)offset.x; rot_offset.y = -(int)offset.y; }
		else if (facing_dir == 0) { rot_offset.x = (int)offset.y; rot_offset.y = -(int)offset.x; }
		else { rot_offset.x = -(int)offset.y; rot_offset.y = (int)offset.x; }
		if (other_grid == my_grid + rot_offset) return true;
	}
	return false;
}
void ChessPiece::notify_blocker_lost(ChessPiece* who) {
	if (blocker_ == who) blocker_ = nullptr;
}
void ChessPiece::_exit_tree() {
	if (Engine::get_singleton()->is_editor_hint() || !get_parent()) return;
	TypedArray<Node> siblings = get_parent()->get_children();
	for (int i = 0;i < siblings.size();++i) {
		ChessPiece* p = Object::cast_to<ChessPiece>(siblings[i]);
		if (p && p != this) p->notify_blocker_lost(this);
	}
}
void ChessPiece::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint() || !stats.is_valid() || is_preview) return;
	bool was_in_combat = is_in_combat;
	current_target = nullptr;
	is_in_combat = false;
	blocked_count_ = 0;
	ChessBoard* board = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (!board) return;
	TypedArray<Node> siblings = get_parent()->get_children();
	if (faction == FACTION_PLAYER && !is_king) {
		Vector2i my_grid = board->world_to_grid(get_position());
		for (int i = 0;i < siblings.size();++i) {
			ChessPiece* e = Object::cast_to<ChessPiece>(siblings[i]);
			if (!e || e == this || e->get_faction() != FACTION_ENEMY || e->get_current_hp() <= 0 || e->get_is_preview() || e->is_queued_for_deletion()) continue;
			if (board->world_to_grid(e->get_position()) == my_grid) {
				if (blocked_count_ < max_block()) {
					e->blocker_ = this;
					blocked_count_++;
				}
			}
		}
	}
	bool hits_all = stats->get_hits_all_in_range();
	if (hits_all) {
		// 全范围攻击：收集范围内所有敌人，只要有一个就进入战斗状态
		all_targets_.clear();
		for (int i = 0; i < siblings.size(); ++i) {
			ChessPiece* other = Object::cast_to<ChessPiece>(siblings[i]);
			if (other && other != this && other->get_faction() != this->faction && other->get_current_hp() > 0 && !other->is_queued_for_deletion() && !other->get_is_preview()) {
				if (in_attack_range(other, board)) {
					all_targets_.push_back(other);
					is_in_combat = true;
					is_moving = false;
				}
			}
		}
	} else {
		for (int i = 0; i < siblings.size(); ++i) {
			ChessPiece* other = Object::cast_to<ChessPiece>(siblings[i]);
			if (other && other != this && other->get_faction() != this->faction && other->get_current_hp() > 0 && !other->is_queued_for_deletion() && !other->get_is_preview()) {
				if (in_attack_range(other, board)) { current_target = other; is_in_combat = true; is_moving = false; break; }
			}
		}
	}
	if (is_in_combat) {
		if (!was_in_combat) attack_timer = get_dynamic_attack_interval();
		attack_timer += static_cast<float>(delta);
		if (attack_timer >= get_dynamic_attack_interval()) {
			if (hits_all) {
				for (ChessPiece* t : all_targets_) {
					if (t && t->is_inside_tree() && !t->is_queued_for_deletion() && t->get_current_hp() > 0)
						fire_projectile(t);
				}
			} else if (current_target != nullptr) {
				attack(current_target);
			}
			attack_timer = 0.0f;
		}
	} else if (faction == FACTION_ENEMY) {
		if (blocker_ && (!blocker_->is_inside_tree() || blocker_->is_queued_for_deletion() || blocker_->get_current_hp() <= 0)) blocker_ = nullptr;
		if (blocker_) {
			is_moving = false;
			return;
		}
		if (!is_moving) {
			if (!path.empty() && path_index < (int)path.size() - 1) {
				start_pos = get_position();
				target_pos = board->grid_to_world(path[path_index + 1]);
				is_moving = true; move_progress = 0.0f;
				float spd = stats->get_move_speed();
				if (spd < 0.1f) spd = 2.0f;
				if (board->get_terrain_at_world(start_pos) == TERRAIN_SNOW || board->get_terrain_at_world(target_pos) == TERRAIN_SNOW) spd *= 0.8f;
				move_duration = tile_size / spd;
			}
		} else {
			move_progress += static_cast<float>(delta) / move_duration;
			if (move_progress >= 1.0f) {
				move_progress = 1.0f; set_position(target_pos); is_moving = false; path_index++;
			} else { set_position(start_pos.lerp(target_pos, move_progress)); }
		}
	}
}
void ChessPiece::fire_projectile(ChessPiece* target) {
	if (!get_parent() || !target || !stats.is_valid()) return;
	Projectile* p = memnew(Projectile);
	get_parent()->add_child(p);
	p->setup(get_position() + Vector3(0.0f, 1.1f, 0.0f), target, stats, stats->get_projectile_speed(), stats->get_is_ranged());
}
void ChessPiece::attack(ChessPiece* target) {
	if (!target) return;
	fire_projectile(target);
}
void ChessPiece::take_damage(const DamageResult& result) {
	current_hp -= result.final_damage;
	update_hp_bar();
	DamageText* text_node = memnew(DamageText);
	get_parent()->add_child(text_node);
	text_node->set_position(get_position() + Vector3(0.0f, 2.0f, 0.0f));
	text_node->setup(result.final_damage, result.crit_tier, result.is_evaded);
	if (current_hp <= 0) {
		BattleManager* bm = Object::cast_to<BattleManager>(get_parent()->get_node_or_null("BattleManager"));
		if (is_king && bm) bm->on_king_destroyed();
		if (faction == FACTION_ENEMY && bm) bm->add_money(15);
		queue_free();
	}
}
void ChessPiece::take_damage_raw(int amount) {
	DamageResult dummy; dummy.final_damage = amount; dummy.crit_tier = 0; dummy.is_evaded = false;
	take_damage(dummy);
}
