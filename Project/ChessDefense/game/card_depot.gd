extends Control

# 卡牌仓库系统 V2
enum ViewMode { COLLECTION, SYNTHESIS, DETAIL }

const RARITY_NAMES  = ["白", "绿", "蓝", "紫", "橙", "红"]
const RARITY_COLORS = [
	Color(0.90, 0.90, 0.90), Color(0.25, 0.85, 0.25), Color(0.25, 0.55, 1.00),
	Color(0.70, 0.25, 0.95), Color(1.00, 0.55, 0.15), Color(1.00, 0.18, 0.18),
]
const SYNTHESIS_COSTS = [1000, 750, 500, 300, 200, 100]
const EXCHANGE_RATES  = [0.375, 0.167, 0.12, 0.133, 0.05]

const ALL_CARDS = [
	{ "id": "operator", "name": "士兵(常见)", "rarity": 0, "type": "P", "attribute": "物理", "scene": "res://operator.tscn" },
	{ "id": "queen",    "name": "皇后(罕见)", "rarity": 2, "type": "Q", "attribute": "狙击", "scene": "res://queen.tscn"    },
	{ "id": "castle",   "name": "城堡(常见)", "rarity": 0, "type": "R", "attribute": "物理", "scene": "res://castle.tscn"  },
]

const TYPE_NAMES = {
	"P": "士兵", "N": "骑士", "B": "主教",
	"R": "城堡", "Q": "皇后", "K": "国王"
}

const CARD_POOLS = {
	0: ["operator", "castle"], 1: [], 2: ["queen"], 3: [], 4: [], 5: [],
}

var current_view: ViewMode = ViewMode.COLLECTION
var selected_card_id: String = ""
var synthesis_rarity: int = -1

var collection_view: Control
var synthesis_view: Control
var detail_view: Control
var synthesis_detail_view: Control
var top_bar: HBoxContainer

func _ready() -> void:
	set_anchors_preset(Control.PRESET_FULL_RECT)
	_build_main_ui()

# ═══════════════════════════════════════════════
#  主框架
# ═══════════════════════════════════════════════
func _build_main_ui() -> void:
	var margin = MarginContainer.new()
	margin.set_anchors_preset(Control.PRESET_FULL_RECT)
	margin.add_theme_constant_override("margin_left",   24)
	margin.add_theme_constant_override("margin_right",  24)
	margin.add_theme_constant_override("margin_top",    16)
	margin.add_theme_constant_override("margin_bottom", 16)
	add_child(margin)

	var root = VBoxContainer.new()
	root.add_theme_constant_override("separation", 0)
	margin.add_child(root)

	top_bar = _build_top_bar()
	root.add_child(top_bar)

	var sep = HSeparator.new()
	root.add_child(sep)

	var content = Control.new()
	content.size_flags_vertical = Control.SIZE_EXPAND_FILL
	root.add_child(content)

	collection_view = _build_collection_view()
	collection_view.set_anchors_preset(Control.PRESET_FULL_RECT)
	content.add_child(collection_view)

	synthesis_view = _build_synthesis_view()
	synthesis_view.set_anchors_preset(Control.PRESET_FULL_RECT)
	content.add_child(synthesis_view)

	detail_view = _build_detail_view()
	detail_view.set_anchors_preset(Control.PRESET_FULL_RECT)
	content.add_child(detail_view)

	synthesis_detail_view = _build_synthesis_detail_view()
	synthesis_detail_view.set_anchors_preset(Control.PRESET_FULL_RECT)
	content.add_child(synthesis_detail_view)

	_switch_view(ViewMode.COLLECTION)

