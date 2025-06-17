#include "merge_kway_manager.hpp"
#include "config.hpp"
#include "blocoComCursor.hpp"

MergeKWayManager::MergeKWayManager(
    LeitorBin& l,
    GravadorDeBlocos& g,
    Logger *pLog,
    int quantidadeDeSlotsBuffer)
    : leitor(&l), gravador(&g), log(pLog), quantidadeDeSlots(quantidadeDeSlotsBuffer)
{
}


void MergeKWayManager::executarMerge() {
    int totalBlocosNoArquivo = leitor->getCabecalho().qtd_total_blocos_no_arquivo;
    int proximoIndiceDeBloco = 0;

    BlocoRegistros blocoSaida(log);

    static const int MAX_BLOCOS = REGRAS::QUANTIDADES_DE_SLOTS_BUFFER;
    BlocoComCursor blocosAtivos[MAX_BLOCOS];
    int quantidadeAtivos = 0;

    while (quantidadeAtivos < quantidadeDeSlots && proximoIndiceDeBloco < totalBlocosNoArquivo) {
        leitor->posicionarParaBloco(proximoIndiceDeBloco);
        BlocoRegistros blocoLido(log);
        if (leitor->lerProximoBloco(blocoLido)) {
            BlocoComCursor tempCursor(blocoLido, proximoIndiceDeBloco);
            blocosAtivos[quantidadeAtivos] = tempCursor;
            quantidadeAtivos++;
        } 

        proximoIndiceDeBloco++;
    }

    while (quantidadeAtivos > 0) {
        int indiceDoMaior = -1;
        float maiorChave = -1.0f;
        for (int i = 0; i < quantidadeAtivos; ++i) {
            if (!blocosAtivos[i].ativo || blocosAtivos[i].indiceAtual >= blocosAtivos[i].indiceFinal) {
                continue;
            }
            Registro reg;
            if (blocosAtivos[i].bloco.getRegistroPorIndice(blocosAtivos[i].indiceAtual, reg)) {
                float chave = reg.getChavePrimaria();
                if (indiceDoMaior == -1 || chave > maiorChave) {
                    indiceDoMaior = i;
                    maiorChave = chave;
                }
            } 
        }
        if (indiceDoMaior == -1) {
            break;
        }
        Registro regSelecionado;
        blocosAtivos[indiceDoMaior].bloco.getRegistroPorIndice(
            blocosAtivos[indiceDoMaior].indiceAtual, regSelecionado);
        blocosAtivos[indiceDoMaior].indiceAtual++;
        blocoSaida.push_back(regSelecionado);
        if (blocoSaida.getContagemRegistros() >= REGRAS::TAMANHO_BUFFER) {
            gravador->escreverBloco(blocoSaida);
            blocoSaida = BlocoRegistros(log);
        }
        if (blocosAtivos[indiceDoMaior].indiceAtual >= blocosAtivos[indiceDoMaior].indiceFinal) {
            if (proximoIndiceDeBloco < totalBlocosNoArquivo) {
                leitor->posicionarParaBloco(proximoIndiceDeBloco);
                BlocoRegistros novoBloco(log);
                if (leitor->lerProximoBloco(novoBloco)) {
                    blocosAtivos[indiceDoMaior] = BlocoComCursor(novoBloco, proximoIndiceDeBloco);
                    proximoIndiceDeBloco++;
                } else {
                    blocosAtivos[indiceDoMaior].ativo = false;
                }
            } else {
                blocosAtivos[indiceDoMaior].ativo = false;
            }
        }
        for (int i = 0; i < quantidadeAtivos;) {
            if (!blocosAtivos[i].ativo) {
                for (int j = i; j < quantidadeAtivos - 1; ++j) {
                    blocosAtivos[j] = blocosAtivos[j + 1];
                }
                quantidadeAtivos--;
            } else {
                ++i;
            }
        }
    }
    if (blocoSaida.getContagemRegistros() > 0) {
        gravador->escreverBloco(blocoSaida);
    }
    gravador->finalizar();
}
