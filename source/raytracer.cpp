#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

int main(int argc, char* argv[])
{
	//TODO Die zu berechnenden Strahlen müssen korrekt berechnet werden.
	// Die Box intersect Methode muss überprüft werden...
  std::shared_ptr<Scene> scene = std::make_shared<Scene>();
  readSDF_File("C:\\Simple-Scene.sdf", *scene);
  
  unsigned const image_width = 1920;
  unsigned const image_height = 1080;
  std::string const filename = "./checkerboard.ppm";

  Renderer renderer{ image_width, image_height, filename };

  renderer.render(*scene);
  //create separate thread to see updates of pixels while rendering
  //std::thread render_thread([&renderer, scene]() {renderer.render(*scene);});

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
