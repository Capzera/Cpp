#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include "chess_piece.h"
namespace godot {
	class PlayerController : public Node {
		GDCLASS(PlayerController, Node)
	private:
		Ref<PackedScene> operator_scene;
		Camera3D* main_camera = nullptr;
		int deploy_state = 0;
		ChessPiece* preview_op = nullptr;
		Vector2i locked_grid;
		bool rotating_cam_ = false;
		Vector3 cam_pivot_ = Vector3(0.0f, 0.0f, 0.0f);
		float cam_yaw_ = 0.0f;
		float cam_pitch_ = 0.785398f;
		float cam_dist_ = 10.0f;
		Vector3 inspect_saved_pivot_;
		float inspect_saved_yaw_ = 0.0f;
		float inspect_saved_pitch_ = 0.0f;
		float inspect_saved_dist_ = 10.0f;
		bool inspecting_ = false;
		ChessPiece* inspect_target_ = nullptr;
		float inspect_lerp_ = 0.0f;
		Vector3 inspect_from_pivot_;
		float inspect_from_yaw_ = 0.0f;
		float inspect_from_pitch_ = 0.0f;
		float inspect_from_dist_ = 10.0f;
		Vector3 inspect_to_pivot_;
		float inspect_to_yaw_ = 0.0f;
		float inspect_to_pitch_ = 0.0f;
		float inspect_to_dist_ = 10.0f;
		bool mouse_pressed_for_direction_ = false;
		std::vector<Node3D*> range_preview_nodes_;
		void apply_camera();
		void update_camera_move(float dt);
		void try_inspect(Vector2 screen_pos);
		void close_inspect();
		void clear_range_preview();
		void show_range_preview(int facing_dir);
	protected:
		static void _bind_methods();
	public:
		PlayerController();
		~PlayerController();
		void _ready() override;
		void _process(double delta) override;
		void _input(const Ref<InputEvent>& event) override;
		void start_deployment(const Ref<PackedScene>& scene);
	};
}
#endif
