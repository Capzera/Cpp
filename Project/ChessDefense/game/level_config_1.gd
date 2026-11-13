extends Node3D
@onready var board: Node3D = $ChessBoard
func _ready() -> void:
	board.set_terrain_at(0, 0, 5)
	board.set_terrain_at(1, 0, 5)
	board.set_terrain_at(3, 4, 5)
	board.set_terrain_at(4, 4, 5)
	board.set_terrain_at(0, 2, 3)
	board.set_terrain_at(4, 2, 3)
	board.set_terrain_at(2, 2, 4)
	board.set_terrain_at(2, 1, 1)
	board.set_terrain_at(2, 3, 1)
	# 出生点（关卡1怪物从右上角出生）
	board.add_spawn_point(Vector2i(4, 0))
	board.call("build_visuals")