func _build_top_bar() -> HBoxContainer:
	var bar = HBoxContainer.new()
	bar.custom_minimum_size = Vector2(0, 56)
	bar.add_theme_constant_override("separation", 10)

	var btn_col = _nav_btn("卡牌图鉴")
	btn_col.pressed.connect(func(): _switch_view(ViewMode.COLLECTION))
	bar.add_child(btn_col)

	var btn_syn = _nav_btn("碎片合成")
	btn_syn.pressed.connect(func(): _switch_view(ViewMode.SYNTHESIS))
	bar.add_child(btn_syn)

	bar.add_spacer(false)

	var currency_lbl = Label.new()
	currency_lbl.name = "CurrencyLabel"
	currency_lbl.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
	currency_lbl.add_theme_font_size_override("font_size", 18)
	bar.add_child(currency_lbl)
	_refresh_currency(currency_lbl)

	var btn_back = _nav_btn("← 返回")
	btn_back.pressed.connect(func(): get_tree().change_scene_to_file("res://level_select.tscn"))
	bar.add_child(btn_back)

	return bar

func _nav_btn(text: String) -> Button:
	var btn = Button.new()
	btn.text = text
	btn.custom_minimum_size = Vector2(130, 44)
	btn.add_theme_font_size_override("font_size", 20)
	return btn

func _refresh_currency(lbl: Label = null) -> void:
	var l = lbl
	if not l:
		l = top_bar.find_child("CurrencyLabel", true, false) as Label
	if l:
		l.text = "⚙ 金币 %d   ◆ 钻石 %d" % [
			GlobalSaveManager.get_gold(),
			GlobalSaveManager.get_diamond()
		]

func _switch_view(mode: ViewMode) -> void:
	current_view = mode
	collection_view.visible       = (mode == ViewMode.COLLECTION)
	synthesis_view.visible        = (mode == ViewMode.SYNTHESIS)
	detail_view.visible           = (mode == ViewMode.DETAIL)
	synthesis_detail_view.visible = false
	_refresh_currency()

# ═══════════════════════════════════════════════
#  图鉴视图
# ═══════════════════════════════════════════════
func _build_collection_view() -> Control:
	var scroll = ScrollContainer.new()
	var margin = MarginContainer.new()
	margin.add_theme_constant_override("margin_top", 12)
	scroll.add_child(margin)
	var grid = GridContainer.new()
	grid.name = "CardGrid"
	grid.columns = 5
	grid.add_theme_constant_override("h_separation", 20)
	grid.add_theme_constant_override("v_separation", 20)
	margin.add_child(grid)
	_refresh_card_grid(grid)
	return scroll

func _refresh_card_grid(grid: GridContainer = null) -> void:
	var g = grid
	if not g:
		g = collection_view.find_child("CardGrid", true, false) as GridContainer
	if not g:
		return
	for child in g.get_children():
		child.queue_free()
	for card in ALL_CARDS:
		g.add_child(_make_card_frame(card))

