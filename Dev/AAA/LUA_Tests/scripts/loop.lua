print("loop.lua")

local i = 0
while true do
	sleep(1)

	i = i + 1
	if i == 20 then
		break
	end
end

finish()
