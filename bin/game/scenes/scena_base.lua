-- scene1.lua
-- Escena principal de ejemplo
scene = { -- entidades en escena
   -- ENTIDAD 1
   camera = { -- nombre de la entidad
   	components = { -- componentes de esa entidad
   		Transform = { -- componente transform
   			-- posicion local
   			position = Vector3.new(1,2,3),
   			-- rotacion local
   			rotation = Quaternion.new(),
   			-- escala local
   			scale = Vector3.new(1),
   			children = {}
   		},
   	},
   },
   -- ENTIDAD 2
   cube = {
   	components = {
   		Transform = {
   			position = Vector3.new(0,1,0),
   			rotation = Quaternion.new(),
   			scale = Vector3.new(0.5),
   			children = {}
			},
		ModelRenderer = {
              ["model folder"] = "./assets/",
              ["near clip distance"] = "cube.obj",
			  ["number of textures"] = 0
   	    }
   	},
   },
}
