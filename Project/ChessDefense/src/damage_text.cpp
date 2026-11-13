#include "damage_text.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/base_material3d.hpp> // <-- 补上了关键的材质引擎头文件

using namespace godot;

DamageText::DamageText() {}
DamageText::~DamageText() {}

void DamageText::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) return;
	
	// Godot 4 标准的 3D 文字朝向摄像机写法 (正确使用 BaseMaterial3D)
	set_billboard_mode(BaseMaterial3D::BILLBOARD_ENABLED); 
	set_pixel_size(0.015f); 
	set_outline_size(4);    
	
	float rand_x = UtilityFunctions::randf_range(-1.0f, 1.0f);
	float rand_z = UtilityFunctions::randf_range(-1.0f, 1.0f);
	velocity = Vector3(rand_x, 3.0f, rand_z);
}

void DamageText::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) return;
	
	current_time += delta;
	if (current_time >= life_time) {
		queue_free(); // 寿命结束，销毁自己
		return;
	}
	
	// 1. 物理计算：模拟重力抛物线
	velocity.y -= gravity * delta;
	Vector3 pos = get_position();
	set_position(pos + velocity * delta);
	
	// 2. 动画计算：后半段逐渐透明 (Fade out)
	if (current_time > life_time * 0.5f) {
		Color c = get_modulate();
		// 算出剩余比例
		float alpha = 1.0f - ((current_time - life_time * 0.5f) / (life_time * 0.5f));
		c.a = alpha;
		set_modulate(c);
	}
}

void DamageText::setup(int damage, int crit_tier, bool is_evaded) {
	if (is_evaded) {
		set_text("Miss");
		set_modulate(Color(0.8f, 0.8f, 0.8f)); // 灰色 Miss
		return;
	}
	
	set_text(String::num_int64(damage));
	
	// 暴击颜色分级机制
	if (crit_tier == 2) {
		set_modulate(Color(1.0f, 0.2f, 0.2f)); // 红字：超暴击
		set_scale(Vector3(1.5f, 1.5f, 1.5f));  // 字体放大
	} else if (crit_tier == 1) {
		set_modulate(Color(1.0f, 0.8f, 0.0f)); // 黄字：普通暴击
		set_scale(Vector3(1.2f, 1.2f, 1.2f));
	} else {
		set_modulate(Color(1.0f, 1.0f, 1.0f)); // 白字：普通攻击
	}
}
