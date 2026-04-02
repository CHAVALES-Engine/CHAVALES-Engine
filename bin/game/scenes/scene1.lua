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
				scale = Vector3.new(1),
				children = {"entidad2"}
			},
			ComponentTest = {
				health = 10,
				alive = true,
				name = "godotillo",
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
				},
				vec1 = {
					Vector2.new(1),
					Vector2.new(1)
				},
				vec2 = {
					Vector4.new(2),
					Vector4.new(2)
				},
				vec3 = {
					Color.new(1,1,1,0),
					Color.new(0.5, 0.75, 0.25, 1),
				},
				vec4 = {
					Quaternion.new(),
					Quaternion.new()
				},
			},
			AudioSource = {
				soundID = "lifeUp",
				mute = false,
				is3D = true,
				loop = true,
				isStream = true,
				playOnReady = false,
				soundVolume = 4.0
			},
			TimerChangescene = {
				["focal length"] = 10,
				vec = {
					1,2,3,4
				}
			}
		}
	}
	,
	entidad2 ={
		components = {
			Transform = {
				position = Vector3.new(4,5,6),
				rotation = Quaternion.new(),
				scale = Vector3.new(2)
			},
			ModelRenderer = {
				["model folder"] = "cube.obj",
				materialPath = "material.tres",
				castShadows = true,
				receiveShadows = true
			}
		}
	}
}