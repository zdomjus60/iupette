#include "iupette.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <memory>

const std::string FONT_PATH = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";

// Global pointer to the main window, so the connect button can access it
std::unique_ptr<Window> main_window_ptr;

int main() {
    try {
        // Initial setup for IP input and Connect button
        Window setup_window("Iupette Setup", 400, 200, "127.0.0.1");

        LabelWidget ip_label("ip_label", 20, 20, "Server IP:", FONT_PATH, 20, {255, 255, 255, 255});
        TextBoxWidget ip_input("ip_input", 120, 20, 200, 30, FONT_PATH, 20);
        ip_input.setText("127.0.0.1"); // Default to localhost

        bool connected_to_server = false;
        ButtonWidget connect_button("connect_button", 120, 70, 100, 40, "Connect", FONT_PATH, 24, [&](){
            std::string server_ip = ip_input.toJson()["text"].get<std::string>();
            std::cout << "Attempting to connect to: " << server_ip << std::endl;
            // For now, we'll just print it.
            // The actual connection logic will be more complex, involving re-initializing the main_window_ptr
            // with the new IP and then switching to the main application loop.
            // This part needs to be handled outside the lambda, after setup_window.run() exits.
            connected_to_server = true;
            setup_window.stop();
        });

        setup_window.addWidget(&ip_label);
        setup_window.addWidget(&ip_input);
        setup_window.addWidget(&connect_button);

        setup_window.run();

        if (connected_to_server) {
            std::string server_ip_str = ip_input.toJson()["text"].get<std::string>();
            const char* server_ip_cstr = server_ip_str.c_str();

            main_window_ptr = std::make_unique<Window>("Iupette App", 800, 600, server_ip_cstr);

            LabelWidget title("main_title", 50, 20, "Iupette Demo", FONT_PATH, 28, {255, 255, 255, 255});

            std::vector<std::string> items = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
            ComboBoxWidget combobox("main_combobox", 50, 80, 300, items, FONT_PATH, 24);

            main_window_ptr->addWidget(&title);
            main_window_ptr->addWidget(&combobox);

            main_window_ptr->run();
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
