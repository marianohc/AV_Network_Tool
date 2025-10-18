#include "Menu.hpp"

int Menu::Mostrar_Menu_Principal() {
    std::cout << ansi::clear << std::endl
              << "**************************************************************" << std::endl
              << "* Select input source:                                       *" << std::endl
              << "* [1] Keyboard (ASCII text)                                  *" << std::endl
              << "* [2] Keyboard (binary data - Format xx yy zz)               *" << std::endl
              << "* [3] Keyboard (binary data - Format xx yy zz) + checksum    *" << std::endl
              << "* [4] Text file                                              *" << std::endl
              << "* [5] Binary file                                            *" << std::endl
              << "* [8] Set new token                                          *" << std::endl
              << "* [9] Exit                                                   *" << std::endl
              << "**************************************************************" << std::endl
              << "Current Token: " << m_context.get_token_id() << std::endl
              << "**************************************************************" << std::endl <<std::endl
              << "Your choice: ";

    int choice = 0;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string Menu::Ingresar_Token() {

    std::cout << ansi::clear 
              << "******************************************************************" << std::endl
              << "* Ingresar nuevo token - Formato 1234567890abcdef (16 digitos)   *" << std::endl
              << "* Nuevo Token > ";

    std::string token;
    std::cin >> token;
    return token;
}

void Menu::Ejecutar_Opcion_Principal(int opcion) {
    switch (opcion) {
        case 1:
            mostrarSubmenuTipoA();
            break;
        case 2:
            // mostrarSubmenuTipoB();
            break;
        case 3:
            // mostrarSubmenuTipoC();
            break;
        case 0:
            std::cout << "Saliendo...\n";
            return;
        default:
            std::cout << "Opción inválida.\n";
            break;
    }
}

void Menu::mostrarSubmenuTipoA() {
    std::cout << "\n=== COMANDOS TIPO A ===\n";
    std::cout << "1. Enviar pago\n";
    std::cout << "2. Consultar saldo\n";
    std::cout << "0. Volver\n";
    std::cout << "Elija una opción: ";

    int sub;
    std::cin >> sub;
    ejecutarComandoTipoA(sub);
}

void Menu::ejecutarComandoTipoA(int sub) {
    switch (sub) {
        case 1: {
            double monto;
            std::cout << "Ingrese el importe: ";
            std::cin >> monto;
            std::cout << "Enviando comando de pago por $" << monto;
//                      << " con token: " << ctx.getToken() << "\n";
            break;
        }
        case 2:
            std::cout << "Consultando saldo...\n";
            break;
        case 0:
            Mostrar_Menu_Principal();
            break;
        default:
            std::cout << "Opción inválida.\n";
            break;
    }
}
