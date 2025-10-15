#include <iostream>
#include <string>
#include <memory>
#include "ArgsHelper.hpp"
#include "TCPClient.hpp"
#include "InputSource.hpp"
#include "Keyboard_Input.hpp"
#include "File_Input.hpp"

static int Mostrar_Menu() {
    std::cout << "Select input source:\n"
              << "  1. Keyboard\n"
              << "  2. File\n"
              << "  9. Exit\n"
              << "Choice: ";

    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

int main(int argc, char* argv[]) {
    ArgsHelper args(argc, argv);
    if (!args.is_valid()) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return 1;
    }

    try {
        TcpClient client;
        client.set_receive_callback([](const std::string& data) {
            std::cout << "[RX] " << data << "\n";
        });

        client.start();
        client.connect(args.host(), static_cast<unsigned short>(args.port()));

        while (true) {
            std::unique_ptr<InputSource> input;
            int ingreso = Mostrar_Menu();

            if (ingreso == 9) break;

            if (ingreso == 2) {
                std::string path;
                std::cout << "Enter file path: ";
                std::getline(std::cin, path);
                input = std::make_unique<FileInput>(path);
            } else {
                std::cout << "Enter text to send: ";
                input = std::make_unique<KeyboardInput>();
            }

            std::string line;
            while (input->get_next(line)) {
                if (line == "exit" || line == "quit") break;
                std::cout << "Then, we need to send: " << line;
                client.send(line + "\n");
            }
        }

        client.stop();
        std::cout << "Disconnected.\n";
    } catch (const std::exception& e) {
        std::cerr << "[Exception] " << e.what() << "\n";
        return 1;
    }

    return 0;
}

