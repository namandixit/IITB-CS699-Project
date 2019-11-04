package.path = "data/scripts/?.lua;" .. package.path

local command = require "command/command"
local lex = require "lex"
require "lib/table"

Loop = {
   Init = function ()
      Game.Line = ""
      Game.Messages = {}
      Game.Text = {""}
      Game.Here_Doc = false
      Game.Parse_Complete = false
      Game.Parse = {{["args"] = ""}}
   end,

   Loop = function (last_frame_time, events)
      local messages = Game.Messages

      do -- Process events and convert them to messages
         for _, event in ipairs(events) do
            if event.Device == "Keyboard" then
               messages[#messages + 1] = {Type = "Key", Key = event.Key, State = event.State}
            elseif event.Device == "Text" then
               if event.Type == "Text" then
                  messages[#messages + 1] = {Type = "Text", Text = event.Text}
               elseif event.Type == "Control" then
                  messages[#messages + 1] = {Type = "Text", Control = event.Control}
               end
            end
         end
      end

      local y_min, y_max = Assets.Fonts.Mono.YMin, Assets.Fonts.Mono.YMax
      local y_dim = y_max - y_min
      local newline = false

      for _, msg in ipairs(messages) do -- Process the text input and convert it into line input
         if msg.Type == "Text" then
            if msg.Text ~= nil then
               Game.Line = Game.Line .. msg.Text
            else
               if msg.Control == "Enter" then
                  newline = true
               elseif msg.Control == "Backspace" then
                  Game.Line = string.sub(Game.Line, 1, string.len(Game.Line) - 1)
               end
            end
         end
      end

      if newline == true then
         if Game.Here_Doc == true then -- Here doc entry
            local index = 0
            for i, p in ipairs(Game.Parse) do
               if p.heredoc_todo then
                  index = i
                  break
               end
            end

            local str_index = #(Game.Parse[index].heredoc_str)
            local maybe_marker = Game.Parse[index].heredoc_str[str_index] .. Game.Line
            if maybe_marker == Game.Parse[index].heredoc_marker and newline then
               Game.Parse[index].heredoc_todo = false
               Game.Here_Doc = false -- We make it true again below if need be
            else
               Game.Parse[index].heredoc_str[str_index] =  Game.Parse[index].heredoc_str[str_index] .. Game.Line
               if newline then
                  Game.Parse[index].heredoc_str[str_index + 1] = ""
               end
            end

            for i, p in ipairs(Game.Parse) do
               if p.heredoc_todo then
                  Game.Here_Doc = true
                  break
               end
            end
         else -- Parsing
            local parse = Game.Parse
            local pos = 1
            local token, pos = lex.NextToken(Game.Line, pos)
            while token ~= "" do
               if token == "&" then
                  parse[#parse].background = true
               elseif token == "&&" then
                  parse[#parse + 1] = {["args"] = ""}
                  parse[#parse].sequence = true
               elseif token == ">" then
                  parse[#parse].output = lex.NextToken()
                  parse[#parse].output_append = false
               elseif token == ">>" then
                  parse[#parse].output, pos = lex.NextToken(Game.Line, pos)
                  parse[#parse].output_append = true
               elseif token == "<" then
                  parse[#parse].input, pos = lex.NextToken(Game.Line, pos)
               elseif token == "<<" then
                  parse[#parse].heredoc = true
                  parse[#parse].heredoc_todo = true
                  parse[#parse].heredoc_str = {""}
                  parse[#parse].heredoc_marker, pos = lex.NextToken(Game.Line, pos)
                  Game.Here_Doc = true
               else
                  parse[#parse].args = parse[#parse].args .. " " .. token
               end

               token, pos = lex.NextToken(Game.Line, pos)
            end
            Game.Parse = parse
            Game.Parse_Complete = true
         end
      end

      if Game.Parse_Complete == true and Game.Here_Doc == false then

         Game.Parse = {{["args"] = ""}}
         Game.Parse_Complete = false
      end

      do -- Game.Text
         Game.Text[#Game.Text] = Game.Line
         if newline then
            Game.Text[#Game.Text + 1] = ""
         end

         if #Game.Text > math.floor(2/y_dim) then
            local diff = #Game.Text - math.floor(2/y_dim)
            for i = diff, #Game.Text do
               Game.Text[i - diff] = Game.Text[i]
            end
            for i =  math.floor(2/y_dim) + 1, #Game.Text do
               Game.Text[i] = nil
            end
         end
      end


      do -- Convert Line input into renderable text and render it
         local render_text = {}

         local line_extra = 0
         for line_num, line in ipairs(Game.Text) do
            local word = ""
            local x = 0

            local function chars(str)
               local strc = {}
               for i = 1, #str do
                  table.insert(strc, string.sub(str, i, i))
               end
               return strc
            end

            local line_char = chars(line)

            render_text[#render_text + 1] = ""

            for i = 1, #line do
               local c = line_char[i]

               if c == " " then
                  if word == "" then
                     local x_inc = Engine.Functions.RenderGetTextDimensions(Assets.Fonts.Mono,
                                                                            " ")
                     x = x + x_inc
                     if x >= 1 then
                        render_text[#render_text + 1] = " "
                        x = 0
                     else
                        render_text[#render_text] = render_text[#render_text] .. " "
                     end
                  else
                     local x_inc = Engine.Functions.RenderGetTextDimensions(Assets.Fonts.Mono,
                                                                            word)
                     if x + x_inc >= 1 then
                        x = 0
                        render_text[#render_text + 1] = word
                     else
                        render_text[#render_text] = render_text[#render_text] .. word
                     end

                     x = x + x_inc

                     local x_inc = Engine.Functions.RenderGetTextDimensions(Assets.Fonts.Mono,
                                                                            " ")
                     if x + x_inc >= 1 then
                        render_text[#render_text + 1] = ""
                        x = 0
                     else
                        render_text[#render_text] = render_text[#render_text] .. " "
                     end
                     x = x + x_inc

                     word  = ""
                  end
               else
                  word = word .. c
               end
            end

            if word ~= "" then
               local x_inc = Engine.Functions.RenderGetTextDimensions(Assets.Fonts.Mono, word)
               if x + x_inc >= 1 then
                  x = 0
                  render_text[#render_text + 1] = word
               else
                  render_text[#render_text] = render_text[#render_text] .. word
               end
               x = x + x_inc
            end
         end

         local render_text_begin = 1
         local render_upper_bound = math.floor(2/y_dim) - 1
         if #render_text > render_upper_bound then
            local diff = #render_text - render_upper_bound
            render_text_begin = diff
         end

         for i = render_text_begin, #render_text do
            Engine.Functions.RenderText(Assets.Fonts.Mono, render_text[i],
                                        Vector(0, 1 - (i - render_text_begin + 1) * y_dim, 0),
                                        Color(0.11, 1, 0.09, 1))
         end
      end

      do -- Finalize
         if newline then
            Game.Line = ""
         end

         Game.Messages = {}
      end
   end,
}
