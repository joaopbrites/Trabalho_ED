#pragma once

#ifndef SEMAFARO
#define SEMAFARO

#include <cstdint>


class Semafaro
{
    private:
        uint8_t *semafaro;
        int tamanho;
    public:
        Semafaro(int tamDesejado);
        ~Semafaro();
        bool setPosVazia(int pos);
        bool setPosInvalida(int pos);
        bool setPosValida(int pos);
        bool getPosStatus(int pos, uint8_t &saida);
        bool getPosVazia(int &saida);
        bool semafaroInvalido();
        bool setAllInvalid();
};

#endif