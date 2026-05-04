-- Escena principal de ejemplo
scene = { -- entidades en escena
    gizmos = true,
    entities = {
        cube = loadPrefab("game/scenes/cube.lua"),
        -- ENTIDAD 1
        camera = { -- nombre de la entidad
            ddol = false, -- dont destroy on loads
            components = { -- componentes de esa entidad
                Transform = { -- componente transform
                    -- posicion local
                    position = Vector3.new(200.0, 200.0, 200.0),
                    -- rotacion local
                    rotation = Quaternion.new(0.0, 0.0, 1.0, 45.0),
                    -- escala local
                    scale = Vector3.new(1)
                },
                Camera = {
                    FOVy = 1.0,
                    nearPlane = 0.1,
                    farPlane = 10000.0,
                    ["focal length"] = 60.0
                    -- ["background color"] = Color.new(0.5,0.5,0.5,1)
                },
                ComponentTest = {
                    velocity = 100
                },
                ChangeTest = {},
                AudioListener = {},
                ScriptComponent = {
                    script = "./game/assets/scripts/luaComponentTest.lua"
                }
            }
        },
        gameManager = {
            ddol = true,
            components = {
                Transform = {
                    position = Vector3.new(0, 0, 0),
                    rotation = Quaternion.new(),
                    scale = Vector3.new(0.5)
                },
                PersistentTest = {}
            }
        },
        esfera = {
            ddol = false,
            components = {
                Transform = {
                    position = Vector3.new(0, 100, 0),
                    rotation = Quaternion.new(),
                    scale = Vector3.new(0.5)
                },
                ModelRenderer = {
                    file = "mesh/sphere.fbx",
                    ["number of textures"] = 0
                },
                RigidBody = {
                    useGravity = true,
                    isKinematic = true,
                    mass = 1.0,
                    position = Vector3.new(0, 100, 0),
                    velocity = Vector3.new(0, 40, 0),
                    damping = 0.0,
                    blockAxes = {true}
                },
                Collider = {
                    type = "CAPSULE",
                    shape = Vector3.new(50, 0, 0),
                    rotation = Vector3.new(),
                    dynamic = true,
                    trigger = false,
                    center = Vector3.new(0, 0, 0)
                },
                PhysicsMaterial = {
                    staticFriction = 0.5,
                    dynamicFriction = 0.5,
                    restitution = 0.0,
                    frictionCombine = 1,
                    bounceCombine = 1
                },
                AudioSource = {
                    soundPath = "sfx/smb_1-up.wav",
                    soundID = "vidaExtra1",
                    is3D = true,
                    loop = false,
                    isStream = false,
                    playOnReady = true,
                    soundVolume = 9.0,
                    minRadius = 150.0,
                    maxRadius = 300.0
                }
            }
        },
        ambientLight = {
            components = {
                AmbientLight = {
                    color = Color.new(0.3, 0.0, 0.0, 0.0)
                }
            }
        },
        light = {
            ddol = false,
            components = {
                Transform = {
                    position = Vector3.new(0, 0, 0),
                    rotation = Quaternion.new(),
                    scale = Vector3.new(0.5)
                },
                Light = {
                    type = 1,
                    color = Color.new(1, 1, 1, 1),
                    intensity = 1.0,
                    inner = 30.0,
                    outer = 60.0,
                    falloff = 1.0
                }
            }
        },
        CanvasUI = {
            ddol = false,
            components = {
                UITransform = {
                    position = Vector2.new(900, 10),
                    children = {"LabelUI", "ButtonUI", "ButtonUI2"}

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
                    position = Vector2.new(100, 60),
                    dimension = Vector2.new(200, 200),
                    zBuffer = 0

                },
                UIButton = {
                    textureName = "images/prueba.jpg",
                    text = "Boton Botonez",
                    opacity = 1.0,
                    fontName = "fonts/horrendo.ttf"

                }
            }
        },
        LabelUI = {
            ddol = false,
            components = {
                UITransform = {
                    position = Vector2.new(600, 60),
                    dimension = Vector2.new(200, 200),
                    zBuffer = 0

                },
                UILabel = {
                    text = "Boton Botonez ( En realidad es un UILabel)",
                    opacity = 1.0,
                    bgColor = Color.new(1.0, 0.0, 0.0, 1),
                    align = "center",
                    fontSize = 16.0,
                    fontName = "fonts/horrendo.ttf",
                    textColor = Color.new(1.0, 1.0, 0.0, 1)
                }
            }
        },
        -- auxiliar = {
        --    ddol = false,
        --     components = {
        --         Transform = {
        --             position = Vector3.new(500, 300, 300),
        --             rotation = Quaternion.new(),
        --             scale = Vector3.new(0.5)
        --         },
        --         ModelRenderer = {
        --             file = "mesh/sphere.fbx",
        --             ["number of textures"] = 0
        --         },
        --         RigidBody = {
        --             useGravity = true,
        --             isKinematic = false,
        --             mass = 1.0,
        --             position = Vector3.new(0, 300, 0),
        --             velocity = Vector3.new(0, 40, 0),
        --             damping = 0.0,
        --             blockAxes = {true}
        --         },
        --         Collider = {
        --             type = "CAPSULE",
        --             shape = Vector3.new(50, 0, 0),
        --             rotation = Vector3.new(),
        --             dynamic = true,
        --             trigger = false,
        --             center = Vector3.new(0, 0, 0)
        --         },
        --         PhysicsMaterial = {
        --             staticFriction = 0.0,
        --             dynamicFriction = 0.0,
        --             restitution = 0.8,
        --             frictionCombine = 2,
        --             bounceCombine = 2
        --         }
        --         -- ,
        --         -- AudioSource = {
        --         --     soundPath = "sfx/smb_1-up.wav",
        --         --     soundID = "vidaExtra2",
        --         --     is3D = true,
        --         --     loop = true,
        --         --     isStream = false,
        --         --     playOnReady = true,
        --         --     soundVolume = 1.0,
        --         --     minRadius = 150.0,
        --         --     maxRadius = 600.0
        --         -- }
        --     }
        -- }
    }
}
