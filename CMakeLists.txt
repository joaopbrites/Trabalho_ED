cmake_minimum_required(VERSION 3.15) # versão mínima para rodar esse o cmake
#verificar no linux do laboratorio

project(TrabalhoPratico VERSION 1.0) #nome provisório 



add_executable(
    ${PROJECT_NAME} 
    src/components/ArquivoManager.cpp
    src/components/Buffer.cpp
    src/components/Inicializador.cpp
    src/ordenacao/OrdenacaoDecrescente.cpp
    src/components/type_block.cpp
    src/components/type_dado.cpp
    src/main.cpp
    src/conversao-segmentacao/SegmentadorBin.cpp
    src/conversao-segmentacao/SegmentadorCSV.cpp
)


# Adiciona o diretório 'include' ao caminho de busca por arquivos .h/.hpp
target_include_directories(${PROJECT_NAME} PUBLIC headers)
target_include_directories(${PROJECT_NAME} PUBLIC protocolos)


# Adiciona flags de compilação que o compilador normalmente ignora.
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -g)


#verificar aaversão de c++ do laboratorio
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
