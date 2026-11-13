#pragma once
#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <map>
#include <vector>
namespace godot {
	class SaveManager : public Node {
		GDCLASS(SaveManager, Node)
	private:
		int current_slot_ = -1;
		int unlocked_level_ = 1;
		int current_level_ = 1;
		bool tutorial_completed_ = false;
		bool dev_mode_ = false;  // 开发者模式（不保存到存档，独立配置文件）
		// 卡牌系统数据
		std::map<String, int> card_levels_;        // 卡牌ID → 等级
		std::map<String, int> card_shards_;        // 卡牌ID → 碎片数
		std::map<String, int> card_stars_;         // 卡牌ID → 星级(1-5)
		std::map<String, bool> card_owned_;        // 卡牌ID → 是否拥有
		std::map<int, int> universal_shards_;      // 等阶(0-5) → 通用碎片数
		// 货币系统
		int gold_ = 0;
		int diamond_ = 0;
		int medal_ = 0;
		// 临时会话数据（不保存到存档）
		std::vector<String> session_deck_;  // 当前局的手牌配置
		int max_deck_size_ = 7;              // 最大手牌数（可升级）
	protected:
		static void _bind_methods();
	public:
		SaveManager();
		~SaveManager();
		bool has_save(int slot);
		void create_new_save(int slot);
		void load_save(int slot);
		void save_game();
		void delete_save(int slot);
		int get_unlocked_level() const;
		void set_unlocked_level(int level);
		int get_current_level() const;
		void set_current_level(int level);
		bool get_tutorial_completed() const;
		void set_tutorial_completed(bool completed);
		
		// 开发者模式接口
		bool get_dev_mode() const;
		void set_dev_mode(bool enabled);
		// 卡牌系统接口
		void set_card_owned(const String& card_id, bool owned);
		bool is_card_owned(const String& card_id) const;
		void set_card_level(const String& card_id, int level);
		int get_card_level(const String& card_id) const;
		void set_card_stars(const String& card_id, int stars);
		int get_card_stars(const String& card_id) const;
		void add_card_shards(const String& card_id, int amount);
		int get_card_shards(const String& card_id) const;
		void add_universal_shards(int rarity, int amount);
		int get_universal_shards(int rarity) const;
		bool spend_universal_shards(int rarity, int amount);
		// 货币系统接口
		void add_gold(int amount);
		int get_gold() const;
		bool spend_gold(int amount);
		void add_diamond(int amount);
		int get_diamond() const;
		bool spend_diamond(int amount);
		void add_medal(int amount);
		int get_medal() const;
		// 临时会话手牌接口
		void clear_session_deck();
		void add_to_session_deck(const String& card_id);
		void remove_from_session_deck(const String& card_id);
		PackedStringArray get_session_deck() const;
		int get_max_deck_size() const;
		void set_max_deck_size(int size);
	};
}
#endif