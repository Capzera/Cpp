#ifndef DAMAGE_CALCULATOR_H
#define DAMAGE_CALCULATOR_H
#include "card_stats.h"
#include <random>
namespace godot {
	struct DamageResult {
		int final_damage = 0;
		bool is_evaded = false;
		int crit_tier = 0;
	};
	class DamageCalculator {
	public:
		static DamageResult calculate_physical_damage(const Ref<CardStats>& attacker, const Ref<CardStats>& defender);
		static DamageResult calculate_magic_damage(const Ref<CardStats>& attacker, const Ref<CardStats>& defender);
	private:
		static float get_random_float();
	};
}
#endif
