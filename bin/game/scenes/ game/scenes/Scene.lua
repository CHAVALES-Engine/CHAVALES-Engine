-- scene1.lua
-- Escena principal de ejemplo
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
   	},
   },
   -- ENTIDAD 2
   entidad2 = {
   	components = {
   		Transform = {
   			position = Vector3.new(0,1,0),
   			rotation = Quaternion.new(),
   			scale = Vector3.new(0.5),
   		},
   	ModelRenderer = {
              ["model folder"] = "cube.obj",
              materialPath = "material.tres",
              castShadows = true,
              receiveShadows = true        
   	    },
   	},
   },
}
