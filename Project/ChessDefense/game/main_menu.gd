extends Control
@onready var start_btn: Button = $VBoxContainer/StartButton
@onready var options_btn: Button = $VBoxContainer/OptionsButton
@onready var help_btn: Button = $VBoxContainer/HelpButton
@onready var quit_btn: Button = $VBoxContainer/QuitButton
func _ready() -> void:
	if GlobalSaveManager.has_save(1):
		start_btn.text = "继续游戏"
	else:
		start_btn.text = "新游戏"
	start_btn.pressed.connect(_on_start_pressed)
	options_btn.pressed.connect(_on_options_pressed)
	help_btn.pressed.connect(_on_help_pressed)
	quit_btn.pressed.connect(_on_quit_pressed)
func _on_start_pressed() -> void:
	if GlobalSaveManager.has_save(1):
		GlobalSaveManager.load_save(1)
		# 如果教程未完成，先进教程
		if not GlobalSaveManager.get_tutorial_completed():
			get_tree().change_scene_to_file("res://tutorial.tscn")
			return
	else:
		GlobalSaveManager.create_new_save(1)
		# 新游戏默认进教程
		get_tree().change_scene_to_file("res://tutorial.tscn")
		return
	get_tree().change_scene_to_file("res://level_select.tscn")
func _on_options_pressed() -> void:
	get_tree().change_scene_to_file("res://options_menu.tscn")
func _on_help_pressed() -> void:
	pass
func _on_quit_pressed() -> void:
	get_tree().quit()
