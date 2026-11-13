#ifndef DAMAGE_TEXT_H
#define DAMAGE_TEXT_H

#include <godot_cpp/classes/label3d.hpp>

namespace godot {
	
	class DamageText : public Label3D {
		GDCLASS(DamageText, Label3D)
		
	private:
		Vector3 velocity;
		float gravity = 4.0f;
		float life_time = 1.0f; // 存活 1 秒
		float current_time = 0.0f;
		
	protected:
		static void _bind_methods() {}
		
	public:
		DamageText();
		~DamageText();
		
		void _ready() override;
		void _process(double delta) override;
		
		// 核心接口：根据伤害结果设置文字和颜色
		void setup(int damage, int crit_tier, bool is_evaded);
	};
	
}
#endif // DAMAGE_TEXT_H
