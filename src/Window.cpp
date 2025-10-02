#include "Window.h"
#include <iostream>
#include <stdexcept>
#include <cstring> // For memset
#include <arpa/inet.h> // For inet_pton
#include <errno.h> // For errno

const int SERVER_PORT = 8080;

Window::Window(const char* title, int width, int height, const char* server_ip) : window_title(title), focused_widget(nullptr), client_socket_fd(-1), running_flag(true) {
    std::cout << "Window constructor with title: " << title << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(SDL_GetError());
    }

    sdl_window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    if (sdl_window == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl_renderer == nullptr) {
        throw std::runtime_error(SDL_GetError());
    }

    connectToServer(server_ip);
}

Window::~Window() {
    std::cout << "Window destructor" << std::endl;
    disconnectFromServer();
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Window::connectToServer(const char* server_ip) {
    if (client_socket_fd != -1) {
        disconnectFromServer();
    }

    client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd == -1) {
        throw std::runtime_error("Failed to create client socket.");
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        close(client_socket_fd);
        client_socket_fd = -1;
        throw std::runtime_error("Invalid address/ Address not supported");
    }

    if (connect(client_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        close(client_socket_fd);
        client_socket_fd = -1;
        throw std::runtime_error("Connection to server failed.");
    }
    std::cout << "Connected to server at " << server_ip << ":" << SERVER_PORT << std::endl;
}

void Window::disconnectFromServer() {
    if (client_socket_fd != -1) {
        close(client_socket_fd);
        client_socket_fd = -1;
    }
}

void Window::stop() {
    running_flag = false;
}

void Window::addWidget(Widget* widget) {
    children.push_back(widget);
    widget_map[widget->getId()] = widget;
}

void Window::run() {
    SDL_Event event;
    while (running_flag) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running_flag = false;
            }
            // Handle widget events
            for (Widget* widget : children) {
                widget->handleEvent(event, sdl_renderer);
            }
        }

        SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(sdl_renderer);

        for (Widget* widget : children) {
            widget->draw(sdl_renderer);
        }

        SDL_RenderPresent(sdl_renderer);
    }
}
