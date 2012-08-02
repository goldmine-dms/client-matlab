if ispc
    % Windows7/Visual Studio 2010
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json\Release\ fromjson.c
    mex -ljson -g LINKFLAGS="$LINKFLAGS /NODEFAULTLIB:MSVSRT.lib /NODEFAULTLIB:LIBCMT.lib" -IC:\Users\panton\code\ -LC:\Users\panton\code\json\Release\ tojson.c 
	mex -lws2_32 -lcurl -IC:\Users\panton\code\curl\include\ -LC:\Users\panton\code\curl\lib\lib-Release\ jsonrpcpost.c
else
    % Linux/Ubuntu/GCC
    mex -ljson -g fromjson.c
    mex -ljson -lm -g tojson.c
	mex -lcurl -g jsonrpcpost.c

end

mex -g setjsonfield.c