file(GLOB SIMDJSON_SOURCES
	"SIMDJSON/singleheader/simdjson.cpp"
	"SIMDJSON/singleheader/simdjson.h"
)
add_library(simdjson STATIC ${SIMDJSON_SOURCES})
target_include_directories(simdjson PUBLIC "SIMDJSON/singleheader")
