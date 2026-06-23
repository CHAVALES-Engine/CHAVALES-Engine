-- Escena principal de ejemplo
scene = { -- entidades en escena
    gizmos = true,
    entities = {
        cube = loadPrefab("game/prefabs/cube.lua"),
        -- ENTIDAD 1
        camera = { -- nombre de la entidad
            ddol = false, -- dont destroy on loads
            components = { -- componentes de esa entidad
                Transform = { -- componente transform
                    -- posicion local
                    position = Vector3.new(5, 4, 5),
                    -- rotacion local
                    rotation = Quaternion.new(0.0, 0.0, 1.0, 45.0),
                    -- escala local
                    scale = Vector3.new(1)
                },
                Camera = {
                    FOVy = 1.0,
                    nearClipDistance = 0.1,
                    farClipDistance = 10000.0,
                    focalLength = 60.0,
                    -- ["background color"] = Color.new(0.5,0.5,0.5,1)
                },
                --ChangeTest = {},
                AudioListener = {},
                ScriptComponent = {
                    script = "./game/assets/scripts/luaComponentTest.lua",
                    edit = true,
                    velocity = 100
                }
                --, ChangeTest = { scene = "scene_prueba" }
            }
        },
        gameManager = {
            ddol = true,
            components = {
                Transform = {
                    position = Vector3.new(0, 0, 0),
                    rotation = Quaternion.new(),
                    scale = Vector3.new(1.0)
                },
                PersistentTest = {}
            }
        },
        esfera = {
            ddol = false,
            components = {
                Transform = {
                    position = Vector3.new(0, 4, 0),
                    rotation = Quaternion.new(),
                    scale = Vector3.new(1.0)
                },
                ModelRenderer = {
                    file = "mesh/Sphere.fbx",
                    ["number of textures"] = 0
                },
                RigidBody = {
                    useGravity = true,
                    isKinematic = false,
                    mass = 1.0,
                    position = Vector3.new(0, 3, 0),
                    velocity = Vector3.new(0, 0, 0),
                    damping = 0.0,
                    blockAxes = {false, false, false}
                },
                Collider = {
                    type = "CAPSULE",
                    shape = Vector3.new(1, 0, 0),
                    rotation = Vector3.new(),
                    isDynamic = true,
                    isTrigger = false,
                    center = Vector3.new(0, 0, 0)
                },
                PhysicsMaterial = {
                    staticFriction = 0.5,
                    dynamicFriction = 0.9,
                    restitution = 0.7,
                    frictionCombine = 0,
                    bounceCombine = 2
                },
                AudioSource = {
                    path = "audio/sfx/smb_1-up.wav",
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
                    innerAngle = 30.0,
                    outerAngle = 60.0,
                    falloff = 1.0
                }
            }
        },
        CanvasUI = {
            ddol = false,
            components = {
                UITransform = {
                    position = Vector2.new(0, 0),
                    dimension = Vector2.new(1280, 720),
                    dLayer = 0,
                    rotation = 0.0,
                    -- position = Vector2.new(900, 10),
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
                    rotation = 0.0,
                    dLayer = 0

                },
                UIButton = {
                    textureName = "images/prueba.jpg",
                    textColor = Color.new(1.0,0.0,0.0,1.0),
                    bgColor = Color.new(0.0,0.0,0.0,1.0),
                    hoverColor = Color.new(0.0,0.0,0.0,1.0),
                    pressColor = Color.new(0.0,0.0,0.0,1.0),
                    text = "Boton Botonez",
                    opacity = 1.0,
                    fontName = "fonts/horrendo.ttf",
                    fontSize = 16.0,
                }
            }
        },
        LabelUI = {
            ddol = false,
            components = {
                UITransform = {
                    position = Vector2.new(600, 60),
                    dimension = Vector2.new(500, 200),
                    rotation = 0.0,
                    dLayer = 0

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
