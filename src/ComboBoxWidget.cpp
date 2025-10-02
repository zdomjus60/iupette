#include "ComboBoxWidget.h"
#include <iostream>

ComboBoxWidget::ComboBoxWidget(const std::string& id, int x, int y, int w, const std::vector<std::string>& items, const std::string& font_path, int font_size) : Widget(id) {
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = 40; // Fixed height for the main box
    this->is_open = false;

    // Create the display box (non-editable for now)
    display_box = std::make_unique<TextBoxWidget>(this->id + "_display_box", x, y, w - 40, 40, font_path, font_size);

    // Create the dropdown button
    dropdown_button = std::make_unique<ButtonWidget>(this->id + "_dropdown_button", x + w - 40, y, 40, 40, "v", font_path, font_size, [this](){
        this->toggleDropdown();
    });

    // Create the options list (positioned below the main box)
    option_list = std::make_unique<ListBoxWidget>(this->id + "_option_list", x, y + 40, w, items, font_path, font_size, [this](int index, const std::string& item){
        this->selectItem(index, item);
    });

    // Select the first item by default
    if (!items.empty()) {
        selectItem(0, items[0]);
    }
}

void ComboBoxWidget::toggleDropdown() {
    is_open = !is_open;
    // When opening, give focus to the listbox to handle hover/clicks
    if (is_open) {
        option_list->onFocus();
    } else {
        option_list->onBlur();
    }
}

void ComboBoxWidget::selectItem(int index, const std::string& item) {
    selected_text = item;
    display_box->setText(item);
    is_open = false;
    option_list->onBlur();
    if (selection_callback) {
        selection_callback(index, item);
    }
}

void ComboBoxWidget::setOnSelectCallback(std::function<void(int, const std::string&)> callback) {
    selection_callback = callback;
}

void ComboBoxWidget::draw(SDL_Renderer* renderer) {
    display_box->draw(renderer);
    dropdown_button->draw(renderer);
    if (is_open) {
        option_list->draw(renderer);
    }
}

void ComboBoxWidget::handleEvent(const SDL_Event& event, SDL_Renderer* renderer) {
    // Always pass events to the button
    dropdown_button->handleEvent(event, renderer);

    // Only pass events to the list if it's open
    if (is_open) {
        option_list->handleEvent(event, renderer);
    }

    // If a click happens outside the combo, close the dropdown
    if (event.type == SDL_MOUSEBUTTONDOWN && is_open) {
        if (!contains(event.button.x, event.button.y)) {
            is_open = false;
            option_list->onBlur();
        }
    }
}

void ComboBoxWidget::onBlur() {
    // When the whole combobox loses focus, close the dropdown
    is_open = false;
    option_list->onBlur();
}

json ComboBoxWidget::toJson() const {
    json j = Widget::toJson();
    j["type"] = "ComboBoxWidget";
    j["selected_text"] = selected_text;
    j["is_open"] = is_open;
    j["display_box"] = display_box->toJson();
    j["dropdown_button"] = dropdown_button->toJson();
    j["option_list"] = option_list->toJson();
    return j;
}

void ComboBoxWidget::fromJson(const json& j) {
    Widget::fromJson(j);
    if (j.contains("selected_text")) {
        selected_text = j["selected_text"].get<std::string>();
        display_box->setText(selected_text);
    }
    if (j.contains("is_open")) {
        is_open = j["is_open"].get<bool>();
    }
    if (j.contains("display_box")) {
        display_box->fromJson(j["display_box"]);
    }
    if (j.contains("dropdown_button")) {
        dropdown_button->fromJson(j["dropdown_button"]);
    }
    if (j.contains("option_list")) {
        option_list->fromJson(j["option_list"]);
    }
}