local resultTable = {}

local function fileExists(path) -- comprueba que el fichero que pasas existe
    if not Debug.isRelease() then
        print("[LUA] Comprobando que existe " .. path)
    end

    local file = io.open(path, "r") -- abre el fichero

    if file then
        if not Debug.isRelease() then
            print("[LUA] Fichero encontrado.")
        end
        local content = file:read("*all") -- lee el ficheross
        file:close()
        dofile(path)
        return content
    else
        if not Debug.isRelease() then
            print("[LUA] Fichero no encontrado.")
        end
        return nil
    end
end

function resultTable.loadPrefab(path) -- le pasas el path del prefab que quieras cargar

    if not fileExists(path) then -- primero comprueba si existe, si no devuelve vacio
        return {}
    end

    return dofile(path)
end

local scenePath = ...

return resultTable