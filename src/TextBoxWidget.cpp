#include "TextBoxWidget.h"
#include <stdexcept>
#include <iostream>

TextBoxWidget::TextBoxWidget(const std::string& id, int x, int y, int w, int h, const std::string& font_path, int font_size) : Widget(id) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;

    if (TTF_Init() == -1) { // Ensure TTF is initialized
        throw std::runtime_error(TTF_GetError());
    }

    font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        throw std::runtime_error(TTF_GetError());
    }

    text = "";
    cursor_pos = 0;
    text_color = {255, 255, 255, 255}; // White
    bg_color = {50, 50, 50, 255}; // Dark grey
    text_texture = nullptr;
    show_cursor = false;
    cursor_blink_time = 0;
    texture_needs_update = true; // Initial text is empty, needs update
}

TextBoxWidget::~TextBoxWidget() {
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
    }
    TTF_CloseFont(font);
}

void TextBoxWidget::onFocus() {
    is_focused = true;
    SDL_StartTextInput();
}

void TextBoxWidget::onBlur() {
    is_focused = false;
    SDL_StopTextInput();
}

void TextBoxWidget::setText(const std::string& new_text) {
    std::cout << "TextBox::setText called with: " << new_text << ", current: " << text << ", needs_update: " << texture_needs_update << std::endl;
    if (text != new_text) {
        text = new_text;
        cursor_pos = text.length();
        texture_needs_update = true;
    }
}

void TextBoxWidget::handleEvent(const SDL_Event& event, SDL_Renderer* renderer) {
    if (!is_focused) return;

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && !text.empty() && cursor_pos > 0) {
            text.erase(cursor_pos - 1, 1);
            cursor_pos--;
            texture_needs_update = true;
        } else if (event.key.keysym.sym == SDLK_DELETE && cursor_pos < text.length()) {
            text.erase(cursor_pos, 1);
            texture_needs_update = true;
        } else if (event.key.keysym.sym == SDLK_LEFT && cursor_pos > 0) {
            cursor_pos--;
        } else if (event.key.keysym.sym == SDLK_RIGHT && cursor_pos < text.length()) {
            cursor_pos++;
        }
    } else if (event.type == SDL_TEXTINPUT) {
        text.insert(cursor_pos, event.text.text);
        cursor_pos += strlen(event.text.text);
        texture_needs_update = true;
    }
}

void TextBoxWidget::draw(SDL_Renderer* renderer) {
    // Regenerate texture if needed
    if (texture_needs_update) {
        std::cout << "TextBox::draw: Recreating texture for: " << text << std::endl;
        if (text_texture) {
            SDL_DestroyTexture(text_texture);
            text_texture = nullptr;
        }
        if (!text.empty()) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text_color);
            if (surface) {
                text_texture = SDL_CreateTextureFromSurface(renderer, surface);
                text_rect.x = x + 5; // Padding
                text_rect.y = y + (height - surface->h) / 2; // Center vertically
                text_rect.w = surface->w;
                text_rect.h = surface->h;
                SDL_FreeSurface(surface);
            }
        }
        texture_needs_update = false;
    }

    // Draw background
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_Rect bg_rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &bg_rect);

    // Draw text
    if (text_texture) {
        SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
    }

    // Draw cursor if focused
    if (is_focused) {
        if (SDL_GetTicks() - cursor_blink_time > 500) { // Blink every 500ms
            show_cursor = !show_cursor;
            cursor_blink_time = SDL_GetTicks();
        }

        if (show_cursor) {
            int cursor_x = x + 5;
            if (cursor_pos > 0) {
                std::string sub = text.substr(0, cursor_pos);
                int w, h;
                TTF_SizeText(font, sub.c_str(), &w, &h);
                cursor_x += w;
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect cursor_rect = {cursor_x, y + 5, 2, height - 10};
            SDL_RenderFillRect(renderer, &cursor_rect);
        }
    }
}

json TextBoxWidget::toJson() const {
    json j = Widget::toJson(); // Start with base class properties
    j["type"] = "TextBoxWidget";
    j["text"] = text;
    return j;
}

void TextBoxWidget::fromJson(const json& j) {
    Widget::fromJson(j); // Update base class properties
    if (j.contains("text")) {
        setText(j["text"].get<std::string>());
    }
}
