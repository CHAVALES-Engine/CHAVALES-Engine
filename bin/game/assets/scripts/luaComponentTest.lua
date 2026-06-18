local velocity = 0
local device = 0
local moveCamera = true

local _transform = nil
local _esfera = nil
local _camera = nila
local girl = nil
local trans_girl = nil

function init(properties)
    -- Equivalente a: setProperty(p, "device", device)
    --                getProperty<int>(p, "velocity")
    velocity = properties.velocity or 0
    device = properties.device or 0
    return true
end

function awake()
    Input:addEventToAction("left", input.KEY_A, device)
    Input:addEventToAction("left", input.BUTTON_GP_LEFT, device)
    Input:addEventToAction("right", input.KEY_D, device)
    Input:addEventToAction("right", input.BUTTON_GP_RIGHT, device)
    Input:addEventToAction("front", input.KEY_W, device)
    Input:addEventToAction("front", input.BUTTON_GP_UP, device)
    Input:addEventToAction("back", input.KEY_S, device)
    Input:addEventToAction("back", input.BUTTON_GP_DOWN, device)

    Input:addEventToAction("lock_h", input.GAMEPAD_AXIS_RIGHT_X, device)
    Input:addEventToAction("lock_h", input.MOUSE_AXIS_REL_X, device)
    Input:addEventToAction("lock_v", input.GAMEPAD_AXIS_RIGHT_Y, device)
    Input:addEventToAction("lock_v", input.MOUSE_AXIS_REL_Y, device)

    Input:isJustPressed(input.KEY_R)
    Input:getAxis(input.MOUSE_AXIS_X)
end

function ready()
    _transform = entity:getComponent("Transform")

    Input:setRelativeMouseMode(false)

    -- Esfera
    _esfera = entity:getScene():findEntityByName("esfera")
    if not _esfera then
        Debug.error("[ComponentTest] No encontrada la esfera")
        return
    end

    local col_esfera = _esfera:getComponent("Collider")
    if not col_esfera then
        Debug.error("[ComponentTest] La esfera no tiene Collider")
        return
    end

    -- Cube
    local cube = entity:getScene():findEntityByName("cube")
    if not cube then
        Debug.error("[ComponentTest] No encontrado el cube")
        return
    end

    local col_cube = cube:getComponent("Collider")
    if not col_cube then
        Debug.error("[ComponentTest] El cube no tiene Collider")
        return
    end
end

function update(deltaTime)
    if not Input:isDeviceConnected(device) then
        return
    end

    local speed = velocity * deltaTime / 1000.0
    local mouseSensitivity = velocity / 100.0

    -- Cambio de escena
    if Input:isJustPressed(input.KEY_R) then
        engine:requestSceneChange("scene_base")
    end

    -- Raycast con KEY_K
    if Input:isJustPressed(input.KEY_K) then
        entity:getScene():findEntityByName("esfera"):getComponent("AudioSource"):call("playSound")

        local camera = entity:getScene():findEntityByName("camera"):getComponent("Camera")
        if camera then
            local mouseX = Input:getAxis(input.MOUSE_AXIS_X)
            local mouseY = Input:getAxis(input.MOUSE_AXIS_Y)
            local mousePos = Vector2.new(mouseX, mouseY)

            local rayOrigin, rayDir = camera:call("screenToWorld", mousePos, engine:getWindowWidth(), engine:getWindowHeight())

            local hit, rayInfo = engine:rayCast(rayOrigin, rayDir, 10000.0)
            if hit then
                Debug.out("HitPos: " .. tostring(rayInfo.hitPos))
                local e = engine:instantiatePrefab("prefabs/testPoint")
                e:getComponent("Transform"):call("setGlobalPosition", rayInfo.hitPos)
            else
                Debug.out("NO HIT")
            end
        end
    end

    -- Empuja la esfera
    if Input:isJustPressed(input.KEY_O) then
        local sphere = engine:getScene():findEntityByName("esfera")
        if sphere then
            local rb = sphere:getComponent("RigidBody")
            if rb then
                rb:call("AddForce", Vector3.new(0, 100, 0), "I")
            end
        end
    end

    -- Asegura el Transform cacheado
    if not _transform then
        _transform = entity:getComponent("Transform")
    end
    if not _transform then
        return
    end

    -- Movimiento WASD
    local forward = _transform:call("forward")
    local right = _transform:call("right")

    if Input:isActionPressed("front", device) then
        _transform:call("translate", forward * -speed)
    end
    if Input:isActionPressed("back", device) then
        _transform:call("translate", forward * speed)
    end
    if Input:isActionPressed("left", device) then
        _transform:call("translate", right * -speed)
    end
    if Input:isActionPressed("right", device) then
        _transform:call("translate", right * speed)
    end

    -- Toggle de control de camara
    if Input:isJustPressed(input.KEY_CTRL) then
        moveCamera = not moveCamera
    end

    if moveCamera then
        Input:setRelativeMouseMode(true)
        local mx = Input:getActionAxis("lock_h", device)
        local my = Input:getActionAxis("lock_v", device)

        if mx ~= 0 then
            _transform:call("rotateGlobal", Vector3.new(0, -mx * mouseSensitivity, 0))
        end
        if my ~= 0 then
            _transform:call("rotateLocal", Vector3.new(-my * mouseSensitivity, 0, 0))
        end
    else
        Input:setRelativeMouseMode(false)
    end
end