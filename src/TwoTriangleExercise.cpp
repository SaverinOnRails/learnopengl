#include <GL/gl.h>
#include <GL/glext.h>
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <iostream>
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
#define RC reinterpret_cast
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *);
void loadGLProcs();

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
  float verticesT1[] = {
      -0.5, 1, 0, -1, 0, 0, 0, 0, 0,
  };
  float verticesT2[] = {
      0.5, 1, 0, 0, 0, 0, 1, 0, 0,
  };
  uint32_t VAOT1;
  glGenVertexArrays(1, &VAOT1);
  glBindVertexArray(VAOT1);

  uint32_t VBOT1;
  glGenBuffers(1, &VBOT1);
  glBindBuffer(GL_ARRAY_BUFFER, VBOT1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT1), verticesT1, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  uint32_t VAOT2;
  glGenVertexArrays(1, &VAOT2);
  glBindVertexArray(VAOT2);

  uint32_t VBOT2;
  glGenBuffers(1, &VBOT2);
  glBindBuffer(GL_ARRAY_BUFFER, VBOT2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT2), verticesT2, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // vertex shader
  const char *vertexShaderSource = R"( 
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
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

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
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
  const char *fragmentShaderSource2 = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    } 
    )";

  uint32_t fragmentShader2;
  fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);
  {
    GLint success;
    char infoLog[512];

    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader2, 512, nullptr, infoLog);
      std::cout << infoLog << std::endl;
    }
  }

  uint32_t shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  uint32_t shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(fragmentShader2);

  glBindVertexArray(VAOT1);
  glBindBuffer(GL_ARRAY_BUFFER, VBOT1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  
  glBindVertexArray(VAOT2);
  glBindBuffer(GL_ARRAY_BUFFER, VBOT2);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAOT1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOT2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
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
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
