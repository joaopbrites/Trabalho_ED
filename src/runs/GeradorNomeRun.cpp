
#include "GeradorNomeRun.hpp"
#include "config.hpp"
#include <string>

using namespace std;
GerarNomeRun::GerarNomeRun(int pEtapa, int pNArquivo) : nomeBase(REGRAS::CAMINHO_BASE_RUN),
                                                        etapa(pEtapa),
                                                        nArquivo(pNArquivo)
{
    nomeBase.append(to_string(etapa));
    nomeBase.append("_");
}

string GerarNomeRun::getNomeRun()
{
    if (nArquivo != -1)
    {
        string aux = nomeBase;
        aux.append(to_string(nArquivo));
        aux.append(".Bin");
        nArquivo++;
        return aux;
    }
    return nomeBase;
}
GerarNomeRun::GerarNomeRun(string pNomeFinal) : nomeBase(pNomeFinal),
                                                etapa(-1),
                                                nArquivo(-1)
{
}
