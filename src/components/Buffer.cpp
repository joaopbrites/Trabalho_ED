#include "type_dado.hpp"
#include "config.hpp"
#include "Buffer.hpp"
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
        if (!this->buffer[i].estaVazio())
        {
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
    }
    return true;
}

BufferClass::BufferClass() {}

BufferClass::BufferClass(LeitorBin *leitor)
{
    bool sentinela = false;
    BlocoRegistros aux;
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER && !sentinela; i++)
    {
        if (leitor->lerProximoBloco(aux))
        {
            buffer[i] = aux;
            buffer[i].ordenarDecrescente();
        }
        else if (leitor->chegouAoFim())
        {
            sentinela = true;
        }
        else
        {
            throw runtime_error("Erro desconhecido na leitura do bloco durante a construção do buffer");
        }
    }
    this->atualizaSlotMaior();
}

bool BufferClass::bufferVazio()
{
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        if (!this->slotVazio(i))
        {
            return false;
        }
    }
    return true;
}

bool BufferClass::slotVazio(int indice)
{
    if (indice >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        throw runtime_error("Tentativa de acesso em posicao invalida no buffer");
        return false;
    }
    return buffer[indice].getContagemRegistros() == 0;
}

bool BufferClass::pullMaiorEvent(Registro &Saida, bool &slotEsvaziou)
{
    if (bufferVazio())
    {
        return false;
    }
    this->buffer[slotMaiorElemento].pullMaiorElemento(Saida);
    this->atualizaSlotMaior();
    slotEsvaziou = false;
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        if (this->slotVazio(i))
        {
            slotEsvaziou = true;
        }
    }
    return true;
}
bool BufferClass::pullMaior(Registro &Saida)
{
    if (bufferVazio())
    {
        return false;
    }
    this->buffer[slotMaiorElemento].pullMaiorElemento(Saida);
    this->atualizaSlotMaior();
    return true;
}
bool BufferClass::pullMaiorSlot(Registro &Saida, int slot)
{
    if (slotVazio(slot))
    {
        throw runtime_error("Buffer vazio para retirar maior");
        return false;
    }
    this->buffer[slot].pullMaiorElemento(Saida);
    if (slot == this->slotMaiorElemento)
    {
        this->atualizaSlotMaior();
    }
    return true;
}

bool BufferClass::setSlot(int indice, BlocoRegistros Novo)
{
    if (indice >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        throw runtime_error("Tentativa de acesso em posicao invalida no buffer");
        return false;
    }
    this->buffer[indice] = Novo;
    this->atualizaSlotMaior();
    return true;
}

// Retorna a posição de um registro vazio ou excluído em um slot
