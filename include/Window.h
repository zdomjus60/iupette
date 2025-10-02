#ifndef WINDOW_H
#define WINDOW_H

#include "Widget.h"
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "json.hpp"

using json = nlohmann::json;

const int CLIENT_BUFFER_SIZE = 4096;

class Window {
public:
    Window(const char* title, int width, int height);
    Window(const char* title, int width, int height, const char* server_ip);
    ~Window();

    void addWidget(Widget* widget);
    void run();
    void stop();

    // Client communication methods
    void connectToServer(const char* server_ip);
    void disconnectFromServer();
    void sendJsonMessage(const json& message);
    json receiveJsonMessage();

private:
    void setFocus(Widget* widget);

    const char* window_title;
    SDL_Window* sdl_window;
    SDL_Renderer* sdl_renderer;
    std::vector<Widget*> children;
    std::map<std::string, Widget*> widget_map; // Map to store widgets by ID
    Widget* focused_widget;
    bool running_flag; // To control the run loop

    // Client socket members
    int client_socket_fd; // Moved here
    sockaddr_in server_address; // Moved here
};

#endif //WINDOW_H
