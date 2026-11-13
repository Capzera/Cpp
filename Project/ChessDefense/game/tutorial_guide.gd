extends CanvasLayer

signal step_completed

enum Step {
	WELCOME,
	DEPLOY_CARD,
	SELECT_DIRECTION,
	INSPECT_PIECE,
	UNDERSTAND_BLOCKING,
	WAVE_START,
	VICTORY
}

var current_step: Step = Step.WELCOME
var dialog_panel: Panel
var dialog_label: Label
var highlight_overlay: ColorRect
var card_deployed: bool = false
var piece_inspected: bool = false

func _ready() -> void:
	_build_ui()
	_start_step(Step.WELCOME)
	
	# 连接信号
	var player_controller = get_node_or_null("../PlayerController")
	if player_controller:
		player_controller.card_deployed.connect(_on_card_deployed)
		player_controller.inspect_opened.connect(_on_inspect_opened)

func _build_ui() -> void:
	# 半透明遮罩
	highlight_overlay = ColorRect.new()
	highlight_overlay.set_anchors_preset(Control.PRESET_FULL_RECT)
	highlight_overlay.color = Color(0, 0, 0, 0.6)
	highlight_overlay.mouse_filter = Control.MOUSE_FILTER_IGNORE  # 不阻挡鼠标
	highlight_overlay.visible = false
	add_child(highlight_overlay)
	
	# 对话框面板
	dialog_panel = Panel.new()
	dialog_panel.set_anchors_preset(Control.PRESET_CENTER)
	dialog_panel.offset_left = -400.0
	dialog_panel.offset_top = -150.0
	dialog_panel.offset_right = 400.0
	dialog_panel.offset_bottom = 150.0
	add_child(dialog_panel)
	
	# 对话文本
	dialog_label = Label.new()
	dialog_label.set_anchors_preset(Control.PRESET_FULL_RECT)
	dialog_label.offset_left = 24.0
	dialog_label.offset_top = 24.0
	dialog_label.offset_right = -24.0
	dialog_label.offset_bottom = -80.0
	dialog_label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	dialog_label.add_theme_font_size_override("font_size", 24)
	dialog_panel.add_child(dialog_label)
	
	# 继续按钮
	var next_btn = Button.new()
	next_btn.text = "继续"
	next_btn.set_anchors_preset(Control.PRESET_BOTTOM_RIGHT)
	next_btn.offset_left = -140.0
	next_btn.offset_top = -60.0
	next_btn.offset_right = -24.0
	next_btn.offset_bottom = -24.0
	next_btn.add_theme_font_size_override("font_size", 20)
	next_btn.pressed.connect(_on_next_pressed)
	dialog_panel.add_child(next_btn)

func _start_step(step: Step) -> void:
	current_step = step
	match step:
		Step.WELCOME:
			_show_dialog("欢迎来到新手教程！\n\n在这里你将学习如何部署干员、阻挡敌人，并保护你的基地。")
		Step.DEPLOY_CARD:
			_show_dialog("点击下方的卡牌，然后点击棋盘上的格子来部署干员。\n\n注意：红色方块是敌人的出生点，敌人会从那里向基地进发！\n\n试着把士兵部署在敌人的行进路线上。")
		Step.SELECT_DIRECTION:
			_show_dialog("干员已放置！现在按住鼠标左键不松开，然后拖动鼠标选择朝向。\n\n朝向决定了干员的攻击方向，蓝色区域就是攻击覆盖范围。\n\n松开鼠标完成部署！")
		Step.INSPECT_PIECE:
			_show_dialog("左键点击棋盘上的干员可以查看详细信息。\n\n试着点击刚才部署的干员。")
			card_deployed = false
		Step.UNDERSTAND_BLOCKING:
			_show_dialog("干员有「阻挡数」属性，可以拦截敌人前进。\n\n当敌人数量超过阻挡数时，多余的敌人会继续前进。")
		Step.WAVE_START:
			_show_dialog("准备好了！敌人即将来袭。\n\n击败所有敌人，保护基地不失血即可获胜！")
		Step.VICTORY:
			_show_dialog("恭喜通关教程！\n\n现在你可以挑战正式关卡了。")

func _show_dialog(text: String) -> void:
	dialog_label.text = text
	dialog_panel.visible = true

func _hide_dialog() -> void:
	dialog_panel.visible = false

func _highlight_cards(enable: bool) -> void:
	highlight_overlay.visible = enable

func _on_next_pressed() -> void:
	match current_step:
		Step.WELCOME:
			_hide_dialog()
			_start_step(Step.DEPLOY_CARD)
		Step.DEPLOY_CARD:
			# 点继续后关闭对话框，让玩家操作
			_hide_dialog()
		Step.SELECT_DIRECTION:
			_hide_dialog()
			_start_step(Step.INSPECT_PIECE)
		Step.INSPECT_PIECE:
			# 点继续后关闭对话框，让玩家操作
			_hide_dialog()
		Step.UNDERSTAND_BLOCKING:
			_hide_dialog()
			_start_step(Step.WAVE_START)
			emit_signal("step_completed")
		Step.WAVE_START:
			_hide_dialog()
		Step.VICTORY:
			_complete_tutorial()

func _on_card_deployed(piece: Node) -> void:
	if current_step == Step.DEPLOY_CARD and not card_deployed:
		card_deployed = true
		_hide_dialog()
		await get_tree().create_timer(0.5).timeout
		_start_step(Step.SELECT_DIRECTION)

func _on_inspect_opened(piece: Node) -> void:
	if current_step == Step.INSPECT_PIECE and not piece_inspected:
		piece_inspected = true
		_hide_dialog()
		await get_tree().create_timer(1.0).timeout
		_start_step(Step.UNDERSTAND_BLOCKING)

func show_victory() -> void:
	_start_step(Step.VICTORY)

func _complete_tutorial() -> void:
	GlobalSaveManager.set_tutorial_completed(true)
	GlobalSaveManager.save_game()
	get_tree().change_scene_to_file("res://level_select.tscn")
