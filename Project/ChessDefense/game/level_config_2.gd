extends Node3D
@onready var board: Node3D = $ChessBoard
func _ready() -> void:
	# 转置后的不可放置区域（原 (0,1) → (1,0)）
	board.set_terrain_at(1, 0, 5)
	board.set_terrain_at(1, 1, 5)
	board.set_terrain_at(0, 1, 5)
	board.set_terrain_at(0, 2, 5)
	board.set_terrain_at(0, 3, 5)
	board.set_terrain_at(5, 1, 5)
	board.set_terrain_at(5, 2, 5)
	board.set_terrain_at(5, 3, 5)
	# 转置后的高台区域（原 (1,1) → (1,1)）
	board.set_terrain_at(1, 1, 3)
	board.set_terrain_at(1, 2, 3)
	board.set_terrain_at(1, 3, 3)
	board.set_terrain_at(2, 3, 3)
	board.set_terrain_at(3, 1, 3)
	board.set_terrain_at(4, 1, 3)
	board.set_terrain_at(4, 2, 3)
	board.set_terrain_at(4, 3, 3)
	# 出生点标注（关卡2有两个出生点）
	board.add_spawn_point(Vector2i(5, 0))  # 路径 A 起点
	board.add_spawn_point(Vector2i(5, 4))  # 路径 B 起点
	board.call("build_visuals")
