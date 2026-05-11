local resultTable = {}
 
function dump(o) -- hace print de la tabla de manera recursiva
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end

local function fileExists(path) -- comprueba que el fichero que pasas existe
    print("[LUAFUNC] Comprobando que existe " .. path)

    local file = io.open(path, "r") -- abre el fichero

    if file then
        print("[LUAFUNC] Fichero encontrado")
        local content = file:read("*all") -- lee el ficheross
        file:close()
        dofile(path)
        -- print("Prefab:", dump(entities))
        return content
    else
        print("[LUAFUNC] Fichero no encontrado")
        return nil
    end
end

local function loadScene (path)
    if fileExists(path) then -- primero comprueba si existe
        for line in io.lines(path) do
        lines[#lines + 1] = line
        end
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