func _make_card_frame(card: Dictionary) -> Control:
	var owned = GlobalSaveManager.is_card_owned(card.id)
	var rarity = card.rarity

	var container = PanelContainer.new()
	container.custom_minimum_size = Vector2(175, 255)

	var style = StyleBoxFlat.new()
	style.bg_color = Color(0.10, 0.10, 0.13)
	style.border_color = RARITY_COLORS[rarity] if owned else Color(0.32, 0.32, 0.32)
	style.set_border_width_all(4)
	style.set_corner_radius_all(8)
	container.add_theme_stylebox_override("panel", style)

	var vbox = VBoxContainer.new()
	vbox.add_theme_constant_override("separation", 5)
	container.add_child(vbox)

	# 立绘区
	var art_wrap = Control.new()
	art_wrap.custom_minimum_size = Vector2(175, 155)
	art_wrap.mouse_filter = Control.MOUSE_FILTER_IGNORE  # 不拦截点击
	vbox.add_child(art_wrap)

	var art = ColorRect.new()
	art.set_anchors_preset(Control.PRESET_FULL_RECT)
	art.color = RARITY_COLORS[rarity].darkened(0.52) if owned else Color(0.16, 0.16, 0.16)
	art.mouse_filter = Control.MOUSE_FILTER_IGNORE  # 不拦截点击
	art_wrap.add_child(art)

	# 类型角标（左上）
	var badge = PanelContainer.new()
	badge.position = Vector2(0, 0)
	badge.size = Vector2(44, 44)
	badge.mouse_filter = Control.MOUSE_FILTER_IGNORE  # 不拦截点击
	var bs = StyleBoxFlat.new()
	bs.bg_color = Color(0.08, 0.08, 0.10, 0.88)
	bs.set_corner_radius(CORNER_TOP_LEFT, 8)
	bs.set_corner_radius(CORNER_BOTTOM_RIGHT, 16)
	badge.add_theme_stylebox_override("panel", bs)
	var bl = Label.new()
	bl.text = card.type
	bl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	bl.vertical_alignment   = VERTICAL_ALIGNMENT_CENTER
	bl.add_theme_font_size_override("font_size", 22)
	bl.add_theme_color_override("font_color", RARITY_COLORS[rarity] if owned else Color(0.5, 0.5, 0.5))
	bl.mouse_filter = Control.MOUSE_FILTER_IGNORE  # 不拦截点击
	badge.add_child(bl)
	art_wrap.add_child(badge)

	# 卡名
	var name_l = Label.new()
	name_l.text = card.name
	name_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	name_l.add_theme_font_size_override("font_size", 15)
	name_l.add_theme_color_override("font_color", Color.WHITE if owned else Color(0.48, 0.48, 0.48))
	vbox.add_child(name_l)

	# 等阶 | 属性
	var sub_l = Label.new()
	sub_l.text = "%s | %s" % [RARITY_NAMES[rarity], card.attribute]
	sub_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	sub_l.add_theme_font_size_override("font_size", 13)
	sub_l.add_theme_color_override("font_color", RARITY_COLORS[rarity].lightened(0.15) if owned else Color(0.38, 0.38, 0.38))
	vbox.add_child(sub_l)

	# 等级 / 星级 / 未解锁
	if owned:
		var lv = GlobalSaveManager.get_card_level(card.id)
		var st = GlobalSaveManager.get_card_stars(card.id)
		var lv_l = Label.new()
		lv_l.text = "Lv.%d  ★%d" % [lv, st]
		lv_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
		lv_l.add_theme_font_size_override("font_size", 13)
		lv_l.add_theme_color_override("font_color", Color(1.0, 0.85, 0.3))
		vbox.add_child(lv_l)
	else:
		var ul = Label.new()
		ul.text = "未解锁"
		ul.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
		ul.add_theme_font_size_override("font_size", 13)
		ul.add_theme_color_override("font_color", Color(0.45, 0.45, 0.45))
		vbox.add_child(ul)

	if owned:
		container.mouse_filter = Control.MOUSE_FILTER_STOP
		container.gui_input.connect(func(ev: InputEvent):
			if ev is InputEventMouseButton and ev.pressed and ev.button_index == MOUSE_BUTTON_LEFT:
				_open_detail(card.id))

	return container

# ═══════════════════════════════════════════════
#  碎片合成总览
# ═══════════════════════════════════════════════
func _build_synthesis_view() -> Control:
	var vbox = VBoxContainer.new()
	vbox.add_theme_constant_override("separation", 28)

	var title = Label.new()
	title.text = "碎片合成"
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	title.add_theme_font_size_override("font_size", 30)
	title.add_theme_constant_override("margin_top", 20)
	vbox.add_child(title)

	var sub = Label.new()
	sub.text = "点击碎片图标进入对应等阶的合成界面"
	sub.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	sub.add_theme_font_size_override("font_size", 16)
	sub.add_theme_color_override("font_color", Color(0.6, 0.6, 0.6))
	vbox.add_child(sub)

	var row = HBoxContainer.new()
	row.alignment = BoxContainer.ALIGNMENT_CENTER
	row.add_theme_constant_override("separation", 28)
	vbox.add_child(row)

	for r in range(6):
		row.add_child(_make_shard_btn(r))

	vbox.add_spacer(false)
	return vbox

