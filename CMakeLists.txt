cmake_minimum_required(VERSION 3.14)
project(Allegro5Droid)

set(CMAKE_CXX_STANDARD 14)

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

set(CONFIGURATION_NAME "Debug")

if (DEBUG_LEVEL)
    add_definitions(-DDEBUG_LEVEL=${DEBUG_LEVEL})
endif ()

include_directories(
        .
        hdr
        hdr/chipmunk
        hdr/script
)

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. -L/usr/local/lib ")

# Define the CXX sources
set ( CXX_SRCS
        src/script/as_callfunc_arm_gcc.S
        src/script/as_callfunc_arm_vita.S
        src/script/as_callfunc_arm_xcode.S
        src/script/as_atomic.cpp
        src/script/as_builder.cpp
        src/script/as_bytecode.cpp
        src/script/as_callfunc_arm.cpp
        src/script/as_callfunc_mips.cpp
        src/script/as_callfunc_ppc_64.cpp
        src/script/as_callfunc_ppc.cpp
        src/script/as_callfunc_sh4.cpp
        src/script/as_callfunc_x64_gcc.cpp
        src/script/as_callfunc_x64_mingw.cpp
        src/script/as_callfunc_x64_msvc.cpp
        src/script/as_callfunc_x86.cpp
        src/script/as_callfunc_xenon.cpp
        src/script/as_callfunc.cpp
        src/script/as_compiler.cpp
        src/script/as_configgroup.cpp
        src/script/as_context.cpp
        src/script/as_datatype.cpp
        src/script/as_gc.cpp
        src/script/as_generic.cpp
        src/script/as_globalproperty.cpp
        src/script/as_memory.cpp
        src/script/as_module.cpp
        src/script/as_objecttype.cpp
        src/script/as_outputbuffer.cpp
        src/script/as_parser.cpp
        src/script/as_restore.cpp
        src/script/as_scriptcode.cpp
        src/script/as_scriptengine.cpp
        src/script/as_scriptfunction.cpp
        src/script/as_scriptnode.cpp
        src/script/as_scriptobject.cpp
        src/script/as_string_util.cpp
        src/script/as_string.cpp
        src/script/as_thread.cpp
        src/script/as_tokenizer.cpp
        src/script/as_typeinfo.cpp
        src/script/as_variablescope.cpp
        src/script/as_callfunc_arm_msvc.asm
        src/script/as_callfunc_x64_msvc_asm.asm
        src/script/scriptarray.cpp
        src/script/scriptbuilder.cpp
        src/script/scriptstdstring.cpp
        src/script/scriptstdstring_utils.cpp

        src/system/sys_main.cpp
        src/system/sys_init.cpp
        hdr/system/sys_init.h
        src/system/sys_gameFrameUpdate.cpp
        hdr/system/sys_gameFrameUpdate.h
        src/system/sys_gameFrameRender.cpp
        hdr/system/sys_gameFrameRender.h
        src/system/sys_shutdown.cpp
        hdr/system/sys_shutdown.h
        src/system/sys_gameEvents.cpp
        hdr/system/sys_gameEvents.h
        src/system/sys_configFile.cpp
        hdr/system/sys_configFile.h
        src/system/sys_font.cpp
        hdr/system/sys_font.h
        src/system/sys_util.cpp
        hdr/system/sys_util.h
        src/system/sys_scriptEngine.cpp hdr/system/sys_scriptEngine.h src/system/sys_scriptConfig.cpp hdr/system/sys_scriptConfig.h src/system/sys_scriptFunctions.cpp hdr/system/sys_scriptFunctions.h src/io/io_logFile.cpp hdr/io/io_logFile.h src/system/sys_eventsEngine.cpp hdr/system/sys_eventsEngine.h src/system/sys_wrapper.cpp hdr/system/sys_wrapper.h src/io/io_console.cpp hdr/io/io_console.h src/io/io_fileSystem.cpp hdr/io/io_fileSystem.h src/io/io_resources.cpp hdr/io/io_resources.h src/system/sys_timerFunctions.cpp hdr/system/sys_timerFunctions.h src/game/gam_hud.cpp hdr/game/gam_hud.h src/io/io_resourceLevel.cpp hdr/io/io_resourceLevel.h src/game/gam_render.cpp hdr/game/gam_render.h src/game/gam_player.cpp hdr/game/gam_player.h src/io/io_keyboard.cpp hdr/io/io_keyboard.h src/gui/gui_text.cpp hdr/gui/gui_text.h src/system/sys_physics.cpp hdr/system/sys_physics.h src/io/io_resourceSprite.cpp hdr/io/io_resourceSprite.h src/game/gam_droidAI.cpp hdr/game/gam_droidAI.h src/game/gam_droidAIShoot.cpp hdr/game/gam_droidAIShoot.h src/game/gam_droidAIFlee.cpp hdr/game/gam_droidAIFlee.h src/game/gam_droidAIHealth.cpp hdr/game/gam_droidAIHealth.h src/game/gam_droidAIPatrol.cpp hdr/game/gam_droidAIPatrol.h src/game/gam_droidAIResume.cpp hdr/game/gam_droidAIResume.h src/game/gam_droids.cpp hdr/game/gam_droids.h src/game/gam_database.cpp hdr/game/gam_database.h src/game/gam_pathFind.cpp hdr/game/gam_pathFind.h src/game/gam_game.cpp hdr/game/gam_game.h src/game/gam_doors.cpp hdr/game/gam_doors.h src/system/sys_audio.cpp hdr/system/sys_audio.h src/game/gam_physicsCollisions.cpp hdr/game/gam_physicsCollisions.h)

