set(PROJECT_NAME cpp_test)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "testApp.cpp"
)
source_group("" FILES ${no_group_source_files})

set(Include_Files
    "../../ocean/cpp/include/BQM.h"
    "../../ocean/cpp/include/Factory.h"
    "../../ocean/cpp/include/Logger.h"
    "../../ocean/cpp/include/Qcondition.h"
    "../../ocean/cpp/include/Qdef.h"
    "../../ocean/cpp/include/Qequation.h"
    "../../ocean/cpp/include/Qexpression.h"
    "../../ocean/cpp/include/Qfunction.h"
    "../../ocean/cpp/include/Qoperand.h"
    "../../ocean/cpp/include/Qops.h"
    "../../ocean/cpp/include/Qrutine.h"
    "../../ocean/cpp/include/Qstatement.h"
    "../../ocean/cpp/include/Qtype.h"
    "../../ocean/cpp/include/Qubo.h"
    "../../ocean/cpp/include/Qvar.h"
    "../../ocean/cpp/include/Utility.h"
)
source_group("Include Files" FILES ${Include_Files})

set(Source_Files
    "../../ocean/cpp/BQM.cpp"
    "../../ocean/cpp/Global.cpp"
    "../../ocean/cpp/Logger.cpp"
    "../../ocean/cpp/Qcondition.cpp"
    "../../ocean/cpp/Qdef.cpp"
    "../../ocean/cpp/Qequation.cpp"
    "../../ocean/cpp/Qexpression.cpp"
    "../../ocean/cpp/Qfunction.cpp"
    "../../ocean/cpp/Qoperand.cpp"
    "../../ocean/cpp/Qops.cpp"
    "../../ocean/cpp/Qrutine.cpp"
    "../../ocean/cpp/Qstatement.cpp"
    "../../ocean/cpp/Qtype.cpp"
    "../../ocean/cpp/Qubo.cpp"
    "../../ocean/cpp/Qvar.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Include_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE TestConsoleApplication)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL     "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE        "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL     "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE        "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../AppData/Local/Continuum/anaconda3/envs/ocean2/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../pybind11/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../ocean/cpp/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../../eigen-master"
        ">"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
            "_USE_DANN5_LOGGER;"
            "_DANN5_LOG_INTO_TXT"
        ">"
        "$<$<CONFIG:MinSizeRel>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:RelWithDebInfo>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:MinSizeRel>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "$<$<CONFIG:RelWithDebInfo>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:MinSizeRel>:
                /O2;
                /Oi;
                /Gy
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            $<$<CONFIG:RelWithDebInfo>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:MinSizeRel>:
                /O2;
                /Oi;
                /Gy
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            $<$<CONFIG:RelWithDebInfo>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:MinSizeRel>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            $<$<CONFIG:RelWithDebInfo>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:MinSizeRel>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            $<$<CONFIG:RelWithDebInfo>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
			"C:/Users/Nebojsa.Vojinovic/AppData/Local/Continuum/anaconda3/envs/ocean38/libs/Python38"
        ">"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

