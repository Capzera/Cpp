extends Node
@export var sfx_deploy: AudioStream
@export var sfx_wave: AudioStream
@export var sfx_hit: AudioStream
var players: Array[AudioStreamPlayer] = []
func _ready() -> void:
	for i in range(6):
		var p := AudioStreamPlayer.new()
		add_child(p)
		players.append(p)
func play_stream(stream: AudioStream) -> void:
	if stream == null:
		return
	for p in players:
		if not p.playing:
			p.stream = stream
			p.play()
			return
	players[0].stream = stream
	players[0].play()
func play_deploy() -> void:
	play_stream(sfx_deploy)
func play_wave() -> void:
	play_stream(sfx_wave)
func play_hit() -> void:
	play_stream(sfx_hit)
