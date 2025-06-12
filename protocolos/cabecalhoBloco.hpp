#pragma once

#ifndef CABECALHO_BLOCO
#define CABECALHO_BLOCO

#include <cstdint>

using namespace std;


struct cabecalhoParaBloco {
    uint32_t id_bloco;            // Identificador único deste bloco
    uint32_t qtd_registros_validos; // Quantidade de registros atualmente ocupados no bloco
    float chave_min_no_bloco; // Tipo da sua chave (ex: int, string fixa)
    float chave_max_no_bloco;   // Tipo da sua chave

};

#endif
