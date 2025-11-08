-- inspired by NazaraEngine (https://github.com/NazaraEngine/NazaraEngine/blob/main/xmake/actions/createclass.lua

local template = {
    header = [[
// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#ifndef %GUARD%
#define %GUARD%

namespace Mcc
{
}

%INL_INCLUDE%#endif
]],

    inline = [[
// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

namespace Mcc
{
}
]],

    source = [[
// Copyright (c) 2025 devalexxx
// Distributed under the MIT License.
// https://opensource.org/licenses/MIT

#include "%TARGET%/%PATH%/%CLASS_NAME%.h"

namespace Mcc
{
}
]]
}

task("create-class")
    set_menu({
        usage = "xmake create-class [option] name",
        options = 
        {
            { nil, "cpp", "kv", true , "Enable .cpp creation (default = true)" },
            { nil, "inl", "kv", false, "Enable .inl creation (default = false)" },

            { nil, "target", "v", nil, "The target (Common|Server|Client)" },
            { nil, "path"  , "v", nil, "The path in the target (do not specify include or src)" },
            { nil, "name"  , "v", nil, "The class name" },
        }
    })

    on_run(function ()
        import("core.base.option")

        local target = option.get("target")
        if not target then
            os.raise("target is mandatory")
        end

        print(target)

        local filePath = option.get("path")
        if not filePath then
            os.raise("path is mandatory")
        end

        print(filePath)

        local className = option.get("name")
        if not className then
            os.raise("name is mandatory")
        end

        print(className)

        local files = {
            { kind = "header", path = path.join(target, "include", target, filePath, className) .. ".h" }
        }

        local inlInclude = ""
        if option.get("inl") then
            inlInclude = "#include \"" .. path.join(target, filePath, className) .. ".inl\"\n\n"
            table.insert(files, { kind = "inline", path = path.join(target, "include", target, filePath, className) .. ".inl" })
        end

        if option.get("cpp") then
            table.insert(files, { kind = "source", path = path.join(target, "src", filePath, className) .. ".cpp" })
        end

        local replacements = {
            GUARD  = "MCC_" .. target:upper() .. "_" .. filePath:gsub("[/\\]", "_"):upper() .. "_" .. className:upper() .. "_H",
            TARGET = target,
            PATH   = filePath,
            CLASS_NAME = className,
            INL_INCLUDE = inlInclude
        }

        for _, file in pairs(files) do
            local content = template[file.kind]:gsub("%%([%u_]+)%%", function(kw)
                local r = replacements[kw]
                if not r then
                    os.raise("missing replacement for " .. kw)
                end

                return r
            end)
            io.writefile(file.path, content)
        end
    end)