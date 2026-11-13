extends Control

# 手牌配置界面

const RARITY_NAMES  = ["白", "绿", "蓝", "紫", "橙", "红"]
const RARITY_COLORS = [
	Color(0.90, 0.90, 0.90), Color(0.25, 0.85, 0.25), Color(0.25, 0.55, 1.00),
	Color(0.70, 0.25, 0.95), Color(1.00, 0.55, 0.15), Color(1.00, 0.18, 0.18),
]

const ALL_CARDS = [
	{ "id": "operator", "name": "士兵", "rarity": 0, "type": "P", "scene": "res://operator.tscn" },
	{ "id": "queen",    "name": "皇后", "rarity": 2, "type": "Q", "scene": "res://queen.tscn"    },
	{ "id": "castle",   "name": "城堡", "rarity": 0, "type": "R", "scene": "res://castle.tscn"  },
]

const MAX_DECK_SIZE  = 7
const CARD_W         = 96
const CARD_H         = 140
const CARD_ART_H     = 76

var target_level: int = 2
var deck_slots: Array[Control] = []
var available_card_nodes: Array[Control] = []  # 可选区卡牌节点，用于飞回动画和暗淡
var session_deck: Array[String] = []

func _ready() -> void:
	target_level = GlobalSaveManager.get_current_level()
	if not GlobalSaveManager.is_card_owned("operator"):
		if GlobalSaveManager.has_save(1):
			GlobalSaveManager.load_save(1)
		else:
			GlobalSaveManager.create_new_save(1)
	session_deck.clear()
	set_anchors_preset(Control.PRESET_FULL_RECT)
	_build_ui()

func _build_ui() -> void:
	var margin = MarginContainer.new()
	margin.set_anchors_preset(Control.PRESET_FULL_RECT)
	margin.add_theme_constant_override("margin_left",   28)
	margin.add_theme_constant_override("margin_right",  28)
	margin.add_theme_constant_override("margin_top",    16)
	margin.add_theme_constant_override("margin_bottom", 16)
	add_child(margin)

	var root = VBoxContainer.new()
	root.add_theme_constant_override("separation", 12)
	margin.add_child(root)

	# ── 标题行 ──
	var header = HBoxContainer.new()
	var title = Label.new()
	title.text = "手牌配置 — 关卡 %d" % target_level
	title.add_theme_font_size_override("font_size", 28)
	header.add_child(title)
	header.add_spacer(false)
	var back_btn = _make_btn("← 返回选关")
	back_btn.pressed.connect(func(): get_tree().change_scene_to_file("res://level_select.tscn"))
	header.add_child(back_btn)
	root.add_child(header)

	# ── 说明行 ──
	var hint = Label.new()
	hint.name = "HintLabel"
	hint.add_theme_font_size_override("font_size", 16)
	hint.add_theme_color_override("font_color", Color(0.7, 0.7, 0.7))
	root.add_child(hint)

	root.add_child(HSeparator.new())

	# ── 手牌槽位区 ──
	var deck_area = VBoxContainer.new()
	deck_area.add_theme_constant_override("separation", 6)
	root.add_child(deck_area)

	var deck_title = Label.new()
	deck_title.text = "当前手牌（点击卡牌移除）"
	deck_title.add_theme_font_size_override("font_size", 18)
	deck_area.add_child(deck_title)

	var deck_row = HBoxContainer.new()
	deck_row.name = "DeckRow"
	deck_row.add_theme_constant_override("separation", 8)
	deck_area.add_child(deck_row)

	for i in range(MAX_DECK_SIZE):
		var slot = _make_deck_slot(i)
		deck_row.add_child(slot)
		deck_slots.append(slot)

	root.add_child(HSeparator.new())

	# ── 可选卡牌区 ──
	var card_area = VBoxContainer.new()
	card_area.size_flags_vertical = Control.SIZE_EXPAND_FILL
	card_area.custom_minimum_size = Vector2(0, 180)
	card_area.add_theme_constant_override("separation", 6)
	root.add_child(card_area)

	var card_title = Label.new()
	card_title.text = "可用卡牌（点击添加到手牌）"
	card_title.add_theme_font_size_override("font_size", 18)
	card_area.add_child(card_title)

	# ScrollContainer 默认支持鼠标滚轮垂直滚动
	var scroll = ScrollContainer.new()
	scroll.size_flags_vertical   = Control.SIZE_EXPAND_FILL
	scroll.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	card_area.add_child(scroll)

	var grid = GridContainer.new()
	grid.name = "AvailableGrid"
	grid.columns = 6
	grid.add_theme_constant_override("h_separation", 10)
	grid.add_theme_constant_override("v_separation", 10)
	grid.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	scroll.add_child(grid)

	available_card_nodes.clear()
	for card in ALL_CARDS:
		if GlobalSaveManager.is_card_owned(card.id):
			var node = _make_available_card(card)
			grid.add_child(node)
			available_card_nodes.append(node)

	root.add_child(HSeparator.new())

	# ── 底部按钮 ──
	var btn_row = HBoxContainer.new()
	btn_row.alignment = BoxContainer.ALIGNMENT_CENTER
	btn_row.add_theme_constant_override("separation", 20)
	root.add_child(btn_row)

	var clear_btn = _make_btn("清空手牌")
	clear_btn.pressed.connect(_on_clear_deck)
	btn_row.add_child(clear_btn)

	var start_btn = _make_btn("开始游戏", true)
	start_btn.name = "StartButton"
	start_btn.pressed.connect(_on_start_game)
	btn_row.add_child(start_btn)

	_refresh_hint()

