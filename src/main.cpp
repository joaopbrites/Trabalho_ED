#include "LeitorBin.hpp"
#include "MergeArquivos.hpp"
#include "GravarBinBlocos.hpp"
#include "config.hpp"
#include "type_dado.hpp"
#include "Inicializador.hpp"
#include "ArquivoManager.hpp"
#include "GeradorRuns.hpp"
#include <cstdio>
#include <iostream>

using namespace std;

/*A finalidade deste trabalho é implementar um
 sistema que converta um arquivo CSV em binário e,
 posteriormente, trabalhe com ordenação em memória secundária*/
/*as operações devem ser realizadas no arquivo binário,
sendo permitido apenas carregar registros individuais
para a memória. Ou seja, não é permitido carregar o arquivo
inteiro na memória para realizar qualquer das operações solicitadas.
*/
// Nosso grupo o trabalho tem que ser em ordem decrescente
void imprimirMenu();

int lerInteiro();

int main()
{
    const char *nomeArquivoEntrada = REGRAS::NOME_ARQUIVO_ENTRADA;
    const char *nomeArquivoSaida = REGRAS::NOME_ARQUIVO_BIN_SAIDA;
    InicializadorSistema iniciar;
    try
    {
        iniciar.start(nomeArquivoEntrada, nomeArquivoSaida);
    }
    catch (const exception &e)
    {
        cerr << e.what() << '\n';
    }
    LeitorBin leitor(nomeArquivoEntrada);
    GravadorDeBlocos gravador(nomeArquivoSaida);

    ArquivoManager arquivo(&leitor, &gravador);
    int comando = -1;
    bool encerrarPrograma = false;
    try
    {
        do
        {
            imprimirMenu();
            comando = lerInteiro();
            switch (comando)
            {
            case 1:
                arquivo.inserirElementoPosicaoEspecifica();
                break;
            case 2:
                arquivo.visualizarRegistrosEntrePosicoes();
                break;
            case 3:
                arquivo.alterarDadosRegistro();
                break;
            case 4:
                arquivo.trocarRegistrosPosicao();
                break;
            case 5:
                arquivo.imprimirTodosRegistros();
                break;
            case 0:
                cout << "Para confirmar o encerramento do arquivo aperte -1"
                     << " ou outro inteiro para voltar ao menu\n";
                comando = lerInteiro();
                if (comando == -1)
                    encerrarPrograma = true;
                break;
            default:
                cout << "Opcão Não encontrada digite novamente outro comando\n";
                break;
            }
        } while (!encerrarPrograma);
    }
    catch (const std::exception &erroEmOperacaoBin)
    {
        cout << erroEmOperacaoBin.what() << '\n';
    }
    return 0;
}
void imprimirMenu()
{
    cout << "=============================================\n";
    cout << "           MENU DE MANIPULACAO BINARIA       \n";
    cout << "=============================================\n";
    cout << "1. Inserir elemento em posicao especifica\n";
    cout << "2. Visualizar registros entre duas posicoes\n";
    cout << "3. Alterar dados de um registro especifico\n";
    cout << "4. Trocar dois registros de posicao (swap)\n";
    cout << "5. Imprimir todos os registros\n";
    cout << "0. Sair do programa\n";
    cout << "---------------------------------------------\n";
    cout << "Digite a opcao desejada: ";
}
int lerInteiro()
{
    int valor;
    bool valorInvalido = true;
    while (valorInvalido)
    {
        cin >> valor;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada inválida. Digite um inteiro e tente novamente.\n";
        }
        else
        {
            valorInvalido = false;
        }
    }
    cin.ignore(1000, '\n');
    // limpa \n que ficou no cin, importante para rogramas que usam getline
    return valor;
}

/*using namespace std;

int GERAR_ARQUIV_TESTE()
{
    const char *caminhoSaidaTxt = "./input/dados_entrada.txt";
    ofstream txtOut(caminhoSaidaTxt);
    srand(time(nullptr));
    int qtd_linhas = 600; // Gere quantas linhas quiser
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
               << survey << ',' << assunto << ',' << descricao << ',' << sexo << "," << "," << "," << "," << '\n';

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

int main()
{
    GERAR_ARQUIV_TESTE();
    const char *caminhoEntradaCSV = "./input/dados_entrada.txt";
    // 1. Particionamento: gera runs a partir do CSV
    LeitorCSV leitorCSV(caminhoEntradaCSV);
    GerarNomeRun nomeBase(0, 0);
    GerarRuns geradorRuns(&leitorCSV, nomeBase);
    geradorRuns.gerarRun();
    int quantidadeRuns = geradorRuns.quantArquivosGerado();

    // 2. Merge: faz merge das runs até restar um único arquivo
    int etapa = 0;
    int quantidade = quantidadeRuns;
    while (quantidade >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        GerarNomeRun nomeEntrada(etapa, 0);
        GerarNomeRun nomeSaida(etapa + 1, 0);
        MergeArquivos mergeador(REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);
        quantidade = mergeador.merge(quantidade, nomeEntrada, nomeSaida);
        etapa++;
    }

    GerarNomeRun nomeEntrada(etapa, 0);
    GerarNomeRun nomeSaida("./data/dados_saida_ordenados.bin");
    MergeArquivos mergeador(REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);
    mergeador.merge(quantidade, nomeEntrada, nomeSaida);
    string nomeArquivoFinal = nomeSaida.getNomeRun();

    imprimirDadosArquivoSaida(nomeArquivoFinal);
    return 0;
}*/
