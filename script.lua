io.write(string.format("[Lua] Hello from %s\n", _VERSION))
io.write("[Lua] Calling howdy() ...\n")
local value = howdy("First", "Second", 112233)
io.write(string.format("[Lua] howdy() returned: %s\n", tostring(value)))

function start(arg1,arg2)
	io.write(string.format("[Lua] start function called with args: %d, %d\n", arg1,arg2))
	--set global
	io.write(string.format("[Lua] Setting global number 1 to the value of 2\n"))
	setglobal(1,2)
	--check global
	io.write(string.format("[Lua] GetGlobal(%d): %d\n", 1,getglobal(1)))
	--callback?
	
	return LUA_OK
end