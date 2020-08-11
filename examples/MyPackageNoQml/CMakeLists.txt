
facelift_add_interface(MyPackageNoQmlInterface
    INTERFACE_DEFINITION_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

add_definitions(-DQML_MODEL_LOCATION=${CMAKE_CURRENT_SOURCE_DIR})

# NOTE: this library addition is trivial, its meant to compile extra code
# which depicts how to create a server in C++

facelift_add_library(MyPackageNoQmlLib
    SOURCES IPCServerRegistration.cpp
    LINK_LIBRARIES MyPackageNoQmlInterface
)

facelift_add_executable(MyPackageNoQml_IPCServer
    SOURCES 
        app/main-ipcserver.cpp
    LINK_LIBRARIES
        Qt5::Core
        MyPackageNoQmlLib
)

facelift_add_executable(MyPackageNoQml_IPCClient
    SOURCES 
        app/main-ipcclient.cpp
    LINK_LIBRARIES
        Qt5::Core
        MyPackageNoQmlLib
)

# Create example scripts to easily launch the two applications similar to 
# the other QML-based example scripts
set(BINARY_NAME MyPackageNoQml_IPCServer)
configure_file(
        launch-example-noqml.in.sh 
        ${CMAKE_BINARY_DIR}/examples/launch-mypackagenoqml-ipcserver.sh 
        @ONLY
)

set(BINARY_NAME MyPackageNoQml_IPCClient)
configure_file(
        launch-example-noqml.in.sh 
        ${CMAKE_BINARY_DIR}/examples/launch-mypackagenoqml-ipcclient.sh 
        @ONLY
)