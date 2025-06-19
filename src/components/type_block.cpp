#include "cabecalhoBloco.hpp"
#include "type_dado.hpp"
#include "config.hpp"
#include "type_block.hpp"
#include "MergeSort.hpp"
#include <cstring>
#include "Logger.hpp"
#include <iostream>

bool BlocoRegistros::estaCheio()
{
    return this->cabecalho.qtd_registros_validos >= REGRAS::QUANTIDADE_REGISTROS;
}
BlocoRegistros::BlocoRegistros() : log(nullptr)
{
    this->esvaziar();
}

BlocoRegistros::BlocoRegistros(Logger *pLog) : log(pLog)
{
    this->esvaziar();
}
BlocoRegistros::BlocoRegistros(const char buffer[], size_t tamanhoBuffer, Logger *pLog) : log(pLog)
{
    if (buffer == nullptr || tamanhoBuffer < sizeof(cabecalhoParaBloco))
    {
        if (log) {
            log->warning("Tamanho Buffer no constrtor desserializador menor que o cabeçalho");
        }
        this->esvaziar();
        return;
    }

    memcpy(&this->cabecalho, buffer, sizeof(cabecalhoParaBloco));

    const char *dadosBuffer = buffer + sizeof(cabecalhoParaBloco);

    const size_t tamanhoRegistro = Registro::sizeofRegistro();

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; i++)
    {
        this->arrayDados[i].desserializar(dadosBuffer + i * tamanhoRegistro);
    }
    if (this->cabecalho.qtd_registros_validos > 0)
    {
        bool precisaAtualizarMinMax =
            (this->maiorElemento == INVALID_VALUES::CHAVE_MAX_NO_BLOCO);

        if (precisaAtualizarMinMax)
        {
            this->atualizarMetadadosMax();
        }
    }
}
bool BlocoRegistros::push_back(const Registro &novo)
{
    if (this->estaCheio() || novo.getStatus() != FLAGS::ATIVO)
    {
        if (log) log->error("Bloco_esta_cheio_ou_registro_invalido");
        return false;
    }
    
    this->arrayDados[this->cabecalho.qtd_registros_validos] = novo;
    this->cabecalho.qtd_registros_validos++;
    
    if (novo.getChavePrimaria() > this->maiorElemento)
    {
        this->maiorElemento = novo.getChavePrimaria();
    }
    
    return true;
}
bool BlocoRegistros::push_position(const Registro &novo, uint32_t pos)
{
    if (pos > this->cabecalho.qtd_registros_validos || 
        this->estaCheio() || 
        novo.getStatus() != FLAGS::ATIVO)
    {
        if (log) log->error("Posicao_invalida_ou_bloco_cheio");
        return false;
    }

    if (this->arrayDados[pos].getStatus() != FLAGS::ATIVO)
    {
        this->arrayDados[pos] = novo;
    }
    else
    {
        // Move os registros para abrir espaço
        for (uint32_t i = this->cabecalho.qtd_registros_validos; i > pos; i--)
        {
            this->arrayDados[i] = this->arrayDados[i-1];
        }
        this->arrayDados[pos] = novo;
    }
    
    this->cabecalho.qtd_registros_validos++;
    this->atualizarMetadadosMax();
    return true;
}
bool BlocoRegistros::atualizarRegistro(const Registro &registroAtualizado, int pos)
{
    if (pos >= REGRAS::TAMANHO_BUFFER)
    {
        return false;
    }

    this->arrayDados[pos] = registroAtualizado;
    this->maiorElemento = registroAtualizado.getChavePrimaria();
    this->atualizarMetadadosMax();
    return true;
}
bool BlocoRegistros::getRegistroPorIndice(int indice, Registro &registroDeSaida) const
{
    if (indice >= REGRAS::TAMANHO_BUFFER)
    {
        return false;
    }
    if (this->cabecalho.qtd_registros_validos == 0)
    {
        return false;
    }
    if (this->arrayDados[indice].getStatus() != FLAGS::ATIVO)
    {
        return false;
    }
    registroDeSaida = this->arrayDados[indice];
    return true;
}
int BlocoRegistros::getContagemRegistros() const
{
    return this->cabecalho.qtd_registros_validos;
}
bool BlocoRegistros::trocarRegistros(int pos1, int pos2)
{
    if (this->cabecalho.qtd_registros_validos == 0)
    {
        return false;
    }
    if (this->arrayDados[pos1].getStatus() != FLAGS::ATIVO)
    {
        return false;
    }
    else if (this->arrayDados[pos2].getStatus() != FLAGS::ATIVO)
    {
        return false;
    }
    swap(this->arrayDados[pos1], this->arrayDados[pos2]);
    return true;
}
void BlocoRegistros::setIdBloco(uint32_t novoId)
{
    this->cabecalho.id_bloco = novoId;
}

