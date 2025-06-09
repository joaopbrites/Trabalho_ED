#include <iostream>
#include <cstdint>

struct cabecalhoParaArquivo {
    uint32_t numero_magico; //define se é um arquivo do merge ou se é o principal
    uint16_t versao_formato_arquivo;  
    uint16_t reservado_alinhamento;
    // --- Contadores Globais para o Arquivo/Segmento ---
    uint64_t qtd_total_blocos_no_arquivo;   // Número total de blocos de dados neste arquivo.
    uint64_t qtd_total_registros_no_arquivo;// Número total de registros neste arquivo.

    // --- Integridade ---
    uint32_t checksum_cabecalho_arquivo; // Checksum/CRC32 apenas desta estrutura de cabeçalho.
};


