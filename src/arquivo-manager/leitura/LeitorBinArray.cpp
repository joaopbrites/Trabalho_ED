#include "LeitorBin.hpp"
#include "Logger.hpp"
#include "LeitorBinArray.hpp"

LeitorBinArray::LeitorBinArray(int size, Logger *log) : m_size(size), m_log(log)
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

bool LeitorBinArray::initialize(const std::string &nomeEntrada)
{
    for (int i = 0; i < m_size; ++i)
    {
        try
        {
            m_slots[i] = new LeitorBin(nomeEntrada, m_log);
        }
        catch (...)
        {
            return false;
        }
    }
    return true;
}

LeitorBin* LeitorBinArray::operator[](int index)
{
    return (index >= 0 && index < m_size) ? m_slots[index] : nullptr;
}