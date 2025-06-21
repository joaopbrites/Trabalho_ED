#pragma once


#ifndef LEITOR_CSV
#define LEITOR_CSV

#include "type_dado.hpp"
#include "type_block.hpp"
#include <fstream>

class LeitorCSV {
private:
    ifstream arquivo;
    string bufferLinha;
    bool cabecalhoIgnorado;

public:
    LeitorCSV(const char* caminhoCSV);
    ~LeitorCSV();
    bool lerProximo(Registro& registroOut);
    bool estaAberto() const;
    void fechar();
    bool gerarBloco(BlocoRegistros &saida);
    bool chegouAoFim() const;
};

#endif
