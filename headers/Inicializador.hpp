#pragma once

#ifndef INICIALIZADOR_SISTEMA_HPP
#define INICIALIZADOR_SISTEMA_HPP


#include <string>
#include "LeitorCSV.hpp"


using namespace std;

class InicializadorSistema
{
public:
    InicializadorSistema();
    void start(const char* nomeArquivoEntrada, const char* nomeArquivoSaida);
private:
    void gerarRuns(LeitorCSV &leitorCSV, int &quantidade);
    void ordenarArquivoDecrescente(int &quantidade, const char* nomeArquivoSaida); // converta a base sorteada para o grupo no Campus Virtual em arquivo binário
    void verificarOrdenacao(); 
    void view();  
};

#endif