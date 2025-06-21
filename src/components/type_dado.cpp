#include "cabecalhoDado.hpp"
#include "config.hpp"
#include "type_dado.hpp"
#include <cstring>
#include <sstream>


using namespace std;

bool Registro::copiarString(char *destino, const string &origem)
{
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
                       series_title()
                       
{
    cabecalhoRegistro.flags = FLAGS::VAZIO;
}
Registro::Registro(float pChave) : series_reference(),
                                                 chave_primaria(pChave),
                                                 data_value(INVALID_VALUES::DATA_VALUE),
                                                 status(),
                                                 units(),
                                                 magnitude(INVALID_VALUES::MAGNITUDE),
                                                 subject(),
                                                 group(),
                                                 series_title()
                                                 

{
    cabecalhoRegistro.flags = FLAGS::VAZIO;
}

// Construtor de cópia
Registro::Registro(const Registro& other) {
    this->cabecalhoRegistro = other.cabecalhoRegistro;
    memcpy(this->series_reference, other.series_reference, REGRAS::TAMANHO_CAMPO_REG);
    this->chave_primaria = other.chave_primaria;
    this->data_value = other.data_value;
    memcpy(this->status, other.status, REGRAS::TAMANHO_CAMPO_REG);
    memcpy(this->units, other.units, REGRAS::TAMANHO_CAMPO_REG);
    this->magnitude = other.magnitude;
    memcpy(this->subject, other.subject, REGRAS::TAMANHO_CAMPO_REG);
    memcpy(this->group, other.group, REGRAS::TAMANHO_CAMPO_REG);
    for (int i = 0; i < 4; ++i) {
        memcpy(this->series_title[i], other.series_title[i], REGRAS::TAMANHO_CAMPO_REG);
    }
}

// Operador de cópia
Registro& Registro::operator=(const Registro& other) {
    if (this != &other) {
        this->cabecalhoRegistro = other.cabecalhoRegistro;
        memcpy(this->series_reference, other.series_reference, REGRAS::TAMANHO_CAMPO_REG);
        this->chave_primaria = other.chave_primaria;
        this->data_value = other.data_value;
        memcpy(this->status, other.status, REGRAS::TAMANHO_CAMPO_REG);
        memcpy(this->units, other.units, REGRAS::TAMANHO_CAMPO_REG);
        this->magnitude = other.magnitude;
        memcpy(this->subject, other.subject, REGRAS::TAMANHO_CAMPO_REG);
        memcpy(this->group, other.group, REGRAS::TAMANHO_CAMPO_REG);
        for (int i = 0; i < 4; ++i) {
            memcpy(this->series_title[i], other.series_title[i], REGRAS::TAMANHO_CAMPO_REG);
        }
    }
    return *this;
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

void Registro::setStatus(const uint8_t pStatus)
{
    cabecalhoRegistro.flags = pStatus;
}

void Registro::setStatusReference(const string &pStatus)
{
    copiarString(this->status, pStatus);
}

void Registro::setUnits(const string &pUnits)
{
    copiarString(units, pUnits);
}

void Registro::setMagnitude(const int pMagnitude)
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

