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
				folder = "metroid-floating/source",
				file = "metroid_final.fbx",
				["number of textures"] = 5,
				texture0 = {
					"diffuse0",
					"metroid-floating/sourceimages/membrane",
					"Base_Color.jpeg"
				},
				texture1 = {
					"diffuse1",
					"metroid-floating/sourceimages/body",
					"Base_Color.jpeg"
				},
				texture2 = {
					"diffuse2",
					"metroid-floating/sourceimages/nuclei",
					"Base_Color.jpeg"
				},
				texture3 = {
					"diffuse3",
					"metroid-floating/sourceimages/nuclei",
					"Base_Color.jpeg"
				},texture4 = {
					"diffuse4",
					"metroid-floating/sourceimages/mandibles",
					"Base_Color.jpeg"
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
