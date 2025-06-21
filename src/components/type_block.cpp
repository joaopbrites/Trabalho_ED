#include "cabecalhoBloco.hpp"
#include "type_dado.hpp"
#include "config.hpp"
#include "type_block.hpp"
#include "MergeSort.hpp"
#include <cstring>
#include <iostream>

bool BlocoRegistros::estaCheio()
{
    return this->cabecalho.qtd_registros_validos >= REGRAS::TAMANHO_BUFFER;
}

BlocoRegistros::BlocoRegistros()
{
    this->esvaziar();
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

BlocoRegistros::BlocoRegistros(const char buffer[], size_t tamanhoBuffer)
{
    if (buffer == nullptr || tamanhoBuffer < sizeof(cabecalhoParaBloco))
    {
        throw runtime_error("Tamanho Buffer no constrtor desserializador menor que o cabeçalho");
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

        this->atualizarMetadadosMax();
    }
}
bool BlocoRegistros::push_back(const Registro &novo)
{
    if (this->estaCheio() || novo.getStatus() != FLAGS::ATIVO)
    {
        return false;
    }

    this->arrayDados[this->cabecalho.qtd_registros_validos] = novo;
    this->cabecalho.qtd_registros_validos++;

    this->atualizarMetadadosMax();

    return true;
}

bool BlocoRegistros::push_backMargem(const Registro &novo)
{
    if (this->cabecalho.qtd_registros_validos >= REGRAS::TAMANHO_BUFFER_MARGEM || novo.getStatus() != FLAGS::ATIVO)
    {
        return false;
    }

    this->arrayDados[this->cabecalho.qtd_registros_validos] = novo;
    this->cabecalho.qtd_registros_validos++;

    this->atualizarMetadadosMax();

    return true;
}


bool BlocoRegistros::push_position(const Registro &novo, uint32_t pos)
{
    if (pos >= REGRAS::TAMANHO_BUFFER || this->estaCheio() || novo.getStatus() != FLAGS::ATIVO)
    {
        throw runtime_error("Posicao_invalida_ou_bloco_cheio");
        return false;
    }

    // Conta quantos registros válidos existem
    uint32_t qtd_validos = 0;
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i) {
        if (this->arrayDados[i].getStatus() == FLAGS::ATIVO) {
            qtd_validos++;
        }
    }
    if (qtd_validos != this->cabecalho.qtd_registros_validos) {
        // Corrige inconsistência se houver
        this->cabecalho.qtd_registros_validos = qtd_validos;
    }

    // Se a posição está vazia ou removida, apenas insere
    if (this->arrayDados[pos].getStatus() != FLAGS::ATIVO) {
        this->arrayDados[pos] = novo;
        this->cabecalho.qtd_registros_validos++;
        this->atualizarMetadadosMax();
        return true;
    }
    bool sentinela = true;
    // Encontrar a primeira posição livre (VAZIO ou REMOVIDO) após ou antes de 'pos'
     uint32_t invalido = -1;
    uint32_t livre_apos = invalido, livre_antes = invalido;
    for (uint32_t i = REGRAS::TAMANHO_BUFFER - 1; i > pos && sentinela; --i) {
        if (this->arrayDados[i].getStatus() != FLAGS::ATIVO) {
            livre_apos = i;
            sentinela = false;
        }
    }
    sentinela = true;
    for (uint32_t i = 0; i < pos && sentinela; ++i) {
        if (this->arrayDados[i].getStatus() != FLAGS::ATIVO) {
            livre_antes = i;
            sentinela = false;
        }
    }
    if (livre_apos != invalido) {
        // Move para frente: abre espaço a partir de pos até livre_apos
        for (int i = livre_apos; i > (int)pos; --i) {
            this->arrayDados[i] = this->arrayDados[i - 1];
        }
        this->arrayDados[pos] = novo;
        this->cabecalho.qtd_registros_validos++;
        this->atualizarMetadadosMax();
        return true;
    } else if (livre_antes != invalido) {
        // Move para trás: abre espaço de livre_antes até pos-1
        for (int i = livre_antes; i < (int)pos; ++i) {
            this->arrayDados[i] = this->arrayDados[i + 1];
        }
        this->arrayDados[pos - 1] = novo;
        this->cabecalho.qtd_registros_validos++;
        this->atualizarMetadadosMax();
        return true;
    }

    // Se não encontrou espaço, não é possível inserir
    throw runtime_error("Nao_ha_espaco_para_insercao_na_posicao_desejada");
    return false;
}
bool BlocoRegistros::atualizarRegistro(const Registro &registroAtualizado, int pos)
{
    if (pos >= REGRAS::TAMANHO_BUFFER)
    {
        return false;
    }

    this->arrayDados[pos] = registroAtualizado;
    this->atualizarMetadadosMax();
    return true;
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
    this->atualizarMetadadosMax();
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
        // throw runtime_error("Tentativa_de_remover_de_bloco_vazio");
        return false;
    }
    if (posMaiorElemento == -1)
    {
        return false;
    }
    if (arrayDados[posMaiorElemento].getStatus() == FLAGS::ATIVO)
    {
        registroDeSaida = arrayDados[posMaiorElemento];
        arrayDados[posMaiorElemento].setStatus(FLAGS::REMOVIDO);
        this->cabecalho.qtd_registros_validos--;
        this->atualizarMetadadosMax();
        return true;
    }
    // throw runtime_error("Maior_elemento_nao_encontrado");
    return false;
}
bool BlocoRegistros::getMaiorRegistro(Registro &registroDeSaida) const
{
    if (this->cabecalho.qtd_registros_validos == 0)
    {
        // throw runtime_error("Bloco_vazio");
        return false;
    }

    if (arrayDados[posMaiorElemento].getStatus() == FLAGS::ATIVO)
    {
        registroDeSaida = arrayDados[posMaiorElemento];
        return true;
    }
    return true;
}
void BlocoRegistros::esvaziar()
{
    this->cabecalho.id_bloco = INVALID_VALUES::ID_BLOCK;
    this->cabecalho.qtd_registros_validos = 0;
    this->posMaiorElemento = -1;

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; i++)
    {
        this->arrayDados[i].setStatus(FLAGS::VAZIO);
    }
}