func _make_shard_btn(rarity: int) -> Button:
	var btn = Button.new()
	btn.custom_minimum_size = Vector2(148, 220)
	btn.flat = true

	var vbox = VBoxContainer.new()
	vbox.alignment = BoxContainer.ALIGNMENT_CENTER
	vbox.add_theme_constant_override("separation", 8)
	vbox.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.set_anchors_preset(Control.PRESET_FULL_RECT)
	btn.add_child(vbox)

	# 图标
	var icon_bg = PanelContainer.new()
	icon_bg.custom_minimum_size = Vector2(110, 110)
	icon_bg.mouse_filter = Control.MOUSE_FILTER_IGNORE
	var icon_style = StyleBoxFlat.new()
	icon_style.bg_color = RARITY_COLORS[rarity].darkened(0.3)
	icon_style.border_color = RARITY_COLORS[rarity]
	icon_style.set_border_width_all(2)
	icon_style.set_corner_radius_all(10)
	icon_bg.add_theme_stylebox_override("panel", icon_style)
	var icon_lbl = Label.new()
	icon_lbl.text = RARITY_NAMES[rarity][0]
	icon_lbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	icon_lbl.vertical_alignment   = VERTICAL_ALIGNMENT_CENTER
	icon_lbl.add_theme_font_size_override("font_size", 48)
	icon_lbl.add_theme_color_override("font_color", RARITY_COLORS[rarity])
	icon_lbl.mouse_filter = Control.MOUSE_FILTER_IGNORE
	icon_bg.add_child(icon_lbl)
	vbox.add_child(icon_bg)

	# 数量
	var have = GlobalSaveManager.get_universal_shards(rarity)
	var need = SYNTHESIS_COSTS[rarity]
	var cnt = Label.new()
	cnt.text = "%d / %d" % [have, need]
	cnt.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	cnt.add_theme_font_size_override("font_size", 16)
	cnt.add_theme_color_override("font_color",
		Color(0.92, 0.82, 0.2) if have >= need else Color(0.65, 0.65, 0.65))
	cnt.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(cnt)

	# 名称
	var name_l = Label.new()
	name_l.text = RARITY_NAMES[rarity] + "色碎片"
	name_l.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	name_l.add_theme_font_size_override("font_size", 15)
	name_l.add_theme_color_override("font_color", RARITY_COLORS[rarity])
	name_l.mouse_filter = Control.MOUSE_FILTER_IGNORE
	vbox.add_child(name_l)

	btn.pressed.connect(func(): _open_synthesis_detail(rarity))
	return btn

# ═══════════════════════════════════════════════
#  碎片合成操作界面（独立全屏）
# ═══════════════════════════════════════════════
func _build_synthesis_detail_view() -> Control:
	var vbox = VBoxContainer.new()
	vbox.add_theme_constant_override("separation", 22)

	var header = HBoxContainer.new()
	var back_btn = _nav_btn("← 返回合成总览")
	back_btn.pressed.connect(func(): _switch_view(ViewMode.SYNTHESIS))
	header.add_child(back_btn)
	vbox.add_child(header)

	# 内容区（由 _open_synthesis_detail 填充）
	var content = VBoxContainer.new()
	content.name = "SynDetailContent"
	content.add_theme_constant_override("separation", 22)
	vbox.add_child(content)

	return vbox

