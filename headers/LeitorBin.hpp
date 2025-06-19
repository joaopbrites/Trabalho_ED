#pragma once

#ifndef LEITOR_BIN
#define LEITOR_BIN

#include "cabecalhoArquivo.hpp"
#include <fstream>
#include "type_block.hpp"

using namespace std;

class LeitorBin {
private:
    ifstream arquivoEntrada;
    cabecalhoParaArquivo cabecalhoArquivo;
    Logger* log;

public:
    LeitorBin(const string caminho, Logger *pLog);
    ~LeitorBin();
    bool lerProximoBloco(BlocoRegistros& bloco);
    bool posicionarParaBloco(int indice);
    cabecalhoParaArquivo getCabecalho() const;
    bool chegouAoFim() const;
    // Lê o próximo bloco sem alterar a posição do arquivo
    bool lerBlocoAtual(BlocoRegistros& bloco);
};

#endif