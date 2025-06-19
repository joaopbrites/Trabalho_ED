#pragma once

#ifndef REGRAS_DE_NEGOCIO
#define REGRAS_DE_NEGOCIO

#include <cstdint>


using namespace std;

namespace REGRAS
{
    constexpr int QUANTIDADE_REGISTROS = 100;

    constexpr int QUANTIDADES_DE_SLOTS_BUFFER = 4;
    constexpr int TAMANHO_BUFFER = QUANTIDADE_REGISTROS / QUANTIDADES_DE_SLOTS_BUFFER;

    //ocupacao por bloco é para deixar uma margem que vai ser usada para inserir por posição 
    //eliminando a necessidade de mexer em todos os bloco seguintes
    constexpr float OCUPACAO_POR_BLOCO = 0.80;

    constexpr int TAMANHO_BUFFER_MARGEM = TAMANHO_BUFFER * OCUPACAO_POR_BLOCO;

    constexpr char NOME_ARQUIVO_ENTRADA[] = "lms-jun22qtr-csv.csv\0";
    constexpr char NOME_ARQUIVO_BIN_SAIDA[] = "db.dat";
    
    constexpr char CAMINHO_BASE_RUN[] = "../../data/runs/run_etapa_";

    constexpr int TAMANHO_CAMPO_REG = 100;

    
}

namespace FLAGS
{
    constexpr uint8_t ATIVO = 0x01;
    constexpr uint8_t REMOVIDO = 0x02;
    constexpr uint8_t VAZIO = 0X03;
    constexpr uint8_t  INVALIDO = 0X04;
}

namespace INVALID_VALUES
{
    constexpr uint32_t ID_BLOCK = -1;           
    constexpr float CHAVE_MIN_NO_BLOCO = -1111.11; 
    constexpr float CHAVE_MAX_NO_BLOCO = -1111.11;

    constexpr float CHAVE_REG = -1111.11;
    constexpr float DATA_VALUE = -1111.11;
    constexpr int MAGNITUDE = -11111;
}


#endif