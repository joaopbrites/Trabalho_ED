#pragma once

#ifndef BUFFER
#define BUFFER

#include "type_block.hpp"
#include "config.hpp"
#include "type_dado.hpp"
#include "LeitorCSV.hpp"
#include "LeitorBin.hpp"

using namespace std;

class BufferClass
{
private:
    BlocoRegistros buffer[REGRAS::QUANTIDADES_DE_SLOTS_BUFFER];
    int slotMaiorElemento = -1;

    bool atualizaSlotMaior();

public:
    BufferClass();
    BufferClass(LeitorBin *leitor);

    bool slotVazio(int indice);
    bool bufferVazio();
    bool bufferCheio();
    bool slotCheio(int indice);

    bool setSlot(int indice, BlocoRegistros Novo);
    bool getSlot(int indice, BlocoRegistros &Saida);

    bool pullMaior(Registro &Saida);
    bool pullMaiorEvent(Registro &Saida, bool &slotEsvaziou);
    bool pullMaiorSlot(Registro &Saida, int slot);
};

#endif