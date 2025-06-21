
#include "Inicializador.hpp"
#include "config.hpp"
#include "GeradorRuns.hpp"
#include "GeradorNomeRun.hpp"
#include "MergeArquivos.hpp"
#include <iostream>
#include <stdexcept>


using namespace std;

void imprimirDadosArquivoSaida(string caminhoSaida)
{
    cout << "\n--- Dados do arquivo de saída ---\n";
    LeitorBin leitorSaida(caminhoSaida);
    int blocoIdx = 0;
    bool sentinela = true;
    while (sentinela)
    {
        BlocoRegistros bloco;
        if (leitorSaida.lerProximoBloco(bloco))
        {
            int qtd = bloco.getContagemRegistros();
            cout << "Bloco " << blocoIdx << ": " << qtd << " registros\n";
            for (int i = 0; i < qtd; ++i)
            {
                Registro reg;
                bool ok = bloco.getRegistroPorIndice(i, reg);
                cout << "  [DEBUG] Indice: " << i << ", getRegistroPorIndice: " << ok << ", Status: " << (int)reg.getStatus() << endl;
                if (ok)
                {
                    string s = reg.gerarStringImpressao();
                    if (!s.empty())
                        cout << s << endl;
                }
            }
        }
        else
        {
            sentinela = false;
        }
        blocoIdx++;
    }
    cout << "--- Fim dos dados ---\n";
}


void InicializadorSistema::gerarRuns(LeitorCSV &leitorCSV, int &quantidadeRuns)
{
    int etapa = 0;
    GerarNomeRun nomeBase(etapa, 0);
    GerarRuns geradorRuns(&leitorCSV, nomeBase);
    if (!geradorRuns.gerarRun())
    {
        throw runtime_error("Erro na gerção de runs");
    }

    quantidadeRuns = geradorRuns.quantArquivosGerado();
}

void InicializadorSistema::ordenarArquivoDecrescente(int &quantidade, const char* nomeArquivoSaida)// converta a base sorteada para o grupo no Campus Virtual em arquivo binário
{
    int etapa = 0;
     while (quantidade >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        GerarNomeRun nomeEntrada(etapa, 0);
        GerarNomeRun nomeSaida(etapa + 1, 0);
        MergeArquivos mergeador(REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);
        quantidade = mergeador.merge(quantidade, nomeEntrada, nomeSaida);
        etapa++;
    }

    GerarNomeRun nomeEntrada(etapa, 0);
    GerarNomeRun nomeSaida(nomeArquivoSaida);
    MergeArquivos mergeador(REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);
    mergeador.merge(quantidade, nomeEntrada, nomeSaida);
}
void InicializadorSistema::verificarOrdenacao()
{

}
void InicializadorSistema::view()
{

}
void InicializadorSistema::start(const char* nomeArquivoEntrada, const char* nomeArquivoSaida)
{
    // 1. Particionamento: gera runs a partir do CSV
    LeitorCSV leitorCSV(nomeArquivoEntrada);
    int quantidade = 0;
    try
    {
        this->gerarRuns(leitorCSV, quantidade);
        this->ordenarArquivoDecrescente(quantidade, nomeArquivoSaida);
    }
    catch(const exception& e)
    {
        cerr << "Erro ao incializar Sistema:"  << e.what() << '\n';
        throw;
    }
    imprimirDadosArquivoSaida(REGRAS::NOME_ARQUIVO_BIN_SAIDA);
}

InicializadorSistema::InicializadorSistema(){}
