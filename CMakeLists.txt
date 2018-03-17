cmake_minimum_required(VERSION 3.0.0)
# cmake version 3.0.0 is needed for block comments, 2.8.11 was the previous
# min version

# bad but needed for OpenMP
if(APPLE)
	# use clang from homebrew for OpenMP support
	set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
	set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
endif(APPLE)
project(gs)

# http://www.cmake.org/Wiki/CMake_Useful_Variables

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# this assumes you are running cmake from a build directory in the source directory
# cmake .. -G "Visual Studio 15 2017 Win64"
if(MSVC)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
	ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
	
	# /Oi enable intrinsic functions
	# /GR- disable RTTI
	SET(CMAKE_C_FLAGS "/openmp /arch:AVX /MP ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "/openmp /MP /std:c++latest /Oi /GR- /arch:AVX ${CMAKE_CXX_FLAGS}")
	
	find_package(OpenGL REQUIRED)
	
	set(VC_PREFIX "C:\\vclibs\\vclibs")
	
	set(LIBS ${LIBS} kernel32.lib)
	set(SDL2_LIBRARIES
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2main.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2maind.lib")
	set(SDL2_LIBRARIES ${SDL2_LIBRARIES}
		optimized "C:\\vclibs\\vclibs\\lib\\SDL2.lib"
		debug "C:\\vclibs\\vclibs\\lib\\SDL2d.lib")
	set(GLEW_LIBRARIES
		optimized "C:\\vclibs\\vclibs\\lib\\glew32.lib"
		debug "C:\\vclibs\\vclibs\\lib\\glew32d.lib")
	
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\libboost_program_options-vc141-mt-x64-1_66.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libboost_program_options-vc141-mt-gd-x64-1_66.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\libboost_system-vc141-mt-x64-1_66.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libboost_system-vc141-mt-gd-x64-1_66.lib")
	set(BOOST_LIBS ${BOOST_LIBS}
		optimized "C:\\vclibs\\vclibs\\lib\\libboost_filesystem-vc141-mt-x64-1_66.lib"
		debug "C:\\vclibs\\vclibs\\lib\\libboost_filesystem-vc141-mt-gd-x64-1_66.lib")
	
	set(SDL_LIBS
		${LIBS}
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	set(QT_DIR "C:\\Qt\\Qt5.10.1\\5.10.1\\msvc2017_64")
	
	#find_package(Qt5Core)
	#find_package(Qt5Widgets)
	#find_package(Qt5OpenGL)
	
	#set(QT_LIBS
	#	${LIBS}
	#	Qt5::Core Qt5::Widgets Qt5::OpenGL
	#	${GLEW_LIBRARIES}
	#	${OPENGL_LIBRARIES}
	#	)
	
	include_directories("C:\\vclibs\\vclibs\\include")
endif(MSVC)

if(MINGW OR MSYS)
	set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")
	set(CMAKE_INCLUDE_PATH "c:/msys64/mingw64/include")
	set(CMAKE_LIBRARY_PATH "c:/msys64/mingw64/lib")
	
	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES GL)
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})
	
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(QT_LIBS
		Qt5::Core Qt5::Widgets Qt5::OpenGL
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
	
	include_directories("c:/msys64/mingw64/include/eigen3")
endif(MINGW OR MSYS)

if(APPLE)
	set(CMAKE_PREFIX_PATH "/usr/local")
	set(CMAKE_INCLUDE_PATH "/usr/local/include")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib")
	
	set(CMAKE_C_COMPILER "/usr/local/opt/llvm/bin/clang")
	set(CMAKE_CXX_COMPILER "/usr/local/opt/llvm/bin/clang++")
	include_directories("/usr/local/opt/llvm/include")
	
	find_package(OpenGL REQUIRED)
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS
		-L/usr/local/lib
		-L/usr/local/opt/llvm/lib
		${LIBS}
		${Boost_LIBRARIES}
	)
	
	INCLUDE(FindPkgConfig)
	PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
	include_directories(${SDL2_INCLUDE_DIR})
	set(SDL_LIBS
		${SDL2_LIBRARIES}
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)

	# If there are problems with qt5 on Apple run 'which qmake' and make sure
	# it is the correct one. Usually you have to run 'brew link qt5 --force'.
	# Anaconda causes problems too since it has qt4 and other libs.
	set(QT_DIR "/usr/local/opt/qt")
	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(QT_LIBS
		-L/usr/local/opt/qt/lib Qt5::Core Qt5::Widgets Qt5::OpenGL
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
	
	include_directories("/usr/local/opt/qt/include")
	
	include_directories("/usr/local/include")
	include_directories("/usr/local/include/eigen3")
endif(APPLE)

# this probably only works on openSUSE Tumbleweed
if(UNIX AND NOT APPLE)
	set(CMAKE_PREFIX_PATH "/usr")
	set(CMAKE_INCLUDE_PATH "/usr/include")
	set(CMAKE_LIBRARY_PATH "/usr/lib")

	find_package(OpenGL REQUIRED)
	find_library(OPENGL_LIBRARY NAMES GL)
	find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
	include_directories(${Boost_INCLUDE_DIRS})
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

	set(BOOST_LIBS ${Boost_LIBRARIES})
	set(LIBS ${LIBS} ${Boost_LIBRARIES})

	find_package(Qt5Core)
	find_package(Qt5Widgets)
	find_package(Qt5OpenGL)
	
	set(QT_LIBS
		Qt5::Core Qt5::Widgets Qt5::OpenGL
		${GLEW_LIBRARIES}
		${OPENGL_LIBRARIES}
		)
 
	include_directories("/usr/include")
	include_directories("/usr/include/eigen3")

endif(UNIX AND NOT APPLE)

include_directories("../common-cpp")

if(NOT MSVC)
	# --------------------------------------------------------------------------
	# WARNING: all warnings are disabled for GCC
	# --------------------------------------------------------------------------
	
	# better optimizations
	# -O3 still IEEE754 compliant
	# -Ofast not IEEE754 compliant, faster than -O3
	# -ffast-math not needed with -Ofast
	# -ftree-vectorize not sure how well this does
	# -mavx
	# -mavx2
	set(CMAKE_C_FLAGS_RELEASE "-O3 -fopenmp")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -w")
	
	# force some better debugging
	# this should be gcc
	if(NOT APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-ggdb3 -Og")
		set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -Og -w")
	endif(NOT APPLE)
	# and this should be clang
	if(APPLE)
		set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
		set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	endif(APPLE)

	# enable C11/C++14 for gcc and clang
	set(EXTRA_CXX_FLAGS "-std=c++14 -Wno-deprecated-declarations -pipe")
	set(EXTRA_C_FLAGS "-std=c11 -Wno-deprecated-declarations -pipe")
	
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
	
	# pipe keeps compilation in memory
	set(CMAKE_LD_FLAGS "-pipe")
endif(NOT MSVC)

set(MAIN_SOURCE_QT
	main_qt.cpp
	qt/qt_window.hpp
	qt/qt_window.cpp
	qt/opengl_window.hpp
	qt/opengl_window.cpp
)
set(SOURCE_COMMON
	../common-cpp/fox/counter.hpp
	../common-cpp/fox/counter.cpp
	)
set(SOURCE_GFX
	gfx/gfx.hpp
	gfx/gfx.cpp
	gfx/gfx_factory.hpp
	gfx/gfx_factory.cpp
	gfx/gfx_opengl.hpp
	gfx/gfx_opengl.cpp
	../common-cpp/fox/gfx/eigen_opengl.hpp
	../common-cpp/fox/gfx/eigen_opengl.cpp
	../common-cpp/fox/obj_model_loader.h
	../common-cpp/fox/obj_model_loader.c
	)

set(SOURCE_PHY
	phy/physics.hpp
	phy/physics.cpp
	)

set(MAIN_SOURCE_SDL
	main_sdl.cpp
	sdl/sdl_window.hpp
	sdl/sdl_window.cpp
)

# static libraries don't work so well on Apple for some reason
if(APPLE)
	set(LIBRARY_TYPE SHARED)
else(APPLE)
	set(LIBRARY_TYPE STATIC)
endif(APPLE)

add_executable(${PROJECT_NAME}_sdl
	${MAIN_SOURCE_SDL}
	${SOURCE_GFX}
	${SOURCE_COMMON}
	${SOURCE_PHY})
target_link_libraries(${PROJECT_NAME}_sdl ${LIBS} ${SDL_LIBS})

# Instruct CMake to run Qt moc automatically when needed.
#set(CMAKE_AUTOMOC ON)
#add_executable(${PROJECT_NAME}_qt
#	${MAIN_SOURCE_QT}
#	${SOURCE_GFX}
#	${SOURCE_COMMON}
#	${SOURCE_PHY})
#target_link_libraries(${PROJECT_NAME}_qt ${LIBS} ${QT_LIBS})
#set(CMAKE_AUTOMOC OFF)


MESSAGE( STATUS "MINGW: " ${MINGW} )
MESSAGE( STATUS "MSYS: " ${MSYS} )
MESSAGE( STATUS "MSVC: " ${MSVC} )
MESSAGE( STATUS "APPLE: " ${APPLE} )
MESSAGE( STATUS "INCLUDE_DIRECTORIES: " ${INCLUDE_DIRECTORIES} )
MESSAGE( STATUS "CMAKE_C_FLAGS: " ${CMAKE_C_FLAGS} )
MESSAGE( STATUS "CMAKE_C_FLAGS_DEBUG: " ${CMAKE_C_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_C_FLAGS_RELEASE: " ${CMAKE_C_FLAGS_RELEASE} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_DEBUG: " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE( STATUS "CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE} )
#get_target_property(QtCore_location Qt5::Core LOCATION)
#MESSAGE( STATUS "QtCore_location: " ${QtCore_location})
#get_target_property(QtWdigets_location Qt5::Widgets LOCATION)
#MESSAGE( STATUS "QtWdigets_location: " ${QtWdigets_location})
