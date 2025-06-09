
#pragma once

#ifndef ARQUIVO_HPP
#define ARQUIVO_HPP

#include <iostream>

using namespace std;

class Arquivo
{
    private:
    string nomeArquivo;

    public:
    Arquivo(string nome);
    ~Arquivo();
    void ordenarArquivoDecrescente();
};

#endif