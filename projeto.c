#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ATTEMPTS 6
#define WORD_LENGTH 5
#define DICTIONARY_FILE "sem_acentos.txt"
#define SCORES_FILE "scores.txt"

void loadDictionary(char dictionary[][WORD_LENGTH + 1], int *numWords) {
    FILE *file = fopen(DICTIONARY_FILE, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo do dicionário.\n");
        exit(EXIT_FAILURE);
    }

    *numWords = 0;
    while ((*numWords < 10000) && (fscanf(file, "%s", dictionary[*numWords]) == 1)) {
        (*numWords)++;
    }

    fclose(file);
}

void getRandomWord(char dictionary[][WORD_LENGTH + 1], int numWords, char *word) {
    strcpy(word, dictionary[rand() % numWords]);
}

int checkWordInDictionary(char dictionary[][WORD_LENGTH + 1], int numWords, char *word) {
    for (int i = 0; i < numWords; i++) {
        if (strcmp(dictionary[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int checkLetter(char *word, char *guess, char letter) {
    int found = 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (word[i] == letter) {
            guess[i] = letter;
            found = 1;
        }
    }
    return found;
}

int main() {
    srand(time(NULL));

    char dictionary[10000][WORD_LENGTH + 1];
    int numWords;
    loadDictionary(dictionary, &numWords);

    char word[WORD_LENGTH + 1];
    getRandomWord(dictionary, numWords, word);

    char guess[WORD_LENGTH + 1];
    memset(guess, '_', WORD_LENGTH);
    guess[WORD_LENGTH] = '\0';

    printf("Bem-vindo ao jogo Wordle!\n");
    printf("Adivinhe a palavra de %d letras.\n", WORD_LENGTH);

    int attempts = 0;
    while (attempts < MAX_ATTEMPTS) {
        printf("\nTentativa %d de %d\n", attempts + 1, MAX_ATTEMPTS);
        printf("Palavra: %s\n", guess);

        char input[WORD_LENGTH + 1];
        printf("Digite sua tentativa: ");
        scanf("%s", input);

        if (strlen(input) != WORD_LENGTH) {
            printf("Por favor, digite uma palavra de %d letras.\n", WORD_LENGTH);
            continue;
        }

        if (!checkWordInDictionary(dictionary, numWords, input)) {
            printf("Palavra não reconhecida. Tente novamente.\n");
            continue;
        }

        if (strcmp(input, word) == 0) {
            printf("Parabéns! Você acertou a palavra.\n");

            char playerName[50];
            printf("Digite seu nome: ");
            scanf("%s", playerName);

            FILE *scoresFile = fopen(SCORES_FILE, "a");
            if (scoresFile == NULL) {
                printf("Erro ao abrir o arquivo de pontuações.\n");
                exit(EXIT_FAILURE);
            }

            fprintf(scoresFile, "%s %s %d %ld\n", playerName, word, attempts + 1, time(NULL));
            fclose(scoresFile);

            break;
        } else {
            printf("Palavra incorreta. Tente novamente.\n");
            attempts++;
        }
    }

    if (attempts == MAX_ATTEMPTS) {
        printf("\nVocê excedeu o número máximo de tentativas. A palavra era: %s\n", word);
    }

    return 0;
}
