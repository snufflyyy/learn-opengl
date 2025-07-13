#pragma once

#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct Window {
    GLFWwindow* glfw_window;
} Window;

Window* window_create(int width, int height, const char* title);
bool window_should_close(Window* window);
void window_update(Window* window);
void window_destroy(Window* window);
