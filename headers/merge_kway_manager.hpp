#pragma once

#ifndef MERGE_KWAY_MANAGER
#define MERGE_KWAY_MANAGER

#include "LeitorBin.hpp"
#include "gravar_blocos_bin.hpp"
#include "Logger.hpp"
#include "type_block.hpp"
#include "blocoComCursor.hpp"

class MergeKWayManager {
private:
    LeitorBin* leitor;
    GravadorDeBlocos* gravador;
    Logger* log;
    int quantidadeDeSlots;

public:
    MergeKWayManager(LeitorBin& l, GravadorDeBlocos& g, Logger *pLog, int quantidadeDeSlotsBuffer);
    void executarMerge();
};

#endif
