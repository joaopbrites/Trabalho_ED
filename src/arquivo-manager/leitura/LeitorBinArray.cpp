#include "LeitorBin.hpp"
#include "LeitorBinArray.hpp"

LeitorBinArray::LeitorBinArray(int size) : m_size(size)
{
    m_slots = new LeitorBin *[m_size];
    for (int i = 0; i < m_size; ++i)
    {
        m_slots[i] = nullptr;
    }
}

LeitorBinArray::~LeitorBinArray()
{
    for (int i = 0; i < m_size; ++i)
    {
        delete m_slots[i];
    }
    delete[] m_slots;
}

bool LeitorBinArray::initialize(const std::string &nomeEntrada, int pos)
{

    try
    {
        m_slots[pos] = new LeitorBin(nomeEntrada);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

LeitorBin *LeitorBinArray::operator[](int index)
{
    LeitorBin *retorno = nullptr;
    if (index >= 0 && index < m_size)
    {
        retorno = m_slots[index];
    }
    return retorno;
}
