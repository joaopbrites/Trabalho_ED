#include <iostream>
#include <cstdint>

struct cabecalhoParaRegistro {
    uint8_t flags; // Para indicar status (ex: 0x01 = ativo/válido, 0x02 = modificado, 0x04 = excluído logicamente)
    uint32_t qtdBytesValifos;
};
