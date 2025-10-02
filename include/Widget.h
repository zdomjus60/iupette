#ifndef WIDGET_H
#define WIDGET_H

#include <SDL2/SDL.h>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class Widget {
public:
    Widget(const std::string& id = "");
    Widget(const std::string& id, int x, int y, int w, int h);
    virtual ~Widget();

    virtual void draw(SDL_Renderer* renderer);
    virtual void handleEvent(const SDL_Event& event, SDL_Renderer* renderer);
    virtual void onFocus();
    virtual void onBlur();

    bool contains(int x, int y) const;

    // Virtual methods for JSON serialization/deserialization
    virtual json toJson() const;
    virtual void fromJson(const json& j);

    std::string getId() const { return id; }

    public:
    std::string id;
    int x, y, width, height;
    bool is_focused;
};

#endif //WIDGET_H
