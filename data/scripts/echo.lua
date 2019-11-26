package.path = "data/scripts/?.lua;" .. package.path
local go = require "command/_getopt"
require "lib/table"

local tbl = go.parse("echo sdsdf ljssdf         d                     f" , "")
local args = tbl["args"]
local text = {}
function echo(tbl)
    if tbl["command"] == "echo" then
        for i = 1, #args  do
            table.insert(text , args[i])
        end
        print(table.show(text))
        print(table.concat(text , " "))
        local ret_text = table.concat(text , " ")
        return {ret_text}
    end
end

print(echo(tbl))
