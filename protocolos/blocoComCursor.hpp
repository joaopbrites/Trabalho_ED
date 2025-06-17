#pragma once

#ifndef BLOCO_COM_CURSOR
#define BLOCO_COM_CURSOR

#include "type_block.hpp"
#include "mock.hpp"

struct BlocoComCursor {
    BlocoRegistros bloco;  // O bloco de registros carregado
    int indiceAtual;       // Posição atual de leitura no bloco
    int indiceFinal;       // Quantidade de registros válidos
    bool ativo;            // Se ainda possui registros a serem lidos
    int idBloco;           // Identificador do bloco original no arquivo

    BlocoComCursor() :  bloco(mock::blocoMock), indiceAtual(0), indiceFinal(0), ativo(false), idBloco(-1) {}

    BlocoComCursor(const BlocoRegistros& b, int id)
        : bloco(b), indiceAtual(0), ativo(true), idBloco(id)
    {
        indiceFinal = b.getContagemRegistros();
    }
};

#endif