# ═══ 工具函数 ═══════════════════════════════════

func _make_btn(text: String, primary: bool = false) -> Button:
	var btn = Button.new()
	btn.text = text
	btn.custom_minimum_size = Vector2(140, 46)
	btn.add_theme_font_size_override("font_size", 18)
	if primary:
		var sty = StyleBoxFlat.new()
		sty.bg_color = Color(0.2, 0.6, 0.3)
		sty.set_corner_radius_all(6)
		btn.add_theme_stylebox_override("normal", sty)
	return btn

func _make_deck_slot(index: int) -> Control:
	var slot = PanelContainer.new()
	slot.custom_minimum_size = Vector2(CARD_W, CARD_H)
	slot.name = "DeckSlot_%d" % index
	var sty = StyleBoxFlat.new()
	sty.bg_color = Color(0.12, 0.12, 0.15)
	sty.border_color = Color(0.35, 0.35, 0.40)
	sty.set_border_width_all(2)
	sty.set_corner_radius_all(6)
	slot.add_theme_stylebox_override("panel", sty)
	_set_slot_empty(slot)
	return slot

func _set_slot_empty(slot: Control) -> void:
	for child in slot.get_children():
		child.queue_free()
	var lbl = Label.new()
	lbl.text = "空"
	lbl.set_anchors_preset(Control.PRESET_FULL_RECT)
	lbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	lbl.vertical_alignment   = VERTICAL_ALIGNMENT_CENTER
	lbl.add_theme_font_size_override("font_size", 15)
	lbl.add_theme_color_override("font_color", Color(0.45, 0.45, 0.45))
	lbl.mouse_filter = Control.MOUSE_FILTER_IGNORE
	slot.add_child(lbl)

