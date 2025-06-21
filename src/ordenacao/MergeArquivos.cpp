#include "MergeArquivos.hpp"
#include "LeitorBin.hpp"
#include "GravarBinBlocos.hpp"
#include "Buffer.hpp"
#include "type_block.hpp"
#include <string>
#include "GeradorNomeRun.hpp"
#include "Semafaro.hpp"
#include "LeitorBinArray.hpp"
#include "config.hpp"
#include <iostream>

MergeArquivos::MergeArquivos(int quantidadeDeSlotsBuffer) : quantidadeDeSlots(quantidadeDeSlotsBuffer)
{
}

int MergeArquivos::carregarBlocosParaBuffer(LeitorBinArray& slots, Semafaro &semafaroArquivo, int tamVariaveis) {
    int arqFinalizados = 0;
    for (int j = 0; j < tamVariaveis; j++) {
        if (buffer.slotVazio(j) && !semafaroArquivo.PosInvalida(j)) {
            BlocoRegistros blocoTemporario;
            if (slots[j] && slots[j]->lerProximoBloco(blocoTemporario)) {
                if (!buffer.setSlot(j, blocoTemporario)) {
                    throw runtime_error("Falha ao carregar bloco no buffer");
                }
            } else {
                semafaroArquivo.setPosInvalida(j);
                arqFinalizados++; // Indica que um arquivo foi completamente processado
            }
        }
    }
    return arqFinalizados;
}

void MergeArquivos::processarBuffer(GravadorDeBlocos& gravador, BlocoRegistros& blocoSaida) {
    bool algumSlotEsvaziou = false;
    while (!buffer.bufferVazio() && !algumSlotEsvaziou) {
        Registro registro;
        bool registroValido = buffer.pullMaiorEvent(registro, algumSlotEsvaziou);
        if (registroValido) {
            if (!blocoSaida.push_backMargem(registro)) {
                if (!gravador.escreverBloco(blocoSaida)) {
                    throw runtime_error("Falha ao escrever bloco residual");
                }
                blocoSaida.esvaziar();
                blocoSaida.push_back(registro);
            }
        }
    }
}

int MergeArquivos::merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida) {
    int novaQtd = 0;
    int arqProcessados = 0;
    
    while (arqProcessados < quantidade) {
        int tamanhoVariaveis = REGRAS::QUANTIDADES_DE_SLOTS_BUFFER;
        if ((quantidade - arqProcessados) < tamanhoVariaveis)
        {
            tamanhoVariaveis = quantidade - arqProcessados;
        }
        LeitorBinArray slots(tamanhoVariaveis);
        Semafaro semafaroArquivo(tamanhoVariaveis);
        for  (int i = 0; i < tamanhoVariaveis && arqProcessados < quantidade;i++)
        {
            slots.initialize(nomeEntrada.getNomeRun(), i);
            arqProcessados++;
        }
        
        GravadorDeBlocos gravador(nomeSaida.getNomeRun());
        BlocoRegistros blocoSaida;

        while (!semafaroArquivo.semafaroInvalido()) {
            this->carregarBlocosParaBuffer(slots, semafaroArquivo, tamanhoVariaveis);
            processarBuffer(gravador, blocoSaida);
        }

        // Escreve bloco residual se houver
        if (blocoSaida.getContagemRegistros() > 0 && !gravador.escreverBloco(blocoSaida)) {
            throw std::runtime_error("Falha ao escrever bloco final");
        }
        
        novaQtd++;
    }
    return novaQtd;
}


/*int MergeArquivos::merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida)
{
    int novaQtd = 0;
    int arqProcessados = 0;
    while (arqProcessados < quantidade)
    {
        // 1. Inicialização de recursos
        LeitorBinArray slots(REGRAS::QUANTIDADES_DE_SLOTS_BUFFER);
        slots.initialize(nomeEntrada.getNomeRun());
        GravadorDeBlocos gravador(nomeSaida.getNomeRun());
        BlocoRegistros blocoSaida;

        while (semafaroArquivo.semafaroInvalido())
        {
            for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
            {
                if (buffer.slotVazio(j))
                {
                    if (!semafaroArquivo.PosInvalida(j))
                    {
                        BlocoRegistros bAux;
                        if (slots[j] && slots[j]->lerProximoBloco(bAux))
                        {
                            if (!buffer.setSlot(j, bAux))
                            {
                                throw runtime_error("Não foi possivel ajusar o slot do buffer");
                            }
                        }
                        else
                        {
                            semafaroArquivo.setPosInvalida(j);
                            arqProcessados++;
                        }
                    }
                }
            }
            bool slotEsvaziou = false;
            while (!buffer.bufferVazio() && !slotEsvaziou)
            {
                Registro r;
                bool controller = buffer.pullMaiorEvent(r, slotEsvaziou);
                if (controller)
                { // Esvazia o slot
                    if (!blocoSaida.push_back(r))
                    {
                        if (!gravador.escreverBloco(blocoSaida))
                        {
                            throw runtime_error("Falha ao escrever bloco residual");
                        }
                        blocoSaida.esvaziar();
                        blocoSaida.push_back(r); // Adiciona o registro no novo bloco
                    }
                }
            }
        }
        if (blocoSaida.getContagemRegistros() > 0)
        {
            if (!gravador.escreverBloco(blocoSaida))
            {
                throw std::runtime_error("Falha ao escrever bloco final");
            }
        }
        novaQtd++;
    }
    return novaQtd;
}
*/