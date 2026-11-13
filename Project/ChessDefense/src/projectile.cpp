#include "projectile.h"
#include "chess_piece.h"
#include "damage_calculator.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/sphere_mesh.hpp>
#include <godot_cpp/classes/box_mesh.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
using namespace godot;
Projectile::Projectile() {}
Projectile::~Projectile() {}
void Projectile::_bind_methods() {}
void Projectile::_ready() {
	if (Engine::get_singleton()->is_editor_hint() || vis_ready_) return;
	vis_ready_ = true;
	MeshInstance3D* m = memnew(MeshInstance3D);
	Ref<StandardMaterial3D> mat; mat.instantiate();
	mat->set_shading_mode(BaseMaterial3D::SHADING_MODE_UNSHADED);
	if (ranged_) {
		Ref<SphereMesh> sm; sm.instantiate(); sm->set_radius(0.14f); sm->set_height(0.28f);
		m->set_mesh(sm);
		mat->set_albedo(Color(0.35f, 0.85f, 1.0f));
		mat->set_emission(Color(0.3f, 0.7f, 1.0f));
	} else {
		Ref<BoxMesh> bm; bm.instantiate(); bm->set_size(Vector3(0.12f, 0.08f, 0.55f));
		m->set_mesh(bm);
		mat->set_albedo(Color(1.0f, 0.92f, 0.35f));
		mat->set_emission(Color(1.0f, 0.8f, 0.2f));
	}
	mat->set_emission_energy_multiplier(2.0f);
	m->set_surface_override_material(0, mat);
	add_child(m);
}
void Projectile::setup(const Vector3& from, ChessPiece* target, const Ref<CardStats>& atk, float speed, bool ranged) {
	set_position(from);
	target_ = target;
	atk_stats_ = atk;
	speed_ = speed;
	ranged_ = ranged;
	if (is_inside_tree() && !vis_ready_) _ready();
}
void Projectile::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) return;
	if (!target_ || target_->is_queued_for_deletion() || !target_->is_inside_tree() || target_->get_current_hp() <= 0) { queue_free(); return; }
	Vector3 dest = target_->get_position() + Vector3(0.0f, 1.1f, 0.0f);
	Vector3 pos = get_position();
	Vector3 d = dest - pos;
	float dist = d.length();
	float step = speed_ * static_cast<float>(delta);
	if (dist > 0.001f) look_at(dest, Vector3(0.0f, 1.0f, 0.0f));
	if (dist <= 0.28f || step >= dist) {
		if (atk_stats_.is_valid()) {
			DamageResult r = DamageCalculator::calculate_physical_damage(atk_stats_, target_->get_stats());
			target_->take_damage(r);
		}
		queue_free();
		return;
	}
	set_position(pos + d * (step / dist));
}
