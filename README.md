# Iupette

Iupette is a C++ GUI library project. This repository contains the core library, a client application, and a server application.

## Project Structure

- `src/`: Contains the source code for the Iupette library.
- `include/`: Contains the header files for the Iupette library.
- `server/`: Contains the source code for the server application.
- `examples/`: Contains example applications (e.g., a widget demo).
- `test_client.py`: A Python client for testing the server.

## Building the Project

This project uses CMake for its build system. Follow these steps to build the project:

1.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

2.  **Run CMake to configure the project:**
    ```bash
    cmake ..
    ```
    *Note: Ensure you have SDL2 and SDL2_ttf development libraries installed on your system. On Debian/Ubuntu, you can install them using `sudo apt-get install libsdl2-dev libsdl2-ttf-dev`.*

3.  **Build the project:**
    ```bash
    make
    ```

This will build the `iupette` shared library, `iupette_app` client, and `iupette_server` executable.

## Running the Applications

1.  **Start the server:**
    Open a terminal, navigate to the `build` directory, and run:
    ```bash
    ./iupette_server
    ```

2.  **Run the client application:**
    Open another terminal, navigate to the `build` directory, and run:
    ```bash
    ./iupette_app
    ```
    The client application will attempt to connect to the server.

## Creating Custom Widgets

To create your own custom widget, you typically need to:

1.  **Create a new header file** (e.g., `include/MyCustomWidget.h`) that inherits from `Widget`.
2.  **Override virtual methods:** At a minimum, you'll likely need to override `draw(SDL_Renderer* renderer)` to define how your widget looks, and `handleEvent(const SDL_Event& event, SDL_Renderer* renderer)` to define how it responds to user input.
3.  **Implement the widget's logic** in a corresponding `.cpp` file (e.g., `src/MyCustomWidget.cpp`).
4.  **Add your new `.cpp` file to `CMakeLists.txt`** in the `add_library(iupette SHARED ...)` command so it gets compiled into the library.
5.  **Include your widget's header** in any application that uses it.

## Acknowledgments

This project makes use of the excellent [JSON for Modern C++](https://github.com/nlohmann/json) library by Niels Lohmann (`json.hpp`), which is included in the `server/include` directory. We are grateful for this powerful and easy-to-use JSON parsing library.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
