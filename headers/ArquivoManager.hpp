#pragma once

#ifndef ARQUIVO_MANAGER
#define ARQUIVO_MANAGER

#include "LeitorBin.hpp"
#include "GravarBinBlocos.hpp"

class ArquivoManager
{
private:
    LeitorBin *leitor;
    GravadorDeBlocos *gravador;

public:
    ArquivoManager(LeitorBin *leitor, GravadorDeBlocos *gravador);
    void inserirElementoPosicaoEspecifica();
    void visualizarRegistrosEntrePosicoes();
    void alterarDadosRegistro();
    void trocarRegistrosPosicao();
    void imprimirTodosRegistros();
};

#endif