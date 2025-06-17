#include "LeitorBin.hpp" 
#include "merge_kway_manager.hpp"
#include "gravar_blocos_bin.hpp"
#include "Logger.hpp"
#include "config.hpp"
#include <cstdio>
#include <iostream>
#include "type_dado.hpp"
#include "mock.hpp"
#include <ctime>
#include <fstream>
#include <limits>

using namespace std;

int GERAR_ARQUIV_TESTE(){
    Logger logger("log_gerador.txt");

    const char* caminhoSaidaBin = "../input/dados_entrada.bin";
    const char* caminhoSaidaTxt = "../input/dados_entrada.txt";
    GravadorDeBlocos gravador(caminhoSaidaBin, &logger);
    ofstream txtOut(caminhoSaidaTxt);

    // Escreve cabeçalho no arquivo texto
    txtOut << "# Bloco | Indice | Chave Primaria | Status | Registro (string)" << endl;

    srand(time(nullptr));
    int qtd_blocos = 12;

    for (int i = 0; i < qtd_blocos; ++i) {
        BlocoRegistros bloco(&logger);
        bloco.setIdBloco(i);
        float minChave = std::numeric_limits<float>::max();
        float maxChave = std::numeric_limits<float>::lowest();

        for (int j = 0; j < REGRAS::TAMANHO_BUFFER; ++j) {
            float chave = static_cast<float>(rand() % 100 + i * 100);
            Registro r(chave, &mock::loggerMock);
            r.setReference("TESTE");
            r.setStatus(FLAGS::ATIVO); // Garante que o registro será impresso
            bloco.push_back(r);
            // Grava também no arquivo texto com informações extras
            txtOut << i << " | " << j << " | " << r.getChavePrimaria() << " | " << (int)r.getStatus() << " | " << r.gerarStringImpressao() << endl;
            if (chave < minChave) minChave = chave;
            if (chave > maxChave) maxChave = chave;
        }
         cabecalhoParaBloco cab = bloco.getCabecalho();
        txtOut << "# CABECALHO BLOCO " << i << ": id=" << cab.id_bloco << ", qtd_registros_validos=" << cab.qtd_registros_validos << ", chave_min_no_bloco=" << cab.chave_min_no_bloco << ", chave_max_no_bloco=" << cab.chave_max_no_bloco << endl;
        txtOut << "# Bloco " << i << ": " << bloco.getContagemRegistros() << " registros, min=" << minChave << ", max=" << maxChave << endl;
        bloco.ordenarDecrescente();
        gravador.escreverBloco(bloco);
    }

    gravador.finalizar();
    txtOut.close();
    logger.info("Arquivo de entrada gerado com sucesso.");
    return 0;
}
// Função auxiliar para imprimir os dados do arquivo de saída
void imprimirDadosArquivoSaida(const char* caminhoSaida, Logger& logger) {
    cout << "\n--- Dados do arquivo de saída ---\n";
    LeitorBin leitorSaida(caminhoSaida, &logger);
    int blocoIdx = 0;
    while (true) {
        BlocoRegistros bloco(&logger);
        if (!leitorSaida.lerProximoBloco(bloco)) break;
        int qtd = bloco.getContagemRegistros();
        cout << "Bloco " << blocoIdx << ": " << qtd << " registros\n";
        for (int i = 0; i < qtd; ++i) {
            Registro reg;
            bool ok = bloco.getRegistroPorIndice(i, reg);
            cout << "  [DEBUG] Indice: " << i << ", getRegistroPorIndice: " << ok << ", Status: " << (int)reg.getStatus() << endl;
            if (ok) {
                string s = reg.gerarStringImpressao();
                if (!s.empty()) cout << s << endl;
            }
        }
        blocoIdx++;
    }
    cout << "--- Fim dos dados ---\n";
}

int main() {
    GERAR_ARQUIV_TESTE();

    // Inicializa logger
    Logger logger("saida_log.txt");

    // Caminhos dos arquivos de entrada e saída
    const char* caminhoEntrada = "../input/dados_entrada.bin";
    const char* caminhoSaida = "../input/dados_saida_ordenados.bin";

    // Inicializa leitor e gravador
    LeitorBin leitor(caminhoEntrada, &logger);
    GravadorDeBlocos gravador(caminhoSaida, &logger);

    // Define número de slots do buffer (pode vir de config.hpp)
    int quantidadeDeSlotsBuffer = REGRAS::QUANTIDADES_DE_SLOTS_BUFFER;

    // Inicializa gerenciador de merge k-way
    MergeKWayManager gerenciador(leitor, gravador, &logger, quantidadeDeSlotsBuffer);

    // Executa o merge
    gerenciador.executarMerge();

    logger.info("Merge K-way finalizado com sucesso.");

    // Impressão dos dados do arquivo de saída no terminal
    imprimirDadosArquivoSaida(caminhoSaida, logger);

    return 0;
}
