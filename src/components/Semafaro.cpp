#include "config.hpp"
#include <cstdint>
#include "Semafaro.hpp"
#include <stdexcept>

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
bool Semafaro::PosVazia(int pos)
{
    if (pos >= tamanho)
    {
        throw runtime_error("Posição maior que a permitida");
    }
    return semafaro[pos] == FLAGS::VAZIO;
}
bool Semafaro::PosInvalida(int pos)
{
    if (pos >= tamanho)
    {
        throw runtime_error("Posição maior que a permitida");
    }
    return semafaro[pos] == FLAGS::INVALIDO;
}
bool Semafaro::PosValida(int pos)
{
    if (pos >= tamanho)
    {
        throw runtime_error("Posição maior que a permitida");
    }
    return semafaro[pos] == FLAGS::ATIVO;
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