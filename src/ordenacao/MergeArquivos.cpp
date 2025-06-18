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

MergeArquivos::MergeArquivos(Logger *pLog, int quantidadeDeSlotsBuffer, int TamSemafaro) : log(pLog),
                                                                                           quantidadeDeSlots(quantidadeDeSlotsBuffer),
                                                                                           buffer(log),
                                                                                           semafaro(TamSemafaro)
{
}

/*int MergeArquivos::merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida)
{
    int novaQtd = 0;

    for (int i = 0; i < quantidade; i++)
    {
        string nomeArquivoSaida = nomeSaida.getNomeRun();

        GravadorDeBlocos gravador(nomeArquivoSaida, this->log);
        BlocoRegistros blocoSaida(this->log);

        LeitorBin **slots = new LeitorBin *[REGRAS::QUANTIDADES_DE_SLOTS_BUFFER];
        for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
        {
            slots[j] = new LeitorBin(nomeEntrada.getNomeRun(), this->log);
        }
        Registro vetMaior[REGRAS::QUANTIDADES_DE_SLOTS_BUFFER];
        for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
        {
            BlocoRegistros aux;
            if (slots[j]->lerProximoBloco(aux))
            {
                if (buffer.setSlot(j, aux))
                {
                    Registro rAux;
                    buffer.pullMaiorSlot(rAux, j);
                    vetMaior[j] = rAux;
                    semafaro.setPosValida(j);
                }
            }
            else
            {
                semafaro.setPosInvalida(j);
            }
        }
        while (!this->semafaro.semafaroInvalido())
        {
            int posRemovida = 0;
            if (blocoSaida.getContagemRegistros() >= REGRAS::TAMANHO_BUFFER_MARGEM)
            {
                gravador.escreverBloco(blocoSaida);
                novaQtd++;
                blocoSaida.esvaziar();
            }
            uint8_t flagAux;
            semafaro.getPosStatus(posRemovida, flagAux);
            if (flagAux == FLAGS::VAZIO)
            {
                BlocoRegistros aux;
                if (slots[posRemovida]->lerProximoBloco(aux))
                {
                    if (buffer.setSlot(posRemovida, aux))
                    {
                        Registro rAux;
                        buffer.pullMaiorSlot(rAux, posRemovida);
                        vetMaior[posRemovida] = rAux;
                        semafaro.setPosValida(posRemovida);
                    }
                }
                else
                {
                    semafaro.setPosInvalida(posRemovida);
                }
            }
            else if (flagAux == FLAGS::ATIVO)
            {
                float cMaior = vetMaior[posRemovida].getChavePrimaria();
                for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
                {
                    float chaveAux = vetMaior[j].getChavePrimaria();
                    if (chaveAux > cMaior)
                    {
                        cMaior = chaveAux;
                        posRemovida = j;
                    }
                }
                if (blocoSaida.push_back(vetMaior[posRemovida]))
                {
                    Registro rAux;
                    if (buffer.pullMaiorSlot(rAux, posRemovida))
                    {
                        vetMaior[posRemovida] = rAux;
                    }
                    else
                    {
                        semafaro.setPosVazia(posRemovida);
                    }
                }
            }
        }
        for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
        {
            delete slots[j];
        }
        delete[] slots;
        if (semafaro.semafaroInvalido())
        {
            gravador.escreverBloco(blocoSaida);
            novaQtd++;
        }
    }
    return novaQtd;
}*/

int MergeArquivos::merge(int quantidade, GerarNomeRun nomeEntrada, GerarNomeRun nomeSaida)
{
    int novaQtd = 0;

    for (int i = 0; i < quantidade; i++)
    {
        // 1. Inicialização de recursos
        LeitorBinArray slots(REGRAS::QUANTIDADES_DE_SLOTS_BUFFER, this->log);
        if (!slots.initialize(nomeEntrada.getNomeRun()))
        {
            this->log->error("Não foi possivel gerar o arquivo de saida");
            return -1;
        }

        GravadorDeBlocos gravador(nomeSaida.getNomeRun(), this->log);
        BlocoRegistros blocoSaida(this->log);
        Registro vetMaior[REGRAS::QUANTIDADES_DE_SLOTS_BUFFER];

        // 2. Carregamento inicial dos buffers
        for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
        {
            BlocoRegistros aux;
            if (slots[j] && slots[j]->lerProximoBloco(aux))
            {
                if (buffer.setSlot(j, aux))
                {
                    Registro rAux;
                    if (buffer.pullMaiorSlot(rAux, j))
                    {
                        vetMaior[j] = rAux;
                        semafaro.setPosValida(j);
                    }
                }
            }
            else
            {
                semafaro.setPosInvalida(j);
            }
        }

        // 3. Processamento principal
        bool processamentoAtivo = true;
        while (processamentoAtivo)
        {
            processamentoAtivo = !this->semafaro.semafaroInvalido();

            if (!processamentoAtivo)
            {

                if (blocoSaida.getContagemRegistros() >= REGRAS::TAMANHO_BUFFER_MARGEM)
                {
                    if (!gravador.escreverBloco(blocoSaida))
                    {
                        this->log->error("Falha ao escrever bloco");
                    }
                    novaQtd++;
                    blocoSaida.esvaziar();
                }

                uint8_t flagAux;
                int posRemovida = 0;
                semafaro.getPosStatus(posRemovida, flagAux);

                if (flagAux == FLAGS::VAZIO)
                {
                    BlocoRegistros aux;
                    if (slots[posRemovida] && slots[posRemovida]->lerProximoBloco(aux))
                    {
                        if (buffer.setSlot(posRemovida, aux))
                        {
                            Registro rAux;
                            if (buffer.pullMaiorSlot(rAux, posRemovida))
                            {
                                vetMaior[posRemovida] = rAux;
                                semafaro.setPosValida(posRemovida);
                            }
                        }
                    }
                    else
                    {
                        semafaro.setPosInvalida(posRemovida);
                    }
                }
                else if (flagAux == FLAGS::ATIVO)
                {
                    bool encontrado = false;
                    float cMaior = INVALID_VALUES::CHAVE_REG;

                    // Encontra o maior registro válido
                    for (int j = 0; j < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; j++)
                    {
                        uint8_t status;
                        semafaro.getPosStatus(j, status);

                        if (status == FLAGS::ATIVO)
                        {
                            float chaveAtual = vetMaior[j].getChavePrimaria();
                            if (!encontrado || chaveAtual > cMaior)
                            {
                                cMaior = chaveAtual;
                                posRemovida = j;
                                encontrado = true;
                            }
                        }
                    }

                    if (encontrado)
                    {
                        if (blocoSaida.push_back(vetMaior[posRemovida]))
                        {
                            Registro rAux;
                            if (buffer.pullMaiorSlot(rAux, posRemovida))
                            {
                                vetMaior[posRemovida] = rAux;
                            }
                            else
                            {
                                semafaro.setPosVazia(posRemovida);
                            }
                        }
                    }
                    else
                    {
                        semafaro.setAllInvalid();
                    }
                }
            }
            else
            {
                processamentoAtivo = false;
            }
        }
        if (blocoSaida.getContagemRegistros() > 0)
        {
            if (!gravador.escreverBloco(blocoSaida))
            {
                throw std::runtime_error("Falha ao escrever bloco final");
            }
            novaQtd++;
        }
    }
    return novaQtd;
}
