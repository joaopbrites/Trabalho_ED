#pragma once

#ifndef INICIALIZADOR_SISTEMA_HPP
#define INICIALIZADOR_SISTEMA_HPP


#include <iostream>
#include <fstream>

using namespace std;

class InicializadorSistema
{
private:
    ifstream arquivoLeitura;
    void lerTemp();
    void converta();
    void ordenarArquivoDecrescente(); // converta a base sorteada para o grupo no Campus Virtual em arquivo binário
    void verificarOrdenacao(); 
    void view();
public:
    InicializadorSistema();
};

#endif