func _open_synthesis_detail(rarity: int) -> void:
	synthesis_rarity = rarity
	collection_view.visible       = false
	synthesis_view.visible        = false
	detail_view.visible           = false
	synthesis_detail_view.visible = true

	var content = synthesis_detail_view.find_child("SynDetailContent", true, false) as VBoxContainer
	for c in content.get_children():
		c.queue_free()

	var have = GlobalSaveManager.get_universal_shards(rarity)
	var need = SYNTHESIS_COSTS[rarity]

	# 标题行
	var title_row = HBoxContainer.new()
	title_row.alignment = BoxContainer.ALIGNMENT_CENTER
	var title = Label.new()
	title.text = "%s色碎片合成" % RARITY_NAMES[rarity]
	title.add_theme_font_size_override("font_size", 30)
	title.add_theme_color_override("font_color", RARITY_COLORS[rarity])
	title_row.add_child(title)
	content.add_child(title_row)

	# 持有量
	var have_lbl = Label.new()
	have_lbl.name = "SynHaveLbl"
	have_lbl.text = "当前持有：%d   每次消耗：%d" % [have, need]
	have_lbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	have_lbl.add_theme_font_size_override("font_size", 20)
	content.add_child(have_lbl)

	# 合成按钮行
	var btn_row = HBoxContainer.new()
	btn_row.alignment = BoxContainer.ALIGNMENT_CENTER
	btn_row.add_theme_constant_override("separation", 16)
	content.add_child(btn_row)
	for times in [1, 5, 10]:
		var can = have >= need * times
		var btn = Button.new()
		btn.text = "合成 %d 次" % times
		btn.custom_minimum_size = Vector2(150, 52)
		btn.add_theme_font_size_override("font_size", 20)
		btn.disabled = not can
		var sty = StyleBoxFlat.new()
		sty.bg_color = Color(0.50, 0.40, 0.12) if can else Color(0.24, 0.24, 0.24)
		sty.set_corner_radius_all(6)
		btn.add_theme_stylebox_override("normal", sty)
		btn.pressed.connect(_on_synthesis_pressed.bind(times))
		btn_row.add_child(btn)

	# 折兑区
	if rarity < 5:
		var ex_sep = HSeparator.new()
		content.add_child(ex_sep)
		var ex_lbl = Label.new()
		var out_amt = int(need * EXCHANGE_RATES[rarity])
		ex_lbl.text = "碎片折兑：%d 个%s色碎片 → %d 个%s色碎片" % [
			need, RARITY_NAMES[rarity], out_amt, RARITY_NAMES[rarity + 1]
		]
		ex_lbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
		ex_lbl.add_theme_font_size_override("font_size", 17)
		ex_lbl.add_theme_color_override("font_color", Color(0.55, 0.75, 1.0))
		content.add_child(ex_lbl)
		var ex_btn = Button.new()
		ex_btn.text = "全部折兑"
		ex_btn.custom_minimum_size = Vector2(200, 48)
		ex_btn.add_theme_font_size_override("font_size", 18)
		ex_btn.disabled = have < need
		ex_btn.pressed.connect(_on_exchange_pressed)
		var ex_center = HBoxContainer.new()
		ex_center.alignment = BoxContainer.ALIGNMENT_CENTER
		ex_center.add_child(ex_btn)
		content.add_child(ex_center)
	else:
		var max_lbl = Label.new()
		max_lbl.text = "红色碎片已是最高等阶，无法继续折兑"
		max_lbl.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
		max_lbl.add_theme_color_override("font_color", Color(0.5, 0.5, 0.5))
		content.add_child(max_lbl)

func _on_synthesis_pressed(times: int) -> void:
	var rarity = synthesis_rarity
	var cost = SYNTHESIS_COSTS[rarity] * times
	if not GlobalSaveManager.spend_universal_shards(rarity, cost):
		return
	var pool: Array = CARD_POOLS.get(rarity, [])
	if pool.is_empty():
		GlobalSaveManager.add_universal_shards(rarity, cost)
		return
	for _i in range(times):
		var card_id: String = pool[randi() % pool.size()]
		if GlobalSaveManager.is_card_owned(card_id):
			GlobalSaveManager.add_card_shards(card_id, 10 + rarity * 5)
		else:
			GlobalSaveManager.set_card_owned(card_id, true)
			GlobalSaveManager.set_card_level(card_id, 1)
			GlobalSaveManager.set_card_stars(card_id, 1)
	GlobalSaveManager.save_game()
	_open_synthesis_detail(rarity)
	_refresh_card_grid()

func _on_exchange_pressed() -> void:
	var rarity = synthesis_rarity
	if rarity >= 5:
		return
	var have = GlobalSaveManager.get_universal_shards(rarity)
	var need = SYNTHESIS_COSTS[rarity]
	var times = have / need
	if times <= 0:
		return
	if GlobalSaveManager.spend_universal_shards(rarity, need * times):
		var out = int(need * times * EXCHANGE_RATES[rarity])
		GlobalSaveManager.add_universal_shards(rarity + 1, out)
		GlobalSaveManager.save_game()
	_open_synthesis_detail(rarity)

