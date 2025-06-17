#include "cabecalhoDado.hpp"
#include "config.hpp"
#include "type_dado.hpp"
#include <cstring>
#include <sstream>
#include "Logger.hpp"

using namespace std;

bool Registro::copiarString(char *destino, const string &origem)
{
    if (origem.size() >= REGRAS::TAMANHO_CAMPO_REG)
    {
        log->warning("Um campo do registro de chave: " + to_string(this->chave_primaria) + " foi truncado");
    }
    strncpy(destino, origem.c_str(), REGRAS::TAMANHO_CAMPO_REG);
    destino[REGRAS::TAMANHO_CAMPO_REG - 1] = '\0'; // Garante terminação nula
    return true;
}
Registro::Registro() : series_reference(),
                       chave_primaria(INVALID_VALUES::CHAVE_REG),
                       data_value(INVALID_VALUES::DATA_VALUE),
                       status(),
                       units(),
                       magnitude(INVALID_VALUES::MAGNITUDE),
                       subject(),
                       group(),
                       series_title(),
                       log(nullptr)
{
    cabecalhoRegistro.flags = FLAGS::VAZIO;
}
Registro::Registro(float pChave, Logger* pLog) : series_reference(),
                                                 chave_primaria(pChave),
                                                 data_value(INVALID_VALUES::DATA_VALUE),
                                                 status(),
                                                 units(),
                                                 magnitude(INVALID_VALUES::MAGNITUDE),
                                                 subject(),
                                                 group(),
                                                 series_title(),
                                                 log(pLog)

{
    cabecalhoRegistro.flags = FLAGS::VAZIO;
}
void Registro::desserializar(const char *buffer)
{
    size_t offset = 0;
    memcpy(&cabecalhoRegistro, buffer + offset, sizeof(cabecalhoParaRegistro));
    offset += sizeof(cabecalhoParaRegistro);

    memcpy(series_reference, buffer + offset, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(&chave_primaria, buffer + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&data_value, buffer + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(status, buffer + offset, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(units, buffer + offset, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(&magnitude, buffer + offset, sizeof(int));
    offset += sizeof(int);

    memcpy(subject, buffer + offset, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;

    memcpy(group, buffer + offset, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;

    for (int i = 0; i < 4; ++i)
    {
        memcpy(series_title[i], buffer + offset, REGRAS::TAMANHO_CAMPO_REG);
        offset += REGRAS::TAMANHO_CAMPO_REG;
    }
}

float Registro::getChavePrimaria() const
{
    return this->chave_primaria;
}
string Registro::gerarStringImpressao() const
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
    for (int i = 0; i < 4; ++i)
    {
        // Verifica se o título não está vazio antes de imprimir
        if (this->series_title[i][0] != '\0')
        {
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
void Registro::serializar(char *destino) const
{
    size_t offset = 0;
    memcpy(destino + offset, &cabecalhoRegistro, sizeof(cabecalhoParaRegistro));
    offset += sizeof(cabecalhoParaRegistro);
    memcpy(destino + offset, series_reference, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;
    memcpy(destino + offset, &chave_primaria, sizeof(float));
    offset += sizeof(float);
    memcpy(destino + offset, &data_value, sizeof(float));
    offset += sizeof(float);
    memcpy(destino + offset, status, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;
    memcpy(destino + offset, units, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;
    memcpy(destino + offset, &magnitude, sizeof(int));
    offset += sizeof(int);
    memcpy(destino + offset, subject, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;
    memcpy(destino + offset, group, REGRAS::TAMANHO_CAMPO_REG);
    offset += REGRAS::TAMANHO_CAMPO_REG;
    for (int i = 0; i < 4; i++)
    {
        memcpy(destino + offset, series_title[i], REGRAS::TAMANHO_CAMPO_REG);
        offset += REGRAS::TAMANHO_CAMPO_REG;
    }
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

void Registro::setTitle(string pTitles[])
{
    for (int i = 0; i < 4; i++)
    {
        copiarString(series_title[i], pTitles[i]);
    }
}

