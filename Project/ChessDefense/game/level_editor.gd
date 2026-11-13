extends Control

# 地形类型枚举（与 ChessBoard 对应）
enum TerrainType {
	NORMAL = 0,
	SNOW = 1,
	OBSTACLE = 5,
	ELEVATED = 3,
	LOWLAND = 4
}

# 编辑模式
enum EditMode {
	TERRAIN,  # 地形编辑
	PATH,     # 路径绘制
	SPAWN,    # 出怪点标记
	BASE      # 基地标记（支持多个）
}

# UI 节点引用
@onready var grid_container: GridContainer = $MainContainer/GridScrollContainer/GridDisplay
@onready var size_x_input: SpinBox = $MainContainer/LeftScrollContainer/LeftPanel/SizeContainer/SizeXRow/SizeX
@onready var size_y_input: SpinBox = $MainContainer/LeftScrollContainer/LeftPanel/SizeContainer/SizeYRow/SizeY

@onready var apply_size_btn: Button = $MainContainer/LeftScrollContainer/LeftPanel/SizeContainer/ApplySize
@onready var mode_tabs: TabContainer = $MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer
@onready var export_btn: Button = $MainContainer/LeftScrollContainer/LeftPanel/ActionContainer/ExportBtn
@onready var import_btn: Button = $MainContainer/LeftScrollContainer/LeftPanel/ActionContainer/ImportBtn
@onready var back_btn: Button = $MainContainer/LeftScrollContainer/LeftPanel/ActionContainer/BackBtn
@onready var file_dialog: FileDialog = $FileDialog

# 地形编辑标签页
@onready var terrain_options: OptionButton = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/地形编辑/TerrainOption"

# 路径绘制标签页
@onready var new_path_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/路径绘制/NewPathBtn"
@onready var finish_path_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/路径绘制/FinishPathBtn"
@onready var clear_path_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/路径绘制/ClearPathBtn"
@onready var path_list: ItemList = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/路径绘制/PathList"

# 出怪点标签页
@onready var clear_spawn_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/出怪点/ClearSpawnBtn"
@onready var spawn_list: ItemList = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/出怪点/SpawnList"

# 基地位置标签页
@onready var base_list: ItemList = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/基地位置/BaseList"
@onready var clear_base_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/基地位置/ClearBaseBtn"

# 波次配置标签页
@onready var wave_list: ItemList = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/WaveList"
@onready var add_wave_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/WaveBtnRow/AddWaveBtn"
@onready var remove_wave_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/WaveBtnRow/RemoveWaveBtn"
@onready var enemy_type_option: OptionButton = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/EnemyTypeRow/EnemyTypeOption"
@onready var enemy_count_spin: SpinBox = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/EnemyCountRow/EnemyCountSpin"
@onready var spawn_interval_spin: SpinBox = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/SpawnIntervalRow/SpawnIntervalSpin"
@onready var wave_interval_spin: SpinBox = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/WaveIntervalRow/WaveIntervalSpin"
@onready var save_wave_btn: Button = $"MainContainer/LeftScrollContainer/LeftPanel/ModeTabContainer/波次配置/SaveWaveBtn"

# 预览区
@onready var preview_btn: Button = $MainContainer/LeftScrollContainer/LeftPanel/PreviewContainer/PreviewBtn
@onready var info_label: Label = $MainContainer/LeftScrollContainer/LeftPanel/PreviewContainer/InfoLabel

# 编辑数据
var grid_size: Vector2i = Vector2i(6, 5)
var terrain_data: Array = []
var paths: Array = []
var current_path: Array = []
var spawn_points: Array = []
var base_points: Array = []       # 改为多基地支持
var base_position: Vector2i = Vector2i(-1, -1)  # 保留向后兼容
var current_mode: EditMode = EditMode.TERRAIN
var current_terrain: TerrainType = TerrainType.NORMAL
var is_dragging: bool = false
var drag_painted: Array = []  # 本次拖拽已涂抹的格子（避免重复）
var mouse_down_pos: Vector2i = Vector2i(-1, -1)  # 鼠标按下时的格子位置

