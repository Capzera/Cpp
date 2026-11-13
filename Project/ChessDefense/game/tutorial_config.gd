extends Node3D

@onready var board: Node3D = $ChessBoard

func _ready() -> void:
	# 简单的 4x3 教程地图
	# 不可放置：基地周围
	board.set_terrain_at(0, 3, 5)  # 基地左侧
	board.set_terrain_at(1, 3, 5)  # 基地上方
	
	# 高台：中央区域
	board.set_terrain_at(1, 1, 3)
	board.set_terrain_at(2, 1, 3)
	
	# 出生点标注（教程中怪物从左上角出生）
	board.add_spawn_point(Vector2i(0, 3))
	
	board.call("build_visuals")
