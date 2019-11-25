package.path = "data/scripts/?.lua;" .. package.path

local command = require "command/command"
local lex = require "lex"
local inode = require "inode"
local fs = require "fs"
local go = require "command/_getopt"
require "lib/table"


-- local tbl = go.parse("ls -al -w g -erty -t --qwe=naman abbas ahmad --prince=kumar -- -z" , "we")
-- local tbl1 = go.parse("ls we-al " , "a")

function Contains(table, element)
    for key, value in pairs(table) do
      if value == element then
        return true
      end
    end
    return false
  end

function Compare(sys_table, user_table, order)
  if sys_table["command"] ~= user_table["command"] then
      return false
  end

  local sys_args = sys_table["args"]
  local user_args = sys_table["args"]

  if #user_args ~= #sys_args then
    return false
  else
    for i = 1, #sys_args do
        if sys_args[i] ~= user_args[i] then
            return false
        end
    end
  end

  local sys_pars = sys_table["pars"]
  local user_pars = user_table["pars"]

  if #user_pars ~= #sys_pars then
    return false
  else
    for key, value in pairs(sys_pars) do
        if Contains(user_pars , value) == false then
            return false
        end
    end

    for key, value in pairs(user_pars) do
        if Contains(sys_pars , value) == false then
            return false
        end
    end
  end
  return true
end

-- print(Compare(tbl,tbl))