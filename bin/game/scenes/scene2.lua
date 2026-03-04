scene = { -- entidades en escena
	-- ENTIDAD 1
	entidad1 = { -- nombre de la entidad
		components = { -- componentes de esa entidad
			Transform = { -- componente transform
				-- posicion local
				lPosX = 0.0,
				lPosY = 0.0,
				lPosZ = 0.0,
				-- rotacion local
				
				-- escala local
				lScaleX = 1.0,
				lScaleY = 1.0,
				lScaleZ = 1.0
			},
			ComponentTest = {
				health = 10,
				alive = true,
				name = "godotillo"
			}
		}
	},
	-- ENTIDAD 2
	entidad2 = { -- nombre de la entidad
		components = { -- componentes de esa entidad
			Transform = { -- componente transform
				-- posicion local
				lPosX = 10.0,
				lPosY = 10.0,
				lPosZ = 10.0,
				-- rotacion local
				
				-- escala local
				lScaleX = 1.0,
				lScaleY = 1.0,
				lScaleZ = 1.0
			}
		}
	} 
}