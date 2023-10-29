cmake_minimum_required(VERSION 3.12)
project(TaxiDriverModule)

# Set the source files for your module
set(SRC ${CMAKE_SOURCE_DIR}/TaxiDriver.c)

# Locate the kernel build directory
execute_process(
    COMMAND uname -r
    OUTPUT_VARIABLE KERNEL_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(KERNEL_BUILD_DIR /lib/modules/${KERNEL_RELEASE}/build)

# Define the target
add_custom_target(TaxiDriverModule ALL
    COMMAND ${CMAKE_MAKE_PROGRAM} -C ${KERNEL_BUILD_DIR} M=${CMAKE_BINARY_DIR} modules
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building TaxiDriver module"
)

# Define the clean target
add_custom_target(clean_TaxiDriverModule
    COMMAND ${CMAKE_MAKE_PROGRAM} -C ${KERNEL_BUILD_DIR} M=${CMAKE_BINARY_DIR} clean
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Cleaning TaxiDriver module"
)

# Specify the files to be cleaned
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/*.ko")
