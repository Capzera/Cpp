extends Node
class_name LevelLoader

# 从 JSON 文件加载关卡配置并应用到 ChessBoard
static func load_level_from_json(json_path: String, board: Node3D) -> Dictionary:
	var file: FileAccess = FileAccess.open(json_path, FileAccess.READ)
	if not file:
		push_error("LevelLoader: 无法打开文件 " + json_path)
		return {}
	
	var json_string: String = file.get_as_text()
	file.close()
	
	var json: JSON = JSON.new()
	var error: int = json.parse(json_string)
	if error != OK:
		push_error("LevelLoader: JSON 解析失败")
		return {}
	
	var data: Dictionary = json.data
	
	# 应用地形数据
	var terrain: Array = data.get("terrain", [])
	for y in range(terrain.size()):
		for x in range(terrain[y].size()):
			var terrain_type: int = terrain[y][x]
			board.set_terrain_at(x, y, terrain_type)
	
	# 应用出怪点
	var spawn_points: Array = data.get("spawn_points", [])
	for sp in spawn_points:
		board.add_spawn_point(Vector2i(sp[0], sp[1]))
	
	# 构建视觉效果
	board.call("build_visuals")
	
	# 返回路径和波次数据供外部使用
	return {
		"paths": data.get("paths", []),
		"waves": data.get("waves", []),
		"base": Vector2i(data.get("base", [-1, -1])[0], data.get("base", [-1, -1])[1])
	}
