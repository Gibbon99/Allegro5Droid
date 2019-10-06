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
        hdr/script
        hdr/Box2D
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


        src/Box2D/Collision/Shapes/b2ChainShape.cpp
        src/Box2D/Collision/Shapes/b2CircleShape.cpp
        src/Box2D/Collision/Shapes/b2EdgeShape.cpp
        src/Box2D/Collision/Shapes/b2PolygonShape.cpp
        src/Box2D/Collision/b2BroadPhase.cpp
        src/Box2D/Collision/b2CollideCircle.cpp
        src/Box2D/Collision/b2CollideEdge.cpp
        src/Box2D/Collision/b2CollidePolygon.cpp
        src/Box2D/Collision/b2Collision.cpp
        src/Box2D/Collision/b2Distance.cpp
        src/Box2D/Collision/b2DynamicTree.cpp
        src/Box2D/Collision/b2TimeOfImpact.cpp
        src/Box2D/Common/b2BlockAllocator.cpp
        src/Box2D/Common/b2Draw.cpp
        src/Box2D/Common/b2Math.cpp
        src/Box2D/Common/b2Settings.cpp
        src/Box2D/Common/b2StackAllocator.cpp
        src/Box2D/Common/b2Timer.cpp
        src/Box2D/Dynamics/b2Body.cpp
        src/Box2D/Dynamics/b2ContactManager.cpp
        src/Box2D/Dynamics/b2Fixture.cpp
        src/Box2D/Dynamics/b2Island.cpp
        src/Box2D/Dynamics/b2World.cpp
        src/Box2D/Dynamics/b2WorldCallbacks.cpp
        src/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
        src/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
        src/Box2D/Dynamics/Contacts/b2CircleContact.cpp
        src/Box2D/Dynamics/Contacts/b2Contact.cpp
        src/Box2D/Dynamics/Contacts/b2ContactSolver.cpp
        src/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
        src/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
        src/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
        src/Box2D/Dynamics/Contacts/b2PolygonContact.cpp
        src/Box2D/Dynamics/Joints/b2DistanceJoint.cpp
        src/Box2D/Dynamics/Joints/b2FrictionJoint.cpp
        src/Box2D/Dynamics/Joints/b2GearJoint.cpp
        src/Box2D/Dynamics/Joints/b2Joint.cpp
        src/Box2D/Dynamics/Joints/b2MotorJoint.cpp
        src/Box2D/Dynamics/Joints/b2MouseJoint.cpp
        src/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
        src/Box2D/Dynamics/Joints/b2PulleyJoint.cpp
        src/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
        src/Box2D/Dynamics/Joints/b2RopeJoint.cpp
        src/Box2D/Dynamics/Joints/b2WeldJoint.cpp
        src/Box2D/Dynamics/Joints/b2WheelJoint.cpp

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
        src/system/sys_scriptEngine.cpp
        hdr/system/sys_scriptEngine.h
        src/system/sys_scriptConfig.cpp
        hdr/system/sys_scriptConfig.h
        src/system/sys_scriptFunctions.cpp
        hdr/system/sys_scriptFunctions.h
        src/io/io_logFile.cpp hdr/io/io_logFile.h
        src/system/sys_eventsEngine.cpp
        hdr/system/sys_eventsEngine.h
        src/system/sys_wrapper.cpp
        hdr/system/sys_wrapper.h
        src/io/io_console.cpp
        hdr/io/io_console.h
        src/io/io_fileSystem.cpp
        hdr/io/io_fileSystem.h
        src/io/io_resources.cpp
        hdr/io/io_resources.h
        src/system/sys_timerFunctions.cpp
        hdr/system/sys_timerFunctions.h
        src/game/gam_hud.cpp
        hdr/game/gam_hud.h
        src/io/io_resourceLevel.cpp
        hdr/io/io_resourceLevel.h
        src/game/gam_render.cpp
        hdr/game/gam_render.h
        src/game/gam_player.cpp
        hdr/game/gam_player.h
        src/io/io_keyboard.cpp
        hdr/io/io_keyboard.h
        src/gui/gui_text.cpp
        hdr/gui/gui_text.h
        src/system/sys_physics.cpp
        hdr/system/sys_physics.h
        src/io/io_resourceSprite.cpp
        hdr/io/io_resourceSprite.h
        src/game/gam_droidAI.cpp
        hdr/game/gam_droidAI.h
        src/game/gam_droidAIShoot.cpp
        hdr/game/gam_droidAIShoot.h
        src/game/gam_droidAIFlee.cpp
        hdr/game/gam_droidAIFlee.h
        src/game/gam_droidAIHealth.cpp
        hdr/game/gam_droidAIHealth.h
        src/game/gam_droidAIPatrol.cpp
        hdr/game/gam_droidAIPatrol.h
        src/game/gam_droidAIResume.cpp
        hdr/game/gam_droidAIResume.h
        src/game/gam_droids.cpp
        hdr/game/gam_droids.h
        src/game/gam_database.cpp
        hdr/game/gam_database.h
        src/game/gam_pathFind.cpp
        hdr/game/gam_pathFind.h
        src/game/gam_game.cpp
        hdr/game/gam_game.h
        src/game/gam_doors.cpp
        hdr/game/gam_doors.h
        src/system/sys_audio.cpp
        hdr/system/sys_audio.h
        src/game/gam_physicsCollisions.cpp
        hdr/game/gam_physicsCollisions.h
        src/gui/gui_sideView.cpp
        hdr/gui/gui_sideView.h
        src/game/gam_lifts.cpp
        hdr/game/gam_lifts.h
        src/game/gam_physicActions.cpp
        hdr/game/gam_physicActions.h
        src/game/gam_bullet.cpp
        hdr/game/gam_bullet.h
        src/game/gam_healing.cpp
        hdr/game/gam_healing.h
        src/gui/gui_main.cpp
        hdr/gui/gui_main.h
        src/gui/gui_scrollBox.cpp
        hdr/gui/gui_scrollBox.h
        src/gui/gui_textBox.cpp
        hdr/gui/gui_textBox.h
        src/gui/gui_render.cpp
        hdr/gui/gui_render.h
        src/gui/gui_label.cpp
        hdr/gui/gui_label.h
        src/gui/gui_image.cpp
        hdr/gui/gui_image.h
        src/gui/gui_button.cpp
        hdr/gui/gui_button.h
        src/gui/gui_checkBox.cpp
        hdr/gui/gui_checkBox.h
        src/gui/gui_dialogBox.cpp
        hdr/gui/gui_dialogBox.h
        src/io/io_resourceImage.cpp
        hdr/io/io_resourceImage.h
        src/game/gam_lineOfSight.cpp
        hdr/game/gam_lineOfSight.h
        src/gui/gui_terminal.cpp hdr/gui/gui_terminal.h src/game/gam_terminal.cpp src/game/gam_terminal.h src/gui/gui_database.cpp hdr/gui/gui_database.h src/gui/gui_deckView.cpp hdr/gui/gui_deckView.h src/game/gam_particles.cpp hdr/game/gam_particles.h)

set_source_files_properties(
        ${CXX_SRCS} PROPERTIES COMPILE_FLAGS
        " -pg -g -O0")
#        " -pg -g -O0 -fsanitize=address -fno-omit-frame-pointer")	# Address sanitizer - and memleak
#        " -pg -g -O0 -fsanitize=thread -fno-omit-frame-pointer")	# Thread sanitizer

add_executable(Allegro5Droid ${RC_SRCS} ${CXX_SRCS})
#target_link_libraries(Allegro5Droid ${LINK_OPTIONS})

target_link_libraries(Allegro5Droid
#        asan		# Used for address sanitizer
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