# 波次数据：每个元素是 { enemy_type, count, spawn_interval, wave_interval }
var waves: Array = []
var selected_wave_index: int = -1

func _ready() -> void:
	# 初始化 UI
	size_x_input.value = grid_size.x
	size_y_input.value = grid_size.y
	apply_size_btn.pressed.connect(_on_apply_size)
	
	# 初始化地形选项
	terrain_options.clear()
	terrain_options.add_item("普通地面", TerrainType.NORMAL)
	terrain_options.add_item("雪地", TerrainType.SNOW)
	terrain_options.add_item("高台", TerrainType.ELEVATED)
	terrain_options.add_item("低谷", TerrainType.LOWLAND)
	terrain_options.add_item("障碍", TerrainType.OBSTACLE)
	terrain_options.item_selected.connect(_on_terrain_selected)
	
	# 路径绘制按钮
	new_path_btn.pressed.connect(_on_new_path)
	finish_path_btn.pressed.connect(_on_finish_path)
	clear_path_btn.pressed.connect(_on_clear_paths)
	
	# 出怪点按钮
	clear_spawn_btn.pressed.connect(_on_clear_spawns)
	
	# 基地按钮
	clear_base_btn.pressed.connect(_on_clear_bases)
	
	# 波次配置按钮
	enemy_type_option.clear()
	enemy_type_option.add_item("普通敌人", 0)
	enemy_type_option.add_item("快跑敌人", 1)
	enemy_type_option.add_item("坦克敌人", 2)
	add_wave_btn.pressed.connect(_on_add_wave)
	remove_wave_btn.pressed.connect(_on_remove_wave)
	save_wave_btn.pressed.connect(_on_save_wave)
	wave_list.item_selected.connect(_on_wave_selected)
	
	# 预览
	preview_btn.pressed.connect(_on_preview_pressed)
	import_btn.pressed.connect(_on_import_pressed)
	back_btn.pressed.connect(_on_back_pressed)
	
	# 文件对话框
	file_dialog.file_selected.connect(_on_file_selected)
	
	# 模式标签页切换
	mode_tabs.tab_changed.connect(_on_mode_changed)
	
	# 初始化网格
	_initialize_grid()
	_rebuild_grid_visual()
	_update_info_label()

func _initialize_grid() -> void:
	terrain_data.clear()
	for y in range(grid_size.y):
		var row: Array = []
		for x in range(grid_size.x):
			row.append(TerrainType.NORMAL)
		terrain_data.append(row)

func _rebuild_grid_visual() -> void:
	# 清空现有网格
	for child in grid_container.get_children():
		child.queue_free()
	
	grid_container.columns = grid_size.x
	
	# 创建网格按钮
	for y in range(grid_size.y):
		for x in range(grid_size.x):
			var btn: Button = Button.new()
			btn.custom_minimum_size = Vector2(64, 64)
			btn.set_meta("grid_pos", Vector2i(x, y))
			btn.button_down.connect(_on_grid_cell_pressed.bind(Vector2i(x, y)))
			btn.button_up.connect(_on_grid_cell_released.bind(Vector2i(x, y)))
			btn.mouse_entered.connect(_on_grid_cell_hover.bind(Vector2i(x, y)))
			grid_container.add_child(btn)
			_update_cell_visual(btn, Vector2i(x, y))

