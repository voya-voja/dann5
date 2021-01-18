set(PROJECT_NAME d5o)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../CMakeLists.txt"
)
source_group("" FILES ${no_group_source_files})

set(Include_Files
    "../ocean/cpp/include/BQM.h"
    "../ocean/cpp/include/Factory.h"
    "../ocean/cpp/include/Logger.h"
    "../ocean/cpp/include/Qcondition.h"
    "../ocean/cpp/include/Qdef.h"
    "../ocean/cpp/include/Qequation.h"
    "../ocean/cpp/include/Qexpression.h"
    "../ocean/cpp/include/Qfunction.h"
    "../ocean/cpp/include/Qoperand.h"
    "../ocean/cpp/include/Qops.h"
    "../ocean/cpp/include/Qroutine.h"
    "../ocean/cpp/include/Qstatement.h"
    "../ocean/cpp/include/Qtype.h"
    "../ocean/cpp/include/Qubo.h"
    "../ocean/cpp/include/Qvar.h"
    "../ocean/cpp/include/Utility.h"
)
source_group("Include Files" FILES ${Include_Files})

set(Source_Files
    "../ocean/cpp/BQM.cpp"
    "../ocean/cpp/Global.cpp"
    "../ocean/cpp/Logger.cpp"
    "../ocean/cpp/main.cpp"
    "../ocean/cpp/Qcondition.cpp"
    "../ocean/cpp/Qdef.cpp"
    "../ocean/cpp/Qequation.cpp"
    "../ocean/cpp/Qexpression.cpp"
    "../ocean/cpp/Qfunction.cpp"
    "../ocean/cpp/Qoperand.cpp"
    "../ocean/cpp/Qops.cpp"
    "../ocean/cpp/Qroutine.cpp"
    "../ocean/cpp/Qstatement.cpp"
    "../ocean/cpp/Qtype.cpp"
    "../ocean/cpp/Qubo.cpp"
    "../ocean/cpp/Qvar.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(py_test
    "../test/py/d5oQuboAdditionTest.py"
    "../test/py/d5oQuboMultiplicationTest.py"
    "../test/py/d5oQuboTest.py"
    "../test/py/DWbqmTest.py"
)
source_group("py_test" FILES ${py_test})

set(ALL_FILES
    ${no_group_source_files}
    ${Include_Files}
    ${Source_Files}
    ${py_test}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG          "d5o.cp38-win_amd64"
        TARGET_NAME_MINSIZEREL     "dann5.ocean.cp37-win_amd64"
        TARGET_NAME_RELEASE        "dann5.ocean.cp37-win_amd64"
        TARGET_NAME_RELWITHDEBINFO "dann5.ocean.cp37-win_amd64"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG          "d5o.cp38-win_amd64"
        TARGET_NAME_MINSIZEREL     "dann5.ocean.cp37-win_amd64"
        TARGET_NAME_RELEASE        "dann5.ocean.cp37-win_amd64"
        TARGET_NAME_RELWITHDEBINFO "dann5.ocean.cp37-win_amd64"
    )
endif()
################################################################################
# Output directory
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG          "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Debug/"
        OUTPUT_DIRECTORY_MINSIZEREL     "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/MinSizeRel/"
        OUTPUT_DIRECTORY_RELEASE        "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Release/"
        OUTPUT_DIRECTORY_RELWITHDEBINFO "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/RelWithDebInfo/"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG          "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Debug/"
        OUTPUT_DIRECTORY_MINSIZEREL     "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/MinSizeRel/"
        OUTPUT_DIRECTORY_RELEASE        "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Release/"
        OUTPUT_DIRECTORY_RELWITHDEBINFO "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/RelWithDebInfo/"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Debug"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/MinSizeRel"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Release"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/RelWithDebInfo"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Debug"
        ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/MinSizeRel"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/Release"
        ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "C:/Users/Nebojsa.Vojinovic/source/repos/dann5/cpp_build/RelWithDebInfo"
    )
