package.path = "data/scripts/?.lua;" .. package.path
local go = require "command/_getopt"
require "lib/table"
local fs = require "fs"
local inode = require "inode"
Game.FS = fs.new(inode)
Game.FS.pwd = Game.FS.inode


local tbl = go.parse("mv f1 f2" , "")
local path1
if parse.args[1]:sub(1, 1) == "/" then
    path1 = parse.args[1]
 else
    path1 = Game.FS:path(Game.FS.pwd) .. parse.args[1]
 end




print(table.show(tbl))
local args = tbl["args"]
local source = node.new("asd","d")
local dest = node.new("xyz" , "")
function mv(source, dest)
    if source.kind == "d" then
        local s = fs.segments(source)
        local last_s = s[#s]
        print(table.show(s))
        if dest.kind ~= "d" then
            print("ERROR: cannot move a directory to a file")
        else
            local d = fs.segments()
            local last_d = d[#d]
        end
   end

end

print(Game.FS.pwd)