func _update_cell_visual(btn: Button, pos: Vector2i) -> void:
	var terrain: int = terrain_data[pos.y][pos.x]
	var color: Color
	var label: String = ""
	
	# 参考关卡内地图配色，明显可区分
	match terrain:
		TerrainType.NORMAL:
			color = Color(0.45, 0.55, 0.42)   # 草地绿
			label = "普"
		TerrainType.SNOW:
			color = Color(0.72, 0.88, 0.98)   # 冰蓝白
			label = "雪"
		TerrainType.ELEVATED:
			color = Color(0.85, 0.75, 0.40)   # 高台黄土
			label = "高"
		TerrainType.LOWLAND:
			color = Color(0.28, 0.45, 0.30)   # 深绿低谷
			label = "低"
		TerrainType.OBSTACLE:
			color = Color(0.18, 0.18, 0.20)   # 近黑障碍
			label = "X"
		_:
			color = Color(0.45, 0.55, 0.42)
			label = ""
	
	# 特殊标记优先级：基地 > 出怪点 > 路径
	var is_base: bool = pos in base_points
	var is_spawn: bool = pos in spawn_points
	
	if is_base:
		color = Color(0.15, 0.78, 0.30)
		label = "🏠"
	elif is_spawn:
		color = Color(0.88, 0.22, 0.18)
		label = "👾"
	else:
		# 已完成路径
		for i in range(paths.size()):
			var path: Array = paths[i]
			if pos in path:
				color = color.lerp(Color(0.95, 0.85, 0.10), 0.55)
				var idx: int = path.find(pos)
				label = "P%d-%d" % [i + 1, idx + 1]
				break
		# 正在编辑的路径
		if pos in current_path:
			color = color.lerp(Color(1.0, 0.55, 0.05), 0.65)
			var idx: int = current_path.find(pos)
			label = "✏%d" % (idx + 1)
	
	btn.modulate = color
	btn.text = label

func _on_grid_cell_pressed(pos: Vector2i) -> void:
	# 鼠标按下时立即涂抹起始格子，同时记录拖拽状态
	mouse_down_pos = pos
	is_dragging = false
	drag_painted.clear()
	drag_painted.append(pos)
	_apply_brush(pos)

func _on_grid_cell_released(pos: Vector2i) -> void:
	# 松开时如果已经拖拽过，什么也不做（已经涂了）
	# 如果没有拖拽（只是在同一个格子按下松开），起始格子已在 pressed 时涂过
	is_dragging = false
	drag_painted.clear()
	mouse_down_pos = Vector2i(-1, -1)

func _on_grid_cell_hover(pos: Vector2i) -> void:
	# 鼠标没按下时不做任何事
	if mouse_down_pos == Vector2i(-1, -1):
		return
	# 已经涂过的格子不重复涂
	if pos in drag_painted:
		return
	# 进入拖拽模式
	is_dragging = true
	drag_painted.append(pos)
	_apply_brush(pos)

func _apply_brush(pos: Vector2i) -> void:
	match current_mode:
		EditMode.TERRAIN:
			terrain_data[pos.y][pos.x] = current_terrain
			_update_all_cells()
		EditMode.PATH:
			if pos not in current_path:
				current_path.append(pos)
				_refresh_path_list()
				_update_all_cells()
		EditMode.SPAWN:
			if not is_dragging:  # 出怪点只在单击时切换，不在拖拽时重复
				if pos in spawn_points:
					spawn_points.erase(pos)
				else:
					spawn_points.append(pos)
				_refresh_spawn_list()
				_update_all_cells()
				_update_info_label()
		EditMode.BASE:
			if not is_dragging:  # 基地只在单击时切换
				if pos in base_points:
					base_points.erase(pos)
				else:
					base_points.append(pos)
				_refresh_base_list()
				_update_all_cells()
				_update_info_label()


func _update_all_cells() -> void:
	for child in grid_container.get_children():
		if child is Button:
			var pos: Vector2i = child.get_meta("grid_pos")
			_update_cell_visual(child, pos)

func _on_apply_size() -> void:
	grid_size.x = int(size_x_input.value)
	grid_size.y = int(size_y_input.value)
	_initialize_grid()
	_rebuild_grid_visual()
	_update_info_label()

func _on_terrain_selected(index: int) -> void:
	current_terrain = terrain_options.get_item_id(index)

func _on_new_path() -> void:
	if current_path.size() > 0:
		paths.append(current_path.duplicate())
		current_path.clear()
	_refresh_path_list()
	_update_all_cells()
	_update_info_label()

func _on_finish_path() -> void:
	if current_path.size() < 2:
		return
	paths.append(current_path.duplicate())
	current_path.clear()
	_refresh_path_list()
	_update_all_cells()
	_update_info_label()