endif()
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_NAME_DEBUG          "d5o"
        ARCHIVE_OUTPUT_NAME_MINSIZEREL     "d5o"
        ARCHIVE_OUTPUT_NAME_RELEASE        "d5o"
        ARCHIVE_OUTPUT_NAME_RELWITHDEBINFO "d5o"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_NAME_DEBUG          "d5o"
        ARCHIVE_OUTPUT_NAME_MINSIZEREL     "d5o"
        ARCHIVE_OUTPUT_NAME_RELEASE        "d5o"
        ARCHIVE_OUTPUT_NAME_RELWITHDEBINFO "d5o"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:MinSizeRel>:
            MultiThreadedDLL
        >
        $<$<CONFIG:Release>:
            MultiThreadedDLL
        >
        $<$<CONFIG:RelWithDebInfo>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:MinSizeRel>,$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:MinSizeRel>:
            MultiThreadedDLL
        >
        $<$<CONFIG:Release>:
            MultiThreadedDLL
        >
        $<$<CONFIG:RelWithDebInfo>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:MinSizeRel>,$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../ocean/cpp/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../eigen-master;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../AppData/Local/Continuum/anaconda3/envs/ocean38/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../AppData/Local/Continuum/anaconda3/envs/ocean38/Lib/site-packages/pybind11/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../ocean/cpp/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../eigen-master;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../AppData/Local/Continuum/anaconda3/envs/ocean38/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../AppData/Local/Continuum/anaconda3/envs/ocean38/Lib/site-packages/pybind11/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "CMAKE_INTDIR=\"Debug\";"
            "_USE_DANN5_LOGGER;"
            "_DANN5_LOG_INTO_TXT"
        ">"
        "$<$<CONFIG:MinSizeRel>:"
            "NDEBUG;"
            "CMAKE_INTDIR=\"MinSizeRel\""
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "CMAKE_INTDIR=\"Release\""
        ">"
        "$<$<CONFIG:RelWithDebInfo>:"
            "NDEBUG;"
            "CMAKE_INTDIR=\"RelWithDebInfo\""
        ">"
        "WIN32;"
        "_WINDOWS;"
        "dann5.ocean_EXPORTS;"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "CMAKE_INTDIR=\"Debug\";"
            "_USE_DANN5_LOGGER;"
            "_DANN5_LOG_INTO_TXT"
        ">"
        "$<$<CONFIG:MinSizeRel>:"
            "NDEBUG;"
            "CMAKE_INTDIR=\"MinSizeRel\""
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "CMAKE_INTDIR=\"Release\""
        ">"
        "$<$<CONFIG:RelWithDebInfo>:"
            "NDEBUG;"
            "CMAKE_INTDIR=\"RelWithDebInfo\""
        ">"
        "WIN32;"
        "_WINDOWS;"
        "dann5.ocean_EXPORTS;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /Ob0;
                /RTC1;
                /Zi
            >
            $<$<CONFIG:MinSizeRel>:
                /GL;
                /O1;
                /Ob1;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            $<$<CONFIG:Release>:
                /GL;
                /O2;
                /Ob2;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            $<$<CONFIG:RelWithDebInfo>:
                /GL;
                /O2;
                /Ob1;
                /Zi
            >
            /MP;
            /std:c++14;
            /W3;
            /TP;
            /GR;
            /bigobj;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od;
                /Ob0;
                /RTC1;
                /Zi
            >
            $<$<CONFIG:MinSizeRel>:
                /GL;
                /O1;
                /Ob1;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            $<$<CONFIG:Release>:
                /GL;
                /O2;
                /Ob2;
                ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
            >
            $<$<CONFIG:RelWithDebInfo>:
                /GL;
                /O2;
                /Ob1;
                /Zi
            >
            /MP;
            /std:c++14;
            /W3;
            /TP;
            /GR;
            /bigobj;
            ${DEFAULT_CXX_EXCEPTION_HANDLING};
            /Y-
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:MinSizeRel>:
                /INCREMENTAL:NO
            >
            $<$<CONFIG:Release>:
                /INCREMENTAL:NO
            >
            $<$<CONFIG:RelWithDebInfo>:
                /DEBUG;
                /INCREMENTAL
            >
            /MANIFEST;
            /SUBSYSTEM:CONSOLE;
            /machine:x64
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:MinSizeRel>:
                /INCREMENTAL:NO
            >
            $<$<CONFIG:Release>:
                /INCREMENTAL:NO
            >
            $<$<CONFIG:RelWithDebInfo>:
                /DEBUG;
                /INCREMENTAL
            >
            /MANIFEST;
            /SUBSYSTEM:CONSOLE;
            /machine:x64
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../AppData/Local/Continuum/anaconda3/envs/ocean38/libs/Python38;"
        "kernel32;"
        "user32;"
        "gdi32;"
        "winspool;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "comdlg32;"
        "advapi32"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../AppData\Local\Continuum\anaconda3\envs\ocean38\libs\Python38;"
        "kernel32;"
        "user32;"
        "gdi32;"
        "winspool;"
        "shell32;"
        "ole32;"
        "oleaut32;"
        "uuid;"
        "comdlg32;"
        "advapi32"
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

