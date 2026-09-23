add_subdirectory(ANGELSCRIPT/sdk/angelscript/projects/cmake)
file(GLOB ANGELSCRIPT_ADDONS
	"ANGELSCRIPT/sdk/add_on/scriptstdstring/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scriptstdstring/*.h"
	"ANGELSCRIPT/sdk/add_on/scriptbuilder/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scriptbuilder/*.h"
	"ANGELSCRIPT/sdk/add_on/weakref/*.cpp"
	"ANGELSCRIPT/sdk/add_on/weakref/*.h"
	"ANGELSCRIPT/sdk/add_on/scripthandle/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scripthandle/*.h"
	"ANGELSCRIPT/sdk/add_on/scriptmath/*.cpp"
	"ANGELSCRIPT/sdk/add_on/scriptmath/*.h"
)
add_library(angelscript_addons STATIC ${ANGELSCRIPT_ADDONS})
target_link_libraries(angelscript_addons PRIVATE angelscript)
target_include_directories(angelscript_addons PUBLIC
	"ANGELSCRIPT/sdk/add_on"
)