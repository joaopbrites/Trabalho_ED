#pragma once

#ifndef LEITOR_BIN_ARRAY
#define LEITOR_BIN_ARRAY

#include "LeitorBin.hpp"

class LeitorBinArray
{
    LeitorBin **m_slots;
    int m_size;

public:
    LeitorBinArray(int size);
    ~LeitorBinArray();
    bool initialize(const string &nomeEntrada, int pos);
    LeitorBin *operator[](int index);
};

#endif