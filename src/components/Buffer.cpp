#include "type_dado.hpp"
#include "config.hpp"
#include "Buffer.hpp"
#include "mock.hpp"
#include "LeitorCSV.hpp"


using namespace std;

bool BufferClass::atualizaSlotMaior()
{
    if (this->bufferVazio())
    {
        return false;
    }
    Registro maior;
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        Registro aux;
        if (this->buffer[i].getMaiorRegistro(aux))
        {
            if (maior.getStatus() != FLAGS::ATIVO)
            {
                this->slotMaiorElemento = i;
                maior = aux;
            }
            else if (aux.getChavePrimaria() > maior.getChavePrimaria())
            {
                this->slotMaiorElemento = i;
                maior = aux;
            }
        }
    }
    return true;
}

BufferClass::BufferClass(Logger *pLog) : log(pLog) {}

BufferClass::BufferClass(LeitorCSV &leitor, Logger *pLog) : log(pLog)
{
    bool sentinela = false;
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        while (!sentinela)
        {
            Registro aux;
            if (leitor.lerProximo(aux))
            {
                if (!buffer[i].push_back(aux))
                    break;
            }
            else if (leitor.chegouAoFim())
            {
                sentinela = true;
                log->info("Leitura do arquivo csv chegou ao fim");
            }
            else
            {
                log->warning("Erro desconhecido na leitura da linha durante a construção do buffer");
                break;
            }
        }
        buffer[i].ordenarDecrescente();
    }
    this->atualizaSlotMaior();
}

BufferClass::BufferClass(LeitorBin &leitor, Logger *pLog) : log(pLog)
{
    bool sentinela = false;
    BlocoRegistros aux(log);
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER && !sentinela; i++)
    {
        if (leitor.lerProximoBloco(aux))
        {
            buffer[i] = aux;
            buffer[i].ordenarDecrescente();
        }
        else if (leitor.chegouAoFim())
        {
            sentinela = true;
            log->info("Leitura do arquivo bin chegou ao fim");
        }
        else
        {
            log->warning("Erro desconhecido na leitura do bloco durante a construção do buffer");
        }
    }
    this->atualizaSlotMaior();
}

bool BufferClass::bufferVazio()
{
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        if (!this->slotVazio(i))
            return false;
    }
    return true;
}

bool BufferClass::slotVazio(int indice)
{
    if (indice >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        log->error("Tentativa de acesso em posicao invalida no buffer");
        return false;
    }
    return buffer[indice].getContagemRegistros() == 0;
}

bool BufferClass::pullMaior(Registro &Saida)
{
    if (bufferVazio())
    {
        log->error("Buffer vazio para retirar maior");
        return false;
    }
    this->buffer[slotMaiorElemento].pullMaiorElemento(Saida);
    this->atualizaSlotMaior();
    return true;
}

bool BufferClass::setSlot(int indice, BlocoRegistros Novo)
{
    if (indice >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        this->log->error("Tentativa de acesso em posicao invalida no buffer");
        return false;
    }
    this->buffer[indice] = Novo;
    this->atualizaSlotMaior();
    return true;
}

// Retorna a posição de um registro vazio ou excluído em um slot
int BufferClass::posicaoVaziaOuExcluidaNoSlot(int slot) const
{
    if (slot < 0 || slot >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
        return -1;
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
    {
        Registro reg;
        if (buffer[slot].getRegistroPorIndice(i, reg))
        {
            if (reg.getStatus() != FLAGS::ATIVO)
            {
                return i;
            }
        }
        else
        {
            // Se não conseguiu pegar, provavelmente está vazio
            return -1;
        }
    }
    return -1;
}

// Função exemplo: mescla maiores de cada slot em bloco auxiliar, faz swap se slot 0 não estiver vazio
void BufferClass::mesclarMaioresPorBloco()
{
    const int nSlots = REGRAS::QUANTIDADES_DE_SLOTS_BUFFER;
    const int tamBloco = REGRAS::TAMANHO_BUFFER;

    for (int slotDestino = 0; slotDestino < nSlots - 1; slotDestino++)
    {
        BlocoRegistros blocoTemp(log);

        // Coleta os maiores elementos disponíveis
        while (blocoTemp.getContagemRegistros() < tamBloco)
        {
            Registro maiorReg;
            int slotMaior = -1;
            float maiorChave = -1e30;

            // Encontra o slot com o maior elemento
            for (int s = slotDestino; s < nSlots; s++)
            {
                if (!slotVazio(s))
                {
                    Registro atual;
                    if (buffer[s].getMaiorRegistro(atual) && 
                        atual.getChavePrimaria() > maiorChave)
                    {
                        maiorChave = atual.getChavePrimaria();
                        slotMaior = s;
                    }
                }
            }

            if (slotMaior == -1)
            {
                // Não há mais elementos para processar
                blocoTemp.getContagemRegistros();
            }
            else
            {
                // Move o maior elemento para o bloco temporário
                Registro aux;
                buffer[slotMaior].pullMaiorElemento(aux);
                blocoTemp.push_back(aux);
            }
        }

        if (blocoTemp.getContagemRegistros() > 0)
        {
            // Move os elementos existentes do slot atual para os próximos slots disponíveis
            Registro regAtual;
            int slotVizinho = 1;
            
            // Continua movendo registros enquanto houver registros ativos no slot
            while (buffer[slotDestino].pullMaiorElemento(regAtual))
            {
                bool registroMovido = false;
                
                // Procura uma posição válida nos slots seguintes
                while (slotDestino + slotVizinho < nSlots && !registroMovido)
                {
                    int posVazia = posicaoVaziaOuExcluidaNoSlot(slotDestino + slotVizinho);
                    if (posVazia != -1)
                    {
                        buffer[slotDestino + slotVizinho].push_position(regAtual, posVazia);
                        registroMovido = true;
                    }
                    else
                    {
                        slotVizinho++;
                    }
                }
            }

            // Atualiza o slot de destino com os maiores elementos
            buffer[slotDestino] = blocoTemp;
            buffer[slotDestino].ordenarDecrescente();
        }
    }

    atualizaSlotMaior();
}