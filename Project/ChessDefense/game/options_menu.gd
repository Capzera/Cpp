extends Control

@onready var dev_mode_check: CheckBox = $MainContainer/DevModeContainer/DevModeCheck
@onready var clear_save_btn: Button = $MainContainer/ClearSaveBtn
@onready var back_btn: Button = $MainContainer/BackBtn
@onready var confirm_dialog: ConfirmationDialog = $ConfirmDialog

func _ready() -> void:
	# 读取当前开发者模式状态
	dev_mode_check.button_pressed = GlobalSaveManager.get_dev_mode()
	dev_mode_check.toggled.connect(_on_dev_mode_toggled)
	
	clear_save_btn.pressed.connect(_on_clear_save_pressed)
	back_btn.pressed.connect(_on_back_pressed)
	
	confirm_dialog.confirmed.connect(_on_clear_save_confirmed)
	confirm_dialog.dialog_text = "确定要清除所有存档吗？\n此操作不可撤销，所有进度将丢失。"

func _on_dev_mode_toggled(enabled: bool) -> void:
	GlobalSaveManager.set_dev_mode(enabled)

func _on_clear_save_pressed() -> void:
	confirm_dialog.popup_centered()

func _on_clear_save_confirmed() -> void:
	GlobalSaveManager.delete_save(1)
	# 返回主菜单并重置状态
	get_tree().change_scene_to_file("res://main_menu.tscn")

func _on_back_pressed() -> void:
	get_tree().change_scene_to_file("res://main_menu.tscn")
