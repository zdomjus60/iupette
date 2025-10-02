#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include "Widget.h"
#include "LabelWidget.h"
#include <functional>
#include <memory>

class ButtonWidget : public Widget {
public:
    ButtonWidget(const std::string& id, int x, int y, int w, int h, const std::string& text, const std::string& font_path, int font_size, std::function<void()> onClick);

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event, SDL_Renderer* renderer) override;

    json toJson() const override;
    void fromJson(const json& j) override;

    void setText(const std::string& new_text);

private:
    std::unique_ptr<LabelWidget> label;
    std::function<void()> click_handler;

    SDL_Color bg_color_up;
    SDL_Color bg_color_down;
    bool is_pressed;
    Uint32 press_start_time;
    bool visually_pressed;
};

#endif //BUTTONWIDGET_H
