#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <vector>
#include "card_stats.h"
#include "damage_calculator.h"
namespace godot {
	enum Faction {FACTION_PLAYER = 0, FACTION_ENEMY = 1};
	class ChessPiece : public Node3D {
		GDCLASS(ChessPiece, Node3D)
	private:
		Ref<CardStats> stats;
		int current_hp = 0;
		Faction faction = FACTION_PLAYER;
		bool is_king = false;
		bool is_preview = false;
		int facing_dir = 0;
		bool is_in_combat = false;
		ChessPiece* current_target = nullptr;
		std::vector<ChessPiece*> all_targets_;
		float attack_timer = 0.0f;
		bool is_moving = false;
		Vector3 start_pos;
		Vector3 target_pos;
		float move_progress = 0.0f;
		float move_duration = 0.5f;
		float tile_size = 2.0f;
		MeshInstance3D* hp_bar = nullptr;
		std::vector<Vector2i> path;
		int path_index = 0;
		int blocked_count_ = 0;
		ChessPiece* blocker_ = nullptr;
		void create_hp_bar();
		void update_hp_bar();
		void fire_projectile(ChessPiece* target);
		float get_dynamic_attack_interval();
		bool in_attack_range(ChessPiece* other, class ChessBoard* board);
		int max_block() const;
	protected:
		static void _bind_methods();
	public:
		ChessPiece();
		~ChessPiece();
		void _ready() override;
		void _process(double delta) override;
		void _exit_tree() override;
		void set_stats(const Ref<CardStats>& p_stats);
		Ref<CardStats> get_stats() const;
		void set_faction(int p_faction);
		int get_faction() const;
		void set_is_king(bool p_king);
		bool get_is_king() const;
		void set_is_preview(bool p_preview);
		bool get_is_preview() const;
		void set_facing_dir(int p_dir);
		int get_facing_dir() const;
		int get_current_hp() const;
		int get_blocked_count() const;
		void take_damage_raw(int amount);
		void take_damage(const DamageResult& result);
		void attack(ChessPiece* target);
		void set_path(const PackedVector2Array& p);
		void notify_blocker_lost(ChessPiece* who);
	};
}
#endif
