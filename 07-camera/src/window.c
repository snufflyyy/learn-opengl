#include "window.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

    glfwSwapInterval(0);

    glfwSetWindowUserPointer(window->glfw_window, window);

    glfwSetFramebufferSizeCallback(window->glfw_window, framebuffer_resize_callback);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window->glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    window->mouse_captured = true;

    glfwSetKeyCallback(window->glfw_window, key_callback);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    printf("[INFO] [Window] Created window!\n");
    return window;
}

static void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
    Window* window_struct = (Window*) glfwGetWindowUserPointer(window);

    window_struct->resized = true;

    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* window_struct = (Window*) glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (!window_struct->mouse_captured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            window_struct->mouse_captured = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            window_struct->mouse_captured = false;
        }
    }
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

    float current_time = glfwGetTime();
    window->delta_time = current_time - window->last_frame_time;
    window->last_frame_time = current_time;
    window->fps = (unsigned int) (1.0f / window->delta_time);
}

void window_destroy(Window* window) {
    glfwDestroyWindow(window->glfw_window);
    glfwTerminate();
    free(window);

    printf("[INFO] [Window] Destroyed window!\n");
}
