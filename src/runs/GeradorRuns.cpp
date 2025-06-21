#include "GeradorRuns.hpp"
#include "LeitorCSV.hpp"
#include "Buffer.hpp"
#include "GravarBinBlocos.hpp"
#include "GeradorNomeRun.hpp"

using namespace std;

GerarRuns::GerarRuns(LeitorCSV *leitorPtr, GerarNomeRun &pNome)
    : leitor(leitorPtr), indice(0), nome(&pNome) {}

GerarRuns::~GerarRuns() {}

bool GerarRuns::gerarRun()
{
    if (leitor->chegouAoFim())
    {
        throw runtime_error("Arquivo de leitura contém erro");
        return false;
    }
    BlocoRegistros saida;
    while (!leitor->chegouAoFim())
    {
        BufferClass buffer;
        BlocoRegistros bAux;
        for (int i =0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER;i++)
        {
            leitor->gerarBloco(bAux);
            buffer.setSlot(i, bAux);
        }
        string nomeArquivo = nome->getNomeRun();
        GravadorDeBlocos gravador(nomeArquivo);
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
            if (saida.getContagemRegistros() > 0)
            {
                gravador.escreverBloco(saida);
                saida.esvaziar();
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