func _make_available_card(card: Dictionary) -> Control:
	var container = PanelContainer.new()
	container.custom_minimum_size = Vector2(CARD_W, CARD_H)
	container.set_meta("card_id", card.id)

	var rarity = card.rarity
	var sty = StyleBoxFlat.new()
	sty.bg_color = Color(0.12, 0.12, 0.15)
	sty.border_color = RARITY_COLORS[rarity]
	sty.set_border_width_all(3)
	sty.set_corner_radius_all(6)
	container.add_theme_stylebox_override("panel", sty)

	var vbox = VBoxContainer.new()
	vbox.add_theme_constant_override("separation", 3)
	vbox.mouse_filter = Control.MOUSE_FILTER_IGNORE
	container.add_child(vbox)

	var art = ColorRect.new()
	art.custom_minimum_size = Vector2(CARD_W, CARD_ART_H)
	art.color = RARITY_COLORS[rarity].darkened(0.5)
	art.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(art)

	var name_l = Label.new()
	name_l.text = card.name
	name_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	name_l.add_theme_font_size_override("font_size", 14)
	name_l.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(name_l)

	var rarity_l = Label.new()
	rarity_l.text = RARITY_NAMES[rarity]
	rarity_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	rarity_l.add_theme_font_size_override("font_size", 12)
	rarity_l.add_theme_color_override("font_color", RARITY_COLORS[rarity])
	rarity_l.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(rarity_l)

	var lv_l = Label.new()
	lv_l.text = "Lv.%d" % GlobalSaveManager.get_card_level(card.id)
	lv_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	lv_l.add_theme_font_size_override("font_size", 11)
	lv_l.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(lv_l)

	container.mouse_filter = Control.MOUSE_FILTER_STOP
	container.gui_input.connect(func(ev: InputEvent):
		if ev is InputEventMouseButton and ev.pressed and ev.button_index == MOUSE_BUTTON_LEFT:
			_on_card_clicked(container))

	return container

# ─── 刷新可选卡暗淡状态 ───
func _refresh_available_dim() -> void:
	for node in available_card_nodes:
		var card_id: String = node.get_meta("card_id", "")
		# 已选过就暗淡且禁用（每张卡只能带一次）
		var already_selected = session_deck.has(card_id)
		node.modulate = Color(0.45, 0.45, 0.45, 1.0) if already_selected else Color(1, 1, 1, 1)
		# 设置鼠标过滤阻止点击
		node.mouse_filter = Control.MOUSE_FILTER_IGNORE if already_selected else Control.MOUSE_FILTER_STOP

# ─── 点击添加 ───
func _on_card_clicked(card_item: Control) -> void:
	if session_deck.size() >= MAX_DECK_SIZE:
		return
	var card_id: String = card_item.get_meta("card_id", "")
	if card_id.is_empty():
		return
	
	# 新增：禁止重复添加同一张卡（每张卡只能带一次）
	if session_deck.has(card_id):
		return

	var target_slot: Control = null
	for i in range(deck_slots.size()):
		if i >= session_deck.size():
			target_slot = deck_slots[i]
			break
	if not target_slot:
		return

	session_deck.append(card_id)
	_refresh_available_dim()
	_refresh_hint()
	_animate_to_slot(card_item, target_slot, card_id)

# ─── 飞向槽位动画 ───
func _animate_to_slot(from_card: Control, to_slot: Control, card_id: String) -> void:
	var flying = from_card.duplicate()
	flying.set_anchors_preset(Control.PRESET_TOP_LEFT)
	flying.global_position = from_card.global_position
	flying.size = from_card.size
	flying.z_index = 100
	flying.mouse_filter = Control.MOUSE_FILTER_IGNORE
	get_tree().root.add_child(flying)

	var tween = create_tween()
	tween.set_ease(Tween.EASE_OUT)
	tween.set_trans(Tween.TRANS_CUBIC)
	tween.tween_property(flying, "global_position", to_slot.global_position, 0.25)
	tween.tween_callback(func():
		flying.queue_free()
		_fill_slot(to_slot, card_id))

