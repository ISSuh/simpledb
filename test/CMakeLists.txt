project(SimpleDB_test)

message("Build SimpleDB Test")

include(CTest) 

#-------------------------------------------------------------------------------
# Unit Tests
#-------------------------------------------------------------------------------
enable_testing()

add_subdirectory(engine)

