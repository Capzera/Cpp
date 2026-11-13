#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <godot_cpp/classes/node3d.hpp>
#include "card_stats.h"
namespace godot {
	class ChessPiece;
	class Projectile : public Node3D {
		GDCLASS(Projectile, Node3D)
	private:
		ChessPiece* target_ = nullptr;
		Ref<CardStats> atk_stats_;
		float speed_ = 16.0f;
		bool ranged_ = false;
		bool vis_ready_ = false;
	protected:
		static void _bind_methods();
	public:
		Projectile();
		~Projectile();
		void _ready() override;
		void _process(double delta) override;
		void setup(const Vector3& from, ChessPiece* target, const Ref<CardStats>& atk, float speed, bool ranged);
	};
}
#endif
