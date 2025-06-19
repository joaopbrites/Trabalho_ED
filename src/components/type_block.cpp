#include "cabecalhoBloco.hpp"
#include "type_dado.hpp"
#include "config.hpp"
#include "type_block.hpp"
#include "MergeSort.hpp"
#include <cstring>
#include "Logger.hpp"

void BlocoRegistros::atualizarMetadadosAdicao(float chaveInserida)
{
    this->cabecalho.qtd_registros_validos++;
    if (this->cabecalho.qtd_registros_validos == 0)
    {
        this->cabecalho.chave_max_no_bloco = chaveInserida;
        this->cabecalho.chave_min_no_bloco = chaveInserida;
    }
    else if (chaveInserida > this->cabecalho.chave_max_no_bloco)
    {
        this->cabecalho.chave_max_no_bloco = chaveInserida;
    }
    else if (chaveInserida < this->cabecalho.chave_min_no_bloco)
    {
        this->cabecalho.chave_min_no_bloco = chaveInserida;
    }
} // atualiza min/max e número de registros validos
void BlocoRegistros::atualizarMetadadosMinMax()
{
    int iterador = 0;
    uint32_t sentinela = 0;
    while (iterador < REGRAS::TAMANHO_BUFFER && sentinela < this->cabecalho.qtd_registros_validos)
    {
        Registro aux = this->arrayDados[iterador];
        if (aux.getStatus() == FLAGS::ATIVO)
        {
            if (aux.getChavePrimaria() > this->cabecalho.chave_max_no_bloco)
            {
                this->cabecalho.chave_max_no_bloco = aux.getChavePrimaria();
            }
            if (aux.getChavePrimaria() < this->cabecalho.chave_min_no_bloco)
            {
                this->cabecalho.chave_min_no_bloco = aux.getChavePrimaria();
            }
            sentinela++;
        }
        iterador++;
    }
}

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
        log->warning("Tamanho Buffer no constrtor desserializador menor que o cabeçalho");
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
            (this->cabecalho.chave_min_no_bloco == INVALID_VALUES::CHAVE_MIN_NO_BLOCO) ||
            (this->cabecalho.chave_max_no_bloco == INVALID_VALUES::CHAVE_MAX_NO_BLOCO);

        if (precisaAtualizarMinMax)
        {
            this->atualizarMetadadosMinMax();
        }
    }
}
bool BlocoRegistros::push_back(const Registro &novo)
{
    if (this->estaCheio())
    {
        return false;
    }
    this->arrayDados[this->cabecalho.qtd_registros_validos] = novo;
    this->atualizarMetadadosAdicao(novo.getChavePrimaria());
    return true;
}
bool BlocoRegistros::push_position(const Registro &novo, uint32_t pos)
{
    if (pos > this->cabecalho.qtd_registros_validos)
    {
        return false;
    }
    if (this->estaCheio())
    {
        return false;
    }
    if (this->arrayDados[pos].getStatus() != FLAGS::ATIVO)
    {
        this->arrayDados[pos] = novo;
    }
    else
    {
        uint32_t i = this->cabecalho.qtd_registros_validos;
        while (i >= pos)
        {
            this->arrayDados[i + 1] = this->arrayDados[i];
            i--;
        }
        this->arrayDados[pos] = novo;
    }
    this->atualizarMetadadosAdicao(novo.getChavePrimaria());
    return true;
}
bool BlocoRegistros::atualizarRegistro(const Registro &registroAtualizado, int pos)
{
    if (pos >= REGRAS::TAMANHO_BUFFER)
    {
        return false;
    }

    this->arrayDados[pos] = registroAtualizado;
    this->cabecalho.chave_min_no_bloco = registroAtualizado.getChavePrimaria();
    this->cabecalho.chave_max_no_bloco = registroAtualizado.getChavePrimaria();
    this->atualizarMetadadosMinMax();
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
    this->atualizarMetadadosMinMax();
}

/*bool BlocoRegistros::removerRegistroPorChave(float chave) {
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i) {
        if (arrayDados[i].getChavePrimaria() == chave && arrayDados[i].getStatus() == FLAGS::ATIVO) {
            arrayDados[i].setStatus(FLAGS::REMOVIDO); // ou VAZIO, conforme sua lógica
            this->cabecalho.qtd_registros_validos--;
            this->atualizarMetadadosMinMax();
            return true;
        }
    }
    return false;
}*/

bool BlocoRegistros::pullMaiorElemento(Registro &registroDeSaida)
{
    float chaveMax = this->cabecalho.chave_max_no_bloco;
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
    {
        if (arrayDados[i].getStatus() == FLAGS::ATIVO && arrayDados[i].getChavePrimaria() == chaveMax)
        {
            registroDeSaida = arrayDados[i];
            arrayDados[i].setStatus(FLAGS::REMOVIDO); // ou VAZIO, conforme sua lógica
            this->cabecalho.qtd_registros_validos--;
            this->atualizarMetadadosMinMax();
            return true;
        }
    }
    return false; // Nenhum registro válido encontrado
}

bool BlocoRegistros::getMaiorRegistro(Registro &registroDeSaida) const
{
    float chaveMin = this->cabecalho.chave_min_no_bloco;
    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; ++i)
    {
        if (arrayDados[i].getStatus() == FLAGS::ATIVO && arrayDados[i].getChavePrimaria() == chaveMin)
        {
            registroDeSaida = arrayDados[i]; 
            return true;
        }
    }
    return false;
}
void BlocoRegistros::esvaziar()
{
    this->cabecalho.id_bloco = INVALID_VALUES::ID_BLOCK;
    this->cabecalho.qtd_registros_validos = 0;
    this->cabecalho.chave_min_no_bloco = INVALID_VALUES::CHAVE_MIN_NO_BLOCO;
    this->cabecalho.chave_max_no_bloco = INVALID_VALUES::CHAVE_MAX_NO_BLOCO;

    for (int i = 0; i < REGRAS::TAMANHO_BUFFER; i++)
    {
        this->arrayDados[i].setStatus(FLAGS::VAZIO);
    }
}
