#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

int main(int argc, char* argv[])
{

	//TODO Die zu berechnenden Strahlen muessen korrekt berechnet werden.
	// Die Box intersect Methode muss ueberprueft werden...
  std::shared_ptr<Scene> scene = std::make_shared<Scene>();

  #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
  readSDF_File("C:\\test2.sdf", *scene);
  #else
  readSDF_File("/home/anbenla/raytracer/programmiersprachen-raytracer/Übung-7/Simple-Scene.sdf",*scene);
  #endif

  
  unsigned const image_width = 480;
  unsigned const image_height = 320;
  std::string const filename = "./checkerboard";

  Renderer renderer{ image_width, image_height, filename };

  renderer.render(*scene, 1);
  //create separate thread to see updates of pixels while rendering
  //std::thread render_thread([&renderer, scene]() {renderer.render(*scene, 1);});

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  //"join" threads, i.e. synchronize main thread with render_thread
  //render_thread.join();
  std::cout << "finished";
  return 0;
}
