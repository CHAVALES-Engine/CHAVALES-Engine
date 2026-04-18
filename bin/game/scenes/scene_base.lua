-- Escena principal de ejemplo
scene = { -- entidades en escena
	gizmos = true,
	entities = {
		-- ENTIDAD 1
		camera = { -- nombre de la entidad
				ddol = false, -- dont destroy on loads
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
					},
					ScriptComponent = {
						script = "./game/assets/scripts/luaComponentTest.lua"
					}
				}
		},
		cube = {
			ddol = false,
			components = {
				Transform = {
					position = Vector3.new(0,0,0),
					rotation = Quaternion.new(),
					scale = Vector3.new(2.0, 0.5, 2.0),
					},
				ModelRenderer = {
					file = "cube.fbx",
					["number of textures"] = 0
				},
				RigidBody = {
					useGravity = false, 
					isKinematic = true,
					mass = 10.0,
					position = Vector3.new(0, 0, 0),     
					velocity = Vector3.new(0, 0, 0),   
					damping = 0.0
				},
				Collider = {
					type = "BOX",
					shape = Vector3.new(400, 100, 400),      
					rotation = Vector3.new(0, 0, 0),--poner los grados que quieras que rote en el eje que quieres que rote
					dynamic = true,
					trigger = false,
					center = Vector3.new(0,0,0)    
				}
			}
		},
		esfera = {
			ddol = false,
			components = {
				Transform = {
					position = Vector3.new(0,300,0),
					rotation = Quaternion.new(),
					scale = Vector3.new(0.5),
					},
				ModelRenderer = {
					file = "sphere.fbx",
					["number of textures"] = 0
				},
				RigidBody = {	
						useGravity = true,
						isKinematic = false;
						mass = 1.0,
						position = Vector3.new(0, 300, 0),     
						velocity = Vector3.new(0, 0, 0),   
						damping = 0.0
				},
				Collider = {
					type = "CAPSULE",
					shape = Vector3.new(50, 0, 0),     
					rotation = Vector3.new(),
					dynamic = true,
					trigger = false,
					center = Vector3.new(0,0,0)    
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
   		},
      	CanvasUI = {
			ddol = false,
			components = {
				UITransform = {
					position = Vector2.new(900,10),
					children = {"LabelUI", "ButtonUI"}
					
				},
				UIPanel = {
					name = "CanvasUI"
				}
			}
		},
		ButtonUI = {
			ddol = false,
			components = {
				UITransform = {
						position = Vector2.new(100,60),
						dimension = Vector2.new(200,200),
						zBuffer = 0

						},
				UIButton = {
					--textureName = "botonPrueba",
					text = "Boton Botonez",
					opacity = 1.0
				}
			}
		},
		LabelUI= {
			ddol = false,
			components = {
				UITransform = {
						position = Vector2.new(600,60),
						dimension = Vector2.new(200,200),
						zBuffer = 0
						
					},
				UILabel = {	
					text = "Boton Botonez ( En realidad es un UILabel)",
					opacity = 1.0,
					bgColor = Color.new(1.0, 0.0, 0.0,1),
					align = "center",
					fontSize = 16.0,
					fontName = "fonts/horrendo.ttf",
					textColor = Color.new(1.0,1.0, 0.0,1)
				}
			}
  		}
		
	}
}
