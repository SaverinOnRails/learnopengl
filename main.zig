const std = @import("std");
const c = @import("c");
const glad = @import("glad");
const Window = c.GLFWwindow;
pub fn main() void {
    std.debug.print("{s}", .{"Learn opengl\n"});
    _ = c.glfwInit();

    c.glfwWindowHint(c.GLFW_CONTEXT_VERSION_MAJOR, 3);
    c.glfwWindowHint(c.GLFW_CONTEXT_VERSION_MINOR, 3);
    c.glfwWindowHint(c.GLFW_OPENGL_PROFILE, c.GLFW_OPENGL_CORE_PROFILE);

    const window = c.glfwCreateWindow(800, 600, "Learnopengl", null, null);
    if (window == null) {
        std.debug.print("{s}", .{"Failed to create window"});
    }
    c.glfwMakeContextCurrent(window);
    if (glad.gladLoadGLLoader(@ptrCast(&c.glfwGetProcAddress)) == 0) {
        std.debug.panic("{s}", .{"Failed to init glad"});
    }

    glad.glViewport(0, 0, 800, 600);
    _ = c.glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    const vertexShaderSource =
        \\#version 330 core
        \\layout (location = 0) in vec3 aPos;
        \\void main(){
        \\   gl_Position = vec4(aPos,1.0);    
        \\}
    ;
    var vertexShader: c_uint = undefined;
    vertexShader = glad.glCreateShader(glad.GL_VERTEX_SHADER);
    glad.glShaderSource(vertexShader, 1, @ptrCast(&vertexShaderSource), null);
    glad.glCompileShader(vertexShader);
    //check shader compilation
    var success: c_int = undefined;
    var infoLog: [512]u8 = undefined;
    glad.glGetShaderiv(vertexShader, glad.GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glad.glGetShaderInfoLog(vertexShader, 512, null, @ptrCast(&infoLog));
        std.debug.print("{s}", .{infoLog});
        std.debug.panic("{s}", .{"Could not compile vertex shader"});
    }

    const fragmentShaderSource =
        \\#version 330 core
        \\out vec4 FragColor;
        \\void main(){
        \\ FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
        \\}
    ;
    var fragmentShader: c_uint = undefined;
    fragmentShader = glad.glCreateShader(glad.GL_FRAGMENT_SHADER);
    glad.glShaderSource(fragmentShader, 1, @ptrCast(&fragmentShaderSource), null);
    glad.glCompileShader(fragmentShader);
    glad.glGetShaderiv(fragmentShader, glad.GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glad.glGetShaderInfoLog(fragmentShader, 512, null, @ptrCast(&infoLog));
        std.debug.print("{s}", .{infoLog});
        std.debug.panic("{s}", .{"Could not compile fragment shader"});
    }

    var shaderProgram: c_uint = undefined;
    shaderProgram = glad.glCreateProgram();
    glad.glAttachShader(shaderProgram, vertexShader);
    glad.glAttachShader(shaderProgram, fragmentShader);
    glad.glLinkProgram(shaderProgram);
    glad.glGetShaderiv(shaderProgram, glad.GL_LINK_STATUS, &success);
    if (success == 0) {
        glad.glGetShaderInfoLog(shaderProgram, 512, null, @ptrCast(&infoLog));
        std.debug.print("{s}", .{infoLog});
        std.debug.panic("{s}", .{"Could not link shaders"});
    }
    glad.glDeleteShader(vertexShader);
    glad.glDeleteShader(fragmentShader);
    const vertices = [9]f32{
        -0.5, -0.5, 0.0,
        0.5,  -0.5, 0.0,
        0.0,  0.5,  0.0,
    };
    var VBO: c_uint = undefined;
    var VAO: c_uint = undefined;
    glad.glGenVertexArrays(1, &VAO);
    glad.glGenBuffers(1, @ptrCast(&VBO));
    glad.glBindVertexArray(VAO);
    glad.glBindBuffer(glad.GL_ARRAY_BUFFER, VBO);
    glad.glBufferData(glad.GL_ARRAY_BUFFER, @sizeOf(@TypeOf(vertices)), &vertices, glad.GL_STATIC_DRAW);
    glad.glVertexAttribPointer(
        0,
        3,
        glad.GL_FLOAT,
        glad.GL_FALSE,
        3 * @sizeOf(f32),
        null,
    );
    glad.glEnableVertexAttribArray(0);
    glad.glBindBuffer(glad.GL_ARRAY_BUFFER, VBO);
    glad.glBindVertexArray(0);

    while (c.glfwWindowShouldClose(window) == 0) {
        processInput(window);
        //render
        glad.glClearColor(0.2, 0.3, 0.3, 1.0);
        glad.glClear(c.GL_COLOR_BUFFER_BIT);

        glad.glUseProgram(shaderProgram);
        glad.glBindVertexArray(VAO);
        glad.glDrawArrays(glad.GL_TRIANGLES, 0, 3);

        c.glfwSwapBuffers(window);
        c.glfwPollEvents();
    }
    c.glfwTerminate();
}
fn frameBufferSizeCallback(_: ?*c.GLFWwindow, w: c_int, h: c_int) callconv(.c) void {
    glad.glViewport(0, 0, w, h);
}

fn processInput(window: ?*Window) void {
    if (c.glfwGetKey(window, c.GLFW_KEY_ESCAPE) == c.GLFW_PRESS) {
        c.glfwSetWindowShouldClose(window, 1);
    }
}
