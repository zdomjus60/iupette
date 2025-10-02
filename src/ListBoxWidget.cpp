#include "ListBoxWidget.h"
#include <stdexcept>
#include <iostream>

ListBoxWidget::ListBoxWidget(const std::string& id, int x, int y, int w, const std::vector<std::string>& items, const std::string& font_path, int font_size, std::function<void(int, const std::string&)> onSelect) : Widget(id) {
    if (TTF_Init() == -1) { // Ensure TTF is initialized
        throw std::runtime_error(TTF_GetError());
    }
    font = TTF_OpenFont(font_path.c_str(), font_size);
    if (!font) {
        throw std::runtime_error(TTF_GetError());
    }

    this->x = x;
    this->y = y;
    this->width = w;
    this->items = items;
    this->select_handler = onSelect;

    this->item_height = TTF_FontHeight(font) + 10;
    this->height = this->item_height * items.size();

    this->selected_index = -1;
    this->hovered_index = -1;

    bg_color = {50, 50, 50, 255};       // Dark Grey
    hover_color = {80, 80, 80, 255};    // Lighter Grey
    select_color = {0, 120, 215, 255}; // Blue
    text_color = {255, 255, 255, 255};  // White
}

ListBoxWidget::~ListBoxWidget() {
    TTF_CloseFont(font);
}

void ListBoxWidget::draw(SDL_Renderer* renderer) {
    SDL_Rect item_rect = {x, y, width, item_height};

    for (int i = 0; i < items.size(); ++i) {
        item_rect.y = y + i * item_height;

        // Determine background color
        if (i == selected_index) {
            SDL_SetRenderDrawColor(renderer, select_color.r, select_color.g, select_color.b, select_color.a);
        } else if (i == hovered_index) {
            SDL_SetRenderDrawColor(renderer, hover_color.r, hover_color.g, hover_color.b, hover_color.a);
        } else {
            SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        }
        SDL_RenderFillRect(renderer, &item_rect);

        // Render text (inefficiently, but ok for now)
        SDL_Surface* surface = TTF_RenderText_Blended(font, items[i].c_str(), text_color);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect text_rect = {item_rect.x + 5, item_rect.y + 5, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, nullptr, &text_rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}

void ListBoxWidget::handleEvent(const SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_MOUSEMOTION) {
        if (contains(event.motion.x, event.motion.y)) {
            hovered_index = (event.motion.y - y) / item_height;
        } else {
            hovered_index = -1;
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int clicked_index = -1;
        if (contains(event.button.x, event.button.y)) {
            clicked_index = (event.button.y - y) / item_height;
        }

        if (event.button.button == SDL_BUTTON_LEFT && clicked_index != -1) {
            selected_index = clicked_index;
            if (select_handler) {
                select_handler(selected_index, items[selected_index]);
            }
        }
    }
}

json ListBoxWidget::toJson() const {
    json j = Widget::toJson();
    j["type"] = "ListBoxWidget";
    j["items"] = items;
    j["selected_index"] = selected_index;
    return j;
}

void ListBoxWidget::fromJson(const json& j) {
    Widget::fromJson(j);
    if (j.contains("items")) {
        items = j["items"].get<std::vector<std::string>>();
        // Recalculate height based on new items
        this->height = this->item_height * items.size();
    }
    if (j.contains("selected_index")) {
        selected_index = j["selected_index"].get<int>();
    }
}