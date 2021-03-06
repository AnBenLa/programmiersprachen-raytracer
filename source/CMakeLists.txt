add_executable(example example.cpp)

target_link_libraries(example ${FRAMEWORK_NAME} ${LIBRARIES})
add_dependencies(example glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS example DESTINATION .)

add_executable(raytracer raytracer.cpp)

target_link_libraries(raytracer ${FRAMEWORK_NAME} ${LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
  )
add_dependencies(raytracer glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

install(TARGETS raytracer DESTINATION .)

# Build tests
add_executable(tests 
  tests.cpp
  ../framework/sphere.cpp
  ../framework/triangle.cpp
  ../framework/cone.cpp
  ../framework/composite.cpp
  ../framework/cylinder.cpp
  ../framework/box.cpp
  ../framework/shape.cpp
  ../framework/renderer.cpp
  ../framework/ppmwriter.cpp
  ../framework/pixel.cpp
  ../framework/shape.cpp
   )
target_link_libraries(tests
  ${GLFW_LIBRARIES}
  )

