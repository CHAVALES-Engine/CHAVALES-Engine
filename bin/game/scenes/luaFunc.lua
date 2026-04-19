local function prefabExists(path) -- comprueba que el prefab que pasas existe
    print("comprobando que existe " .. path)

    if true then
        print("como pares........")
    end

    print("no he parado")

    local file = assert(io.open("prueba.txt", "r"), "No se pudo abrir el archivo") -- abre el fichero

    print("comprobando que existe 2")

    if file then
        local content = file:read("*all") -- lee el fichero
        print(content)
        file:close()
    else
        print("LUAFUNC: Fichero de prefab no encontrado")
    end
end

local function loadPrefab (path) -- le pasas el path del prefab que quieras cargar
    print("he entrado")

    if not prefabExists(path) then return {} end -- primero comprueba si existe, si no devuelve vacio

    print("estoy aqui")

    local lines = {}
    for line in io.lines(path) do
        lines[#lines + 1] = line
    end

    return lines
end

local p = ...
print(p)
loadPrefab(p)