#include "LabelWidget.h"
#include <stdexcept>

LabelWidget::LabelWidget(const std::string& id, int x, int y, const std::string& text, const std::string& font_path, int font_size, SDL_Color color) : Widget(id) {
    if (TTF_Init() == -1) {
        throw std::runtime_error(TTF_GetError());
    }

    font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        throw std::runtime_error(TTF_GetError());
    }

    this->x = x;
    this->y = y;
    this->label_text = text;
    this->text_color = color;
    this->text_texture = nullptr; // Texture will be created on the first draw
}

LabelWidget::~LabelWidget() {
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
    }
    TTF_CloseFont(font);
    TTF_Quit();
}

void LabelWidget::draw(SDL_Renderer* renderer) {
    if (!text_texture) {
        createTexture(renderer);
    }
    SDL_RenderCopy(renderer, text_texture, nullptr, &text_rect);
}

void LabelWidget::setText(const std::string& text) {
    label_text = text;
    if (text_texture) {
        SDL_DestroyTexture(text_texture);
        text_texture = nullptr; // Mark for recreation
    }
}

void LabelWidget::createTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, label_text.c_str(), text_color);
    if (!surface) {
        throw std::runtime_error(TTF_GetError());
    }

    text_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!text_texture) {
        SDL_FreeSurface(surface);
        throw std::runtime_error(SDL_GetError());
    }

    text_rect.x = x;
    text_rect.y = y;
    text_rect.w = surface->w;
    text_rect.h = surface->h;

    SDL_FreeSurface(surface);
}

json LabelWidget::toJson() const {
    json j = Widget::toJson();
    j["type"] = "LabelWidget";
    j["text"] = label_text;
    // Note: font_path, font_size, and color are not serialized as they are constructor parameters
    return j;
}

void LabelWidget::fromJson(const json& j) {
    Widget::fromJson(j);
    if (j.contains("text")) {
        setText(j["text"].get<std::string>());
    }
}