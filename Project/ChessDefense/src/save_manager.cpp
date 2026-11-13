#include "save_manager.h"
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
using namespace godot;
SaveManager::SaveManager() {}
SaveManager::~SaveManager() {}
void SaveManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("has_save", "slot"), &SaveManager::has_save);
	ClassDB::bind_method(D_METHOD("create_new_save", "slot"), &SaveManager::create_new_save);
	ClassDB::bind_method(D_METHOD("load_save", "slot"), &SaveManager::load_save);
	ClassDB::bind_method(D_METHOD("save_game"), &SaveManager::save_game);
	ClassDB::bind_method(D_METHOD("get_unlocked_level"), &SaveManager::get_unlocked_level);
	ClassDB::bind_method(D_METHOD("set_unlocked_level", "level"), &SaveManager::set_unlocked_level);
	ClassDB::bind_method(D_METHOD("get_current_level"), &SaveManager::get_current_level);
	ClassDB::bind_method(D_METHOD("set_current_level", "level"), &SaveManager::set_current_level);
	ClassDB::bind_method(D_METHOD("get_tutorial_completed"), &SaveManager::get_tutorial_completed);
	ClassDB::bind_method(D_METHOD("set_tutorial_completed", "completed"), &SaveManager::set_tutorial_completed);
	ClassDB::bind_method(D_METHOD("delete_save", "slot"), &SaveManager::delete_save);
	ClassDB::bind_method(D_METHOD("get_dev_mode"), &SaveManager::get_dev_mode);
	ClassDB::bind_method(D_METHOD("set_dev_mode", "enabled"), &SaveManager::set_dev_mode);
	ClassDB::bind_method(D_METHOD("set_card_owned", "card_id", "owned"), &SaveManager::set_card_owned);
	ClassDB::bind_method(D_METHOD("is_card_owned", "card_id"), &SaveManager::is_card_owned);
	ClassDB::bind_method(D_METHOD("set_card_level", "card_id", "level"), &SaveManager::set_card_level);
	ClassDB::bind_method(D_METHOD("get_card_level", "card_id"), &SaveManager::get_card_level);
	ClassDB::bind_method(D_METHOD("set_card_stars", "card_id", "stars"), &SaveManager::set_card_stars);
	ClassDB::bind_method(D_METHOD("get_card_stars", "card_id"), &SaveManager::get_card_stars);
	ClassDB::bind_method(D_METHOD("add_card_shards", "card_id", "amount"), &SaveManager::add_card_shards);
	ClassDB::bind_method(D_METHOD("get_card_shards", "card_id"), &SaveManager::get_card_shards);
	ClassDB::bind_method(D_METHOD("add_universal_shards", "rarity", "amount"), &SaveManager::add_universal_shards);
	ClassDB::bind_method(D_METHOD("get_universal_shards", "rarity"), &SaveManager::get_universal_shards);
	ClassDB::bind_method(D_METHOD("spend_universal_shards", "rarity", "amount"), &SaveManager::spend_universal_shards);
	ClassDB::bind_method(D_METHOD("add_gold", "amount"), &SaveManager::add_gold);
	ClassDB::bind_method(D_METHOD("get_gold"), &SaveManager::get_gold);
	ClassDB::bind_method(D_METHOD("spend_gold", "amount"), &SaveManager::spend_gold);
	ClassDB::bind_method(D_METHOD("add_diamond", "amount"), &SaveManager::add_diamond);
	ClassDB::bind_method(D_METHOD("get_diamond"), &SaveManager::get_diamond);
	ClassDB::bind_method(D_METHOD("spend_diamond", "amount"), &SaveManager::spend_diamond);
	ClassDB::bind_method(D_METHOD("add_medal", "amount"), &SaveManager::add_medal);
	ClassDB::bind_method(D_METHOD("get_medal"), &SaveManager::get_medal);
	// 临时会话手牌接口
	ClassDB::bind_method(D_METHOD("clear_session_deck"), &SaveManager::clear_session_deck);
	ClassDB::bind_method(D_METHOD("add_to_session_deck", "card_id"), &SaveManager::add_to_session_deck);
	ClassDB::bind_method(D_METHOD("remove_from_session_deck", "card_id"), &SaveManager::remove_from_session_deck);
	ClassDB::bind_method(D_METHOD("get_session_deck"), &SaveManager::get_session_deck);
	ClassDB::bind_method(D_METHOD("get_max_deck_size"), &SaveManager::get_max_deck_size);
	ClassDB::bind_method(D_METHOD("set_max_deck_size", "size"), &SaveManager::set_max_deck_size);
}
bool SaveManager::has_save(int slot) {
	Ref<ConfigFile> config; config.instantiate();
	return config->load("user://save_" + String::num_int64(slot) + ".cfg") == OK;
}
void SaveManager::create_new_save(int slot) {
	current_slot_ = slot;
	unlocked_level_ = 1;
	tutorial_completed_ = false;
	
	// 初始化货币
	gold_    = 5000;   // 初始金币
	diamond_ = 100;    // 初始钻石
	medal_   = 0;
	
	// 初始化卡牌数据：给玩家士兵卡
	card_owned_.clear();
	card_levels_.clear();
	card_shards_.clear();
	card_stars_.clear();
	card_owned_["operator"] = true;
	card_levels_["operator"] = 1;
	card_shards_["operator"] = 0;
	card_stars_["operator"]  = 1;
	
	// 初始化碎片：给一些测试碎片
	universal_shards_.clear();
	universal_shards_[0] = 1500;  // 白色碎片
	universal_shards_[1] = 0;
	universal_shards_[2] = 600;   // 蓝色碎片
	universal_shards_[3] = 0;
	universal_shards_[4] = 0;
	universal_shards_[5] = 0;
	
	save_game();
}
void SaveManager::load_save(int slot) {
	Ref<ConfigFile> config; config.instantiate();
	if (config->load("user://save_" + String::num_int64(slot) + ".cfg") == OK) {
		current_slot_ = slot;
		unlocked_level_ = config->get_value("Progress", "unlocked_level", 1);
		tutorial_completed_ = config->get_value("Progress", "tutorial_completed", false);
		
		// 加载货币
		gold_    = config->get_value("Currency", "gold",    0);
		diamond_ = config->get_value("Currency", "diamond", 0);
		medal_   = config->get_value("Currency", "medal",   0);
		
		// 加载卡牌数据
		card_owned_.clear(); card_levels_.clear();
		card_shards_.clear(); card_stars_.clear();
		Array owned_cards = config->get_value("Cards", "owned", Array());
		for (int i = 0; i < owned_cards.size(); ++i) {
			String card_id = owned_cards[i];
			card_owned_[card_id]  = true;
			card_levels_[card_id] = config->get_value("Cards", "level_" + card_id, 1);
			card_shards_[card_id] = config->get_value("Cards", "shards_" + card_id, 0);
			card_stars_[card_id]  = config->get_value("Cards", "stars_" + card_id, 1);
		}
		
		// 加载通用碎片
		universal_shards_.clear();
		for (int r = 0; r < 6; ++r)
			universal_shards_[r] = config->get_value("Shards", "rarity_" + String::num_int64(r), 0);
	}
}
void SaveManager::save_game() {
	if (current_slot_ == -1) return;
	Ref<ConfigFile> config; config.instantiate();
	config->set_value("Progress", "unlocked_level", unlocked_level_);
	config->set_value("Progress", "tutorial_completed", tutorial_completed_);
	
	// 保存货币
	config->set_value("Currency", "gold",    gold_);
	config->set_value("Currency", "diamond", diamond_);
	config->set_value("Currency", "medal",   medal_);
	
	// 保存卡牌数据
	Array owned_cards;
	for (auto& kv : card_owned_) {
		if (kv.second) {
			owned_cards.push_back(kv.first);
			config->set_value("Cards", "level_"  + kv.first, get_card_level(kv.first));
			config->set_value("Cards", "shards_" + kv.first, get_card_shards(kv.first));
			config->set_value("Cards", "stars_"  + kv.first, get_card_stars(kv.first));
		}
	}
	config->set_value("Cards", "owned", owned_cards);
	
	// 保存通用碎片
	for (int r = 0; r < 6; ++r)
		config->set_value("Shards", "rarity_" + String::num_int64(r), get_universal_shards(r));
	
	config->save("user://save_" + String::num_int64(current_slot_) + ".cfg");
}
int SaveManager::get_unlocked_level() const { return unlocked_level_; }
void SaveManager::set_unlocked_level(int level) { unlocked_level_ = level; }
int SaveManager::get_current_level() const { return current_level_; }
void SaveManager::set_current_level(int level) { current_level_ = level; }
bool SaveManager::get_tutorial_completed() const { return tutorial_completed_; }
void SaveManager::set_tutorial_completed(bool completed) { tutorial_completed_ = completed; }

