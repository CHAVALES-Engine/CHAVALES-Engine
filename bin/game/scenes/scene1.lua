scene = { -- entidades en escena
	-- ENTIDAD 1
	entidad1 = { -- nombre de la entidad
		components = { -- componentes de esa entidad
			Transform = { -- componente transform
				-- posicion local
				position = Vector3.new(1,2,3),
				-- rotacion local
				rotation = Quaternion.new(),
				-- escala local
				scale = Vector3.new(1)
			},
			-- TimerChangescene = {
				
			-- },
			ComponentTest = {
				health = 10,
				alive = true,
				-- name = "godotillo",
				vecBools = {
					false,
					true,
					false
				},
				vecString = {
					"uno",
					"dos",
					"tres"
				},
				vecInts = {
					1,
					2,
					3
				},
				vecFloats = {
					6.9,
					9.6,
					0.0
				}
			},
			AudioSource = {
				soundPath = "smb_1-up.wav",
				soundID = "example",
				mute = false,
				is3D = true,
				loop = true,
				isStream = true,
				playOnReady = false,
				soundVolume = 4.0
			}
		}
	}
}