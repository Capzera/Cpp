#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H
#include <godot_cpp/classes/node.hpp>
namespace godot {
	class BattleManager : public Node {
		GDCLASS(BattleManager, Node)
	private:
		int current_money = 100;
		bool auto_regen_unlocked = false;
		float regen_timer = 0.0f;
		float regen_interval = 5.0f;
		int regen_amount = 5;
		bool battle_over_ = false;
	protected:
		static void _bind_methods();
	public:
		BattleManager();
		~BattleManager();
		void _ready() override;
		void _process(double delta) override;
		int get_money() const;
		void add_money(int amount);
		bool spend_money(int amount);
		void set_auto_regen(bool unlocked);
		void on_king_destroyed();
		void end_battle(bool victory);
	};
}
#endif
