#include "wave_manager.h"
#include "chess_board.h"
#include "chess_piece.h"
#include "card_stats.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
using namespace godot;
WaveManager::WaveManager() {}
WaveManager::~WaveManager() {}
void WaveManager::_bind_methods() {
	ADD_SIGNAL(MethodInfo("wave_changed", PropertyInfo(Variant::INT, "current"), PropertyInfo(Variant::INT, "total")));
	ClassDB::bind_method(D_METHOD("set_enemy_scene", "scene"), &WaveManager::set_enemy_scene);
	ClassDB::bind_method(D_METHOD("get_enemy_scene"), &WaveManager::get_enemy_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "enemy_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_enemy_scene", "get_enemy_scene");
	ClassDB::bind_method(D_METHOD("set_runner_scene", "scene"), &WaveManager::set_runner_scene);
	ClassDB::bind_method(D_METHOD("get_runner_scene"), &WaveManager::get_runner_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "runner_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_runner_scene", "get_runner_scene");
	ClassDB::bind_method(D_METHOD("set_tank_scene", "scene"), &WaveManager::set_tank_scene);
	ClassDB::bind_method(D_METHOD("get_tank_scene"), &WaveManager::get_tank_scene);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "tank_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_tank_scene", "get_tank_scene");
	ClassDB::bind_method(D_METHOD("is_waves_complete"), &WaveManager::is_waves_complete);
	ClassDB::bind_method(D_METHOD("get_current_wave"), &WaveManager::get_current_wave);
	ClassDB::bind_method(D_METHOD("get_total_waves"), &WaveManager::get_total_waves);
	ClassDB::bind_method(D_METHOD("set_wave_interval", "v"), &WaveManager::set_wave_interval);
	ClassDB::bind_method(D_METHOD("get_wave_interval"), &WaveManager::get_wave_interval);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wave_interval"), "set_wave_interval", "get_wave_interval");
	ClassDB::bind_method(D_METHOD("set_spawn_interval", "v"), &WaveManager::set_spawn_interval);
	ClassDB::bind_method(D_METHOD("get_spawn_interval"), &WaveManager::get_spawn_interval);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spawn_interval"), "set_spawn_interval", "get_spawn_interval");
	ClassDB::bind_method(D_METHOD("set_first_wait", "v"), &WaveManager::set_first_wait);
	ClassDB::bind_method(D_METHOD("get_first_wait"), &WaveManager::get_first_wait);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "first_wait"), "set_first_wait", "get_first_wait");
}
void WaveManager::set_enemy_scene(const Ref<PackedScene>& scene) { enemy_scene = scene; }
Ref<PackedScene> WaveManager::get_enemy_scene() const { return enemy_scene; }
void WaveManager::set_runner_scene(const Ref<PackedScene>& scene) { runner_scene = scene; }
Ref<PackedScene> WaveManager::get_runner_scene() const { return runner_scene; }
void WaveManager::set_tank_scene(const Ref<PackedScene>& scene) { tank_scene = scene; }
Ref<PackedScene> WaveManager::get_tank_scene() const { return tank_scene; }
bool WaveManager::is_waves_complete() const { return phase_ == 3; }
int WaveManager::get_current_wave() const { return current_wave_; }
int WaveManager::get_total_waves() const { return WAVE_MAX; }
void WaveManager::set_wave_interval(float v) { wave_interval_ = v; }
float WaveManager::get_wave_interval() const { return wave_interval_; }
void WaveManager::set_spawn_interval(float v) { spawn_interval_ = v; }
float WaveManager::get_spawn_interval() const { return spawn_interval_; }
void WaveManager::set_first_wait(float v) { first_wait_ = v; }
float WaveManager::get_first_wait() const { return first_wait_; }
void WaveManager::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) return;
	set_process(true);
	path = {Vector2i(4, 0), Vector2i(3, 0), Vector2i(2, 0), Vector2i(2, 1), Vector2i(2, 2), Vector2i(2, 3), Vector2i(1, 3), Vector2i(1, 4), Vector2i(0, 4)};
	phase_ = 0; timer_ = 0.0f; current_wave_ = 0; spawned_in_wave_ = 0;
	call_deferred("emit_signal", "wave_changed", 0, WAVE_MAX);
}
void WaveManager::clear_preview() {
	if (preview_line_) { preview_line_->queue_free(); preview_line_ = nullptr; }
	line_mesh_.unref();
}
void WaveManager::start_preview() {
	clear_preview();
	ChessBoard* b = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (!b || path.size() < 2) { phase_ = 2; timer_ = 0.0f; return; }
	line_mesh_.instantiate();
	preview_line_ = memnew(MeshInstance3D);
	preview_line_->set_mesh(line_mesh_);
	Ref<StandardMaterial3D> mat; mat.instantiate();
	mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	mat->set_albedo(Color(1.0f, 0.85f, 0.2f));
	mat->set_emission(Color(1.0f, 0.85f, 0.2f));
	mat->set_emission_energy_multiplier(2.5f);
	preview_line_->set_material_override(mat);
	get_parent()->add_child(preview_line_);
	preview_t_ = 0.0f;
	emit_signal("wave_changed", current_wave_ + 1, WAVE_MAX);
}
void WaveManager::update_preview_line(float t) {
	if (!line_mesh_.is_valid()) return;
	ChessBoard* b = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (!b || path.empty()) return;
	line_mesh_->clear_surfaces();
	line_mesh_->surface_begin(Mesh::PRIMITIVE_LINE_STRIP);
	float total = 0.0f;
	for (size_t i = 0;i < path.size() - 1;++i) {
		Vector3 a = b->grid_to_world(path[i]); a.y = 0.8f;
		Vector3 c = b->grid_to_world(path[i + 1]); c.y = 0.8f;
		total += a.distance_to(c);
	}
	float cur = 0.0f, target = t * total;
	for (size_t i = 0;i < path.size();++i) {
		Vector3 p = b->grid_to_world(path[i]); p.y = 0.8f;
		if (i > 0) {
			Vector3 prev = b->grid_to_world(path[i - 1]); prev.y = 0.8f;
			float seg = prev.distance_to(p);
			if (cur + seg > target) {
				float u = (target - cur) / seg;
				line_mesh_->surface_add_vertex(prev.lerp(p, u));
				line_mesh_->surface_end();
				return;
			}
			cur += seg;
		}
		line_mesh_->surface_add_vertex(p);
	}
	line_mesh_->surface_end();
}
void WaveManager::spawn_one(int kind) {
	Ref<PackedScene> sc = enemy_scene;
	if (kind == 1 && runner_scene.is_valid()) sc = runner_scene;
	if (kind == 2 && tank_scene.is_valid()) sc = tank_scene;
	if (!sc.is_valid()) return;
	ChessBoard* b = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (!b) return;
	Node* inst = sc->instantiate();
	ChessPiece* e = Object::cast_to<ChessPiece>(inst);
	if (!e) { inst->queue_free(); return; }
	e->set_faction(1);
	PackedVector2Array packed_path;
	for (const Vector2i& v : path) packed_path.append(Vector2(v.x, v.y));
	e->set_path(packed_path);
	e->set_position(b->grid_to_world(path[0]));
	get_parent()->add_child(e);
}
void WaveManager::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) return;
	if (phase_ == 3) return;
	float dt = static_cast<float>(delta);
	if (phase_ == 0) {
		timer_ += dt;
		float wait = (current_wave_ == 0) ? first_wait_ : wave_interval_;
		if (timer_ >= wait) { timer_ = 0.0f; phase_ = 1; start_preview(); }
		return;
	}
	if (phase_ == 1) {
		ChessBoard* b = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
		if (!b || !line_mesh_.is_valid() || path.size() < 2) { clear_preview(); phase_ = 2; spawned_in_wave_ = 0; timer_ = 0.0f; return; }
		preview_t_ += dt * preview_speed_;
		if (preview_t_ >= 1.0f) {
			clear_preview();
			phase_ = 2;
			spawned_in_wave_ = 0;
			timer_ = spawn_interval_;
			return;
		}
		update_preview_line(preview_t_);
		return;
	}
	if (phase_ == 2) {
		timer_ += dt;
		if (timer_ < spawn_interval_) return;
		timer_ = 0.0f;
		bool last = spawned_in_wave_ == wave_sizes_[current_wave_] - 1;
		int kind = 0;
		if (current_wave_ >= 1 && spawned_in_wave_ % 3 == 2) kind = 1;
		if (current_wave_ == WAVE_MAX - 1 && last) kind = 2;
		spawn_one(kind);
		spawned_in_wave_++;
		if (spawned_in_wave_ >= wave_sizes_[current_wave_]) {
			current_wave_++;
			if (current_wave_ >= WAVE_MAX) { phase_ = 3; emit_signal("wave_changed", WAVE_MAX, WAVE_MAX); }
			else { phase_ = 0; timer_ = 0.0f; }
		}
	}
}
