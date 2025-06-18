#pragma once

#ifndef GERADOR_RUNS
#define GERADOR_RUNS

#include "LeitorCSV.hpp"
#include "Logger.hpp"
#include "Buffer.hpp"
#include "type_dado.hpp" // Supondo que existe um tipo de dado a ser gravado


class GerarRuns
{
private:
    LeitorCSV* leitor;
    Logger* log;
    size_t indice;

public:
    GerarRuns(LeitorCSV* leitorPtr, Logger* logPtr);
    ~GerarRuns();

    // Gera uma run e grava no arquivo especificado
    bool gerarRun(const char* nomeArquivo);
    int quantArquivosGerado();
};



#endif