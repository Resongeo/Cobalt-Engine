file(GLOB RPMALLOC_SOURCES
	"RPMALLOC/rpmalloc/*.h"
	"RPMALLOC/rpmalloc/*.c"
)
add_library(rpmalloc STATIC ${RPMALLOC_SOURCES})
target_include_directories(rpmalloc PUBLIC "RPMALLOC/rpmalloc")
target_compile_options(rpmalloc PRIVATE
	$<$<C_COMPILER_ID:MSVC>:/experimental:c11atomics>
)