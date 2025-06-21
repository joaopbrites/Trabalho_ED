// gravador_blocos.hpp
#pragma once

#ifndef GRAVAR_BLOCO_BIN
#define GRAVAR_BLOCO_BIN


#include "type_block.hpp"
#include "type_dado.hpp"
#include "cabecalhoArquivo.hpp"
#include "config.hpp"
#include <fstream>

class GravadorDeBlocos {
private:
    ofstream arquivoSaida;
    cabecalhoParaArquivo cabecalhoArquivo;
    string nomeAquivo;
public:
    GravadorDeBlocos(const string caminhoSaida);
    ~GravadorDeBlocos();
    bool escreverBloco(BlocoRegistros& bloco);
    void finalizar();
};

#endif