# ═══════════════════════════════════════════════
#  卡牌详情视图（独立全屏）
# ═══════════════════════════════════════════════
func _build_detail_view() -> Control:
	var vbox = VBoxContainer.new()
	vbox.add_theme_constant_override("separation", 0)

	var header = HBoxContainer.new()
	header.custom_minimum_size = Vector2(0, 50)
	var close_btn = _nav_btn("← 返回图鉴")
	close_btn.pressed.connect(func(): _switch_view(ViewMode.COLLECTION))
	header.add_child(close_btn)
	vbox.add_child(header)

	var content = HBoxContainer.new()
	content.name = "DetailContent"
	content.size_flags_vertical = Control.SIZE_EXPAND_FILL
	content.add_theme_constant_override("separation", 24)
	vbox.add_child(content)

	# 左侧操作区
	var left = VBoxContainer.new()
	left.name = "DetailLeft"
	left.custom_minimum_size = Vector2(300, 0)
	left.add_theme_constant_override("separation", 14)
	content.add_child(left)

	# 右侧信息区
	var right = VBoxContainer.new()
	right.name = "DetailRight"
	right.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	right.add_theme_constant_override("separation", 16)
	content.add_child(right)

	return vbox

func _open_detail(card_id: String) -> void:
	selected_card_id = card_id
	var card = _card_data(card_id)
	if card.is_empty():
		return
	_switch_view(ViewMode.DETAIL)

	var left  = detail_view.find_child("DetailLeft",  true, false) as VBoxContainer
	var right = detail_view.find_child("DetailRight", true, false) as VBoxContainer
	for c in left.get_children():  c.queue_free()
	for c in right.get_children(): c.queue_free()

	var rarity = card.rarity
	var lv     = GlobalSaveManager.get_card_level(card_id)
	var stars  = GlobalSaveManager.get_card_stars(card_id)

	# ── 左侧 ──
	var name_l = Label.new()
	name_l.text = card.name
	name_l.add_theme_font_size_override("font_size", 26)
	name_l.add_theme_color_override("font_color", RARITY_COLORS[rarity])
	left.add_child(name_l)

	var info_l = Label.new()
	info_l.text = "类型：%s    属性：%s\n等阶：%s\n等级：Lv.%d    星级：★%d" % [
		TYPE_NAMES.get(card.type, card.type), card.attribute, RARITY_NAMES[rarity], lv, stars
	]
	info_l.add_theme_font_size_override("font_size", 17)
	left.add_child(info_l)

	left.add_child(HSeparator.new())

	# 升级按钮
	var upgrade_cost_lbl = Label.new()
	upgrade_cost_lbl.name = "UpgradeCostLbl"
	var gold_cost = lv * 200
	var dia_cost  = max(0, (lv - 10) * 50)
	if dia_cost > 0:
		upgrade_cost_lbl.text = "升级消耗：金币 %d + 钻石 %d" % [gold_cost, dia_cost]
	else:
		upgrade_cost_lbl.text = "升级消耗：金币 %d" % gold_cost
	upgrade_cost_lbl.add_theme_font_size_override("font_size", 15)
	upgrade_cost_lbl.add_theme_color_override("font_color", Color(0.8, 0.8, 0.6))
	left.add_child(upgrade_cost_lbl)

	var upgrade_btn = Button.new()
	upgrade_btn.text = "升级"
	upgrade_btn.custom_minimum_size = Vector2(0, 44)
	upgrade_btn.add_theme_font_size_override("font_size", 18)
	upgrade_btn.pressed.connect(_on_upgrade_card.bind(card_id))
	left.add_child(upgrade_btn)

	# 升星按钮
	var star_hint = Label.new()
	star_hint.text = "升星消耗：1 张相同卡牌" if stars < 5 else "已达最高星级"
	star_hint.add_theme_font_size_override("font_size", 15)
	star_hint.add_theme_color_override("font_color", Color(0.8, 0.8, 0.6))
	left.add_child(star_hint)

	var star_btn = Button.new()
	star_btn.text = "升星"
	star_btn.custom_minimum_size = Vector2(0, 44)
	star_btn.add_theme_font_size_override("font_size", 18)
	star_btn.disabled = (stars >= 5)
	star_btn.pressed.connect(_on_star_up.bind(card_id))
	left.add_child(star_btn)

	# 技能预留
	left.add_child(HSeparator.new())
	var skill_lbl = Label.new()
	match rarity:
		0, 1: skill_lbl.text = "（白/绿卡无技能）"
		2:    skill_lbl.text = "技能槽：主动 ×1"
		3:    skill_lbl.text = "技能槽：主动 ×1 + 被动 ×1"
		4:    skill_lbl.text = "技能槽：主动 ×1 + 被动 ×2"
		5:    skill_lbl.text = "技能槽：主动 ×2 + 被动 ×3"
	skill_lbl.add_theme_font_size_override("font_size", 15)
	skill_lbl.add_theme_color_override("font_color", Color(0.5, 0.5, 0.5))
	left.add_child(skill_lbl)

	# ── 右侧 ──
	var art = ColorRect.new()
	art.custom_minimum_size = Vector2(0, 200)
	art.color = RARITY_COLORS[rarity].darkened(0.55)
	right.add_child(art)

	# 范围 + 属性 并排行
	var info_row = HBoxContainer.new()
	info_row.size_flags_vertical = Control.SIZE_EXPAND_FILL
	info_row.add_theme_constant_override("separation", 16)
	right.add_child(info_row)

	# 左：攻击范围可视化（固定宽度，不拉伸）
	var range_wrap = _build_range_viz(card)
	range_wrap.size_flags_horizontal = Control.SIZE_SHRINK_BEGIN
	info_row.add_child(range_wrap)

	# 分隔线
	var vsep = VSeparator.new()
	vsep.size_flags_vertical = Control.SIZE_EXPAND_FILL
	info_row.add_child(vsep)

	# 右：属性滚动区（占剩余空间）
	var scroll = ScrollContainer.new()
	scroll.size_flags_horizontal = Control.SIZE_EXPAND_FILL
	scroll.size_flags_vertical   = Control.SIZE_EXPAND_FILL
	var stats_grid = GridContainer.new()
	stats_grid.name = "StatsGrid"
	stats_grid.columns = 4  # 标签 值 标签 值
	stats_grid.add_theme_constant_override("h_separation", 20)
	stats_grid.add_theme_constant_override("v_separation", 8)
	scroll.add_child(stats_grid)
	info_row.add_child(scroll)
	_populate_stats(stats_grid, card)

