#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "src/imgui.h"
#include "src/imgui_internal.h"
#include "src/imstb_truetype.h"
#include "src/imstb_textedit.h"
#include "src/imstb_rectpack.h"
#include "src/backends/imgui_impl_android.h"
#include "src/backends/imgui_impl_opengl3.h"

#include "inc/touch.h"
#include "inc/persistence.h"
#include "inc/helpers.h"
#include "inc/custom_theme.h"

#define STB_IMAGE_IMPLEMENTATION
#include "inc/stb_image.h"

void ImGui_ClearHoverEffect() {
    static std::chrono::steady_clock::time_point lastMouseUpTime;
    static bool wasMouseDown = false;

    ImGuiIO& io = ImGui::GetIO();
    if (wasMouseDown && !io.MouseDown[0]) lastMouseUpTime = std::chrono::steady_clock::now();
    wasMouseDown = io.MouseDown[0];

    if (!io.MouseDown[0]) {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMouseUpTime).count() <= 3) return;
        io.MousePos = ImVec2(-1.0f, -1.0f);
    }
}

#include <GLES2/gl2.h>
#include <EGL/egl.h>
GLuint LoadTextureFromMemory(const unsigned char* data, size_t size) {
    int width, height, channels;
    unsigned char* image = stbi_load_from_memory(data, size, &width, &height, &channels, 4);
    if (!image) return 0;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Use linear filtering for smoother quality (matches Android ImageView)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Clamp texture at edges to prevent wrapping artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    stbi_image_free(image);
    return texture;
}