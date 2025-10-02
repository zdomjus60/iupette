#ifndef LABELWIDGET_H
#define LABELWIDGET_H

#include "Widget.h"
#include <SDL2/SDL_ttf.h>
#include <string>

class LabelWidget : public Widget {
public:
    LabelWidget(const std::string& id, int x, int y, const std::string& text, const std::string& font_path, int font_size, SDL_Color color);
    ~LabelWidget();

    void draw(SDL_Renderer* renderer) override;
    void setText(const std::string& text);

    json toJson() const override;
    void fromJson(const json& j) override;

private:
    std::string label_text;
    SDL_Color text_color;
    TTF_Font* font;
    SDL_Texture* text_texture;
    SDL_Rect text_rect;

    void createTexture(SDL_Renderer* renderer);
};

#endif //LABELWIDGET_H
