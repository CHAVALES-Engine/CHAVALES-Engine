-- scene1.lua
-- Escena principal de ejemplo
scene = { -- entidades en escena
   -- ENTIDAD 1
   camera2 = { -- nombre de la entidad
   		components = { -- componentes de esa entidad
   			Transform = { -- componente transform
   				-- posicion local
   				position = Vector3.new(1,2,3),
   				-- rotacion local
   				rotation = Quaternion.new(),
   				-- escala local
   				scale = Vector3.new(1),
   			},
			Camera = {
				  FOVy = 1.0,
				  nearPlane = 0.1,
				  farPlane = 100.0,
				  ["focal length"] = 60.0,
				  --["background color"] = Color.new(0.5,0.5,0.5,1)
   			},
			ComponentTest = {
				health = 1000
			}
		}
   },
   -- ENTIDAD 2
   cube = {
		components = {
			Transform = {
				position = Vector3.new(0,0,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(0.5),
				},
			ModelRenderer = {
				file = "metroid",
				["number of textures"] = 5,
				texture0 = {
					"membrane", 
					"0"
				},
				texture1 = {
					"body", 
					"1"
				},
				texture2 = {
					"nuclei", 
					"2"
				},
				texture3 = {
					"nuclei", 
					"10"
				},texture4 = {
					"mandibles", 
					"6"
				}
			}
		},
   },
   light = {
		components = {
			Transform = {
				position = Vector3.new(0,0,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(0.5),
				},
			Light = {
				type = 1,
				color = Color.new(1,1,1,1),
				intensity = 1.0, 
				inner   = 30.0,
				outer   = 60.0,
				falloff = 1.0
			}
		}
   }
}
