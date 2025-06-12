#pragma once

#ifndef TYPE_BLOCK
#define TYPE_BLOCK

#include <cabecalhoBloco.hpp>
#include <type_dado.hpp>
#include <config.hpp>

class BlocoRegistros
{
private:
    cabecalhoParaBloco cabecalho;
    Registro arrayDados[REGRAS::TAMANHO_BUFFER];
    void atualizarMetadadosAdicao(float chaveInserida);//operacoes ao adicionar um arquivo
    void atualizarMetadadosMinMax(); // atualiza min/max 
    bool estaCheio();

public:
    BlocoRegistros();
    BlocoRegistros(const char* buffer, size_t tamanhoBuffer);
    bool push_back(const Registro &novo);
    bool push_position(const Registro &novo, uint32_t pos);
    bool atualizarRegistro(const Registro &registroAtualizado, int pos);
    bool getRegistroPorIndice(int indice, Registro &registroDeSaida) const;
    int getContagemRegistros() const;
    bool trocarRegistros(int pos1, int pos2);
    void setIdBloco(uint32_t novoId);

};
#endif