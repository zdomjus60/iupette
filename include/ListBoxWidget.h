#ifndef LISTBOXWIDGET_H
#define LISTBOXWIDGET_H

#include "Widget.h"
#include <vector>
#include <string>
#include <functional>
#include <SDL2/SDL_ttf.h>

class ListBoxWidget : public Widget {
public:
    ListBoxWidget(const std::string& id, int x, int y, int w, const std::vector<std::string>& items, const std::string& font_path, int font_size, std::function<void(int, const std::string&)> onSelect);
    ~ListBoxWidget();

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event, SDL_Renderer* renderer) override;

    json toJson() const override;
    void fromJson(const json& j) override;

private:
    std::vector<std::string> items;
    std::function<void(int, const std::string&)> select_handler;

    TTF_Font* font;
    int item_height;
    int selected_index;
    int hovered_index;

    SDL_Color bg_color;
    SDL_Color hover_color;
    SDL_Color select_color;
    SDL_Color text_color;
};

#endif //LISTBOXWIDGET_H
