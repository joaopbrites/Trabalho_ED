
#include "cabecalhoDado.hpp"
#include <config.hpp>
#include <type_dado.hpp>
#include <cstring>
#include <sstream>

using namespace std;

bool Registro::copiarString(char *destino, const string &origem)
{
    if (origem.size() >= REGRAS::TAMANHO_CAMPO_REG)
    {
        return false; // String muito grande para o campo
    }
    strncpy(destino, origem.c_str(), REGRAS::TAMANHO_CAMPO_REG);
    destino[REGRAS::TAMANHO_CAMPO_REG - 1] = '\0'; // Garante terminação nula
    return true;
}
Registro::Registro() : series_reference{},
                       chave_primaria(INVALID_VALUES::CHAVE_REG),
                       data_value(INVALID_VALUES::DATA_VALUE),
                       status{},
                       units{},
                       magnitude(INVALID_VALUES::MAGNITUDE),
                       subject{},
                       group{},
                       series_title{}
{
    cabecalhoRegistro.flags = FLAGS::VAZIO;
}
Registro::Registro(float pChave) : series_reference{},
                       chave_primaria(pChave),
                       data_value(INVALID_VALUES::DATA_VALUE),
                       status{},
                       units{},
                       magnitude(INVALID_VALUES::MAGNITUDE),
                       subject{},
                       group{},
                       series_title{}
{
    cabecalhoRegistro.flags = FLAGS::VAZIO;
}
void Registro::desserializar(const char *buffer)
{
    memcpy(&this->cabecalhoRegistro, buffer, sizeof(cabecalhoParaRegistro));
    buffer += sizeof(cabecalhoParaRegistro);

    memcpy(this->series_reference, buffer, REGRAS::TAMANHO_CAMPO_REG);
    buffer += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(&this->chave_primaria, buffer, sizeof(float));
    buffer += sizeof(float);

    memcpy(&this->data_value, buffer, sizeof(float));
    buffer += sizeof(float);

    memcpy(this->status, buffer, REGRAS::TAMANHO_CAMPO_REG);
    buffer += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(this->units, buffer, REGRAS::TAMANHO_CAMPO_REG);
    buffer += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(&this->magnitude, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(this->subject, buffer, REGRAS::TAMANHO_CAMPO_REG);
    buffer += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(this->group, buffer, REGRAS::TAMANHO_CAMPO_REG);
    buffer += REGRAS::TAMANHO_CAMPO_REG;

    for (int i = 0; i < 4; i++)
    {
        memcpy(this->series_title[i], buffer, REGRAS::TAMANHO_CAMPO_REG);
        buffer += REGRAS::TAMANHO_CAMPO_REG;
    }
}
float Registro::getChavePrimaria() const
{
    return this->chave_primaria;
}
string Registro::gerarStringImpressão() const
{
    if (this->cabecalhoRegistro.flags != FLAGS::ATIVO)
    {
        return "\0";
    }
    stringstream ss;

    ss << "[" << this->series_reference << "] ";
    ss << "[" << this->chave_primaria << "] ";
    ss << "[" << this->data_value << "] ";
    ss << "[" << this->status << "] ";
    ss << "[: " << this->units << "] ";
    ss << "[" << this->magnitude << "] ";
    ss << "[" << this->subject << "] ";
    ss << "[" << this->group << "] ";
    
    // Tratamento especial para o array de títulos
    ss << "Títulos da Série:\n";
    for (int i = 0; i < 4; ++i) {
        // Verifica se o título não está vazio antes de imprimir
        if (this->series_title[i][0] != '\0') {
            ss << " [" << i + 1 << ": " << this->series_title[i] << "]\n";
        }
    }
    // Retorna a string completa que foi construída no stringstream
    return ss.str();
}

uint8_t Registro::getStatus() const
{
    return this->cabecalhoRegistro.flags;
}


void Registro::setReference(const string &pReference)
{
    copiarString(series_reference, pReference);
}
void Registro::setDataValue(float pData)
{
    data_value = pData;
}

void Registro::setStatus(const float &pStatus)
{
    cabecalhoRegistro.flags = static_cast<uint8_t>(pStatus);
}

void Registro::setUnits(const string &pUnits)
{
    copiarString(units, pUnits);
}

void Registro::setMagnitude(int pMagnitude)
{
    magnitude = pMagnitude;
}

void Registro::setSubject(const string &pSubject)
{
    copiarString(subject, pSubject);
}

void Registro::setGroup(const string &pGroup)
{
    copiarString(group, pGroup);
}

void Registro::setTitle(const string pTitles[])
{
    for (int i = 0; i < 4; i++)
    {
        copiarString(series_title[i], pTitles[i]);
    }
}