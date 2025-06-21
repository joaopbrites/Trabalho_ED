#pragma once

#ifndef MERGE_ARQUIVOS
#define MERGE_ARQUIVOS

#include <string>
#include "Semafaro.hpp"
#include "Buffer.hpp"
#include "GeradorNomeRun.hpp"
#include "LeitorBinArray.hpp"
#include "GravarBinBlocos.hpp"

class MergeArquivos {
private:

    int quantidadeDeSlots;
    BufferClass buffer;

    int carregarBlocosParaBuffer(LeitorBinArray& slots, Semafaro &semafaroArquivos, int tamVariaveis);
    void processarBuffer(GravadorDeBlocos& gravador, BlocoRegistros& blocoSaida);

public:
    MergeArquivos(int quantidadeDeSlotsBuffer);
    //int merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida);
    int merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida);

};

#endif