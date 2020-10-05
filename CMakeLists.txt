cmake_minimum_required(VERSION 3.6)
# cmake version 3.6 is required for Visual Studio startup project

project(grav_sim)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 15 2017 Win64"
# cmake .. -G "Visual Studio 16 2019" -A x64
if(MSVC)
	set(CMAKE_PREFIX_PATH "C:\\usr\\vclibs")
	set(CMAKE_INCLUDE_PATH "C:\\usr\\vclibs\\include")
	set(CMAKE_LIBRARY_PATH "C:\\usr\\vclibs\\lib")

	# /Oi enable intrinsic functions
	SET(CMAKE_C_FLAGS "/openmp /arch:AVX2 /MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/openmp /MP /std:c++17 /Oi /arch:AVX2 ${CMAKE_CXX_FLAGS}")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES OpenGL32 opengl32)
	
	set(Boost_USE_STATIC_LIBS OFF )
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)
	
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	set(BOOST_LIBS ${Boost_LIBRARIES})
	
	set(VC_LIB_DIR "C:\\usr\\vclibs\\lib\\")
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL2_LIBRARIES
		optimized "${VC_LIB_DIR}SDL2main.lib"
		debug "${VC_LIB_DIR}SDL2maind.lib")
	set(SDL2_LIBRARIES ${SDL2_LIBRARIES}
		optimized "${VC_LIB_DIR}SDL2.lib"
		debug "${VC_LIB_DIR}SDL2d.lib")
	set(GLEW_LIBRARIES
		optimized "${VC_LIB_DIR}glew32.lib"
		debug "${VC_LIB_DIR}glew32d.lib")
	
	set(SDL_LIBS
		${LIBS}
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	#find_package(Qt5Core)
	#find_package(Qt5Widgets)
	#find_package(Qt5OpenGL)
	
	#set(QT_LIBS
	#	${LIBS}
	#	Qt5::Core Qt5::Widgets Qt5::OpenGL
	#	${GLEW_LIBRARIES}
	#	${OPENGL_LIBRARIES}
	#	)
	
	include_directories("C:\\usr\\vclibs\\include")
endif(MSVC)

if(MINGW OR MSYS OR UNIX)
	if(UNIX)
        set(OpenGL_GL_PREFERENCE GLVND)
		find_package(OpenGL REQUIRED)
		find_library(OPENGL_LIBRARY NAMES OpenGL)
		set(LIBS ${LIBS} -lpthread)
	elseif(MINGW OR MSYS)
		find_package(OpenGL REQUIRED)
		find_library(OPENGL_LIBRARY NAMES OpenGL32 opengl32)
		set(LIBS ${LIBS} -lmingw32)
		set(QT_DIR "c:/msys64/mingw64")
	endif(UNIX)

	#find_package(Qt5Core)
	#find_package(Qt5Widgets)
	#find_package(Qt5Gui)
	#find_package(Qt5OpenGL)

	#set(QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL)
	
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	set(BOOST_LIBS ${Boost_LIBRARIES})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})

	set(SDL_LIBS
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
	if(MINGW OR MSYS)
		set(SDL_LIBS -lglew32 ${SDL_LIBS})
	endif()

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})
	
	if(UNIX)
		include_directories("/usr/include")
		include_directories("/usr/include/eigen3")
	elseif(MINGW OR MSYS)
		include_directories("c:/msys64/mingw64/include")
		include_directories("c:/msys64/mingw64/include/eigen3")
	endif(UNIX)
	
	set(CMAKE_C_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	
	set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Og")
	set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Og -w")

	set(EXTRA_CXX_FLAGS "-std=c++17 -fopenmp -mavx2 -Wno-deprecated-declarations -pipe -frtti")
	set(EXTRA_C_FLAGS "-std=c17 -fopenmp -mavx2 -Wno-deprecated-declarations -pipe")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
	
	set(CMAKE_LD_FLAGS "-pipe")

	include_directories("common-cpp")
endif(MINGW OR MSYS OR UNIX)

set(MAIN_SOURCE_QT
	main_qt.cpp
	qt/qt_window.hpp
	qt/qt_window.cpp
	qt/opengl_window.hpp
	qt/opengl_window.cpp
)
set(SOURCE_COMMON
	common-cpp/fox/counter.hpp
	common-cpp/fox/counter.cpp
	)
set(SOURCE_GFX
	gfx/gfx.hpp
	gfx/gfx.cpp
	gfx/gfx_factory.hpp
	gfx/gfx_factory.cpp
	gfx/gfx_opengl.hpp
	gfx/gfx_opengl.cpp
	common-cpp/fox/gfx/eigen_opengl.hpp
	common-cpp/fox/gfx/eigen_opengl.cpp
	common-cpp/fox/obj_model_loader.h
	common-cpp/fox/obj_model_loader.c
	)

set(SOURCE_PHY
	phy/physics.hpp
	phy/physics.cpp
	phy/physics_simd_test.hpp
	phy/physics_simd_test.cpp
	)

set(MAIN_SOURCE_SDL
	main_sdl.cpp
	sdl/sdl_window.hpp
	sdl/sdl_window.cpp
)


set(LIBRARY_TYPE STATIC)

add_executable(${PROJECT_NAME}
	${MAIN_SOURCE_SDL}
	${SOURCE_GFX}
	${SOURCE_COMMON}
	${SOURCE_PHY})
target_link_libraries(${PROJECT_NAME} ${LIBS} ${SDL_LIBS})

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif(MSVC)

# Instruct CMake to run Qt moc automatically when needed.
#set(CMAKE_AUTOMOC ON)
#add_executable(${PROJECT_NAME}_qt
#	${MAIN_SOURCE_QT}
#	${SOURCE_GFX}
#	${SOURCE_COMMON}
#	${SOURCE_PHY})
#target_link_libraries(${PROJECT_NAME}_qt ${LIBS} ${QT_LIBS})
#set(CMAKE_AUTOMOC OFF)
