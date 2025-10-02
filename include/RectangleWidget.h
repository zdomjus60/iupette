#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

#include "Widget.h"
#include <SDL2/SDL.h>

class RectangleWidget : public Widget {
public:
    RectangleWidget(int x, int y, int w, int h, SDL_Color color);
    void draw(SDL_Renderer* renderer) override;
    void setColor(SDL_Color color);
    void handleEvent(const SDL_Event& event, SDL_Renderer* renderer) override;
    void setClickHandler(std::function<void()> handler);

private:
    SDL_Rect rect;
    SDL_Color rect_color;
    std::function<void()> click_handler;
};

#endif //RECTANGLEWIDGET_H
