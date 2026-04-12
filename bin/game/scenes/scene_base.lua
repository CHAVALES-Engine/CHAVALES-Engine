-- Escena principal de ejemplo
scene = { -- entidades en escena
   -- ENTIDAD 1
   camera = { -- nombre de la entidad
   		components = { -- componentes de esa entidad
   			Transform = { -- componente transform
   				-- posicion local
   				position = Vector3.new(200.0,200.0,200.0),
   				-- rotacion local
   				rotation = Quaternion.new(0.0,0.0,1.0,45.0),
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
		components = {
			Transform = {
				position = Vector3.new(0,0,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(1.0),
				},
			ModelRenderer = {
				file = "gizmo",
				["number of textures"] = 0
			},
			AudioSource = {
				soundID = "MySound",
				is3D = true,
				loop = true,
				isStream = false,
				playOnReady = true,
				soundVolume = 0.2
 			}
		}
   },
   cube2 = {
		components = {
			Transform = {
				position = Vector3.new(100,100,100),
				rotation = Quaternion.new(),
				scale = Vector3.new(1.0),
				},
			ModelRenderer = {
				file = "gizmo",
				["number of textures"] = 0
			}
		}
   },
   cube3 = {
		components = {
			Transform = {
				position = Vector3.new(50,50,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(5.0),
				},
			ModelRenderer = {
				file = "gizmo",
				["number of textures"] = 0
			}
			--,
			
			-- Collider = {
    		-- 	box = Vector3.new(1, 1, 1),      
    		-- 	dynamic = true,
   			-- 	kinematic = false,
			-- 	trigger = false,
    		-- 	center = Vector3.new(0,0,0)    
			-- }   
		}
   },
   light = {
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
   },
   CanvasUI ={
	components = {
		UIPanel = {
			name = "CanvasUI"
		}
	}
   },
   BottonUI = {
	components = {
		Transform = {
				position = Vector3.new(100,60,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(0.5),
				type = "ui"
				},
		UIButton = {
			textureName = "botonPrueba",
			dimension = Vector2.new(200,200),
			text = "Boton Botonez",
			panelName = "CanvasUI",
			opacity = 1.0
		}
	}
   },
   LabelUI= {
	components = {
		Transform = {
				position = Vector3.new(900,10,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(0.5),
				type = "ui"
				},
		UILabel = {
			
			dimension = Vector2.new(300,300),
			text = "Boton Botonez ( En realidad es un UILabel)",
			panelName = "CanvasUI",
			opacity = 1.0,
			bgColor = Color.new(1.0, 0.0, 0.0,1),
			align = "center",
			fontSize = 65.0,
			fontName = "fontPrueba",
			textColor = Color.new(1.0,1.0, 0.0,1)
		}
	}
  }
}
