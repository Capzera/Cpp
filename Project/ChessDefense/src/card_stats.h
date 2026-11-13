#ifndef CARD_STATS_H
#define CARD_STATS_H
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/string.hpp>
namespace godot {
	class CardStats : public Resource {
		GDCLASS(CardStats, Resource)
	private:
		String piece_type = "P";
		String display_name = "";
		String card_attribute = "物理";
		int deploy_cost = 15;
		float redeploy_time = 30.0f;
		int max_hp = 300;
		int max_mana = 0;
		int physical_atk = 30;
		int physical_def = 10;
		int physical_pen = 0;
		int magic_atk = 0;
		int magic_def = 0;
		int magic_pen = 0;
		float attack_interval = 1.5f;
		int attack_range = 1;
		int block_count = 1;
		float move_speed = 2.0f;
		float projectile_speed = 18.0f;
		bool is_ranged = false;
		bool hits_all_in_range = false;
		float effect_prob = 0.0f;
		float evasion_rate = 0.0f;
		float crit_rate = 0.0f;
		float crit_damage = 1.5f;
		String range_map = "O X";
		PackedVector2Array attack_pattern;
	protected:
		static void _bind_methods();
	public:
		CardStats();
		~CardStats();
		void set_piece_type(String p_value); String get_piece_type() const;
		void set_display_name(String p_value); String get_display_name() const;
		void set_card_attribute(String p_value); String get_card_attribute() const;
		void set_deploy_cost(int p_value); int get_deploy_cost() const;
		void set_redeploy_time(float p_value); float get_redeploy_time() const;
		void set_max_hp(int p_value); int get_max_hp() const;
		void set_max_mana(int p_value); int get_max_mana() const;
		void set_physical_atk(int p_value); int get_physical_atk() const;
		void set_physical_def(int p_value); int get_physical_def() const;
		void set_physical_pen(int p_value); int get_physical_pen() const;
		void set_magic_atk(int p_value); int get_magic_atk() const;
		void set_magic_def(int p_value); int get_magic_def() const;
		void set_magic_pen(int p_value); int get_magic_pen() const;
		void set_attack_interval(float p_value); float get_attack_interval() const;
		void set_attack_range(int p_value); int get_attack_range() const;
		void set_block_count(int p_value); int get_block_count() const;
		void set_move_speed(float p_value); float get_move_speed() const;
		void set_projectile_speed(float p_value); float get_projectile_speed() const;
		void set_is_ranged(bool p_value); bool get_is_ranged() const;
		void set_hits_all_in_range(bool p_value); bool get_hits_all_in_range() const;
		void set_effect_prob(float p_value); float get_effect_prob() const;
		void set_evasion_rate(float p_value); float get_evasion_rate() const;
		void set_crit_rate(float p_value); float get_crit_rate() const;
		void set_crit_damage(float p_value); float get_crit_damage() const;
		void set_range_map(String p_value); String get_range_map() const;
		PackedVector2Array get_attack_pattern() const;
	};
}
#endif