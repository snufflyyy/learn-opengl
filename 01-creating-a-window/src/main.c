#include "window.h"

#include <glad/glad.h>

int main() {
    Window* window = window_create(1280, 720, "learn-opengl");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!window_should_close(window)) {
        window_update(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    window_destroy(window);

    return 0;
}
