local getopt = require "command/_getopt"
local tutorial = {}

local function step (command, instructions)
   local input = coroutine.yield(instructions)

   while getopt.compare(getopt.parse(command), getopt.parse(input)) == false do
      input = coroutine.yield()
   end
end

function tutorial.tutorial ()
   step("cd home", {
         "Directories can be navigated by using \"cd\" command.",
         "    Please change to home directory by entering:",
         "",
         "            cd home"
   })

   step("cd ..", {
         "Now, move back one directory up by entering:",
         "            cd .."
   })

   coroutine.yield({"Well Done!"})
end

return tutorial
