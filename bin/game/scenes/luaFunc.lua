local function prefabExists(path) -- comprueba que el prefab que pasas existe
    print("comprobando que existe " .. path)

    local file = io.open(path, "r") -- abre el fichero

    if file then
        print("LUAFUNC: Fichero de prefab encontrado")

        local content = file:read("*all") -- lee el ficheross
        file:close()

        return content
    else
        print("LUAFUNC: Fichero de prefab no encontrado")

        return nil
    end
end

local function loadPrefab (path) -- le pasas el path del prefab que quieras cargar

    if not prefabExists(path) then -- primero comprueba si existe, si no devuelve vacio
        return {}
    end

    local lines = {}

    for line in io.lines(path) do
        lines[#lines + 1] = line
    end

    return lines
end

local p = ...
print(p)

local lines = loadPrefab(p)

-- debug
for k,v in pairs(lines) do
  print('line[' .. k .. ']', v)
end

return {
    cube = {
        ddol = false,
        components = {
            Transform = {
                
            }
        }
    }
}