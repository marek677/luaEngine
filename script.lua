io.write(string.format("[Lua] Hello from %s\n", _VERSION))
function delayed(arg1,arg2)
	io.write(string.format("[Lua] Delayed function called with args: %d, %d\n", arg1,arg2))
end
function start(arg1,arg2)
	io.write("[Lua] Calling howdy() ...\n")
	local value = howdy("First", "Second", 112233)
	io.write(string.format("[Lua] howdy() returned: %s\n", tostring(value)))
	io.write(string.format("[Lua] start function called with args: %d, %s\n", arg1,arg2))
	--set global
	io.write(string.format("[Lua] Setting global number 1 to the value of 2\n"))
	setglobal(1,2)
	--check global
	io.write(string.format("[Lua] GetGlobal(%d): %d\n", 1,getglobal(1)))
	--callback?
	executeDelayed(0,"delayed",12,17)
	return LUA_OK
end
function timerfunc(arg)
	io.write(string.format("[Lua] timerfunc called with args: %d\n", arg))
	return LUA_OK
end