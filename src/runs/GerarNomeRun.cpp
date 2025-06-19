
#include "GeradorNomeRun.hpp"
#include "config.hpp"
#include <string>

GerarNomeRun::GerarNomeRun(int pEtapa, int pNArquivo) : nomeBase(REGRAS::CAMINHO_BASE_RUN + to_string(pEtapa) + "_"),
                                                        etapa(pEtapa),
                                                        nArquivo(pNArquivo)
{
}
string GerarNomeRun::getNomeRun()
{
    nArquivo++;
    return nomeBase + to_string(nArquivo - 1);
}