#pragma once

#ifndef GERADOR_NOMES_RUN
#define GERADOR_NOMES_RUN

#include <string>
#include "config.hpp"


class GerarNomeRun
{
private: 
    string nomeBase;
    const int etapa;
    int nArquivo;
public:
    GerarNomeRun(int pEtapa, int pNArquivo);
    GerarNomeRun(string pNomeFinal);
    string getNomeRun();
};

#endif