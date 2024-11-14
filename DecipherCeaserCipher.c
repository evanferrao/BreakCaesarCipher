#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALPHABET_SIZE 26

const double ENGLISH_LETTER_FREQUENCIES[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

int my_isalpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int my_isupper(char c) {
    return (c >= 'A' && c <= 'Z');
}

void countLetterFrequencies(const char *text, int *frequencies) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        frequencies[i] = 0;
    }
    for (int i = 0; text[i] != '\0'; i++) {
        if (my_isalpha(text[i])) {
            frequencies[my_tolower(text[i]) - 'a']++;
        }
    }
}

void shiftFrequencies(const int *frequencies, int *shiftedFrequencies, int shift) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        shiftedFrequencies[i] = 0;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        shiftedFrequencies[(i + shift) % ALPHABET_SIZE] = frequencies[i];
    }
}

double calculateScore(const int *frequencies) {
    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double observedFrequency = frequencies[i];
        double expectedFrequency = ENGLISH_LETTER_FREQUENCIES[i];
        score += fabs(observedFrequency - expectedFrequency);
    }
    return score;
}

int findBestShift(const int *letterFrequencies) {
    int bestShift = 0;
    double bestScore = __DBL_MAX__;
    for (int shift = 1; shift < ALPHABET_SIZE; shift++) {
        int shiftedFrequencies[ALPHABET_SIZE];
        shiftFrequencies(letterFrequencies, shiftedFrequencies, shift);
        double score = calculateScore(shiftedFrequencies);
        if (score < bestScore) {
            bestShift = shift;
            bestScore = score;
        }
    }
    return bestShift;
}

void shiftText(const char *text, char *shiftedText, int shift) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (my_isalpha(text[i])) {
            char base = 'a';
            shiftedText[i] = (char) ((my_tolower(text[i]) - base + shift) % ALPHABET_SIZE + base);
        } else {
            shiftedText[i] = text[i];
        }
    }
    shiftedText[strlen(text)] = '\0';
}

void restoreCase(const char *originalText, const char *shiftedText, char *finalText) {
    for (int i = 0; originalText[i] != '\0'; i++) {
        if (my_isupper(originalText[i])) {
            finalText[i] = shiftedText[i] - ('a' - 'A');
        } else {
            finalText[i] = shiftedText[i];
        }
    }
    finalText[strlen(originalText)] = '\0';
}

void decipher(const char *encryptedText, char *decryptedText) {
    int letterFrequencies[ALPHABET_SIZE];
    countLetterFrequencies(encryptedText, letterFrequencies);
    int bestShift = findBestShift(letterFrequencies);

    char shiftedText[1024];
    shiftText(encryptedText, shiftedText, bestShift);

    restoreCase(encryptedText, shiftedText, decryptedText);
}

int main() {
    char encryptedText[1024];
    printf("Enter the message to decipher: ");
    fgets(encryptedText, sizeof(encryptedText), stdin);
    encryptedText[strcspn(encryptedText, "\n")] = '\0'; // Remove newline character

    char decryptedText[1024];
    decipher(encryptedText, decryptedText);
    printf("Decrypted text: %s\n", decryptedText);

    return 0;
}