cabecalhoParaBloco BlocoRegistros::getCabecalho()
{
    return cabecalho;
}
void BlocoRegistros::ordenarDecrescente()
{
    int qtd = this->cabecalho.qtd_registros_validos;
    if (qtd > 1)
    {
        MergeSort::ordenar(this->arrayDados, qtd);
    }
    this->atualizarMetadadosMax();
}

/*bool BlocoRegistros::removerRegistroPorChave(float chave) {
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i) {
        if (arrayDados[i].getChavePrimaria() == chave && arrayDados[i].getStatus() == FLAGS::ATIVO) {
            arrayDados[i].setStatus(FLAGS::REMOVIDO); // ou VAZIO, conforme sua lógica
            this->cabecalho.qtd_registros_validos--;
            this->atualizarMetadadosMax();
            return true;
        }
    }
    return false;
}*/

bool BlocoRegistros::pullMaiorElemento(Registro &registroDeSaida)
{
    if (this->cabecalho.qtd_registros_validos == 0)
    {
        if (log) log->warning("Tentativa_de_remover_de_bloco_vazio");
        return false;
    }

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
    {
        if (arrayDados[i].getStatus() == FLAGS::ATIVO && 
            arrayDados[i].getChavePrimaria() == this->maiorElemento)
        {
            registroDeSaida = arrayDados[i];
            arrayDados[i].setStatus(FLAGS::REMOVIDO);
            this->cabecalho.qtd_registros_validos--;
            this->atualizarMetadadosMax();
            return true;
        }
    }
    
    if (log) log->error("Maior_elemento_nao_encontrado");
    return false;
}
bool BlocoRegistros::getMaiorRegistro(Registro &registroDeSaida) const
{
    if (this->cabecalho.qtd_registros_validos == 0)
    {
        if (log) log->warning("Bloco_vazio");
        return false;
    }

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
    {
        if (arrayDados[i].getStatus() == FLAGS::ATIVO && 
            arrayDados[i].getChavePrimaria() == this->maiorElemento)
        {
            registroDeSaida = arrayDados[i];
            return true;
        }
    }
    
    if (log) log->error("Maior_elemento_nao_encontrado");
    return false;
}
void BlocoRegistros::esvaziar()
{
    this->cabecalho.id_bloco = INVALID_VALUES::ID_BLOCK;
    this->cabecalho.qtd_registros_validos = 0;
    this->maiorElemento = INVALID_VALUES::CHAVE_MAX_NO_BLOCO;

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; i++)
    {
        this->arrayDados[i].setStatus(FLAGS::VAZIO);
    }
}

void BlocoRegistros::atualizarMetadadosMax()
{
    this->maiorElemento = INVALID_VALUES::CHAVE_MAX_NO_BLOCO;
    uint32_t count = 0;
    
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER && count < this->cabecalho.qtd_registros_validos; i++)
    {
        if (this->arrayDados[i].getStatus() == FLAGS::ATIVO)
        {
            if (this->arrayDados[i].getChavePrimaria() > this->maiorElemento)
            {
                this->maiorElemento = this->arrayDados[i].getChavePrimaria();
            }
            count++;
        }
    }
}


