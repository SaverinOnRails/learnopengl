
// GL TRIANGLE
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdint>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *);
void loadGLProcs();
#define RC reinterpret_cast

// PFNGLGENBUFFERSPROC glGenBuffers;
// PFNGLBINDBUFFERPROC glBindBuffer;
// PFNGLBUFFERDATAPROC glBufferData;
// PFNGLCREATESHADERPROC glCreateShader;
// PFNGLSHADERSOURCEPROC glShaderSource;
// PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  loadGLProcs();

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glViewport(0, 0, 800, 600);

  float vertices[] = {-0.5f, 1,    0.0f, 1.0f, 0.0f, 0.0f, 0,    0,    0.0f,
                      0.0f,  1.0f, 0.0f, 0.5,  1,    0.0f, 0.0f, 0.0f, 1.0f};

  uint32_t VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  uint32_t VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // vertex shader
  const char *vertexShaderSource = R"( 
    #version 330 core
    layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
    layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
    uniform float x_offset;
    out vec3 ourColor; // output a color to the fragment shader
    out vec4 finalPos;
    void main()
    {
        finalPos = vec4(aPos.x + x_offset, aPos.y, aPos.z, 1.0);
        gl_Position = finalPos;
        ourColor = aColor; // set ourColor to the input color we got from the vertex data
    }
)";

  uint32_t vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  {

    GLint success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      std::cout << infoLog << std::endl;
    }
  }
  const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;  
    in vec4 finalPos;
    void main()
    {
        FragColor = finalPos;
    }
    )";

  uint32_t fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  {
    GLint success;
    char infoLog[512];

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
      std::cout << infoLog << std::endl;
    }
  }
  uint32_t shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int x_offset = glGetUniformLocation(shaderProgram, "x_offset");
    glUniform1f(x_offset, 0.2);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void loadGLProcs() {
  // glGenBuffers = RC<PFNGLGENBUFFERSPROC>(glfwGetProcAddress("glGenBuffers"));
  // glBindBuffer = RC<PFNGLBINDBUFFERPROC>(glfwGetProcAddress("glBindBuffer"));
  // glBufferData = RC<PFNGLBUFFERDATAPROC>(glfwGetProcAddress("glBufferData"));
  // glCreateShader =
  //     RC<PFNGLCREATESHADERPROC>(glfwGetProcAddress("glCreateShader"));
  // glShaderSource =
  //     RC<PFNGLSHADERSOURCEPROC>(glfwGetProcAddress("glShaderSource"));
  glGenVertexArrays =
      RC<PFNGLGENVERTEXARRAYSPROC>(glfwGetProcAddress("glGenVertexArrays"));
  glBindVertexArray =
      RC<PFNGLBINDVERTEXARRAYPROC>(glfwGetProcAddress("glBindVertexArray"));
}
