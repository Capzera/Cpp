extends Control
@onready var grid_container: GridContainer = $MainContainer/GridContainer
@onready var back_btn: Button = $ColorRect/BackButton
@onready var depot_btn: Button = $ColorRect/DepotButton
@onready var editor_btn: Button = $ColorRect/EditorButton

func _ready() -> void:
	var unlocked: int = GlobalSaveManager.get_unlocked_level()
	var level_buttons: Array = grid_container.get_children()
	for i in range(level_buttons.size()):
		var btn: Button = level_buttons[i]
		var level_num: int = i + 1
		if level_num <= unlocked:
			btn.disabled = false
			btn.text = "关卡 " + str(level_num)
		else:
			btn.disabled = true
			btn.text = "未解锁"
		btn.pressed.connect(_on_level_pressed.bind(level_num))
	back_btn.pressed.connect(_on_back_pressed)
	if depot_btn:
		depot_btn.pressed.connect(_on_depot_pressed)
	if editor_btn:
		editor_btn.pressed.connect(_on_editor_pressed)
		# 根据开发者模式显示/隐藏编辑器按钮
		editor_btn.visible = GlobalSaveManager.get_dev_mode()
func _on_level_pressed(level_num: int) -> void:
	GlobalSaveManager.set_current_level(level_num)
	if level_num == 1:
		get_tree().change_scene_to_file("res://MainLevel_1.tscn")
	else:
		# 关卡2+：先清空上次会话手牌，进入手牌配置界面
		GlobalSaveManager.clear_session_deck()
		get_tree().change_scene_to_file("res://deck_config.tscn")
func _on_back_pressed() -> void:
	get_tree().change_scene_to_file("res://main_menu.tscn")

func _on_depot_pressed() -> void:
	get_tree().change_scene_to_file("res://card_depot.tscn")

func _on_editor_pressed() -> void:
	get_tree().change_scene_to_file("res://level_editor.tscn")
