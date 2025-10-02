#ifndef TEXTBOXWIDGET_H
#define TEXTBOXWIDGET_H

#include "LabelWidget.h" // Reuse Label for text rendering

class TextBoxWidget : public Widget {
public:
    TextBoxWidget(const std::string& id, int x, int y, int w, int h, const std::string& font_path, int font_size);
    ~TextBoxWidget();

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event, SDL_Renderer* renderer) override;
    void onFocus() override;
    void onBlur() override;

    void setText(const std::string& new_text);

    json toJson() const override;
    void fromJson(const json& j) override;
    void updateTextTexture(SDL_Renderer* renderer);

    std::string text;
    size_t cursor_pos;

    // Rendering
    TTF_Font* font;
    SDL_Color text_color;
    SDL_Color bg_color;
    SDL_Texture* text_texture;
    SDL_Rect text_rect;

    // Cursor
    Uint32 cursor_blink_time;
    bool show_cursor;
    bool texture_needs_update;
};

#endif //TEXTBOXWIDGET_H