func _on_clear_paths() -> void:
	paths.clear()
	current_path.clear()
	_refresh_path_list()
	_update_all_cells()
	_update_info_label()

func _on_clear_spawns() -> void:
	spawn_points.clear()
	_refresh_spawn_list()
	_update_all_cells()
	_update_info_label()

func _on_clear_bases() -> void:
	base_points.clear()
	_refresh_base_list()
	_update_all_cells()
	_update_info_label()

func _refresh_path_list() -> void:
	path_list.clear()
	for i in range(paths.size()):
		path_list.add_item("路径 " + str(i + 1) + "（" + str(paths[i].size()) + " 个节点）")
	if current_path.size() > 0:
		path_list.add_item("（编辑中：" + str(current_path.size()) + " 个节点）")

func _refresh_spawn_list() -> void:
	spawn_list.clear()
	for sp in spawn_points:
		spawn_list.add_item("出怪点 (%d, %d)" % [sp.x, sp.y])

func _refresh_base_list() -> void:
	base_list.clear()
	for bp in base_points:
		base_list.add_item("基地 (%d, %d)" % [bp.x, bp.y])

func _on_mode_changed(tab: int) -> void:
	current_mode = tab

func _on_add_wave() -> void:
	var new_wave: Dictionary = {
		"enemy_type": 0,
		"count": 5,
		"spawn_interval": 1.0,
		"wave_interval": 10.0
	}
	waves.append(new_wave)
	_refresh_wave_list()
	_update_info_label()

func _on_remove_wave() -> void:
	if selected_wave_index >= 0 and selected_wave_index < waves.size():
		waves.remove_at(selected_wave_index)
		selected_wave_index = -1
		_refresh_wave_list()
		_update_info_label()

func _on_save_wave() -> void:
	if selected_wave_index >= 0 and selected_wave_index < waves.size():
		waves[selected_wave_index]["enemy_type"] = enemy_type_option.get_selected_id()
		waves[selected_wave_index]["count"] = int(enemy_count_spin.value)
		waves[selected_wave_index]["spawn_interval"] = spawn_interval_spin.value
		waves[selected_wave_index]["wave_interval"] = wave_interval_spin.value
		_refresh_wave_list()
		wave_list.select(selected_wave_index)

func _on_wave_selected(index: int) -> void:
	selected_wave_index = index
	if index >= 0 and index < waves.size():
		var wave: Dictionary = waves[index]
		enemy_type_option.select(wave["enemy_type"])
		enemy_count_spin.value = wave["count"]
		spawn_interval_spin.value = wave["spawn_interval"]
		wave_interval_spin.value = wave["wave_interval"]

func _refresh_wave_list() -> void:
	wave_list.clear()
	for i in range(waves.size()):
		var w: Dictionary = waves[i]
		var enemy_name: String = ["普通", "快跑", "坦克"][w["enemy_type"]]
		wave_list.add_item("波次 %d：%s x%d" % [i + 1, enemy_name, w["count"]])

func _on_export_pressed() -> void:
	file_dialog.file_mode = FileDialog.FILE_MODE_SAVE_FILE
	file_dialog.filters = ["*.json ; JSON Files"]
	file_dialog.current_file = "level_" + str(GlobalSaveManager.get_current_level() + 1) + ".json"
	file_dialog.popup_centered(Vector2i(800, 600))

func _on_import_pressed() -> void:
	file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
	file_dialog.filters = ["*.json ; JSON Files"]
	file_dialog.popup_centered(Vector2i(800, 600))

func _on_file_selected(path: String) -> void:
	if file_dialog.file_mode == FileDialog.FILE_MODE_SAVE_FILE:
		_export_to_json(path)
	else:
		_import_from_json(path)

