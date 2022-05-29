cmake_minimum_required(VERSION 3.16.3)

option(PSXF_STDMEM "Use standard libc memory allocators instead of the fast custom one" OFF)
set(PSXF_GL "MODERN" CACHE STRING "Which version of OpenGL to use: 'MODERN' for OpenGL Core 3.2, 'LEGACY' for OpenGL 2.1, and 'ES' for OpenGL ES 2.0")
option(PSXF_NETWORK "Enable networking" OFF)

project(funkin LANGUAGES C)

add_executable(funkin
	"src/main.c"
	"src/main.h"
	"src/mem.h"
	"src/mutil.c"
	"src/mutil.h"
	"src/random.c"
	"src/random.h"
	"src/archive.c"
	"src/archive.h"
	"src/font.c"
	"src/font.h"
	"src/trans.c"
	"src/trans.h"
	"src/loadscr.c"
	"src/loadscr.h"
	"src/menu.c"
	"src/menu.h"
	"src/stage.c"
	"src/stage.h"
	"src/pc/psx.c"
	"src/psx.h"
	"src/pc/io.c"
	"src/io.h"
	"src/pc/gfx.c"
	"src/gfx.h"
	"src/pc/audio.c"
	"src/audio.h"
	"src/pc/pad.c"
	"src/pad.h"
	"src/pc/timer.c"
	"src/timer.h"
	"src/pc/movie.c"
	"src/movie.h"
	"src/pc/dr_mp3.h"
	"src/pc/miniaudio.h"
    "src/stage/beach.c"
    "src/stage/park.c" 
    "src/stage/week1.c" 
    "src/stage/week2.c" 
    "src/stage/week3.c" 
    "src/stage/school.c" 
	"src/animation.c"
	"src/animation.h"
	"src/character.c"
	"src/character.h"
	"src/character/bf.c"
    "src/character/bfmenu.c" 
    "src/character/steven.c" 
    "src/character/spinel.c" 
    "src/character/mordecai.c" 
    "src/character/spook.c" 
    "src/character/pico.c" 
    "src/character/bfc.c" 
    "src/character/finn.c" 
	"src/character/jake.c" 
	"src/character/bubble.c" 
	"src/object.c"
	"src/object.h"
	"src/object/combo.c"
	"src/object/combo.h"
	"src/object/splash.c"
	"src/object/splash.h"
)

if (PSXF_GL STREQUAL "ES")
	# Enable OpenGL ES 2.0 code
	target_compile_definitions(funkin PRIVATE PSXF_GL=PSXF_GL_ES)

	# Link OpenGL ES 2.0 library
	target_link_libraries(funkin PRIVATE GLESv2)
	# Raspberry Pi Broadcom GLESv2 driver.
	# Would be nice to test someday, but it doesn't work during X sessions,
	# and this port cannot be ran from the terminal (yet).
	# For now, we'll just have to settle with the open-source Mesa driver.
	#target_link_libraries(funkin PRIVATE "/opt/vc/lib/libbrcmGLESv2.so")
else()
	if(PSXF_GL STREQUAL "LEGACY")
		target_compile_definitions(funkin PRIVATE PSXF_GL=PSXF_GL_LEGACY)
	elseif(PSXF_GL STREQUAL "MODERN")
		target_compile_definitions(funkin PRIVATE PSXF_GL=PSXF_GL_MODERN)
	else()
		message(FATAL_ERROR "Invalid PSXF_GL specified")
	endif()

	# Include the 'glad' OpenGL loader
	target_sources(funkin PRIVATE
		"src/pc/glad/glad.c"
		"src/pc/glad/glad.h"
	)
endif()

# Make it so the code knows it's targetting PC
target_compile_definitions(funkin PRIVATE PSXF_PC)

# Use standard memory allocators if requested to
if(PSXF_STDMEM)
	target_compile_definitions(funkin PRIVATE PSXF_STDMEM)
endif()

# Use networking if requested to
if(PSXF_NETWORK)
	target_compile_definitions(funkin PRIVATE PSXF_NETWORK)
	target_sources(funkin PRIVATE
		"src/pc/network.c"
		"src/network.h"
	)
endif()

# Link GLFW3
find_package(glfw3 REQUIRED)
target_link_libraries(funkin PRIVATE glfw)

# miniaudio requires libm, libpthread, and libdl
find_library(LIBM m)
if(LIBM)
	target_link_libraries(funkin PRIVATE ${LIBM})
endif()

find_library(LIBPTHREAD pthread)
if(LIBPTHREAD)
	target_link_libraries(funkin PRIVATE ${LIBPTHREAD})
endif()

target_link_libraries(funkin PRIVATE ${CMAKE_DL_LIBS})
