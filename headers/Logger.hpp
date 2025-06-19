#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <string>
#include <fstream>



using namespace std;

class Logger {
public:
    // Métodos de log (níveis de erro)
    void error(const string& message);
    void warning(const string& message);
    void info(const string& message);

    // Construtor recebe um nome base para o arquivo (opcional)
    Logger();
    ~Logger();

private:
    string infoName;
    string warningName;
    string errorName;
    ofstream infoFile;
    ofstream warningFile;
    ofstream errorFile;
    string getCurrentTime() const;
    string generateLogFileName(const string& level) const;
    void writeLog(ofstream& file, const string& level, const string& message);
};

#endif