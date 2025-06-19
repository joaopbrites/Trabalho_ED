#pragma once

#ifndef GERADOR_NOMES_RUN
#define GERADOR_NOMES_RUN

#include <string>
#include "config.hpp"


class GerarNomeRun
{
private: 
    const string nomeBase;
    const int etapa;
    int nArquivo;
public:
    GerarNomeRun(int pEtapa, int pNArquivo);
    string getNomeRun();
};

#endif