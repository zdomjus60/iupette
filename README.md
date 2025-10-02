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

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
