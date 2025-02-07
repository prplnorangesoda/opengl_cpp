#include "../include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "../include/include.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

class GlfwWindowWrapper {
public:
  GLFWwindow *window;

private:
  GlfwWindowWrapper() = default;
  ~GlfwWindowWrapper() { glfwDestroyWindow(this->window); }
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
  static auto clear_all_windows() {
    if (instance != NULL) {
      delete instance;
    }
    instance = NULL;
    glfwTerminate();
  }
};

const char *fragment_shader_text =
#include "../glsl/frag_shader.glsl"
    ;

const char *vertex_shader_text =
#include "../glsl/vert_shader.glsl"
    ;

// clang-format off
float vertices[] = {
  -0.5f,-0.5f,0.0f,
  0.5f, -0.2f,0.0f,
  0.0f, 0.5f, 0.0f
};
// clang-format on

struct RenderInfo {
  GLuint vao;
  GLuint vbo;
  GLuint program;
};

const RenderInfo set_up_render() {
  GLuint VBO = 0;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Copy vertex data into the bound array buffer VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create the vertex shader
  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vert_shader);

  int success;
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);

  if (success != GL_TRUE) {
    char infoLog[512];
    glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
    std::cout << "ERROR COMPILING VERTEX SHADER: " << infoLog;
    exit(2);
  }

  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(frag_shader);

  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {
    char infoLog[512];
    glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
    std::cout << "ERROR COMPILING FRAGMENT SHADER: " << infoLog;
    exit(3);
  }

  let program = glCreateProgram();

  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success != GL_TRUE) {
    char infoLog[512];
    glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
    std::cout << "ERROR LINKING PROGRAM: " << infoLog;
    exit(4);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
  glEnableVertexAttribArray(0);

  return RenderInfo{.vao = VAO, .vbo = VBO, .program = program};
}

void clear() {
  glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void render(GLuint program, GLuint vao) {
  clear();

  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {

  std::cout << "Sizeof(int): " << sizeof(int) << "\n";
  std::cout << "Sizeof(long): " << sizeof(long) << "\n";

  mut window_wrapper = GlfwWindowWrapper::get();

  glViewport(0, 0, 800, 600);

  let info = set_up_render();

  mut window = window_wrapper->window;
  while (!glfwWindowShouldClose(window)) {
    render(info.program, info.vao);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  GlfwWindowWrapper::clear_all_windows();
  return 0;
}
