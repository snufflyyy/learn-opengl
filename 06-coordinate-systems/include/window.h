#pragma once

#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct Window {
    GLFWwindow* glfw_window;
    bool resized;
} Window;

Window* window_create(int width, int height, const char* title);
bool window_should_close(Window* window);
int window_get_width(Window* window);
int window_get_height(Window* window);
void window_update(Window* window);
void window_destroy(Window* window);
