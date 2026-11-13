#include "damage_calculator.h"
#include <algorithm>
using namespace godot;
float DamageCalculator::get_random_float() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }
DamageResult DamageCalculator::calculate_physical_damage(const Ref<CardStats>& attacker, const Ref<CardStats>& defender) {
	DamageResult result;
	if (get_random_float() < defender->get_evasion_rate()) { result.is_evaded = true; result.final_damage = 0; return result; }
	int actual_def = std::max(0, defender->get_physical_def() - attacker->get_physical_pen());
	int base_damage = std::max(attacker->get_physical_atk() - actual_def, static_cast<int>(attacker->get_physical_atk() * 0.05f));
	float crit_rate = attacker->get_crit_rate(), crit_mult = attacker->get_crit_damage(), final_mult = 1.0f;
	if (crit_rate > 1.0f) {
		result.crit_tier = 1; final_mult = 1.0f + crit_mult;
		if (get_random_float() < (crit_rate - 1.0f)) { result.crit_tier = 2; final_mult = 2.0f + crit_mult; }
	} else if (crit_rate > 0.0f && get_random_float() < crit_rate) { result.crit_tier = 1; final_mult = 1.0f + crit_mult; }
	result.final_damage = static_cast<int>(base_damage * final_mult);
	return result;
}
DamageResult DamageCalculator::calculate_magic_damage(const Ref<CardStats>& attacker, const Ref<CardStats>& defender) {
	DamageResult result;
	int actual_mdef = std::max(0, defender->get_magic_def() - attacker->get_magic_pen());
	float reduction = std::min(static_cast<float>(actual_mdef) / (actual_mdef + 2500.0f), 0.90f);
	float raw = attacker->get_magic_atk() * (1.0f - reduction);
	float crit_rate = attacker->get_crit_rate(), crit_mult = attacker->get_crit_damage(), final_mult = 1.0f;
	if (crit_rate > 1.0f) {
		result.crit_tier = 1; final_mult = 1.0f + crit_mult;
		if (get_random_float() < (crit_rate - 1.0f)) { result.crit_tier = 2; final_mult = 2.0f + crit_mult; }
	} else if (crit_rate > 0.0f && get_random_float() < crit_rate) { result.crit_tier = 1; final_mult = 1.0f + crit_mult; }
	result.final_damage = static_cast<int>(raw * final_mult);
	return result;
}
