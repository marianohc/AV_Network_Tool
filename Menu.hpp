#pragma once

#include <string>
#include <iostream>
#include "AppContext.hpp"
#include "Helper_ANSI.hpp"

class Menu {

    AppContext& m_context;

public:

    Menu(AppContext& context) : m_context{context} {}
    int Mostrar_Menu_Principal();
    std::string Ingresar_Token();

    void Ejecutar_Opcion_Principal(int opcion);
    void mostrarSubmenuTipoA();
    void ejecutarComandoTipoA(int subopcion);


};