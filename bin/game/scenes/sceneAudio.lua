-- Escena de prueba para audio
scene = { -- entidades en escena
	gizmos = true,
    entities = {
   -- ENTIDAD 1
   camera = { -- nombre de la entidad
		ddol = false, -- dont destroy on loads
   		components = { -- componentes de esa entidad
   			Transform = { -- componente transform
   				-- posicion local
   				position = Vector3.new(100.0,0.0,0.0),
   				-- rotacion local
   				rotation = Quaternion.new(0.0,0.0,0.0,1.0),
   				-- escala local
   				scale = Vector3.new(1)
   			},
			Camera = {
				  FOVy = 1.0,
				  nearPlane = 0.1,
				  farPlane = 10000.0,
				  ["focal length"] = 60.0
				  --["background color"] = Color.new(0.5,0.5,0.5,1)
   			},
			ComponentTest = {
				velocity = 100
			},
			AudioListener = {
			}
		}
   },
   cube = {
		ddol = false,
		components = {
			Transform = {
				position = Vector3.new(0,0,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(1.0),
				},
			ModelRenderer = {
				file = "mesh/gizmo.fbx",
				["number of textures"] = 0
			},
			AudioSource = {
				soundPath = "sfx/smb_1-up.wav",
                    soundID = "vidaExtra1",
                    is3D = true,
                    loop = true,
                    isStream = false,
                    playOnReady = true,
                    soundVolume = 2.0,
                    minRadius = 150.0,
                    maxRadius = 1000.0
 			}
		}
   },
   light = {
		ddol = false,
		components = {
			Transform = {
				position = Vector3.new(0,0,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(0.5)
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
}
