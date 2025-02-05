#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

#include "../include/include.hpp"

struct MyStruct {
  int h;
};

class MyClass {
public:
  int d = 5;
  MyClass(int d) { d = d; }
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

class GlfwWindowWrapper {
public:
  GLFWwindow *window;

private:
  GlfwWindowWrapper() = default;
  static std::optional<GlfwWindowWrapper *> create() {
    std::cout << "Making ::create()\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "GLFW window", NULL, NULL);

    if (window == NULL) {
      std::cout << "Failed to create GLFW window\n";
      glfwTerminate();
      return std::nullopt;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    mut result = new GlfwWindowWrapper();
    result->window = window;
    std::cout << "Created\n";
    return std::make_optional(result);
  }
  inline static GlfwWindowWrapper *instance = NULL;

public:
  static auto get() noexcept(true) {
    if (instance != NULL) {
      std::cout << "Instance not null\n";
      return instance;
    } else {
      std::cout << "Instance null\n";
      mut new_instance = create();
      if (!new_instance.has_value()) {
        exit(1);
      }
      instance = new_instance.value();
      return instance;
    }
  };
  static auto clear() {
    if (instance != NULL) {
      delete instance;
    }
    instance = NULL;
  }
};

// clang-format off
float vertices[] = {
  -0.5f,-0.5f,0.0f,
  0.5f, -0.5f,0.0f,
  0.0f, 0.5f, 0.0f
};

const char* fragment_shader_text = 
#include "../glsl/frag_shader.glsl"
;

const char* vertex_shader_text =
#include "../glsl/vert_shader.glsl"
;

// clang-format on
void render() {

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {

  std::cout << "Sizeof(int): " << sizeof(int) << "\n";
  std::cout << "Sizeof(long): " << sizeof(long) << "\n";

  mut window_wrapper = GlfwWindowWrapper::get();

  glViewport(0, 0, 800, 600);

  mut window = window_wrapper->window;
  while (!glfwWindowShouldClose(window)) {
    render();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  return 0;
}
