#include "chess_board.h"
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <cmath>
using namespace godot;
ChessBoard::ChessBoard() {}
ChessBoard::~ChessBoard() {}
void ChessBoard::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_columns", "v"), &ChessBoard::set_columns);
	ClassDB::bind_method(D_METHOD("get_columns"), &ChessBoard::get_columns);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "set_columns", "get_columns");
	ClassDB::bind_method(D_METHOD("set_rows", "v"), &ChessBoard::set_rows);
	ClassDB::bind_method(D_METHOD("get_rows"), &ChessBoard::get_rows);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows"), "set_rows", "get_rows");
	ClassDB::bind_method(D_METHOD("set_base_grid", "v"), &ChessBoard::set_base_grid);
	ClassDB::bind_method(D_METHOD("get_base_grid"), &ChessBoard::get_base_grid);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "base_grid"), "set_base_grid", "get_base_grid");
	ClassDB::bind_method(D_METHOD("set_terrain_at", "x", "y", "type"), &ChessBoard::set_terrain_at);
	ClassDB::bind_method(D_METHOD("add_spawn_point", "pos"), &ChessBoard::add_spawn_point);
	ClassDB::bind_method(D_METHOD("build_visuals"), &ChessBoard::build_visuals);
	ClassDB::bind_method(D_METHOD("world_to_grid", "world_pos"), &ChessBoard::world_to_grid);
	ClassDB::bind_method(D_METHOD("grid_to_world", "grid_pos"), &ChessBoard::grid_to_world);
	ClassDB::bind_method(D_METHOD("is_valid_grid_pos", "grid_pos"), &ChessBoard::is_valid_grid_pos);
	ClassDB::bind_method(D_METHOD("get_grid_from_mouse", "screen_pos", "camera"), &ChessBoard::get_grid_from_mouse);
}
void ChessBoard::set_columns(int v) { if (!built_) columns = v; }
int ChessBoard::get_columns() const { return columns; }
void ChessBoard::set_rows(int v) { if (!built_) rows = v; }
int ChessBoard::get_rows() const { return rows; }
void ChessBoard::set_base_grid(Vector2i v) { base_grid_ = v; }
Vector2i ChessBoard::get_base_grid() const { return base_grid_; }
void ChessBoard::set_terrain_at(int x, int y, int type) {
	if (x >= 0 && x < columns && y >= 0 && y < rows) grid_map[x][y].terrain = static_cast<TerrainType>(type);
}
void ChessBoard::add_spawn_point(Vector2i pos) {
	spawn_points_.push_back(pos);
}
void ChessBoard::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) return;
	grid_map.clear();
	grid_map.resize(columns, std::vector<CellData>(rows));
	built_ = false;
}
void ChessBoard::build_visuals() {
	if (built_) return;
	built_ = true;
	Ref<StandardMaterial3D> m_w; m_w.instantiate(); m_w->set_albedo(Color(0.9f, 0.9f, 0.9f));
	Ref<StandardMaterial3D> m_b; m_b.instantiate(); m_b->set_albedo(Color(0.2f, 0.2f, 0.2f));
	Ref<StandardMaterial3D> m_h; m_h.instantiate(); m_h->set_albedo(Color(0.8f, 0.8f, 0.2f));
	Ref<StandardMaterial3D> m_l; m_l.instantiate(); m_l->set_albedo(Color(0.2f, 0.4f, 0.2f));
	Ref<StandardMaterial3D> m_o; m_o.instantiate(); m_o->set_albedo(Color(0.1f, 0.1f, 0.1f));
	Ref<StandardMaterial3D> m_s; m_s.instantiate(); m_s->set_albedo(Color(0.72f, 0.86f, 0.95f));
	Ref<BoxMesh> t_m; t_m.instantiate(); t_m->set_size(Vector3(tile_size, 0.1f, tile_size));
	for (int x = 0; x < columns; ++x) {
		for (int z = 0; z < rows; ++z) {
			MeshInstance3D* t = memnew(MeshInstance3D);
			t->set_mesh(t_m);
			TerrainType type = grid_map[x][z].terrain;
			if (type == TERRAIN_OBSTACLE) t->set_surface_override_material(0, m_o);
			else if (type == TERRAIN_HIGH) t->set_surface_override_material(0, m_h);
			else if (type == TERRAIN_LOW) t->set_surface_override_material(0, m_l);
			else if (type == TERRAIN_SNOW) t->set_surface_override_material(0, m_s);
			else t->set_surface_override_material(0, ((x + z) % 2 == 0) ? m_w : m_b);
			Vector3 w_pos = grid_to_world(Vector2i(x, z));
			w_pos.y = (type == TERRAIN_HIGH) ? 0.4f : (type == TERRAIN_LOW) ? -0.4f : -0.05f;
			t->set_position(w_pos);
			add_child(t);
		}
	}
	// 基地标注（蓝色半透明方块）
	MeshInstance3D* base = memnew(MeshInstance3D);
	Ref<BoxMesh> b_m; b_m.instantiate(); b_m->set_size(Vector3(1.8f, 1.8f, 1.8f));
	base->set_mesh(b_m);
	Ref<StandardMaterial3D> b_mat; b_mat.instantiate();
	b_mat->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
	b_mat->set_albedo(Color(0.0f, 0.5f, 1.0f, 0.6f));
	base->set_surface_override_material(0, b_mat);
	Vector3 base_pos = grid_to_world(base_grid_);
	base_pos.y = 1.0f;
	base->set_position(base_pos);
	add_child(base);
	// 出生点标注（红色半透明方块，略高于地面）
	Ref<BoxMesh> sp_m; sp_m.instantiate(); sp_m->set_size(Vector3(1.8f, 1.8f, 1.8f));
	Ref<StandardMaterial3D> sp_mat; sp_mat.instantiate();
	sp_mat->set_transparency(BaseMaterial3D::TRANSPARENCY_ALPHA);
	sp_mat->set_albedo(Color(1.0f, 0.1f, 0.1f, 0.55f));
	for (const Vector2i& sp : spawn_points_) {
		MeshInstance3D* sp_node = memnew(MeshInstance3D);
		sp_node->set_mesh(sp_m);
		sp_node->set_surface_override_material(0, sp_mat);
		Vector3 sp_pos = grid_to_world(sp);
		sp_pos.y = 1.0f;
		sp_node->set_position(sp_pos);
		add_child(sp_node);
	}
}
TerrainType ChessBoard::get_terrain_at_world(Vector3 world_pos) const {
	Vector2i g = world_to_grid(world_pos);
	if (g.x >= 0 && g.x < columns && g.y >= 0 && g.y < rows) return grid_map[g.x][g.y].terrain;
	return TERRAIN_NORMAL;
}
Vector3 ChessBoard::grid_to_world(Vector2i p) const {
	return Vector3((p.x - columns / 2.0f+0.5f) * tile_size, 0.0f, (p.y - rows / 2.0f+0.5f) * tile_size);
}
Vector2i ChessBoard::world_to_grid(Vector3 w) const {
	return Vector2i(static_cast<int>(std::floor(w.x / tile_size + columns / 2.0f)), static_cast<int>(std::floor(w.z / tile_size + rows / 2.0f)));
}
bool ChessBoard::is_valid_grid_pos(Vector2i p) const {
	return p.x >= 0 && p.x < columns && p.y >= 0 && p.y < rows;
}
Vector2i ChessBoard::get_grid_from_mouse(Vector2 screen_pos, Camera3D* camera) const {
	if (!camera) return Vector2i(-1, -1);
	Vector3 ray_origin = camera->project_ray_origin(screen_pos);
	Vector3 ray_dir = camera->project_ray_normal(screen_pos);
	if (std::abs(ray_dir.y) < 0.001f) return Vector2i(-1, -1);
	float t = -ray_origin.y / ray_dir.y;
	Vector3 world_pos = ray_origin + ray_dir * t;
	return world_to_grid(world_pos);
}
