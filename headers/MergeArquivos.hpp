#pragma once

#ifndef MERGE_ARQUIVOS
#define MERGE_ARQUIVOS

#include "Logger.hpp"
#include <string>
#include "Semafaro.hpp"
#include "Buffer.hpp"
#include "GeradorNomeRun.hpp"

class MergeArquivos {
private:
    Logger* log;
    int quantidadeDeSlots;
    BufferClass buffer;
    Semafaro semafaro;
    

public:
    MergeArquivos(Logger *pLog, int quantidadeDeSlotsBuffer, int TamSemafaro);
    //int merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida);
    int merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida);
};

#endif