func _build_range_viz(card: Dictionary) -> Control:
	var box = VBoxContainer.new()
	box.add_theme_constant_override("separation", 6)

	var title = Label.new()
	title.text = "攻击范围"
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	title.add_theme_font_size_override("font_size", 16)
	box.add_child(title)

	# 从 CardStats 获取攻击范围
	var pattern = PackedVector2Array()
	var origin_x = 0
	var origin_y = 0
	var scene_path = card.get("scene", "")
	
	if ResourceLoader.exists(scene_path):
		var sc = load(scene_path) as PackedScene
		if sc:
			var inst = sc.instantiate()
			var st = inst.get("stats")
			if st and st.has_method("call"):
				pattern = st.call("get_attack_pattern")
			inst.queue_free()
	
	# 如果没有攻击范围，显示提示
	if pattern.size() == 0:
		var hint = Label.new()
		hint.text = "（该卡牌无攻击范围）"
		hint.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
		hint.add_theme_color_override("font_color", Color(0.5, 0.5, 0.5))
		box.add_child(hint)
		return box
	
	# 计算边界（包含原点 O 和所有攻击点 X）
	var min_x = 0
	var max_x = 0
	var min_y = 0
	var max_y = 0
	
	for p in pattern:
		var px = int(p.x)
		var py = int(p.y)
		min_x = min(min_x, px)
		max_x = max(max_x, px)
		min_y = min(min_y, py)
		max_y = max(max_y, py)
	
	var cols = max_x - min_x + 1
	var rows = max_y - min_y + 1
	
	# 创建网格
	var grid = GridContainer.new()
	grid.columns = cols
	grid.add_theme_constant_override("h_separation", 3)
	grid.add_theme_constant_override("v_separation", 3)
	box.add_child(grid)
	
	# 绘制范围
	for y in range(rows):
		for x in range(cols):
			var world_x = x + min_x
			var world_y = y + min_y
			
			var is_origin = (world_x == 0 and world_y == 0)
			var is_attack = false
			
			for p in pattern:
				if int(p.x) == world_x and int(p.y) == world_y:
					is_attack = true
					break
			
			var cell = PanelContainer.new()
			cell.custom_minimum_size = Vector2(32, 32)
			var sty = StyleBoxFlat.new()
			
			if is_origin:
				# O：实心白格
				sty.bg_color = Color(1.0, 1.0, 1.0)
			elif is_attack:
				# X：灰色空心格
				sty.bg_color = Color(0.15, 0.15, 0.15)
				sty.border_color = Color(0.65, 0.65, 0.65)
				sty.set_border_width_all(2)
				sty.set_corner_radius_all(4)
			else:
				# 无内容格子（不应该出现，但保留）
				sty.bg_color = Color(0.08, 0.08, 0.08, 0.2)
			
			cell.add_theme_stylebox_override("panel", sty)
			grid.add_child(cell)
	
	return box

