cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CHIP-8 Disassembler")

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(SOURCE_FILES
	${SOURCE_DIR}/main.cpp
	${SOURCE_DIR}/Disassembler.cpp
	${SOURCE_DIR}/Disassembler.h
	${SOURCE_DIR}/fileReader.cpp
	${SOURCE_DIR}/fileReader.h
	${SOURCE_DIR}/buffer_operations.cpp
	${SOURCE_DIR}/buffer_operations.h
	${SOURCE_DIR}/instruction_set.h
)

add_executable(disassembler ${SOURCE_FILES})