# Define the C sources
set(C_SRCS
        src/chipmunk/chipmunk.c
        src/chipmunk/cpArbiter.c
        src/chipmunk/cpArray.c
        src/chipmunk/cpBBTree.c
        src/chipmunk/cpBody.c
        src/chipmunk/cpCollision.c
        src/chipmunk/cpConstraint.c
        src/chipmunk/cpDampedRotarySpring.c
        src/chipmunk/cpDampedSpring.c
        src/chipmunk/cpGearJoint.c
        src/chipmunk/cpGrooveJoint.c
        src/chipmunk/cpHashSet.c
        src/chipmunk/cpHastySpace.c
        src/chipmunk/cpMarch.c
        src/chipmunk/cpPinJoint.c
        src/chipmunk/cpPivotJoint.c
        src/chipmunk/cpPolyline.c
        src/chipmunk/cpPolyShape.c
        src/chipmunk/cpRatchetJoint.c
        src/chipmunk/cpRobust.c
        src/chipmunk/cpRotaryLimitJoint.c
        src/chipmunk/cpShape.c
        src/chipmunk/cpSimpleMotor.c
        src/chipmunk/cpSlideJoint.c
        src/chipmunk/cpSpace.c
        src/chipmunk/cpSpaceComponent.c
        src/chipmunk/cpSpaceDebug.c
        src/chipmunk/cpSpaceHash.c
        src/chipmunk/cpSpaceQuery.c
        src/chipmunk/cpSpaceStep.c
        src/chipmunk/cpSpatialIndex.c
        src/chipmunk/cpSweep1D.c
        )


set_source_files_properties(
        ${CXX_SRCS} PROPERTIES COMPILE_FLAGS
        " -pg -g -O0")
#        " -pg -g -O0 -fsanitize=address -fno-omit-frame-pointer")	# Address sanitizer - and memleak
#        " -pg -g -O0 -fsanitize=thread -fno-omit-frame-pointer")	# Thread sanitizer

add_executable(Allegro5Droid ${RC_SRCS} ${CXX_SRCS} ${C_SRCS})
#target_link_libraries(Allegro5Droid ${LINK_OPTIONS})

target_link_libraries(Allegro5Droid
        # asan		# Used for address sanitizer
        #		tsan		# Used for thread sanitizer
        allegro
        allegro_dialog
        allegro_font
        allegro_primitives
        allegro_image
        allegro_physfs
        allegro_audio
        allegro_acodec
        allegro_ttf
        allegro_memfile
        physfs
        pthread
        )
