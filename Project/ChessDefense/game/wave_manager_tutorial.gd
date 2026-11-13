extends Node

signal wave_changed(current: int, total: int)

@export var enemy_scene: PackedScene
@export var wave_interval: float = 5.0
@export var spawn_interval: float = 1.2
@export var first_wait: float = 999.0  # 等教程引导完成后再开始

const WAVE_MAX: int = 1
const WAVE_SIZES: Array[int] = [4]

const PATH_A: Array[Vector2i] = [
	Vector2i(0, 3), Vector2i(0, 2), Vector2i(0, 1), Vector2i(0, 0),
	Vector2i(1, 0), Vector2i(2, 0)
]

var current_wave_: int = 0
var spawned_in_wave_: int = 0
var phase_: int = 0
var timer_: float = 0.0
var preview_t_: float = 0.0
var preview_speed_: float = 2.2
var preview_lines_: Array[MeshInstance3D] = []
var board_: Node = null
var wave_unlocked_: bool = false

func _ready() -> void:
	board_ = get_parent().get_node_or_null("ChessBoard")
	set_process(true)
	phase_ = 0
	timer_ = 0.0
	current_wave_ = 0
	spawned_in_wave_ = 0
	emit_signal.call_deferred("wave_changed", 0, WAVE_MAX)

func get_current_wave() -> int: return current_wave_
func get_total_waves() -> int: return WAVE_MAX
func is_waves_complete() -> bool: return phase_ == 3

# 由教程引导脚本在适当时机调用，解锁波次开始
func unlock_wave() -> void:
	wave_unlocked_ = true
	first_wait = 0.0
	timer_ = 0.0

func _clear_preview() -> void:
	for ln in preview_lines_:
		if is_instance_valid(ln):
			ln.queue_free()
	preview_lines_.clear()

func _make_line_mesh(path: Array[Vector2i], t: float) -> MeshInstance3D:
	if board_ == null:
		return null
	var im := ImmediateMesh.new()
	im.surface_begin(Mesh.PRIMITIVE_LINE_STRIP)
	var pts: Array[Vector3] = []
	for gp in path:
		var wp: Vector3 = board_.grid_to_world(gp)
		wp.y = 0.8
		pts.append(wp)
	var total: float = 0.0
	for i in range(pts.size() - 1):
		total += pts[i].distance_to(pts[i + 1])
	var target: float = t * total
	var cur: float = 0.0
	for i in range(pts.size()):
		if i > 0:
			var seg: float = pts[i - 1].distance_to(pts[i])
			if cur + seg > target:
				var u: float = (target - cur) / seg
				im.surface_add_vertex(pts[i - 1].lerp(pts[i], u))
				break
			cur += seg
		im.surface_add_vertex(pts[i])
	im.surface_end()
	var mat := StandardMaterial3D.new()
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
	mat.albedo_color = Color(1.0, 0.85, 0.2)
	mat.emission_enabled = true
	mat.emission = Color(1.0, 0.85, 0.2)
	mat.emission_energy_multiplier = 2.5
	var mi := MeshInstance3D.new()
	mi.mesh = im
	mi.material_override = mat
	return mi

func _start_preview() -> void:
	_clear_preview()
	if board_ == null:
		phase_ = 2
		timer_ = 0.0
		return
	preview_t_ = 0.0
	emit_signal("wave_changed", current_wave_ + 1, WAVE_MAX)
	var ln := _make_line_mesh(PATH_A, 0.0)
	if ln != null:
		get_parent().add_child(ln)
		preview_lines_.append(ln)

func _update_preview(t: float) -> void:
	_clear_preview()
	if board_ == null:
		return
	var ln := _make_line_mesh(PATH_A, t)
	if ln != null:
		get_parent().add_child(ln)
		preview_lines_.append(ln)

func _spawn_one() -> void:
	if enemy_scene == null or board_ == null:
		return
	var inst: Node = enemy_scene.instantiate()
	if not inst is Node3D:
		inst.queue_free()
		return
	inst.set("faction", 1)
	inst.call("set_path", PATH_A)
	inst.set_position(board_.grid_to_world(PATH_A[0]))
	get_parent().add_child(inst)

func _process(delta: float) -> void:
	if phase_ == 3:
		return
	if phase_ == 0:
		if not wave_unlocked_:
			return
		timer_ += delta
		var wait: float = first_wait if current_wave_ == 0 else wave_interval
		if timer_ >= wait:
			timer_ = 0.0
			phase_ = 1
			_start_preview()
		return
	if phase_ == 1:
		preview_t_ += delta * preview_speed_
		if preview_t_ >= 1.0:
			_clear_preview()
			phase_ = 2
			spawned_in_wave_ = 0
			timer_ = spawn_interval
			return
		_update_preview(preview_t_)
		return
	if phase_ == 2:
		timer_ += delta
		if timer_ < spawn_interval:
			return
		timer_ = 0.0
		var total: int = WAVE_SIZES[current_wave_]
		_spawn_one()
		spawned_in_wave_ += 1
		if spawned_in_wave_ >= total:
			current_wave_ += 1
			if current_wave_ >= WAVE_MAX:
				phase_ = 3
				emit_signal("wave_changed", WAVE_MAX, WAVE_MAX)
			else:
				phase_ = 0
				timer_ = 0.0
