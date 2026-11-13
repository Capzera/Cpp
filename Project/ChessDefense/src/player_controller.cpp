#include "player_controller.h"
#include "chess_board.h"
#include "battle_manager.h"
#include "card_stats.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>
using namespace godot;
PlayerController::PlayerController() {}
PlayerController::~PlayerController() {}
void PlayerController::_bind_methods() {
	ADD_SIGNAL(MethodInfo("card_deployed", PropertyInfo(Variant::OBJECT, "op_node")));
	ADD_SIGNAL(MethodInfo("inspect_opened", PropertyInfo(Variant::OBJECT, "piece")));
	ADD_SIGNAL(MethodInfo("inspect_closed"));
	ClassDB::bind_method(D_METHOD("start_deployment", "scene"), &PlayerController::start_deployment);
}
void PlayerController::apply_camera() {
	if (!main_camera) return;
	if (cam_pitch_ < 0.15f) cam_pitch_ = 0.15f;
	if (cam_pitch_ > 1.45f) cam_pitch_ = 1.45f;
	if (cam_dist_ < 5.5f) cam_dist_ = 5.5f;
	if (cam_dist_ > 28.0f) cam_dist_ = 28.0f;
	float cp = std::cos(cam_pitch_), sp = std::sin(cam_pitch_);
	float sy = std::sin(cam_yaw_), cy = std::cos(cam_yaw_);
	Vector3 pos = cam_pivot_ + Vector3(sy * cp, sp, cy * cp) * cam_dist_;
	main_camera->set_global_position(pos);
	main_camera->look_at(cam_pivot_, Vector3(0.0f, 1.0f, 0.0f));
}
void PlayerController::update_camera_move(float dt) {
	if (!main_camera) return;
	Input* in = Input::get_singleton();
	Vector3 fwd(-std::sin(cam_yaw_), 0.0f, -std::cos(cam_yaw_));
	Vector3 right(std::cos(cam_yaw_), 0.0f, -std::sin(cam_yaw_));
	Vector3 move;
	if (in->is_key_pressed(KEY_W)) move += fwd;
	if (in->is_key_pressed(KEY_S)) move -= fwd;
	if (in->is_key_pressed(KEY_A)) move -= right;
	if (in->is_key_pressed(KEY_D)) move += right;
	if (in->is_key_pressed(KEY_SPACE)) move.y += 1.0f;
	if (in->is_key_pressed(KEY_CTRL)) move.y -= 1.0f;
	if (move.length_squared() > 0.0001f) {
		move.normalize();
		cam_pivot_ += move * (10.0f * dt);
		apply_camera();
	}
	if (in->is_key_pressed(KEY_H)) {
		cam_pivot_ = Vector3(0.0f, 0.0f, 0.0f);
		cam_yaw_ = 0.0f;
		cam_pitch_ = 0.785398f;
		cam_dist_ = 10.0f;
		apply_camera();
	}
}
void PlayerController::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) return;
	set_process(true);
	main_camera = get_viewport()->get_camera_3d();
	if (main_camera) {
		Vector3 pos = main_camera->get_global_position();
		cam_pivot_ = Vector3(0.0f, 0.0f, 0.0f);
		Vector3 off = pos - cam_pivot_;
		cam_dist_ = off.length();
		if (cam_dist_ < 0.001f) cam_dist_ = 10.0f;
		cam_yaw_ = std::atan2(off.x, off.z);
		cam_pitch_ = std::atan2(off.y, std::sqrt(off.x * off.x + off.z * off.z));
		apply_camera();
	}
}
void PlayerController::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint() || !main_camera) return;
	double ts = Engine::get_singleton()->get_time_scale();
	float dt = (ts > 0.0001) ? static_cast<float>(delta / ts) : 0.0f;
	if (inspecting_ && inspect_lerp_ < 1.0f) {
		inspect_lerp_ += dt * 3.5f;
		if (inspect_lerp_ > 1.0f) inspect_lerp_ = 1.0f;
		float t = inspect_lerp_;
		t = t * t * (3.0f - 2.0f * t);
		cam_pivot_ = inspect_from_pivot_.lerp(inspect_to_pivot_, t);
		cam_yaw_ = inspect_from_yaw_ + (inspect_to_yaw_ - inspect_from_yaw_) * t;
		cam_pitch_ = inspect_from_pitch_ + (inspect_to_pitch_ - inspect_from_pitch_) * t;
		cam_dist_ = inspect_from_dist_ + (inspect_to_dist_ - inspect_from_dist_) * t;
		apply_camera();
	} else if (!inspecting_ && inspect_lerp_ < 1.0f && inspect_from_dist_ > 0.01f) {
		inspect_lerp_ += dt * 3.5f;
		if (inspect_lerp_ > 1.0f) inspect_lerp_ = 1.0f;
		float t = inspect_lerp_;
		t = t * t * (3.0f - 2.0f * t);
		cam_pivot_ = inspect_from_pivot_.lerp(inspect_to_pivot_, t);
		cam_yaw_ = inspect_from_yaw_ + (inspect_to_yaw_ - inspect_from_yaw_) * t;
		cam_pitch_ = inspect_from_pitch_ + (inspect_to_pitch_ - inspect_from_pitch_) * t;
		cam_dist_ = inspect_from_dist_ + (inspect_to_dist_ - inspect_from_dist_) * t;
		apply_camera();
	}
	update_camera_move(dt);
}
void PlayerController::start_deployment(const Ref<PackedScene>& scene) {
	if (inspecting_) close_inspect();
	if (deploy_state != 0 && preview_op) { preview_op->queue_free();preview_op = nullptr; }
	operator_scene = scene;
	deploy_state = 1;
	Engine::get_singleton()->set_time_scale(0.3f);
}
void PlayerController::close_inspect() {
	if (!inspecting_) return;
	inspecting_ = false;
	inspect_target_ = nullptr;
	inspect_lerp_ = 0.0f;
	inspect_from_pivot_ = cam_pivot_;
	inspect_from_yaw_ = cam_yaw_;
	inspect_from_pitch_ = cam_pitch_;
	inspect_from_dist_ = cam_dist_;
	inspect_to_pivot_ = inspect_saved_pivot_;
	inspect_to_yaw_ = inspect_saved_yaw_;
	inspect_to_pitch_ = inspect_saved_pitch_;
	inspect_to_dist_ = inspect_saved_dist_;
	Engine::get_singleton()->set_time_scale(1.0f);
	emit_signal("inspect_closed");
}
void PlayerController::clear_range_preview() {
	for (Node3D* node : range_preview_nodes_) {
		if (node && node->is_inside_tree()) {
			node->queue_free();
		}
	}
	range_preview_nodes_.clear();
}
void PlayerController::show_range_preview(int facing_dir) {
	clear_range_preview();
	if (!preview_op) return;
	ChessBoard* board = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (!board) return;
	Ref<CardStats> stats = preview_op->get_stats();
	if (!stats.is_valid()) return;
	
	Vector2i my_grid = locked_grid;
	PackedVector2Array pattern = stats->get_attack_pattern();
	
	Ref<BoxMesh> box_mesh; box_mesh.instantiate();
	box_mesh->set_size(Vector3(1.8f, 0.15f, 1.8f));
	Ref<StandardMaterial3D> mat; mat.instantiate();
	mat->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
	mat->set_albedo(Color(0.3f, 0.6f, 1.0f, 0.5f));
	mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	
	for (int j = 0; j < pattern.size(); ++j) {
		Vector2 offset = pattern[j];
		Vector2i rot_offset;
		// 根据朝向旋转攻击范围
		if (facing_dir == 1) { rot_offset.x = (int)offset.x; rot_offset.y = (int)offset.y; }
		else if (facing_dir == 3) { rot_offset.x = -(int)offset.x; rot_offset.y = -(int)offset.y; }
		else if (facing_dir == 0) { rot_offset.x = (int)offset.y; rot_offset.y = -(int)offset.x; }
		else { rot_offset.x = -(int)offset.y; rot_offset.y = (int)offset.x; }
		
		Vector2i target_grid = my_grid + rot_offset;
		if (!board->is_valid_grid_pos(target_grid)) continue;
		
		MeshInstance3D* preview_node = memnew(MeshInstance3D);
		preview_node->set_mesh(box_mesh);
		preview_node->set_surface_override_material(0, mat);
		Vector3 world_pos = board->grid_to_world(target_grid);
		world_pos.y = 0.3f;
		preview_node->set_position(world_pos);
		get_parent()->add_child(preview_node);
		range_preview_nodes_.push_back(preview_node);
	}
}
void PlayerController::try_inspect(Vector2 screen_pos) {
	if (!main_camera) return;
	ChessBoard* board = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
	if (!board) return;
	Vector2i g = board->get_grid_from_mouse(screen_pos, main_camera);
	ChessPiece* hit = nullptr;
	if (board->is_valid_grid_pos(g)) {
		TypedArray<Node> siblings = get_parent()->get_children();
		for (int i = 0;i < siblings.size();++i) {
			ChessPiece* p = Object::cast_to<ChessPiece>(siblings[i]);
			if (!p || p->get_is_preview() || p->get_current_hp() <= 0) continue;
			if (board->world_to_grid(p->get_position()) == g) { hit = p; break; }
		}
	}
	if (!hit) { if (inspecting_) close_inspect(); return; }
	if (!inspecting_) {
		inspect_saved_pivot_ = cam_pivot_;
		inspect_saved_yaw_ = cam_yaw_;
		inspect_saved_pitch_ = cam_pitch_;
		inspect_saved_dist_ = cam_dist_;
	}
	inspecting_ = true;
	inspect_target_ = hit;
	inspect_lerp_ = 0.0f;
	inspect_from_pivot_ = cam_pivot_;
	inspect_from_yaw_ = cam_yaw_;
	inspect_from_pitch_ = cam_pitch_;
	inspect_from_dist_ = cam_dist_;
	inspect_to_pivot_ = hit->get_position() + Vector3(1.2f, 0.4f, -1.2f);
	inspect_to_yaw_ = cam_yaw_;
	inspect_to_pitch_ = cam_pitch_;
	inspect_to_dist_ = cam_dist_;
	Engine::get_singleton()->set_time_scale(0.25f);
	emit_signal("inspect_opened", hit);
}
void PlayerController::_input(const Ref<InputEvent>& event) {
	if (Engine::get_singleton()->is_editor_hint()) return;
	Ref<InputEventMouseButton> mouse = event;
	if (mouse.is_valid()) {
		if (mouse->get_button_index() == MOUSE_BUTTON_WHEEL_UP && mouse->is_pressed()) { cam_dist_ -= 1.2f; apply_camera(); }
		if (mouse->get_button_index() == MOUSE_BUTTON_WHEEL_DOWN && mouse->is_pressed()) { cam_dist_ += 1.2f; apply_camera(); }
		if (mouse->get_button_index() == MOUSE_BUTTON_RIGHT) {
			if (mouse->is_pressed()) {
				if (inspecting_) { close_inspect(); rotating_cam_ = false; return; }
				if (deploy_state != 0) {
					if (preview_op) { preview_op->queue_free();preview_op = nullptr; }
					clear_range_preview();
					deploy_state = 0;
					mouse_pressed_for_direction_ = false;
					Engine::get_singleton()->set_time_scale(1.0f);
					rotating_cam_ = false;
					return;
				}
				rotating_cam_ = true;
			} else {
				rotating_cam_ = false;
			}
			return;
		}
	}
	Ref<InputEventMouseMotion> motion = event;
	if (motion.is_valid() && rotating_cam_ && deploy_state == 0) {
		Vector2 rel = motion->get_relative();
		cam_yaw_ -= rel.x * 0.005f;
		cam_pitch_ += rel.y * 0.005f;
		apply_camera();
	}
	if (deploy_state == 0) {
		if (mouse.is_valid() && mouse->is_pressed() && mouse->get_button_index() == MOUSE_BUTTON_LEFT) try_inspect(mouse->get_position());
		return;
	}
	if (motion.is_valid()) {
		ChessBoard* board = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
		if (!board || !main_camera) return;
		if (deploy_state == 1) {
			Vector2i grid_pos = board->get_grid_from_mouse(motion->get_position(), main_camera);
			if (board->is_valid_grid_pos(grid_pos)) {
				if (!preview_op) {
					Node3D* node = Object::cast_to<Node3D>(operator_scene->instantiate());
					preview_op = Object::cast_to<ChessPiece>(node);
					if (preview_op) {
						preview_op->set_faction(0);
						preview_op->set_is_preview(true);
						preview_op->set_is_king(false);
						add_child(preview_op);
					}
				}
				if (preview_op) preview_op->set_position(board->grid_to_world(grid_pos));
			}
			else if (preview_op) { preview_op->set_position(Vector3(0, -1000, 0)); }
		}
		else if (deploy_state == 2 && preview_op) {
			if (mouse_pressed_for_direction_) {
				Vector2i mouse_grid = board->get_grid_from_mouse(motion->get_position(), main_camera);
				int dx = mouse_grid.x - locked_grid.x;
				int dy = mouse_grid.y - locked_grid.y;
				if (dx != 0 || dy != 0) {
					int new_dir;
					if (std::abs(dx) >= std::abs(dy)) new_dir = dx > 0 ? 1 : 3;
					else new_dir = dy > 0 ? 2 : 0;
					preview_op->set_facing_dir(new_dir);
					show_range_preview(new_dir);
				}
			}
		}
	}
	if (mouse.is_valid() && mouse->is_pressed() && mouse->get_button_index() == MOUSE_BUTTON_LEFT) {
		if (deploy_state == 1 && preview_op) {
			ChessBoard* board = Object::cast_to<ChessBoard>(get_parent()->get_node_or_null("ChessBoard"));
			locked_grid = board->world_to_grid(preview_op->get_position());
			TerrainType t = board->get_terrain_at_world(preview_op->get_position());
			if (t == TERRAIN_OBSTACLE || t == TERRAIN_LOW) return;
			bool occupied = false;
			TypedArray<Node> siblings = get_parent()->get_children();
			for (int i = 0;i < siblings.size();++i) {
				ChessPiece* p = Object::cast_to<ChessPiece>(siblings[i]);
				if (p && board->world_to_grid(p->get_position()) == locked_grid) { occupied = true;break; }
			}
			if (occupied) return;
			deploy_state = 2;
			preview_op->set_facing_dir(2);
			mouse_pressed_for_direction_ = true;
			show_range_preview(2);
		}
		else if (deploy_state == 2 && preview_op) {
			if (mouse_pressed_for_direction_) {
				// 仍在长按中，不做处理
			}
		}
	}
	if (mouse.is_valid() && !mouse->is_pressed() && mouse->get_button_index() == MOUSE_BUTTON_LEFT) {
		if (deploy_state == 2 && preview_op && mouse_pressed_for_direction_) {
			mouse_pressed_for_direction_ = false;
			clear_range_preview();
			BattleManager* bm = Object::cast_to<BattleManager>(get_parent()->get_node_or_null("BattleManager"));
			Ref<CardStats> stats = preview_op->get_stats();
			if (bm && stats.is_valid()) {
				if (bm->spend_money(stats->get_deploy_cost())) {
					preview_op->set_is_preview(false);
					preview_op->set_is_king(false);
					preview_op->reparent(get_parent());
					emit_signal("card_deployed", preview_op);
					preview_op = nullptr;
					deploy_state = 0;
					Engine::get_singleton()->set_time_scale(1.0f);
				} else {
					if (preview_op) { preview_op->queue_free(); preview_op = nullptr; }
					deploy_state = 0;
					Engine::get_singleton()->set_time_scale(1.0f);
				}
			}
		}
	}
}
