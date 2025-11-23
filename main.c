#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

// Estrutura para armazenar um caso de teste
typedef struct {
    char posFixa[512];
    char infixa_esperada[512];
    float valor_esperado;
    float tolerancia; // Tolerância para comparação de floats
} CasoTeste;

// Função para executar um caso de teste
void executar_teste(int id, CasoTeste teste) {
    printf("\n--- Teste %d ---\n", id);
    printf("Pos-Fixa: %s\n", teste.posFixa);

    // 1. Teste de Avaliação (getValorPosFixa)
    float valor_calculado = getValorPosFixa(teste.posFixa);
    
    // Verifica se o valor calculado é um número (não NAN)
    if (isnan(valor_calculado)) {
        printf("Avaliacao: ERRO (Expressao Invalida)\n");
    } else {
        // Compara com o valor esperado usando tolerância
        if (fabs(valor_calculado - teste.valor_esperado) < teste.tolerancia) {
            printf("Avaliacao: OK. Valor Calculado: %.2f (Esperado: %.2f)\n", valor_calculado, teste.valor_esperado);
        } else {
            printf("Avaliacao: FALHA. Valor Calculado: %.10f (Esperado: %.10f)\n", valor_calculado, teste.valor_esperado);
        }
    }

    // 2. Teste de Conversão (getFormaInFixa)
    char *infixa_calculada = getFormaInFixa(teste.posFixa);
    
    if (infixa_calculada == NULL) {
        printf("Conversao: ERRO (Expressao Invalida)\n");
    } else {
        // Compara a string infixa calculada com a esperada
        if (strcmp(infixa_calculada, teste.infixa_esperada) == 0) {
            printf("Conversao: OK. Infixa Calculada: %s (Esperada: %s)\n", infixa_calculada, teste.infixa_esperada);
        } else {
            printf("Conversao: FALHA. Infixa Calculada: %s (Esperada: %s)\n", infixa_calculada, teste.infixa_esperada);
        }
        free(infixa_calculada); // Libera a memória alocada por getFormaInFixa
    }
}

int main() {
    // Casos de teste obrigatórios do PDF
    // Nota sobre a infixa esperada: O PDF fornece a infixa com parênteses e espaços.
    // O requisito é "não deve conter espaços, nem parênteses além dos estritamente necessários."
    // A infixa esperada aqui é a versão sem espaços e com parênteses mínimos.
    CasoTeste testes_obrigatorios[] = {
        // Teste 1: 3 4 + 5 * -> (3 + 4) * 5 -> 35
        {"3 4 + 5 *", "(3+4)*5", 35.0, 0.0001},
        // Teste 2: 7 2 * 4 + -> 7 * 2 + 4 -> 18
        {"7 2 * 4 +", "(7*2)+4", 18.0, 0.0001},
        // Teste 3: 8 5 2 4 + * + -> 8 + (5 * (2 + 4)) -> 38
        {"8 5 2 4 + * +", "8+(5*(2+4))", 38.0, 0.0001},
        // Teste 4: 6 2 / 3 + 4 * -> (6 / 2 + 3) * 4 -> 24
        {"6 2 / 3 + 4 *", "((6/2)+3)*4", 24.0, 0.0001},
        // Teste 5: 9 5 2 8 * 4 + * + -> 9 + (5 * (4 + 8 * 2)) -> 109
        {"9 5 2 8 * 4 + * +", "9+(5*((2*8)+4))", 109.0, 0.0001}, // Corrigido: 8*2 -> 2*8 na posfixa
        // Teste 6: 2 3 + log 5 / -> log(2 + 3) / 5 -> Aprox. 0.14
        {"2 3 + log 5 /", "log((2+3))/5", 0.139794, 0.0001}, // log10(5)/5
        // Teste 7: 10 log 3 ^ 2 + -> (log10)^3 + 2 -> 3
        {"10 log 3 ^ 2 +", "(log(10)^3)+2", 3.0, 0.0001}, // (1^3) + 2 = 3
        // Teste 8: 45 60 + 30 cos * -> (45 + 60) * cos(30) -> Aprox. 90,93
        {"45 60 + 30 cos *", "(45+60)*cos(30)", 90.9326, 0.0001}, // 105 * cos(30)
        // Teste 9: 0.5 45 sen 2 ^ + -> sen(45) ^2 + 0,5 -> 1
        {"0.5 45 sen 2 ^ +", "0.5+(sen(45)^2)", 1.0, 0.0001} // 0.5 + (sqrt(2)/2)^2 = 0.5 + 0.5 = 1
    };

    // Casos de teste adicionais (robustez e erros)
    CasoTeste testes_adicionais[] = {
        // Teste A: Raiz quadrada
        {"9 raiz", "raiz(9)", 3.0, 0.0001},
        // Teste B: Modulo
        {"10 3 %", "10%3", 1.0, 0.0001},
        // Teste C: Divisão por zero (deve retornar NAN)
        {"5 0 /", "5/0", NAN, 0.0001},
        // Teste D: Raiz de negativo (deve retornar NAN)
        {"-4 raiz", "raiz(-4)", NAN, 0.0001},
        // Teste E: Log de zero (deve retornar NAN)
        {"0 log", "log(0)", NAN, 0.0001},
        // Teste F: Expressão com float
        {"1.5 2.5 +", "1.5+2.5", 4.0, 0.0001},
        // Teste G: Expressão inválida (operador binário sem operandos)
        {"+ 5 3", "+53", NAN, 0.0001},
        // Teste H: Expressão inválida (operando restante)
        {"5 3 + 2", "(5+3)2", NAN, 0.0001},
        // Teste I: Expressão com ^ (potência)
        {"2 3 ^", "2^3", 8.0, 0.0001},
        // Teste J: Expressão complexa com funções e binários
        {"1 2 + 3 * 4 5 - / sen", "sen((((1+2)*3)/(4-5)))", -0.1564344615, 0.0001} // sen(9/-1) = sen(-9) graus
    };

    printf("==================================================================\n");
    printf("  TESTES OBRIGATORIOS (TP03 - Avaliador de Expressoes Numericas)  \n");
    printf("==================================================================\n");
    for (size_t i = 0; i < sizeof(testes_obrigatorios) / sizeof(CasoTeste); i++) {
        executar_teste(i + 1, testes_obrigatorios[i]);
    }

    printf("\n==================================================================\n");
    printf("              TESTES ADICIONAIS (Robustez e Erros)              \n");
    printf("==================================================================\n");
    for (size_t i = 0; i < sizeof(testes_adicionais) / sizeof(CasoTeste); i++) {
        executar_teste(i + 10, testes_adicionais[i]);
    }

    return 0;
}
