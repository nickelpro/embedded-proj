cmake_minimum_required(VERSION 3.24)
project(embedded C)

add_executable(flash main.c)
target_compile_features(flash PRIVATE cxx_std_20)
target_compile_options(flash PRIVATE
  -mmcu=atmega32u4 -Wall -Wno-overflow -fno-exceptions -fno-threadsafe-statics
  -fpermissive -ffunction-sections -fdata-sections -Wno-array-bounds -Wno-volatile
)
target_link_options(flash PRIVATE -mmcu=atmega32u4)

# set_target_properties(flash PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
