const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const root = b.createModule(.{ .target = target, .optimize = optimize, .root_source_file = b.path("main.zig"), .link_libc = true });
    const exe = b.addExecutable(.{
        .name = "learnopengl",
        .root_module = root,
    });
    const run_step = b.step("run", "Run countown");
    const run_exe = b.addRunArtifact(exe);
    if (b.args) |args| {
        run_exe.addArgs(args);
    }
    run_step.dependOn(&run_exe.step);

    const c= b.addTranslateC(.{
        .root_source_file = b.addWriteFiles().add("./glfw.h",
            \\#include <GLFW/glfw3.h>
        ),
        .target = target,
        .optimize = optimize,
    });
    const glad = b.addTranslateC(.{ .root_source_file = b.path("glad/include/glad/glad.h"), .optimize = optimize, .target = target });
    root.addCSourceFile(.{ .file = b.path("glad/src/glad.c") });
    root.linkSystemLibrary("glfw", .{});
    root.addImport("c", c.createModule());
    root.addImport("glad", glad.createModule());
    b.installArtifact(exe);
}
