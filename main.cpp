#include <iostream>
#include <string>
#include <memory>
#include "ArgsHelper.hpp"
#include "TCPClient.hpp"
#include "InputSource.hpp"
#include "Keyboard_Input.hpp"
#include "File_Input.hpp"
#include "Helper_ANSI.hpp"
#include "Parser_Helper.hpp"
#include "AppContext.hpp"
#include "Menu.hpp"




//static int Mostrar_Menu_Inicial(AppContext& cont) {
//
//    std::cout << ansi::clear 
//              << "Optins:" << std::endl
//              << "1. Change Token (Actual Token: " << cont.get_token_id() << ")" << std::endl
//              << "2. Keyboard (binary data - Format xx yy zz)" << std::endl
//              << "  3. Keyboard (binary data - Format xx yy zz) + checksum" << std::endl
//              << "  4. Text file" << std::endl
//              << "  5. Binary file" << std::endl
//              << " 99. Set Token" << std::endl
//              << "  9. Exit" << std::endl << std::endl
//              << std::endl
//              << "Token: " << cont.get_token_id() << std::endl
//              << "Choice: ";
//
//    int choice = 0;
//    std::cin >> choice;
//    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//    return choice;
//}

int main(int argc, char* argv[]) {

    AppContext context;
    Menu menu(context);

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

        // Bloqueo aca a proposito, hasta que se conecte al servidor. Mas adelante agregarle
        // un timeout, o el escape con alguna tecla.
        while(false == client.get_conection_state()) {
        }

        while (true) {
            std::unique_ptr<InputSource> input;
            int ingreso = menu.Mostrar_Menu_Principal();
            
            if (ingreso == 1) {
                input = std::make_unique<KeyboardInput>();
                std::string line;
                do {
                    
                    std::cout << "Type text to send (exit to finish) > ";

                    input->get_next(line);
                    if (line == "exit") {
                        std::cout << ansi::clear;
                        break;
                    }
                    std::cout << "Sending message..." << std::endl;
                    client.send(line + "\n");

                    while(false == client.get_last_message_state());
                    std::cout << ansi::green << "Message sent!" << ansi::white << std::endl << std::endl;
                } while(true);

            } else if (ingreso == 2) {

                input = std::make_unique<KeyboardInput>();
                std::string line;
                do {
                    
                    
                    std::cout << "Type text to send - Format 00 0a fe (binary number spaces separated) > ";

                    input->get_next(line);
                    if (line == "exit") {
                        std::cout << ansi::clear;
                        break;
                    }
                    
                    std::vector<uint8_t> binary_vector = Parse_Helper::parse_hex_string(line);
                    std::cout << "Sending binary data..." << std::endl;
                    client.send(binary_vector);

                    while(false == client.get_last_message_state());
                    std::cout << ansi::green << "Message sent!" << ansi::white << std::endl << std::endl;
                } while(true);

            } else if (ingreso == 3) {

                input = std::make_unique<KeyboardInput>();
                std::string line;

                do {
                                        
                    std::cout << "Type text to send - Format 00 0a fe (binary number spaces separated) > ";

                    input->get_next(line);
                    if (line == "exit") {
                        std::cout << ansi::clear;
                        break;
                    }
                    
                    std::vector<uint8_t> binary_vector = Parse_Helper::parse_hex_string(line);
                    std::cout << "Sending binary data..." << std::endl;
                    client.send(binary_vector);

                    while(false == client.get_last_message_state());
                    std::cout << ansi::green << "Message sent!" << ansi::white << std::endl << std::endl;
                } while(true);

            } else if (ingreso == 4) {
                std::string path;
                std::cout << "Enter file path: ";
                std::getline(std::cin, path);
                input = std::make_unique<FileInput>(path);
            } else if (ingreso == 5) {

            } else if (ingreso == 9) {
                break;
            } else if (ingreso == 8) {

                std::string token_aux;
                token_aux = menu.Ingresar_Token();
                if (token_aux.size() != 16) {
                    std::cout << "Token no aceptado. Debe tener 16 digitos" << std::endl;     
                } else {
                    context.set_token_id(token_aux);
                }
            }
        }

        client.stop();
        std::cout << ansi::cyan << "H" <<
                     ansi::white << "a" <<
                     ansi::cyan << "v" <<
                     ansi::white << "e " <<
                     ansi::cyan << "a " <<
                     ansi::white << "n" <<
                     ansi::cyan << "i" <<
                     ansi::white << "c" <<
                     ansi::cyan << "e " <<
                     ansi::white << "d" <<
                     ansi::cyan << "a" <<
                     ansi::white << "y" << std::endl <<std::endl;
                  
    } catch (const std::exception& e) {
        std::cerr << "[Exception] " << e.what() << "\n";
        return 1;
    }

    return 0;
}

