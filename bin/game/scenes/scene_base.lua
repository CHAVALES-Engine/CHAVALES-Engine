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
			}
		}
   },
   cube2 = {
		components = {
			Transform = {
				position = Vector3.new(150,100,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(1.0),
				},
			ModelRenderer = {
				file = "gizmo",
				["number of textures"] = 0
			},
			RigidBody = {
					RBid = 1,
					mass = 10.0,
					position = Vector3.new(150, 100, 0),     
					velocity = Vector3.new(-1, 0, 0),   
					useGravity = false,
					gravity = Vector3.new(0, -0.009981, 0)   
			 }
			 ,
			Collider = {
    			box = Vector3.new(1, 1, 1),      
    			dynamic = true,
   				kinematic = false,
				trigger = false,
    			center = Vector3.new(0,0,0)    
			}   
		}
   },
   cube3 = {
		components = {
			Transform = {
				position = Vector3.new(50,100,0),
				rotation = Quaternion.new(),
				scale = Vector3.new(1.0),
				},
			ModelRenderer = {
				file = "gizmo",
				["number of textures"] = 0
			}
			,
			RigidBody = {
					RBid = 1,
					mass = 10.0,
					position = Vector3.new(50, 100, 0),     
					velocity = Vector3.new(1, 0, 0),   
					useGravity = false,
					gravity = Vector3.new(0, -0.009981, 0)   
			 }
			 ,
			Collider = {
    			box = Vector3.new(1, 1, 1),      
    			dynamic = true,
   				kinematic = false,
				trigger = false,
    			center = Vector3.new(0,0,0)    
			}   
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
--    cube4 = {
-- 		components = {
-- 			Transform = {
-- 				position = Vector3.new(0,0,0),
-- 				rotation = Quaternion.new(),
-- 				scale = Vector3.new(0.5),
-- 				scale = Vector3.new(1.0),
-- 				},
-- 			ModelRenderer = {
-- 				file = "metroid",
-- 				["number of textures"] = 5,
-- 				texture0 = {
-- 					"membrane", 
-- 					"0"
-- 				},
-- 				texture1 = {
-- 					"body", 
-- 					"1"
-- 				},
-- 				texture2 = {
-- 					"nuclei", 
-- 					"2"
-- 				},
-- 				texture3 = {
-- 					"nuclei", 
-- 					"10"
-- 				},texture4 = {
-- 					"mandibles", 
-- 					"6"
-- 				}
-- 				}
-- 		}
-- 	},
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
			fontSize = 16.0,
			fontName = "fontPrueba",
			textColor = Color.new(1.0,1.0, 0.0,1)
		}
	}
  }
}
