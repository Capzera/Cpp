#include "card_stats.h"
#include <godot_cpp/variant/packed_string_array.hpp>
using namespace godot;
CardStats::CardStats() { set_range_map("O X"); }
CardStats::~CardStats() {}
void CardStats::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_piece_type", "p_value"), &CardStats::set_piece_type);
	ClassDB::bind_method(D_METHOD("get_piece_type"), &CardStats::get_piece_type);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "piece_type"), "set_piece_type", "get_piece_type");
	ClassDB::bind_method(D_METHOD("set_display_name", "p_value"), &CardStats::set_display_name);
	ClassDB::bind_method(D_METHOD("get_display_name"), &CardStats::get_display_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), "set_display_name", "get_display_name");
	ClassDB::bind_method(D_METHOD("set_card_attribute", "p_value"), &CardStats::set_card_attribute);
	ClassDB::bind_method(D_METHOD("get_card_attribute"), &CardStats::get_card_attribute);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "card_attribute", PROPERTY_HINT_ENUM, "物理,狙击,法术,奥术,时间,支援,坦克"), "set_card_attribute", "get_card_attribute");
	ClassDB::bind_method(D_METHOD("set_deploy_cost", "p_value"), &CardStats::set_deploy_cost);
	ClassDB::bind_method(D_METHOD("get_deploy_cost"), &CardStats::get_deploy_cost);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "deploy_cost"), "set_deploy_cost", "get_deploy_cost");
	ClassDB::bind_method(D_METHOD("set_redeploy_time", "p_value"), &CardStats::set_redeploy_time);
	ClassDB::bind_method(D_METHOD("get_redeploy_time"), &CardStats::get_redeploy_time);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "redeploy_time"), "set_redeploy_time", "get_redeploy_time");
	ClassDB::bind_method(D_METHOD("set_max_hp", "p_value"), &CardStats::set_max_hp);
	ClassDB::bind_method(D_METHOD("get_max_hp"), &CardStats::get_max_hp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_hp"), "set_max_hp", "get_max_hp");
	ClassDB::bind_method(D_METHOD("set_max_mana", "p_value"), &CardStats::set_max_mana);
	ClassDB::bind_method(D_METHOD("get_max_mana"), &CardStats::get_max_mana);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_mana"), "set_max_mana", "get_max_mana");
	ClassDB::bind_method(D_METHOD("set_physical_atk", "p_value"), &CardStats::set_physical_atk);
	ClassDB::bind_method(D_METHOD("get_physical_atk"), &CardStats::get_physical_atk);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "physical_atk"), "set_physical_atk", "get_physical_atk");
	ClassDB::bind_method(D_METHOD("set_physical_def", "p_value"), &CardStats::set_physical_def);
	ClassDB::bind_method(D_METHOD("get_physical_def"), &CardStats::get_physical_def);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "physical_def"), "set_physical_def", "get_physical_def");
	ClassDB::bind_method(D_METHOD("set_physical_pen", "p_value"), &CardStats::set_physical_pen);
	ClassDB::bind_method(D_METHOD("get_physical_pen"), &CardStats::get_physical_pen);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "physical_pen"), "set_physical_pen", "get_physical_pen");
	ClassDB::bind_method(D_METHOD("set_magic_atk", "p_value"), &CardStats::set_magic_atk);
	ClassDB::bind_method(D_METHOD("get_magic_atk"), &CardStats::get_magic_atk);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "magic_atk"), "set_magic_atk", "get_magic_atk");
	ClassDB::bind_method(D_METHOD("set_magic_def", "p_value"), &CardStats::set_magic_def);
	ClassDB::bind_method(D_METHOD("get_magic_def"), &CardStats::get_magic_def);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "magic_def"), "set_magic_def", "get_magic_def");
	ClassDB::bind_method(D_METHOD("set_magic_pen", "p_value"), &CardStats::set_magic_pen);
	ClassDB::bind_method(D_METHOD("get_magic_pen"), &CardStats::get_magic_pen);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "magic_pen"), "set_magic_pen", "get_magic_pen");
	ClassDB::bind_method(D_METHOD("set_attack_interval", "p_value"), &CardStats::set_attack_interval);
	ClassDB::bind_method(D_METHOD("get_attack_interval"), &CardStats::get_attack_interval);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_interval"), "set_attack_interval", "get_attack_interval");
	ClassDB::bind_method(D_METHOD("set_attack_range", "p_value"), &CardStats::set_attack_range);
	ClassDB::bind_method(D_METHOD("get_attack_range"), &CardStats::get_attack_range);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "attack_range"), "set_attack_range", "get_attack_range");
	ClassDB::bind_method(D_METHOD("set_block_count", "p_value"), &CardStats::set_block_count);
	ClassDB::bind_method(D_METHOD("get_block_count"), &CardStats::get_block_count);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "block_count"), "set_block_count", "get_block_count");
	ClassDB::bind_method(D_METHOD("set_move_speed", "p_value"), &CardStats::set_move_speed);
	ClassDB::bind_method(D_METHOD("get_move_speed"), &CardStats::get_move_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_speed"), "set_move_speed", "get_move_speed");
	ClassDB::bind_method(D_METHOD("set_projectile_speed", "p_value"), &CardStats::set_projectile_speed);
	ClassDB::bind_method(D_METHOD("get_projectile_speed"), &CardStats::get_projectile_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_speed"), "set_projectile_speed", "get_projectile_speed");
	ClassDB::bind_method(D_METHOD("set_is_ranged", "p_value"), &CardStats::set_is_ranged);
	ClassDB::bind_method(D_METHOD("get_is_ranged"), &CardStats::get_is_ranged);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_ranged"), "set_is_ranged", "get_is_ranged");
	ClassDB::bind_method(D_METHOD("set_hits_all_in_range", "p_value"), &CardStats::set_hits_all_in_range);
	ClassDB::bind_method(D_METHOD("get_hits_all_in_range"), &CardStats::get_hits_all_in_range);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hits_all_in_range"), "set_hits_all_in_range", "get_hits_all_in_range");
	ClassDB::bind_method(D_METHOD("set_effect_prob", "p_value"), &CardStats::set_effect_prob);
	ClassDB::bind_method(D_METHOD("get_effect_prob"), &CardStats::get_effect_prob);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "effect_prob"), "set_effect_prob", "get_effect_prob");
	ClassDB::bind_method(D_METHOD("set_evasion_rate", "p_value"), &CardStats::set_evasion_rate);
	ClassDB::bind_method(D_METHOD("get_evasion_rate"), &CardStats::get_evasion_rate);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "evasion_rate"), "set_evasion_rate", "get_evasion_rate");
	ClassDB::bind_method(D_METHOD("set_crit_rate", "p_value"), &CardStats::set_crit_rate);
	ClassDB::bind_method(D_METHOD("get_crit_rate"), &CardStats::get_crit_rate);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "crit_rate"), "set_crit_rate", "get_crit_rate");
	ClassDB::bind_method(D_METHOD("set_crit_damage", "p_value"), &CardStats::set_crit_damage);
	ClassDB::bind_method(D_METHOD("get_crit_damage"), &CardStats::get_crit_damage);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "crit_damage"), "set_crit_damage", "get_crit_damage");
	ClassDB::bind_method(D_METHOD("set_range_map", "p_value"), &CardStats::set_range_map);
	ClassDB::bind_method(D_METHOD("get_range_map"), &CardStats::get_range_map);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "range_map", PROPERTY_HINT_MULTILINE_TEXT), "set_range_map", "get_range_map");
	ClassDB::bind_method(D_METHOD("get_attack_pattern"), &CardStats::get_attack_pattern);
}
void CardStats::set_piece_type(String p_value) { piece_type = p_value; } String CardStats::get_piece_type() const { return piece_type; }
void CardStats::set_display_name(String p_value) { display_name = p_value; } String CardStats::get_display_name() const { return display_name; }
void CardStats::set_card_attribute(String p_value) { card_attribute = p_value; } String CardStats::get_card_attribute() const { return card_attribute; }
void CardStats::set_deploy_cost(int p_value) { deploy_cost = p_value; } int CardStats::get_deploy_cost() const { return deploy_cost; }
void CardStats::set_redeploy_time(float p_value) { redeploy_time = p_value; } float CardStats::get_redeploy_time() const { return redeploy_time; }
void CardStats::set_max_hp(int p_value) { max_hp = p_value; } int CardStats::get_max_hp() const { return max_hp; }
void CardStats::set_max_mana(int p_value) { max_mana = p_value; } int CardStats::get_max_mana() const { return max_mana; }
void CardStats::set_physical_atk(int p_value) { physical_atk = p_value; } int CardStats::get_physical_atk() const { return physical_atk; }
void CardStats::set_physical_def(int p_value) { physical_def = p_value; } int CardStats::get_physical_def() const { return physical_def; }
void CardStats::set_physical_pen(int p_value) { physical_pen = p_value; } int CardStats::get_physical_pen() const { return physical_pen; }
void CardStats::set_magic_atk(int p_value) { magic_atk = p_value; } int CardStats::get_magic_atk() const { return magic_atk; }
void CardStats::set_magic_def(int p_value) { magic_def = p_value; } int CardStats::get_magic_def() const { return magic_def; }
void CardStats::set_magic_pen(int p_value) { magic_pen = p_value; } int CardStats::get_magic_pen() const { return magic_pen; }
void CardStats::set_attack_interval(float p_value) { attack_interval = p_value; } float CardStats::get_attack_interval() const { return attack_interval; }
void CardStats::set_attack_range(int p_value) { attack_range = p_value; } int CardStats::get_attack_range() const { return attack_range; }
void CardStats::set_block_count(int p_value) { block_count = p_value; } int CardStats::get_block_count() const { return block_count; }
void CardStats::set_move_speed(float p_value) { move_speed = p_value; } float CardStats::get_move_speed() const { return move_speed; }
void CardStats::set_projectile_speed(float p_value) { projectile_speed = p_value; } float CardStats::get_projectile_speed() const { return projectile_speed; }
void CardStats::set_is_ranged(bool p_value) { is_ranged = p_value; } bool CardStats::get_is_ranged() const { return is_ranged; }
void CardStats::set_hits_all_in_range(bool p_value) { hits_all_in_range = p_value; } bool CardStats::get_hits_all_in_range() const { return hits_all_in_range; }
void CardStats::set_effect_prob(float p_value) { effect_prob = p_value; } float CardStats::get_effect_prob() const { return effect_prob; }
void CardStats::set_evasion_rate(float p_value) { evasion_rate = p_value; } float CardStats::get_evasion_rate() const { return evasion_rate; }
void CardStats::set_crit_rate(float p_value) { crit_rate = p_value; } float CardStats::get_crit_rate() const { return crit_rate; }
void CardStats::set_crit_damage(float p_value) { crit_damage = p_value; } float CardStats::get_crit_damage() const { return crit_damage; }
void CardStats::set_range_map(String p_value) {
	range_map = p_value;
	attack_pattern.clear();
	PackedStringArray lines = range_map.split("\n");
	int or_r = -1, or_c = -1;
	for (int r = 0;r < lines.size();++r) {
		String l = lines[r].replace(" ", "").replace("\t", "").replace("\r", "").strip_edges();
		int idx = l.find("O");
		if (idx != -1) { or_r = r;or_c = idx;break; }
	}
	if (or_r == -1) return;
	for (int r = 0;r < lines.size();++r) {
		String l = lines[r].replace(" ", "").replace("\t", "").replace("\r", "").strip_edges();
		for (int c = 0;c < l.length();++c) {
			if (l[c] == 'X') attack_pattern.append(Vector2(c - or_c, r - or_r));
		}
	}
}
String CardStats::get_range_map() const { return range_map; }
PackedVector2Array CardStats::get_attack_pattern() const { return attack_pattern; }