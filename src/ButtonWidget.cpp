#include "ButtonWidget.h"

ButtonWidget::ButtonWidget(const std::string& id, int x, int y, int w, int h, const std::string& text, const std::string& font_path, int font_size, std::function<void()> onClick) : Widget(id) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
    this->click_handler = onClick;
    this->is_pressed = false;
    this->visually_pressed = false;
    this->press_start_time = 0;

    bg_color_up = {100, 100, 100, 255}; // Grey
    bg_color_down = {180, 180, 180, 255};   // Lighter Grey

    // Create a label and center it
    label = std::make_unique<LabelWidget>(this->id + "_label", x, y, text, font_path, font_size, SDL_Color{255, 255, 255, 255});
    // Note: Centering logic would go here, for now it's just at x,y
}

void ButtonWidget::draw(SDL_Renderer* renderer) {
    // Update visually_pressed state
    if (visually_pressed && SDL_GetTicks() - press_start_time > 100) { // Flash for 100ms
        visually_pressed = false;
    }

    SDL_Color current_color = visually_pressed ? bg_color_down : bg_color_up;
    SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, current_color.a);
    SDL_Rect bg_rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &bg_rect);

    // Draw the label on top
    label->draw(renderer);
}

void ButtonWidget::handleEvent(const SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT && contains(event.button.x, event.button.y)) {
            is_pressed = true;
            visually_pressed = true;
            press_start_time = SDL_GetTicks();
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT && is_pressed) {
            is_pressed = false;
            if (contains(event.button.x, event.button.y)) {
                if (click_handler) {
                    click_handler();
                }
            }
        }
    } else if (event.type == SDL_MOUSEMOTION) {
        if (is_pressed && !contains(event.motion.x, event.motion.y)) {
            is_pressed = false; // Mouse dragged out
        }
    }
}

json ButtonWidget::toJson() const {
    json j = Widget::toJson();
    j["type"] = "ButtonWidget";
    j["label"] = label->toJson();
    return j;
}

void ButtonWidget::fromJson(const json& j) {
    Widget::fromJson(j);
    if (j.contains("label")) {
        label->fromJson(j["label"]);
    }
}

void ButtonWidget::setText(const std::string& new_text) {
    label->setText(new_text);
}