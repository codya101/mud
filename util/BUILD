load("@rules_cc//cc:defs.bzl", "cc_binary")
load("@//:global_copts.bzl", "global_copts")

package(features = ["-default_compile_flags"])

cc_binary(
    name = "cppversion",
    srcs = ["cppversion.cpp"],
    copts = global_copts(),
    deps = [
        "@fmt//:main",
        "@ncurses//:main",
    ],
)

cc_binary(
    name = "scratch",
    srcs = ["scratch.cpp"],
    copts = global_copts(),
    deps = [
        "@fmt//:main",
        "@ncurses//:main",
    ],
)
