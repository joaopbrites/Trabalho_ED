#pragma once


#ifndef LEITOR_CSV
#define LEITOR_CSV

#include "type_dado.hpp"
#include "Logger.hpp"
#include <fstream>

class LeitorCSV {
private:
    ifstream arquivo;
    Logger* log;
    string bufferLinha;
    bool cabecalhoIgnorado;

public:
    LeitorCSV(const char* caminhoCSV, Logger* logger);
    ~LeitorCSV();
    bool lerProximo(Registro& registroOut);
    bool estaAberto() const;
    void fechar();
    bool chegouAoFim();
};

#endif
