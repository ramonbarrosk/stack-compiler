#include "../utils/utils.cpp";

void lexer(const string &input_file)
{
    ifstream input(input_file);
    if (!input.is_open())
    {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return;
    }
    string line;
    int line_number = 0;
    while (getline(input, line))
    {
        line_number++;
        size_t i = 0;
        while (i < line.size())
        {
            // Ignorar espaços em branco
            while (i < line.size() && is_whitespace(line[i]))
            {
                i++;
            }
            // Verificar delimitadores e símbolos
            if (i < line.size() && is_delimiter(line[i]))
            {
                cout << "DELIMITADOR\t" << line[i] << "\tlinha " << line_number << endl;
                i++;
            }
            else if (i < line.size() && is_symbol(line[i]))
            {
                // Verificar se o símbolo é um operador de dois caracteres
                if (i + 1 < line.size() && is_operator(line[i]) && is_operator(line[i + 1]))
                {
                    cout << "OPERADOR\t" << line.substr(i, 2) << "\tlinha " << line_number << endl;
                    i += 2;
                }
                else
                {
                    cout << "OPERADOR\t" << line[i] << "\tlinha " << line_number << endl;
                    i++;
                }
            }
            else
            {
                // Ler a próxima palavra
                size_t j = i;
                while (j < line.size() && !is_delimiter(line[j]) && !is_symbol(line[j]))
                {
                    j++;
                }
                string word = line.substr(i, j - i);
                // Verificar se a palavra é uma palavra-chave
                if (is_keyword(word))
                {
                    cout << "PALAVRA-CHAVE\t" << word << "\tlinha " << line_number << endl;
                }
                else
                {
                    // Verificar se a palavra é um número
                    if (is_number(word))
                    {
                        cout << "NUMERO\t" << word << "\tlinha " << line_number << endl;
                    }
                    else
                    {
                        // Se não for palavra-chave nem número, é um identificador
                        cout << "IDENTIFICADOR\t" << word << "\tlinha " << line_number << endl;
                    }
                }
                i = j;
            }
        }
    }
    input.close();
}