cmake_minimum_required(VERSION 2.8.3)
project(ultrasonic_multiple)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -O3")

find_package(catkin REQUIRED COMPONENTS nav_msgs roscpp tf  rospy std_msgs)

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

link_directories(
  ${ultrasonic_multiple_SOURCE_DIR}/libs/
  /lib/x86_64-linux-gnu/
  /usr/lib/x86_64-linux-gnu/
)

###############################################################
# Build library
#target_link_libraries(hidapi-libusb ${catkin_LIBRARIES} hidapi-libusb)
###############################################################
# Build the executable
add_executable(${PROJECT_NAME} src/main.cc src/usbhid.cpp)

# Add dependency
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} hidapi-libusb usb-1.0)

###############################################################
# Install the executable
install(TARGETS #${PROJECT_NAME}  
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