func _populate_stats(grid: GridContainer, card: Dictionary) -> void:
	var scene_path = card.get("scene", "")
	if not ResourceLoader.exists(scene_path):
		return
	var sc = load(scene_path) as PackedScene
	if not sc:
		return
	var inst = sc.instantiate()
	var st = inst.get("stats")
	if not st:
		inst.queue_free()
		return

	var rows = [
		["生命值",   str(st.get_max_hp()),          "部署费用", str(st.get_deploy_cost())],
		["物理攻击", str(st.get_physical_atk()),     "物理防御", str(st.get_physical_def())],
		["物理穿透", str(st.get_physical_pen()),     "法术攻击", str(st.get_magic_atk())],
		["法术防御", str(st.get_magic_def()),        "法术穿透", str(st.get_magic_pen())],
		["攻击间隔", "%.2fs" % st.get_attack_interval(), "阻挡数",  str(st.get_block_count())],
		["暴击率",   "%.1f%%" % (st.get_crit_rate() * 100),   "暴击伤害", "%.1f%%" % (st.get_crit_damage() * 100)],
		["闪避率",   "%.1f%%" % (st.get_evasion_rate() * 100), "",         ""],
	]

	for row in rows:
		for i in range(4):
			var lbl = Label.new()
			lbl.text = row[i]
			lbl.add_theme_font_size_override("font_size", 16)
			if i % 2 == 0:  # 标签列
				lbl.add_theme_color_override("font_color", Color(0.72, 0.72, 0.72))
			else:            # 数值列
				lbl.add_theme_color_override("font_color", Color(1.0, 1.0, 1.0))
			grid.add_child(lbl)

	inst.queue_free()

func _card_data(card_id: String) -> Dictionary:
	for c in ALL_CARDS:
		if c.id == card_id:
			return c
	return {}

# ═══════════════════════════════════════════════
#  操作回调
# ═══════════════════════════════════════════════
func _on_upgrade_card(card_id: String) -> void:
	var lv = GlobalSaveManager.get_card_level(card_id)
	var gold_cost = lv * 200
	var dia_cost  = max(0, (lv - 10) * 50)
	if dia_cost > 0 and not GlobalSaveManager.spend_diamond(dia_cost):
		return
	if not GlobalSaveManager.spend_gold(gold_cost):
		if dia_cost > 0:
			GlobalSaveManager.add_diamond(dia_cost)  # 回滚
		return
	GlobalSaveManager.set_card_level(card_id, lv + 1)
	GlobalSaveManager.save_game()
	_refresh_currency()
	_refresh_card_grid()  # 刷新图鉴
	_open_detail(card_id)

func _on_star_up(card_id: String) -> void:
	# 占位：升星需要重复卡牌，具体库存系统待实现
	var st = GlobalSaveManager.get_card_stars(card_id)
	if st >= 5:
		return
	# TODO: 检查重复卡牌数量并消耗
	GlobalSaveManager.set_card_stars(card_id, st + 1)
	GlobalSaveManager.save_game()
	_refresh_card_grid()  # 刷新图鉴
	_open_detail(card_id)
