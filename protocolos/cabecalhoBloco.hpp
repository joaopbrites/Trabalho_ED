#pragma once

#ifndef CABECALHO_BLOCO
#define CABECALHO_BLOCO

#include <cstdint>

using namespace std;


struct cabecalhoParaBloco {
    uint64_t id_bloco;            // Identificador único deste bloco
    uint64_t qtd_registros_validos; // Quantidade de registros atualmente ocupados no bloco
};

#endif