void BlocoRegistros::atualizarMetadadosMax()
{
    this->posMaiorElemento = -1;
    uint32_t count = 0;

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER && count < this->cabecalho.qtd_registros_validos; i++)
    {
        if (this->arrayDados[i].getStatus() == FLAGS::ATIVO)
        {
            if (this->arrayDados[posMaiorElemento].getStatus() != FLAGS::ATIVO)
            {
                this->posMaiorElemento = i;
            }
            else if (this->arrayDados[i].getChavePrimaria() > this->arrayDados[posMaiorElemento].getChavePrimaria())
            {
                this->posMaiorElemento = i;
            }
            count++;
        }

    }
}

bool BlocoRegistros::estaVazio()
{
    return (this->cabecalho.qtd_registros_validos == 0);
}

// Operador de cópia
BlocoRegistros &BlocoRegistros::operator=(const BlocoRegistros &other)
{
    if (this != &other)
    {
        this->cabecalho = other.cabecalho;
        this->posMaiorElemento = other.posMaiorElemento;
        for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
        {
            this->arrayDados[i] = other.arrayDados[i];
        }
    }
    return *this;
}

// Construtor de cópia
BlocoRegistros::BlocoRegistros(const BlocoRegistros &other)
{
    this->cabecalho = other.cabecalho;
    this->posMaiorElemento = other.posMaiorElemento;
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
    {
        this->arrayDados[i] = other.arrayDados[i];
    }
}

int BlocoRegistros::getContagemRegistros() const
{
    return this->cabecalho.qtd_registros_validos;
}
