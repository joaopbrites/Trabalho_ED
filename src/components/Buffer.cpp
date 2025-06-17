
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
            }
            else if (aux.getChavePrimaria() > maior.getChavePrimaria())
            {
                this->slotMaiorElemento = i;
            }
        }
    }
    return true;
}

BufferClass::BufferClass(Logger *pLog) : log(pLog){}

BufferClass::BufferClass(LeitorCSV &leitor, Logger *pLog) : log(pLog)
{
    bool sentinela = false;
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        while (!this->slotCheio(i) && !sentinela)
        {
            Registro aux;
            if (leitor.lerProximo(aux))
            {
                buffer[i].push_back(aux);
            }
            else if (leitor.chegouAoFim())
            {
                sentinela = true;
                log->info("Leitura do arquivo csv chegou ao fim\0");
            }
            else
            {
                log->warning("Erro desconhecido na leitura da linha durante a construção do buffer\0");
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
            log->info("Leitura do arquivo bin chegou ao fim\0");
        }
        else
        {
            log->warning("Erro desconhecido na leitura do bloco durante a construção do buffer\0");
        }
    }
    this->atualizaSlotMaior();
}
bool BufferClass::bufferCheio()
{
    for (int i = 0; i < REGRAS::QUANTIDADES_DE_SLOTS_BUFFER; i++)
    {
        if (!this->slotCheio(i))
        {
            return false;
        }
    }
    return true;
}

bool BufferClass::slotCheio(int indice)
{
    if (indice >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        log->error("Tentativa de acesso em posicao invalida no buffer");
        return true;
    }
    if (buffer[indice].estaCheio())
    {
        return true;
    }
    return false;
};

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
        log->error("Tentativa de acesso em posicao invalida no buffer");
        return false;
    }
    if (buffer[indice].getContagemRegistros() == 0)
    {
        return true;
    }
    return false;
};
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
bool BufferClass::getSlot(int indice, BlocoRegistros &Saida)
{
    if (indice >= REGRAS::QUANTIDADES_DE_SLOTS_BUFFER)
    {
        this->log->error("Tentativa de acesso em posicao invalida no buffer");
        return false;
    }
    Saida = this->buffer[indice];
    return true;
}