#pragma once

#ifndef GERADOR_RUNS
#define GERADOR_RUNS

#include "LeitorCSV.hpp"
#include "Logger.hpp"
#include "Buffer.hpp"
#include "type_dado.hpp"
#include "GeradorNomeRun.hpp" // Supondo que existe um tipo de dado a ser gravado


class GerarRuns
{
private:
    LeitorCSV* leitor;
    Logger* log;
    size_t indice;
    GerarNomeRun *nome;

public:
    GerarRuns(LeitorCSV* leitorPtr, Logger* logPtr, GerarNomeRun &pNome);
    ~GerarRuns();

    // Gera uma run e grava no arquivo especificado
    bool gerarRun();
    int quantArquivosGerado();
};



#endif