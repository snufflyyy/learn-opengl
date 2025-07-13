#include "window.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

Window* window_create(int width, int height, const char* title) {
    Window* window = (Window*) malloc(sizeof(Window));
    if (!window) {
        fprintf(stderr, "[ERROR] [Window] Failed to allocate memory for window!\n");
        return NULL;
    }

    if (glfwInit() != GLFW_TRUE) {
        fprintf(stderr, "[ERROR] [Window] Failed to initialize GLFW!\n");
        free(window);
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
    #endif

    window->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window->glfw_window) {
        fprintf(stderr, "[ERROR] [Window] Failed create GLFW window!\n");
        glfwTerminate();
        free(window);
        return NULL;
    }

    glfwMakeContextCurrent(window->glfw_window);

    if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) == 0) {
        fprintf(stderr, "[ERROR] [Window] Failed load GLAD!\n");
        glfwDestroyWindow(window->glfw_window);
        glfwTerminate();
        free(window);
        return NULL;
    }

    glfwSetWindowUserPointer(window->glfw_window, window);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window->glfw_window, framebuffer_resize_callback);

    printf("[INFO] [Window] Created window!\n");
    return window;
}

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    Window* window_struct = (Window*) glfwGetWindowUserPointer(window);

    window_struct->resized = true;

    glViewport(0, 0, width, height);
}

bool window_should_close(Window* window) {
    return glfwWindowShouldClose(window->glfw_window);
}

int window_get_width(Window* window) {
    int width;
    glfwGetWindowSize(window->glfw_window, &width, NULL);
    return width;
}

int window_get_height(Window* window) {
    int height;
    glfwGetWindowSize(window->glfw_window, NULL, &height);
    return height;
}

void window_update(Window* window) {
    glfwSwapBuffers(window->glfw_window);
    glfwPollEvents();
}

void window_destroy(Window* window) {
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
    free(window);

    printf("[INFO] [Window] Destroyed window!\n");
}
