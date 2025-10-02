#ifndef COMBOBOXWIDGET_H
#define COMBOBOXWIDGET_H

#include "Widget.h"
#include "TextBoxWidget.h"
#include "ButtonWidget.h"
#include "ListBoxWidget.h"
#include <memory>

class ComboBoxWidget : public Widget {
public:
    ComboBoxWidget(const std::string& id, int x, int y, int w, const std::vector<std::string>& items, const std::string& font_path, int font_size);

    void draw(SDL_Renderer* renderer) override;
    void handleEvent(const SDL_Event& event, SDL_Renderer* renderer) override;
    void onBlur() override;

    json toJson() const override;
    void fromJson(const json& j) override;

    void setOnSelectCallback(std::function<void(int, const std::string&)> callback);

private:
    void toggleDropdown();
    void selectItem(int index, const std::string& item);

    std::unique_ptr<TextBoxWidget> display_box;
    std::unique_ptr<ButtonWidget> dropdown_button;
    std::unique_ptr<ListBoxWidget> option_list;

    bool is_open;
    std::string selected_text;
    std::function<void(int, const std::string&)> selection_callback;
};

#endif //COMBOBOXWIDGET_H
