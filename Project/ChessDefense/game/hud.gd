extends CanvasLayer
const SETTLEMENT_SCENE: PackedScene = preload("res://settlement_panel.tscn")
@onready var money_label: Label = $UI_Root/EconomyContainer/MoneyLable
@onready var battle_manager: Node = $"../BattleManager"
@onready var player_controller: Node = $"../PlayerController"
var wave_manager: Node = null
var sfx: Node
var settlement_panel: CanvasLayer
var active_card: Button = null
var level_label: Label
var wave_label: Label
var inspect_panel: Panel
var inspect_label: Label
func _ready() -> void:
	add_to_group("hud")
	sfx = Node.new()
	sfx.set_script(load("res://sfx_player.gd"))
	add_child(sfx)
	_build_top_hud()
	_build_inspect_panel()
	settlement_panel = SETTLEMENT_SCENE.instantiate()
	add_child(settlement_panel)
	if battle_manager:
		battle_manager.money_changed.connect(_on_money_changed)
		battle_manager.battle_ended.connect(_on_battle_ended)
		var initial_money: int = battle_manager.get_money()
		money_label.text = "￥" + str(initial_money)
		get_tree().call_group("cards", "update_money", initial_money)
	if player_controller:
		player_controller.card_deployed.connect(_on_card_deployed)
		player_controller.inspect_opened.connect(_on_inspect_opened)
		player_controller.inspect_closed.connect(_on_inspect_closed)
	if wave_manager:
		wave_manager.wave_changed.connect(_on_wave_changed)
		_on_wave_changed(wave_manager.get_current_wave(), wave_manager.get_total_waves())
	else:
		var wave_manager2: Node = get_node_or_null("../WaveManager2")
		if wave_manager2:
			wave_manager2.wave_changed.connect(_on_wave_changed)
			_on_wave_changed(wave_manager2.get_current_wave(), wave_manager2.get_total_waves())
		else:
			var wave_manager_tut: Node = get_node_or_null("../WaveManagerTutorial")
			if wave_manager_tut:
				wave_manager_tut.wave_changed.connect(_on_wave_changed)
				_on_wave_changed(wave_manager_tut.get_current_wave(), wave_manager_tut.get_total_waves())
	# 教程引导连接
	var tutorial_guide: Node = get_node_or_null("../TutorialGuide")
	if tutorial_guide:
		tutorial_guide.step_completed.connect(_on_tutorial_step_completed)
func _build_top_hud() -> void:
	var top: Control = Control.new()
	top.set_anchors_preset(Control.PRESET_TOP_WIDE)
	top.offset_bottom = 72.0
	add_child(top)
	level_label = Label.new()
	level_label.position = Vector2(24, 16)
	level_label.add_theme_font_size_override("font_size", 28)
	top.add_child(level_label)
	wave_label = Label.new()
	wave_label.set_anchors_preset(Control.PRESET_CENTER_TOP)
	wave_label.offset_left = -160.0
	wave_label.offset_right = 160.0
	wave_label.offset_top = 16.0
	wave_label.offset_bottom = 56.0
	wave_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	wave_label.add_theme_font_size_override("font_size", 28)
	top.add_child(wave_label)
	level_label.text = "第 " + str(GlobalSaveManager.get_current_level()) + " 关"
func _build_inspect_panel() -> void:
	inspect_panel = Panel.new()
	inspect_panel.visible = false
	inspect_panel.set_anchors_preset(Control.PRESET_CENTER_RIGHT)
	inspect_panel.offset_left = -360.0
	inspect_panel.offset_top = -180.0
	inspect_panel.offset_right = -24.0
	inspect_panel.offset_bottom = 180.0
	inspect_panel.modulate = Color(1, 1, 1, 0.88)
	add_child(inspect_panel)
	inspect_label = Label.new()
	inspect_label.set_anchors_preset(Control.PRESET_FULL_RECT)
	inspect_label.offset_left = 16.0
	inspect_label.offset_top = 16.0
	inspect_label.offset_right = -16.0
	inspect_label.offset_bottom = -16.06
	inspect_label.add_theme_font_size_override("font_size", 20)
	inspect_panel.add_child(inspect_label)
func _on_inspect_opened(piece: Node) -> void:
	if piece == null:
		return
	var st = piece.get_stats()
	if st == null:
		return
	inspect_panel.visible = true
	inspect_label.text = str(st.get_piece_type()) + "\nHP " + str(piece.get_current_hp()) + "/" + str(st.get_max_hp()) + "\n物攻 " + str(st.get_physical_atk()) + "  物防 " + str(st.get_physical_def()) + "\n法攻 " + str(st.get_magic_atk()) + "  法防 " + str(st.get_magic_def()) + "\n攻速间隔 " + str(st.get_attack_interval()) + "\n阻挡 " + str(piece.get_blocked_count()) + "/" + str(st.get_block_count()) + "\n费用 " + str(st.get_deploy_cost())
func _on_inspect_closed() -> void:
	inspect_panel.visible = false
func _on_money_changed(new_amount: int) -> void:
	money_label.text = "￥" + str(new_amount)
	get_tree().call_group("cards", "update_money", new_amount)
func _on_wave_changed(current: int, total: int) -> void:
	if current <= 0:
		wave_label.text = "准备中"
	else:
		wave_label.text = "第 " + str(current) + " / " + str(total) + " 波"
		if sfx:
			sfx.play_wave()
func start_card_deploy(card_btn: Button, scene: PackedScene) -> void:
	active_card = card_btn
	player_controller.start_deployment(scene)
func _on_card_deployed(op_node: Node) -> void:
	if sfx:
		sfx.play_deploy()
	if active_card:
		active_card.on_deployed(op_node)
		active_card = null

func _on_tutorial_step_completed() -> void:
	var wave_manager_tut: Node = get_node_or_null("../WaveManagerTutorial")
	if wave_manager_tut and wave_manager_tut.has_method("unlock_wave"):
		wave_manager_tut.unlock_wave()

func _on_battle_ended(victory: bool) -> void:
	var tutorial_guide: Node = get_node_or_null("../TutorialGuide")
	if tutorial_guide and victory:
		tutorial_guide.show_victory()
		return
	if settlement_panel:
		settlement_panel.setup(victory)
