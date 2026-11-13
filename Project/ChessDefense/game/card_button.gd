extends Button
@export var op_scene: PackedScene
var cost: int = 0
var cd_time: float = 30.0
var current_money: int = 0
var card_state: int = 0
var cd_timer: float = 0.0
var cd_overlay: ColorRect
func _ready() -> void:
	add_to_group("cards")
	cd_overlay = ColorRect.new()
	cd_overlay.color = Color(0, 0, 0, 0.8)
	cd_overlay.mouse_filter = Control.MOUSE_FILTER_IGNORE
	cd_overlay.visible = false
	add_child(cd_overlay)
	if op_scene:
		var inst: Node = op_scene.instantiate()
		var st = inst.get_stats()
		if st:
			cost = st.get_deploy_cost()
			cd_time = st.get_redeploy_time()
			text = st.get_piece_type() + "\n\n￥" + str(cost)
		inst.free()
	var bm: Node = get_tree().get_root().find_child("BattleManager", true, false)
	if bm:
		current_money = bm.get_money()
	_update_visuals()
func update_money(amount: int) -> void:
	current_money = amount
	if card_state == 0:
		_update_visuals()
func _process(delta: float) -> void:
	if card_state == 2:
		cd_timer -= delta
		if cd_timer <= 0.0:
			card_state = 0
			cd_overlay.visible = false
			_update_visuals()
		else:
			cd_overlay.size = Vector2(size.x, size.y * (cd_timer / cd_time))
func _pressed() -> void:
	if card_state == 0 and current_money >= cost:
		get_tree().call_group("hud", "start_card_deploy", self, op_scene)
func on_deployed(op_node: Node) -> void:
	card_state = 1
	_update_visuals()
	if op_node:
		op_node.tree_exited.connect(_on_op_died)
func _on_op_died() -> void:
	card_state = 2
	cd_timer = cd_time
	cd_overlay.visible = true
	cd_overlay.size = size
	_update_visuals()
func _update_visuals() -> void:
	if card_state == 0:
		disabled = current_money < cost
		modulate = Color(1, 1, 1, 1) if current_money >= cost else Color(0.5, 0.5, 0.5, 1)
	elif card_state == 1:
		disabled = true
		modulate = Color(0.3, 0.3, 0.3, 1)
	elif card_state == 2:
		disabled = true
		modulate = Color(0.5, 0.5, 0.5, 1)