func _export_to_json(path: String) -> void:
	# 确认当前路径
	if current_path.size() > 0:
		paths.append(current_path.duplicate())
		current_path.clear()
	
	# 向后兼容：如果只有一个基地，也设置 base 字段
	var base_export: Array = [-1, -1]
	if base_points.size() > 0:
		base_export = [base_points[0].x, base_points[0].y]
	
	var data: Dictionary = {
		"size": [grid_size.x, grid_size.y],
		"terrain": terrain_data.duplicate(true),
		"paths": paths.duplicate(true),
		"spawn_points": spawn_points.duplicate(),
		"base": base_export,
		"bases": base_points.duplicate(),  # 新增：多基地支持
		"waves": waves.duplicate(true)
	}
	
	var json_string: String = JSON.stringify(data, "\t")
	var file: FileAccess = FileAccess.open(path, FileAccess.WRITE)
	if file:
		file.store_string(json_string)
		file.close()
		print("关卡导出成功: ", path)
	else:
		print("导出失败！")

func _import_from_json(path: String) -> void:
	var file: FileAccess = FileAccess.open(path, FileAccess.READ)
	if not file:
		print("导入失败：文件不存在")
		return
	
	var json_string: String = file.get_as_text()
	file.close()
	
	var json: JSON = JSON.new()
	var error: int = json.parse(json_string)
	if error != OK:
		print("JSON 解析失败")
		return
	
	var data: Dictionary = json.data
	
	grid_size = Vector2i(data["size"][0], data["size"][1])
	terrain_data = data["terrain"].duplicate(true)
	paths = data["paths"].duplicate(true)
	spawn_points = data["spawn_points"].duplicate()
	
	# 加载基地：优先使用新的 bases 数组，否则用旧的 base 单点
	base_points.clear()
	if data.has("bases") and data["bases"].size() > 0:
		for bp in data["bases"]:
			base_points.append(Vector2i(bp[0], bp[1]))
	elif data.has("base"):
		var base_arr: Array = data["base"]
		if base_arr[0] >= 0:
			base_points.append(Vector2i(base_arr[0], base_arr[1]))
	
	# 加载波次数据
	waves.clear()
	for w in data.get("waves", []):
		waves.append({
			"enemy_type": int(w.get("enemy_type", 0)),
			"count": int(w.get("count", 5)),
			"spawn_interval": float(w.get("spawn_interval", 1.0)),
			"wave_interval": float(w.get("wave_interval", 10.0))
		})
	
	# 更新 UI
	size_x_input.value = grid_size.x
	size_y_input.value = grid_size.y
	_refresh_path_list()
	_refresh_spawn_list()
	_refresh_base_list()
	_refresh_wave_list()
	_rebuild_grid_visual()
	_update_info_label()
	print("关卡导入成功: ", path)

func _on_back_pressed() -> void:
	get_tree().change_scene_to_file("res://level_select.tscn")

func _on_preview_pressed() -> void:
	# 高亮所有路径格子
	for path_idx in range(paths.size()):
		for node in paths[path_idx]:
			_highlight_path_node(node, path_idx)
	
	# 如果有正在编辑的路径也显示
	for node in current_path:
		_highlight_path_node(node, -1)
	
	_update_info_label()

func _highlight_path_node(pos: Vector2i, path_idx: int) -> void:
	for child in grid_container.get_children():
		if child is Button:
			var cell_pos: Vector2i = child.get_meta("grid_pos")
			if cell_pos == pos:
				var tween: Tween = create_tween()
				tween.tween_property(child, "modulate:a", 0.3, 0.3)
				tween.tween_property(child, "modulate:a", 1.0, 0.3)
				break

func _update_info_label() -> void:
	var info: String = "地图尺寸：%d x %d\n" % [grid_size.x, grid_size.y]
	info += "路径数：%d\n" % paths.size()
	info += "出怪点：%d\n" % spawn_points.size()
	info += "波次数：%d\n" % waves.size()
	
	if base_position.x >= 0:
		info += "基地：(%d, %d)\n" % [base_position.x, base_position.y]
	else:
		info += "基地：未设置\n"
	
	var total_enemies: int = 0
	for w in waves:
		total_enemies += int(w["count"])
	info += "总敌人数：%d" % total_enemies
	
	info_label.text = info
