#include "config.hpp"
#include <cstdint>
#include "Semafaro.hpp"

Semafaro::Semafaro(int tamDesejado) : tamanho(tamDesejado)
{
    semafaro = new uint8_t[tamanho];
    for (int i = 0; i < tamanho; i++)
    {
        semafaro[i] = FLAGS::VAZIO;
    }
}
Semafaro::~Semafaro()
{
    delete semafaro;
}
bool Semafaro::setPosVazia(int pos)
{
    if (pos >= tamanho)
    {
        return false;
    }
    semafaro[pos] = FLAGS::VAZIO;
    return true;
}
bool Semafaro::setPosInvalida(int pos)
{
    if (pos >= tamanho)
    {
        return false;
    }
    semafaro[pos] = FLAGS::INVALIDO;
    return true;
}
bool Semafaro::setPosValida(int pos)
{
    if (pos >= tamanho)
    {
        return false;
    }
    semafaro[pos] = FLAGS::ATIVO;
    return true;
}
bool Semafaro::getPosStatus(int pos, uint8_t &saida)
{
    if (pos >= tamanho)
    {
        return false;
    }
    saida = semafaro[pos];
    return true;
}
bool Semafaro::getPosVazia(int &saida)
{
    for (int i = 0; i < this->tamanho; i++)
    {
        if (semafaro[i] == FLAGS::VAZIO)
        {
            saida = semafaro[i];
            return true;
        }
    }
    return false;
}
bool Semafaro::semafaroInvalido()
{
    for (int i = 0; i < this->tamanho; i++)
    {
        if (this->semafaro[i] != FLAGS::INVALIDO)
        {
            return false;
        }
    }
    return true;
}

bool Semafaro::setAllInvalid()
{
    for (int i = 0; i < tamanho; i++)
    {
        semafaro[i] = FLAGS::INVALIDO;
    }
    return true;
}