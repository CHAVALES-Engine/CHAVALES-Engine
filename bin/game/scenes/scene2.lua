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
				position = Vector3.new(1,2,3),
				-- rotacion local
				rotation = Quaternion.new(),
				-- escala local
				scale = Vector3.new(1)
			},
		}
	} 
}