prefab = {
    ddol = false,
    components = {
        Transform = {
            position = Vector3.new(0,0,0),
            rotation = Vector3.new(0.0,45.0,0.0),
            scale = Vector3.new(2.0, 0.5, 2.0),
            },
        ModelRenderer = {
            file = "mesh/cube.fbx",
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
}

return prefab