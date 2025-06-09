#include <iostream>
#include <cstdint>

using namespace std;


struct cabecalhoParaBloco {
    uint32_t id_bloco;            // Identificador único deste bloco
    uint32_t qtd_registros_validos; // Quantidade de registros atualmente ocupados no bloco
    uint32_t checksum_ou_hash;    // Hash/CRC para verificação de integridade e dirty bit
    // ChavePrimeiroRegistro chave_min_no_bloco; // Tipo da sua chave (ex: int, string fixa)
    // ChaveUltimoRegistro chave_max_no_bloco;   // Tipo da sua chave
};