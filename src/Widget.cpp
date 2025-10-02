#include "Widget.h"
#include <iostream>

Widget::Widget(const std::string& id) : Widget(id, 0, 0, 100, 100) {
    // Delegated to the other constructor
}

Widget::Widget(const std::string& id, int x, int y, int w, int h)
    : id(id), x(x), y(y), width(w), height(h), is_focused(false) {
    std::cout << "Widget constructor with id: " << id << ", x:" << x << ", y:" << y << ", w:" << w << ", h:" << h << std::endl;
}

Widget::~Widget() {
    std::cout << "Widget destructor" << std::endl;
}

void Widget::draw(SDL_Renderer* renderer) {
    // Base widget does nothing, but derived widgets will override this.
}

void Widget::handleEvent(const SDL_Event& event, SDL_Renderer* renderer) {
    // Do nothing by default
}

void Widget::onFocus() {
    is_focused = true;
}

void Widget::onBlur() {
    is_focused = false;
}

bool Widget::contains(int mx, int my) const {
    return (mx >= x && mx < (x + width) && my >= y && my < (y + height));
}

json Widget::toJson() const {
    json j;
    j["id"] = id;
    j["x"] = x;
    j["y"] = y;
    j["width"] = width;
    j["height"] = height;
    return j;
}

void Widget::fromJson(const json& j) {
    if (j.contains("x")) x = j["x"];
    if (j.contains("y")) y = j["y"];
    if (j.contains("width")) width = j["width"];
    if (j.contains("height")) height = j["height"];
    // id is typically set at construction and not changed via fromJson
}
