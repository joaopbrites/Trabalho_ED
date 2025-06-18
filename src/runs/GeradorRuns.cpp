#include "GeradorRuns.hpp"
#include "LeitorCSV.hpp"
#include "Logger.hpp"
#include "Buffer.hpp"
#include "GravarBinBlocos.hpp"
#include "GeradorNomeRun.hpp"

using namespace std;

GerarRuns::GerarRuns(LeitorCSV *leitorPtr, Logger *logPtr)
    : leitor(leitorPtr), log(logPtr), indice(0) {}

GerarRuns::~GerarRuns() {}

bool GerarRuns::gerarRun()
{
    GerarNomeRun nome(0, 0);
    if (leitor->chegouAoFim())
    {
        if (log)
            log->error("Arquivo de leitura contém erro");
        return false;
    }
    while (!leitor->chegouAoFim())
    {

        BufferClass buffer(*leitor, log);
        string nomeArquivo = nome.getNomeRun();;
        GravadorDeBlocos gravador(nomeArquivo, log);

        // Escreve todos os blocos do buffer no arquivo
        while (!buffer.bufferVazio())
        {
            BlocoRegistros saida;
            Registro maior;
            for (int i = 0; i < REGRAS::OCUPACAO_POR_BLOCO && buffer.pullMaior(maior);i++)
            {
                saida.push_back(maior);
            }
            gravador.escreverBloco(saida);
        }
        if (log)
            log->info("Run numero (" + to_string(indice) + ") gerada e salva em: " + nomeArquivo);
        indice++;
    } // Avança o índice para a próxima run
    return true;
}

int GerarRuns::quantArquivosGerado()
{
    return indice;
}