void SaveManager::set_card_owned(const String& card_id, bool owned) { card_owned_[card_id] = owned; }
bool SaveManager::is_card_owned(const String& card_id) const {
	auto it = card_owned_.find(card_id);
	return it != card_owned_.end() && it->second;
}
void SaveManager::set_card_level(const String& card_id, int level) { card_levels_[card_id] = level; }
int SaveManager::get_card_level(const String& card_id) const {
	auto it = card_levels_.find(card_id);
	return it != card_levels_.end() ? it->second : 1;
}
void SaveManager::set_card_stars(const String& card_id, int stars) { card_stars_[card_id] = stars; }
int SaveManager::get_card_stars(const String& card_id) const {
	auto it = card_stars_.find(card_id);
	return it != card_stars_.end() ? it->second : 1;
}
void SaveManager::add_card_shards(const String& card_id, int amount) { card_shards_[card_id] += amount; }
int SaveManager::get_card_shards(const String& card_id) const {
	auto it = card_shards_.find(card_id);
	return it != card_shards_.end() ? it->second : 0;
}
void SaveManager::add_universal_shards(int rarity, int amount) { universal_shards_[rarity] += amount; }
int SaveManager::get_universal_shards(int rarity) const {
	auto it = universal_shards_.find(rarity);
	return it != universal_shards_.end() ? it->second : 0;
}
bool SaveManager::spend_universal_shards(int rarity, int amount) {
	if (get_universal_shards(rarity) >= amount) {
		universal_shards_[rarity] -= amount;
		return true;
	}
	return false;
}
void SaveManager::add_gold(int amount) { gold_ += amount; }
int SaveManager::get_gold() const { return gold_; }
bool SaveManager::spend_gold(int amount) {
	if (gold_ >= amount) { gold_ -= amount; return true; }
	return false;
}
void SaveManager::add_diamond(int amount) { diamond_ += amount; }
int SaveManager::get_diamond() const { return diamond_; }
bool SaveManager::spend_diamond(int amount) {
	if (diamond_ >= amount) { diamond_ -= amount; return true; }
	return false;
}
void SaveManager::add_medal(int amount) { medal_ += amount; }
int SaveManager::get_medal() const { return medal_; }

