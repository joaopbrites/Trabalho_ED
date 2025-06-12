#pragma once

#ifndef CABECALHO_DADO
#define CABECALHO_DADO

#include <cstdint>

struct cabecalhoParaRegistro {
    uint8_t flags; // Para indicar status (ex: 0x01 = ativo/válido, 0x04 = excluído logicamente)
};

#endif

