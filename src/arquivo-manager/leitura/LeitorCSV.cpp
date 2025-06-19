// leitura/LeitorCSV.cpp
#include "LeitorCSV.hpp"
#include <cstring>
#include <cstdlib>
#include <sstream>

LeitorCSV::LeitorCSV(const char *caminhoCSV, Logger *logger) : log(logger), cabecalhoIgnorado(false)
{
    arquivo.open(caminhoCSV);
    if (!arquivo.is_open())
    {
        log->error("Falha ao abrir o arquivo CSV.");
    }
}

LeitorCSV::~LeitorCSV()
{
    fechar();
}

bool LeitorCSV::estaAberto() const
{
    return arquivo.is_open();
}

void LeitorCSV::fechar()
{
    if (arquivo.is_open())
    {
        arquivo.close();
    }
}

bool LeitorCSV::lerProximo(Registro &registroOut)
{
    if (!arquivo.is_open())
        return false;

    if (!getline(arquivo, bufferLinha))
        return false;
    

    istringstream ss(bufferLinha);
    string campo;

    string reference;
    float chavePrimaria = 0.0f;
    float dataValue = 0.0f;
    string status;
    string units;
    int magnitude = 0;
    string subject;
    string group;
    string titulos[4];

    for (int i = 0; i < 13; ++i)
    {
        if (!getline(ss, campo, ','))
            break;
        switch (i)
        {
        case 0:
            reference = campo;
            break;
        case 1:
            chavePrimaria = stof(campo);
            break;
        case 2:
            dataValue = stof(campo);
            break;
        case 3:
            status = campo;
            break;
        case 4:
            units = campo;
            break;
        case 5:
            magnitude = stoi(campo);
            break;
        case 6:
            subject = campo;
            break;
        case 7:
            group = campo;
            break;
        case 8:
            titulos[0] = campo;
            break;
        case 9:
            titulos[1] = campo;
            break;
        case 10:
            titulos[2] = campo;
            break;
        case 11:
            titulos[3] = campo;
            break;
        default:
            break;
        }
    }

    registroOut = Registro(chavePrimaria, log);
    registroOut.setStatus(FLAGS::ATIVO);
    registroOut.setReference(reference);
    registroOut.setDataValue(dataValue);
    registroOut.setStatusReference(status);
    registroOut.setUnits(units);
    registroOut.setMagnitude(magnitude);
    registroOut.setSubject(subject);
    registroOut.setGroup(group);
    registroOut.setTitle(titulos);

    return true;
}

bool LeitorCSV::chegouAoFim() {
    return (!arquivo.is_open() || arquivo.eof());
}