# ─── 填充槽位 ───
func _fill_slot(slot: Control, card_id: String) -> void:
	for child in slot.get_children():
		child.queue_free()

	var card = _find_card(card_id)
	if card.is_empty():
		return

	var mini = PanelContainer.new()
	mini.set_anchors_preset(Control.PRESET_FULL_RECT)
	mini.set_meta("card_id", card_id)
	var sty = StyleBoxFlat.new()
	sty.bg_color = RARITY_COLORS[card.rarity].darkened(0.55)
	sty.border_color = RARITY_COLORS[card.rarity]
	sty.set_border_width_all(2)
	sty.set_corner_radius_all(4)
	mini.add_theme_stylebox_override("panel", sty)

	var vbox = VBoxContainer.new()
	vbox.alignment = BoxContainer.ALIGNMENT_CENTER
	vbox.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.set_anchors_preset(Control.PRESET_FULL_RECT)
	mini.add_child(vbox)

	var lbl = Label.new()
	lbl.text = card.name
	lbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	lbl.add_theme_font_size_override("font_size", 14)
	lbl.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(lbl)

	var rlbl = Label.new()
	rlbl.text = RARITY_NAMES[card.rarity]
	rlbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	rlbl.add_theme_font_size_override("font_size", 11)
	rlbl.add_theme_color_override("font_color", RARITY_COLORS[card.rarity].lightened(0.2))
	rlbl.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(rlbl)

	mini.mouse_filter = Control.MOUSE_FILTER_STOP
	mini.gui_input.connect(func(ev: InputEvent):
		if ev is InputEventMouseButton and ev.pressed and ev.button_index == MOUSE_BUTTON_LEFT:
			_on_slot_clicked(slot))

	slot.add_child(mini)

# ─── 点击移除（飞回） ───
func _on_slot_clicked(slot: Control) -> void:
	var mini = slot.get_child(0) if slot.get_child_count() > 0 else null
	if not mini:
		return
	var card_id: String = mini.get_meta("card_id", "")
	if card_id.is_empty():
		return

	var idx = session_deck.find(card_id)
	if idx >= 0:
		session_deck.remove_at(idx)

	# 找到对应的可选卡节点，飞回去
	var target_card_node: Control = null
	for node in available_card_nodes:
		if node.get_meta("card_id", "") == card_id:
			target_card_node = node
			break

	_animate_from_slot(slot, target_card_node)
	_rearrange_slots()
	_refresh_available_dim()
	_refresh_hint()

# ─── 飞回可选区动画 ───
func _animate_from_slot(from_slot: Control, to_card: Control) -> void:
	if not to_card:
		return
	var flying = from_slot.duplicate()
	flying.set_anchors_preset(Control.PRESET_TOP_LEFT)
	flying.global_position = from_slot.global_position
	flying.size = from_slot.size
	flying.z_index = 100
	flying.mouse_filter = Control.MOUSE_FILTER_IGNORE
	get_tree().root.add_child(flying)

	var tween = create_tween()
	tween.set_ease(Tween.EASE_IN)
	tween.set_trans(Tween.TRANS_CUBIC)
	tween.tween_property(flying, "global_position", to_card.global_position, 0.22)
	tween.tween_callback(func(): flying.queue_free())

func _rearrange_slots() -> void:
	for i in range(deck_slots.size()):
		var slot = deck_slots[i]
		if i < session_deck.size():
			_fill_slot(slot, session_deck[i])
		else:
			_set_slot_empty(slot)

func _refresh_hint() -> void:
	var lbl = find_child("HintLabel", true, false) as Label
	if lbl:
		lbl.text = "手牌数量：%d / %d " % [session_deck.size(), MAX_DECK_SIZE]
	var start_btn = find_child("StartButton", true, false) as Button
	if start_btn:
		start_btn.disabled = session_deck.is_empty()

func _on_clear_deck() -> void:
	session_deck.clear()
	_rearrange_slots()
	_refresh_available_dim()
	_refresh_hint()

func _on_start_game() -> void:
	if session_deck.is_empty():
		return
	get_tree().set_meta("session_deck", session_deck)
	var tween = create_tween()
	tween.tween_property(self, "modulate:a", 0.0, 0.35)
	tween.tween_callback(func():
		get_tree().change_scene_to_file("res://MainLevel_%d.tscn" % target_level))

func _find_card(card_id: String) -> Dictionary:
	for card in ALL_CARDS:
		if card.id == card_id:
			return card
	return {}
