#include "RectangleWidget.h"
#include <iostream>

RectangleWidget::RectangleWidget(int x, int y, int w, int h, SDL_Color color) : Widget("", x, y, w, h), click_handler(nullptr) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    rect_color = color;
}

void RectangleWidget::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, rect_color.r, rect_color.g, rect_color.b, rect_color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void RectangleWidget::setColor(SDL_Color color) {
    rect_color = color;
}

void RectangleWidget::handleEvent(const SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT && contains(event.button.x, event.button.y)) {
            if (click_handler) {
                click_handler();
            }
        }
    }
}

void RectangleWidget::setClickHandler(std::function<void()> handler) {
    click_handler = handler;
}
