#pragma once

#ifndef LEITOR_BIN_ARRAY
#define LEITOR_BIN_ARRAY

#include "LeitorBin.hpp"
#include "Logger.hpp"

class LeitorBinArray
{
    LeitorBin **m_slots;
    int m_size;
    Logger *m_log;

public:
    LeitorBinArray(int size, Logger *log);
    ~LeitorBinArray();
    bool initialize(const std::string &nomeEntrada);
    LeitorBin *operator[](int index);
};

#endif