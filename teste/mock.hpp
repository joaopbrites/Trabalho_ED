#pragma once

#ifndef MOCK_TEST
#define MOCK_TEST

#include "type_block.hpp"
#include "Logger.hpp"

namespace mock {
    // Logger de teste (pode ser global ou estático, pois não será usado de verdade)
    static Logger loggerMock("mock");
    // BlocoRegistros vazio para testes
    static BlocoRegistros blocoMock(&loggerMock);
}

#endif
