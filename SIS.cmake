set(APPSIS_NAME appSIS)				#Naziv prvog projekta u solution-u

file(GLOB TESTGUI_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTGUI_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTGUI_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTGUI_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTGUI_INC_RPT  ${MY_INC}/rpt/*.h)
file(GLOB TESTGUI_INC_GUI ${MY_INC}/gui/*.h)

# add executable
add_executable(${APPSIS_NAME} ${TESTGUI_INCS} ${TESTGUI_SOURCES} ${TESTGUI_INC_TD}  ${TESTGUI_INC_GUI} ${TESTGUI_INC_RPT})

source_group("inc"            FILES ${TESTGUI_INCS})
source_group("src"            FILES ${TESTGUI_SOURCES})
source_group("inc\\td"        FILES ${TESTALLREPS_INC_TD})
source_group("inc\\gui"        FILES ${TESTALLREPS_INC_GUI})
source_group("inc\\rpt"        FILES ${TESTALLREPS_INC_RPT})


target_link_libraries(${APPSIS_NAME} debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} optimized ${MU_LIB_RELEASE}  optimized ${NATGUI_LIB_RELEASE} 
						debug ${DP_LIB_DEBUG} optimized ${DP_LIB_RELEASE} debug ${NATRPT_LIB_DEBUG} optimized ${NATRPT_LIB_RELEASE})

setTargetPropertiesForGUIApp(${APPSIS_NAME} ${TESTGUI_PLIST})

setIDEPropertiesForGUIExecutable(${APPSIS_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${APPSIS_NAME})
