#include "merge_kway_manager.hpp"
#include "LeitorBin.hpp"
#include "gravar_blocos_bin.hpp"
#include "Buffer.hpp"
#include "type_block.hpp"
#include <string>

MergeKWayManager::MergeKWayManager(Logger *pLog, int quantidadeDeSlotsBuffer)
    : log(pLog), quantidadeDeSlots(quantidadeDeSlotsBuffer) {}

void MergeKWayManager::mergeGrupoDeRuns(const string* arquivos, int quantidade, const string& nomeSaida) {
    LeitorBin* leitores[quantidadeDeSlots];
    bool leitoresAtivos[quantidadeDeSlots];
    BufferClass buffer(this->log);
    
    // Inicialização dos leitores e primeiro carregamento
    for (int i = 0; i < quantidade && i < this->quantidadeDeSlots; ++i) {
        leitores[i] = new LeitorBin(arquivos[i].c_str(), this->log);
        leitoresAtivos[i] = true;
        BlocoRegistros bloco(this->log);
        if (leitores[i]->lerProximoBloco(bloco)) {
            buffer.setSlot(i, bloco);
        }
    }

    GravadorDeBlocos gravador(nomeSaida, this->log);
    BlocoRegistros blocoSaida(this->log);
    bool dadosRestantes = true;

    while (dadosRestantes) {
        Registro maiorReg;
        if (buffer.pullMaior(maiorReg)) {
            blocoSaida.push_back(maiorReg);
            
            if (blocoSaida.getContagemRegistros() >= REGRAS::TAMANHO_BUFFER_MARGEM) {
                gravador.escreverBloco(blocoSaida);
                blocoSaida = BlocoRegistros(this->log);
            }
        } else {
            dadosRestantes = false;
        }

        // Recarrega os slots vazios
        for (int i = 0; i < this->quantidadeDeSlots; ++i) {
            if (leitoresAtivos[i] && buffer.slotVazio(i) && !leitores[i]->chegouAoFim()) {
                BlocoRegistros novoBloco(this->log);
                if (leitores[i]->lerProximoBloco(novoBloco)) {
                    buffer.setSlot(i, novoBloco);
                } else {
                    leitoresAtivos[i] = false;
                }
            }
        }

        if (buffer.bufferVazio()) {
            bool todosTerminaram = true;
            for (int i = 0; i < this->quantidadeDeSlots && todosTerminaram; ++i) {
                if (leitoresAtivos[i] && !leitores[i]->chegouAoFim()) {
                    todosTerminaram = false;
                }
            }
            if (todosTerminaram) dadosRestantes = false;
        }
    }

    if (blocoSaida.getContagemRegistros() > 0) {
        gravador.escreverBloco(blocoSaida);
    }

    gravador.finalizar();
    for (int i = 0; i < this->quantidadeDeSlots; ++i) {
        delete leitores[i];
    }
}
