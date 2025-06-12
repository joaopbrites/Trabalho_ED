#pragma once

#ifndef TYPE_DADO
#define TYPE_DADO

#include "cabecalhoDado.hpp"
#include <string>
#include <config.hpp>

using namespace std;

class Registro
{
private:
    cabecalhoParaRegistro cabecalhoRegistro; // 20 bytes
    char series_reference[REGRAS::TAMANHO_CAMPO_REG];
    float chave_primaria; // 4 bytes
    float data_value;     // 4bytes
    char status[REGRAS::TAMANHO_CAMPO_REG];
    char units[REGRAS::TAMANHO_CAMPO_REG];
    int magnitude; // 4bytes
    char subject[REGRAS::TAMANHO_CAMPO_REG];
    char group[REGRAS::TAMANHO_CAMPO_REG];
    char series_title[4][REGRAS::TAMANHO_CAMPO_REG];
    bool copiarString(char *destino, const string &origem);

public:
    Registro(float pChave);
    Registro();
    void desserializar(const char *buffer);
    float getChavePrimaria() const;
    string gerarStringImpressão() const;
    uint8_t getStatus() const;
    static constexpr size_t sizeofRegistro()
    {
        size_t tamanhoRegistro = sizeof(cabecalhoParaRegistro) +
                                 REGRAS::TAMANHO_CAMPO_REG +
                                 sizeof(float) +
                                 sizeof(float) +
                                 REGRAS::TAMANHO_CAMPO_REG +
                                 REGRAS::TAMANHO_CAMPO_REG +
                                 sizeof(int) +
                                 REGRAS::TAMANHO_CAMPO_REG +
                                 REGRAS::TAMANHO_CAMPO_REG +
                                 (4 * REGRAS::TAMANHO_CAMPO_REG);
        return tamanhoRegistro;
    }

    void setReference(const string &pReference);
    void setDataValue(float pData);
    void setStatus(const float &pStatus);
    void setUnits(const string &pUnits);
    void setMagnitude(int pMagnitude);
    void setSubject(const string &pSubject);
    void setGroup(const string &pGroup);
    void setTitle(const string pTitles[]);
};

#endif