
find_package(LLVM REQUIRED CONFIG)

if("${LLVM_VERSION_MAJOR}" VERSION_LESS 18)
  message(FATAL_ERROR "Found LLVM ${LLVM_VERSION_MAJOR}, but need LLVM 18 or above")
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LT_LLVM_INSTALL_DIR}")
message(STATUS "LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  Targets     ${LLVM_TARGETS_TO_BUILD}"
)

set(LLVM_Pass_Sources src/DumpPass.cc)
add_library(LLVM_Pass SHARED ${LLVM_Pass_Sources})
target_compile_definitions(LLVM_Pass PRIVATE ${LLVM_DEFINITIONS})
target_include_directories(LLVM_Pass PRIVATE ${LLVM_INCLUDE_DIRS})
#target_link_libraries(LLVM_Pass ${LLVM_LIBRARY_DIRS})
