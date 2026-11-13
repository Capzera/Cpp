#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <vector>
namespace godot {
	enum TerrainType {TERRAIN_NORMAL = 0, TERRAIN_SNOW = 1, TERRAIN_LAVA = 2, TERRAIN_HIGH = 3, TERRAIN_LOW = 4, TERRAIN_OBSTACLE = 5};
	struct CellData {
		TerrainType terrain = TERRAIN_NORMAL;
		bool is_occupied = false;
	};
	class ChessBoard : public Node3D {
		GDCLASS(ChessBoard, Node3D)
	private:
		int columns = 5;
		int rows = 5;
		float tile_size = 2.0f;
		Vector2i base_grid_ = Vector2i(0, 4);
		std::vector<std::vector<CellData>> grid_map;
		std::vector<Vector2i> spawn_points_;
		bool built_ = false;
		void build_visuals();
	protected:
		static void _bind_methods();
	public:
		ChessBoard();
		~ChessBoard();
		void _ready() override;
		void set_columns(int v);
		int get_columns() const;
		void set_rows(int v);
		int get_rows() const;
		void set_base_grid(Vector2i v);
		Vector2i get_base_grid() const;
		void set_terrain_at(int x, int y, int type);
		void add_spawn_point(Vector2i pos);
		TerrainType get_terrain_at_world(Vector3 world_pos) const;
		Vector2i world_to_grid(Vector3 world_pos) const;
		Vector3 grid_to_world(Vector2i grid_pos) const;
		bool is_valid_grid_pos(Vector2i grid_pos) const;
		Vector2i get_grid_from_mouse(Vector2 screen_pos, Camera3D* camera) const;
	};
}
#endif
