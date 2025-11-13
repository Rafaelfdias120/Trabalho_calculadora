#ifndef CALCULADORA_H
#define CALCULADORA_H

// O cabeçalho expressao.h (ou calculadora.h) não deve ser alterado.
// Estrutura e protótipos extraídos do PDF.

/**
 * @brief Estrutura para armazenar a expressão em diferentes formas e seu valor.
 */
typedef struct {
    char posFixa[512]; // Expressão na forma pos-fixa, como 3 12 4 + *
    char inFixa[512];  // Expressão na forma in-fixa, como 3*(12+4)
    float valor;       // Valor numérico da expressão
} Expressao;

/**
 * @brief Converte uma expressão pós-fixa para a forma infixa.
 * 
 * @param Str A string contendo a expressão pós-fixa.
 * @return char* A string contendo a expressão infixa. Deve retornar NULL em caso de erro.
 * A string retornada não deve conter espaços nem parênteses desnecessários.
 */
char * getFormaInFixa(char *Str);

/**
 * @brief Calcula o valor numérico de uma expressão pós-fixa.
 * 
 * @param Str A string contendo a expressão pós-fixa.
 * @return float O valor numérico da expressão.
 */
float getValorPosFixa(char *Str);

#endif // CALCULADORA_H
