#pragma once

#ifndef BUFFER
#define BUFFER

#include "type_block.hpp"
#include "config.hpp"
#include "type_dado.hpp"
#include "Logger.hpp"
#include "LeitorCSV.hpp"
#include "LeitorBin.hpp"

using namespace std;

class BufferClass
{
private:
    Logger *log;
    BlocoRegistros buffer[REGRAS::QUANTIDADES_DE_SLOTS_BUFFER];
    int slotMaiorElemento;

    bool atualizaSlotMaior();

public:
    BufferClass(Logger *pLog);
    BufferClass(LeitorCSV &leitor, Logger *pLog);
    BufferClass(LeitorBin &leitor, Logger *pLog);

    bool slotVazio(int indice);
    bool bufferVazio();
    bool bufferCheio();
    bool slotCheio(int indice);

    bool setSlot(int indice, BlocoRegistros Novo);
    bool getSlot(int indice, BlocoRegistros &Saida);

    bool pullMaior(Registro &Saida);
    bool pullMaiorSlot(Registro &Saida, int slot);
};

#endif