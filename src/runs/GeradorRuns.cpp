#include "GeradorRuns.hpp"
#include "LeitorCSV.hpp"
#include "Logger.hpp"
#include "Buffer.hpp"
#include "GravarBinBlocos.hpp"
#include "GeradorNomeRun.hpp"

using namespace std;

GerarRuns::GerarRuns(LeitorCSV *leitorPtr, Logger *logPtr, GerarNomeRun &pNome)
    : leitor(leitorPtr), log(logPtr), indice(0), nome(&pNome) {}

GerarRuns::~GerarRuns() {}

bool GerarRuns::gerarRun()
{
    if (leitor->chegouAoFim())
    {
        if (log)
            log->error("Arquivo de leitura contém erro");
        return false;
    }
    BlocoRegistros saida(log);
    while (!leitor->chegouAoFim())
    {
        BufferClass buffer(log);
        BlocoRegistros bAux(log);
        for (int i =0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER;i++)
        {
            leitor->gerarBloco(bAux);
            buffer.setSlot(i, bAux);
        }
        string nomeArquivo = nome->getNomeRun();
        GravadorDeBlocos gravador(nomeArquivo, log);
        // Escreve todos os blocos do buffer no arquiv
        while (!buffer.bufferVazio())
        {
            Registro maior;
            while (buffer.pullMaior(maior))
            {
                saida.push_back(maior);
                if (saida.estaCheio())
                {
                    gravador.escreverBloco(saida);
                    saida.esvaziar();
                }
            } 
        }
        indice++;
    } // Avança o índice para a próxima run
    return true;
}

int GerarRuns::quantArquivosGerado()
{
    return indice;
}
