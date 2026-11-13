#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/immediate_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <vector>
namespace godot {
	class WaveManager : public Node {
		GDCLASS(WaveManager, Node)
	private:
		static const int WAVE_MAX = 5;
		int wave_sizes_[5] = {5, 7, 10, 15, 21};
		int current_wave_ = 0;
		int spawned_in_wave_ = 0;
		int phase_ = 0;
		float timer_ = 0.0f;
		float wave_interval_ = 8.0f;
		float spawn_interval_ = 0.8f;
		float first_wait_ = 2.0f;
		float preview_speed_ = 2.5f;
		float preview_t_ = 0.0f;
		Ref<PackedScene> enemy_scene;
		Ref<PackedScene> runner_scene;
		Ref<PackedScene> tank_scene;
		std::vector<Vector2i> path;
		MeshInstance3D* preview_line_ = nullptr;
		Ref<ImmediateMesh> line_mesh_;
		void start_preview();
		void clear_preview();
		void update_preview_line(float t);
		void spawn_one(int kind);
	protected:
		static void _bind_methods();
	public:
		WaveManager();
		~WaveManager();
		void _ready() override;
		void _process(double delta) override;
		void set_enemy_scene(const Ref<PackedScene>& scene);
		Ref<PackedScene> get_enemy_scene() const;
		void set_runner_scene(const Ref<PackedScene>& scene);
		Ref<PackedScene> get_runner_scene() const;
		void set_tank_scene(const Ref<PackedScene>& scene);
		Ref<PackedScene> get_tank_scene() const;
		bool is_waves_complete() const;
		int get_current_wave() const;
		int get_total_waves() const;
		void set_wave_interval(float v);
		float get_wave_interval() const;
		void set_spawn_interval(float v);
		float get_spawn_interval() const;
		void set_first_wait(float v);
		float get_first_wait() const;
	};
}
#endif
