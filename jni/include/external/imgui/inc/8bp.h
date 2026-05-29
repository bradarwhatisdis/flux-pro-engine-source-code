void DrawEightBallLoading(ImDrawList* draw_list) {
    auto& io = ImGui::GetIO();
    
    // Match game's timing: 360 degrees per second
    static double effect_angle = 0.0;
    effect_angle = fmod(effect_angle + io.DeltaTime * 360.0 * (PI / 180.0), 2 * PI); // Keep angle normalized

    // Center of the screen table
    // ImVec2 center((float)((TABLE_LEFT + TABLE_RIGHT) * 0.5), (float)((TABLE_TOP + TABLE_BOTTOM) * 0.5));
    ImVec2 center = WorldToScreen(Vec2d(0, 0));
    // Load textures if not already loaded
    static GLuint ball_texture = LoadTextureFromMemory(spinning_ball_png, spinning_ball_png_len);
    static GLuint effect_texture = LoadTextureFromMemory(spinning_ball_effect_png, spinning_ball_effect_png_len);
    
    // Match game's sizing: ball = height/12, effect = height/8
    float ball_size = (Height / 12.0f) * 0.5f;
    float effect_size = (Height / 8.0f) * 0.5f;

    // Draw the main spinning ball in center
    ImVec2 ball_min(center.x - ball_size, center.y - ball_size);
    ImVec2 ball_max(center.x + ball_size, center.y + ball_size);
    draw_list->AddImage((void*)(intptr_t)ball_texture, ball_min, ball_max);
    
    // Draw the rotating effect centered on the ball
    ImVec2 effect_min(center.x - effect_size, center.y - effect_size);
    ImVec2 effect_max(center.x + effect_size, center.y + effect_size);
    
    // Calculate rotation matrix for the effect (like Android's RotateAnimation)
    const float cos_a = cosf(effect_angle);
    const float sin_a = sinf(effect_angle);
    
    // Calculate rotated corners around the center point
    auto rotate_point = [&](float x, float y) -> ImVec2 {
        const float dx = x - center.x;
        const float dy = y - center.y;
        return ImVec2(
            dx * cos_a - dy * sin_a + center.x,
            dx * sin_a + dy * cos_a + center.y
        );
    };
    
    // Get the four corners of the effect quad
    ImVec2 p0 = rotate_point(effect_min.x, effect_min.y);
    ImVec2 p1 = rotate_point(effect_max.x, effect_min.y);
    ImVec2 p2 = rotate_point(effect_max.x, effect_max.y);
    ImVec2 p3 = rotate_point(effect_min.x, effect_max.y);
    
    // Draw the rotated effect with fixed UVs
    draw_list->AddImageQuad(
        (void*)(intptr_t)effect_texture,
        p0, p1, p2, p3,
        ImVec2(0, 0), ImVec2(1, 0), ImVec2(1, 1), ImVec2(0, 1)
    );
}