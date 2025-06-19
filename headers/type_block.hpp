#pragma once

#ifndef TYPE_BLOCK
#define TYPE_BLOCK

#include "cabecalhoBloco.hpp"
#include "type_dado.hpp"
#include "config.hpp"
#include "Logger.hpp"

class BlocoRegistros
{
private:
    cabecalhoParaBloco cabecalho;
    float maiorElemento;
    Registro arrayDados[REGRAS::TAMANHO_BUFFER];

    void atualizarMetadadosMax(); // atualiza maior 
    Logger* log;



public:
    BlocoRegistros();
    BlocoRegistros(Logger *pLog);
    BlocoRegistros(const char* buffer, size_t tamanhoBuffer,Logger *log);
    void esvaziar();
    bool push_back(const Registro &novo);
    bool push_position(const Registro &novo, uint32_t pos);
    bool atualizarRegistro(const Registro &registroAtualizado, int pos);
    bool getRegistroPorIndice(int indice, Registro &registroDeSaida) const;
    int getContagemRegistros() const;
    bool trocarRegistros(int pos1, int pos2);
    void setIdBloco(uint32_t novoId);
    cabecalhoParaBloco getCabecalho();
    bool estaCheio();
    // Ordena os registros válidos do bloco em ordem decrescente pela chave primária
    void ordenarDecrescente();
    //bool removerRegistroPorChave(float chave);
    // Retorna o maior registro válido (ATIVO) e o remove do bloco (marca como REMOVIDO)
    bool pullMaiorElemento(Registro &registroDeSaida);
    // Retorna o maior registro válido do bloco (com base no cabeçalho)
    bool getMaiorRegistro(Registro &registroDeSaida) const;
    
};
#endif