// ═══════════════════════════════════════════════
// 临时会话手牌接口
// ═══════════════════════════════════════════════
void SaveManager::clear_session_deck() {
	session_deck_.clear();
}

void SaveManager::add_to_session_deck(const String& card_id) {
	if ((int)session_deck_.size() >= max_deck_size_) {
		return;  // 已达上限
	}
	session_deck_.push_back(card_id);
}

void SaveManager::remove_from_session_deck(const String& card_id) {
	for (auto it = session_deck_.begin(); it != session_deck_.end(); ++it) {
		if (*it == card_id) {
			session_deck_.erase(it);
			break;
		}
	}
}

PackedStringArray SaveManager::get_session_deck() const {
	PackedStringArray result;
	for (const auto& card_id : session_deck_) {
		result.push_back(card_id);
	}
	return result;
}

int SaveManager::get_max_deck_size() const {
	return max_deck_size_;
}

void SaveManager::set_max_deck_size(int size) {
	max_deck_size_ = size;
}

// ═══════════════════════════════════════════════
// 删除存档
// ═══════════════════════════════════════════════
void SaveManager::delete_save(int slot) {
	Ref<ConfigFile> config;
	config.instantiate();
	String save_path = "user://save_" + String::num_int64(slot) + ".cfg";
	
	// 删除文件
	Ref<godot::DirAccess> dir = godot::DirAccess::open("user://");
	if (dir.is_valid()) {
		dir->remove(save_path);
	}
	
	// 如果是当前槽位，重置内存数据
	if (current_slot_ == slot) {
		current_slot_ = -1;
		unlocked_level_ = 1;
		tutorial_completed_ = false;
		card_owned_.clear();
		card_levels_.clear();
		card_shards_.clear();
		card_stars_.clear();
		universal_shards_.clear();
		gold_ = 0;
		diamond_ = 0;
		medal_ = 0;
	}
}

// ═══════════════════════════════════════════════
// 开发者模式（独立配置文件）
// ═══════════════════════════════════════════════
bool SaveManager::get_dev_mode() const {
	Ref<ConfigFile> config;
	config.instantiate();
	if (config->load("user://settings.cfg") == OK) {
		return config->get_value("Dev", "dev_mode", false);
	}
	return false;
}

void SaveManager::set_dev_mode(bool enabled) {
	dev_mode_ = enabled;
	Ref<ConfigFile> config;
	config.instantiate();
	config->load("user://settings.cfg");  // 加载现有配置（如果存在）
	config->set_value("Dev", "dev_mode", enabled);
	config->save("user://settings.cfg");
}
