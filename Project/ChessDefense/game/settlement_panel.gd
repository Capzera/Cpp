extends CanvasLayer
@onready var title_label: Label = $TitleLabel
@onready var action_btn: Button = $ButtonContainer/ActionBtn
@onready var menu_btn: Button = $ButtonContainer/MenuBtn
var victory: bool = false
func _ready() -> void:
	process_mode = Node.PROCESS_MODE_ALWAYS
	visible = false
	action_btn.pressed.connect(_on_action_pressed)
	menu_btn.pressed.connect(_on_menu_pressed)
func setup(p_victory: bool) -> void:
	victory = p_victory
	visible = true
	get_tree().paused = true
	if victory:
		title_label.text = "胜利"
		action_btn.text = "下一关"
		var current: int = GlobalSaveManager.get_current_level()
		var next: int = current + 1
		if next > GlobalSaveManager.get_unlocked_level():
			GlobalSaveManager.set_unlocked_level(next)
			GlobalSaveManager.save_game()
	else:
		title_label.text = "失败"
		action_btn.text = "重新挑战"
func _on_action_pressed() -> void:
	get_tree().paused = false
	if victory:
		var next_level: int = GlobalSaveManager.get_current_level() + 1
		GlobalSaveManager.set_current_level(next_level)
		var scene_map: Dictionary = {
			1: "res://MainLevel_1.tscn",
			2: "res://MainLevel_2.tscn",
		}
		if scene_map.has(next_level):
			get_tree().change_scene_to_file(scene_map[next_level])
		else:
			get_tree().change_scene_to_file("res://level_select.tscn")
	else:
		var level_num: int = GlobalSaveManager.get_current_level()
		var scene_map: Dictionary = {
			1: "res://MainLevel_1.tscn",
			2: "res://MainLevel_2.tscn",
		}
		if scene_map.has(level_num):
			get_tree().change_scene_to_file(scene_map[level_num])
		else:
			get_tree().change_scene_to_file("res://MainLevel_1.tscn")
func _on_menu_pressed() -> void:
	get_tree().paused = false
	get_tree().change_scene_to_file("res://level_select.tscn")
