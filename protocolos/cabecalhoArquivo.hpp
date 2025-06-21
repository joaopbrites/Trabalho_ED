#pragma once

#ifndef CABECALHO_ARQUIVO
#define CABECALHO_ARQUIVO



#include <cstdint>

struct cabecalhoParaArquivo {
    uint64_t qtd_total_blocos_no_arquivo;   // Número total de blocos de dados neste arquivo.
    uint64_t qtd_total_registros_no_arquivo;// Número total de registros neste arquivo.
    uint64_t proximo_id_bloco;
};

#endif
