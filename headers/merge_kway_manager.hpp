#pragma once

#ifndef MERGE_KWAY_MANAGER
#define MERGE_KWAY_MANAGER

#include "Logger.hpp"
#include <string>

class MergeKWayManager {
private:
    Logger* log;
    int quantidadeDeSlots;
    void mergeGrupoDeRuns(const string* arquivos, int quantidade, const string& nomeSaida);

public:
    MergeKWayManager(Logger *pLog, int quantidadeDeSlotsBuffer);
    // Executa o merge multiway em múltiplas etapas
    // nomeBase: base dos nomes das runs (ex: "run")
    // quantidadeRuns: número de runs geradas
    // Retorna o nome do arquivo final
    string executarMergeMultiEtapas(const string& nomeBase, int quantidadeRuns);
};

#endif
