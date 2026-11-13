extends Node

# 在关卡启动时初始化卡槽
# 关卡1固定给卡，关卡2+读取出战配置

const CARD_BUTTON_SCENE = preload("res://card_button.tscn")

# 卡牌ID → 场景路径映射
const CARD_SCENES = {
	"operator": "res://operator.tscn",
	"queen":    "res://queen.tscn",
	"castle":   "res://castle.tscn",
}

func _ready() -> void:
	var level_num = GlobalSaveManager.get_current_level()
	var hud = get_node_or_null("../HUD")
	if not hud:
		return
	
	var card_slot = hud.find_child("CardSlot", true, false)
	if not card_slot:
		return
	
	# 清空现有卡槽（场景中预设的）
	for child in card_slot.get_children():
		child.queue_free()
	
	var card_ids: Array[String] = []
	
	if level_num == 1:
		# 关卡1：固定士兵卡
		card_ids = ["operator"]
	else:
		# 关卡2+：从 SceneTree 元数据读取手牌配置（C++ 编译前临时方案）
		if get_tree().has_meta("session_deck"):
			var deck = get_tree().get_meta("session_deck")
			for card_id in deck:
				card_ids.append(card_id)
		else:
			# 如果没有配置，默认给士兵
			card_ids = ["operator"]
	
	# 动态生成卡槽
	for card_id in card_ids:
		if not CARD_SCENES.has(card_id):
			continue
		var scene_path = CARD_SCENES[card_id]
		if not ResourceLoader.exists(scene_path):
			continue
		
		var card_btn = CARD_BUTTON_SCENE.instantiate() as Button
		card_btn.set("op_scene", load(scene_path))
		card_slot.add_child(card_btn)
