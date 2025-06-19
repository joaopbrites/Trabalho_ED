#include "LeitorBin.hpp"
#include "MergeArquivos.hpp"
#include "GravarBinBlocos.hpp"
#include "Logger.hpp"
#include "config.hpp"
#include <cstdio>
#include <iostream>
#include "type_dado.hpp"
#include <ctime>
#include <fstream>
#include <limits>
#include "GeradorRuns.hpp"

using namespace std;

int GERAR_ARQUIV_TESTE()
{
    const char *caminhoSaidaTxt = "./input/dados_entrada.txt";
    ofstream txtOut(caminhoSaidaTxt);
    srand(time(nullptr));
    int qtd_linhas = 200; // Gere quantas linhas quiser
    int ano = 1986;
    float trimestre = 0.03f;
    int valor = 950;

    for (int i = 0; i < qtd_linhas; ++i)
    {
        // Gera campos variando alguns valores
        string referencia = "HLFQ.S1A1S";
        char data[16];
        snprintf(data, sizeof(data), "%d.%02d", ano, int(trimestre * 100));
        int valorAtual = valor - (i % 50); // só para variar
        string status = "REVISED";
        string unidade = "Number";
        int magnitude = 3;
        string survey = "Household Labour Force Survey - HLF";
        string assunto = "Labour Force Status by Sex: Seasonally Adjusted";
        string descricao = "Persons Employed in Labour Force";
        string sexo = "Male";

        txtOut << referencia << ',' << data << ',' << valorAtual << ',' << status << ',' << unidade << ',' << magnitude << ','
               << survey << ',' << assunto << ',' << descricao << ',' << sexo << '\n';

        // Avança trimestre e ano
        trimestre += 0.03f;
        if (trimestre > 0.12f)
        {
            trimestre = 0.03f;
            ano++;
        }
    }

    txtOut.close();
    return 0;
}
// Função auxiliar para imprimir os dados do arquivo de saída
void imprimirDadosArquivoSaida(string caminhoSaida, Logger &logger)
{
    cout << "\n--- Dados do arquivo de saída ---\n";
    LeitorBin leitorSaida(caminhoSaida, &logger);
    int blocoIdx = 0;
    bool sentinela = true;
    while (sentinela)
    {
        BlocoRegistros bloco(&logger);
        if (!leitorSaida.lerProximoBloco(bloco))
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

int main()
{
    Logger logger;
    GERAR_ARQUIV_TESTE();
    const char *caminhoEntradaCSV = "./input/dados_entrada.txt";
    // 1. Particionamento: gera runs a partir do CSV
    LeitorCSV leitorCSV(caminhoEntradaCSV, &logger);
    GerarNomeRun nomeBase(0, 0);
    GerarRuns geradorRuns(&leitorCSV, &logger, nomeBase);
    geradorRuns.gerarRun();
    int quantidadeRuns = geradorRuns.quantArquivosGerado();

    // 2. Merge: faz merge das runs até restar um único arquivo
    int etapa = 0;
    int quantidade = quantidadeRuns;
    while (quantidade >= 4)
    {
        GerarNomeRun nomeEntrada(etapa, 0);
        GerarNomeRun nomeSaida(etapa + 1, 0);
        MergeArquivos mergeador(&logger, REGRAS::QUANTIDADES_DE_SLOTS_BUFFER, REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);
        quantidade = mergeador.merge(quantidade, nomeEntrada, nomeSaida);
        etapa++;
    }

    GerarNomeRun nomeEntrada(etapa, 0);

    GerarNomeRun nomeSaida("../data/dados_saida_ordenados.bin");
    MergeArquivos mergeador(&logger, REGRAS::QUANTIDADES_DE_SLOTS_BUFFER, REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);

    string nomeArquivoFinal = nomeSaida.getNomeRun();

    logger.info("Merge K-way finalizado com sucesso.");
    //imprimirDadosArquivoSaida(nomeArquivoFinal, logger);
    return 0;
}
