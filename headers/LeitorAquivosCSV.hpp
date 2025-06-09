#pragma once

#ifndef LEITOR_ARQUIVO_CSV_HPP
#define LEITOR_ARQUIVO_CSV_HPP


#include <iostream>
#include <fstream>

using namespace std;

class LeitorAquivosCSV
{
    private:

    public:
    LeitorAquivosCSV(string nome);
    ifstream getArq();
};

#endif