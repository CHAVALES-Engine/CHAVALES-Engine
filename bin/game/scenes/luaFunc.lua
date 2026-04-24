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
    print("comprobando que existe " .. path)

    local file = io.open(path, "r") -- abre el fichero

    if file then
        print("LUAFUNC: Fichero encontrado")

        local content = file:read("*all") -- lee el ficheross
        file:close()
 
        dofile(path)

        resultTable[#resultTable+1] = scene

        local entities = {}
        entities[#entities+1] = resultTable

        print("Prefab:", dump(entities))

        for k,v in pairs(entities) do
            print("holaaaaaaaaaaaaaaaaaaa")
            print('['..k..'] = ', v)
        end


        return content
    else
        print("LUAFUNC: Fichero no encontrado")
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
    
    -- local lines = {}
    
    -- for line in io.lines(path) do
    --     lines[#lines + 1] = line
    -- end
    
    -- -- table.insert(resultTable, 1, lines)
    
    -- -- debug
    -- for k,v in pairs(resultTable) do
    --     print('line[' .. k .. ']', v)
    -- end
    
    -- return lines
end

local scenePath = ...
print(scenePath)

-- local lines = loadPrefab(scenePath)

-- debug
-- for k,v in pairs(lines) do
--  print('line[' .. k .. ']', v)
-- end

return resultTable