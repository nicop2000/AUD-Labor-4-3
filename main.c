#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_POSSIBLE_WORDLENGTH 20

typedef struct _WordStoreEntry {
    char word[MAX_POSSIBLE_WORDLENGTH + 1];
    int count;
    struct _WordStoreEntry *pNext;
} WordStoreEntry;

typedef struct {
    WordStoreEntry *start;
} TWordStore;

TWordStore *Init(); // erzeugt neues 'WordStore'-Objekt (Konstruktor)
void Done(TWordStore *ws); // Löscht Objekt (Destruktor)
void Put(TWordStore *ws, char *word); // Wort 'word' hinzufügen
int GetCount(TWordStore *ws, char *word); // gibt zurück, wie oft das Wort 'word' hinzugefügt wurde
// 'func' ist ein Zeiger auf eine Funktion, sie wird von
// 'ForEach' für jedes Wort aufgerufen, das mindestens einmal
// hinzugefügt wurde; an die Funktion übergeben wird das Wort sowie // dessen Anzahl.
// Syntax (z.B.): 'func ("Wort", 17);'
void ForEach(TWordStore *ws, void (*func)(char *, int));

WordStoreEntry *createNewElement(char *word);




TWordStore *Init() {
    TWordStore *new = (TWordStore *) malloc(sizeof(TWordStore));
    if (new) {
        new->start = NULL;
    }
    return new;
}


WordStoreEntry *createNewElement(char *word) {
    WordStoreEntry *newEntry = malloc(sizeof(WordStoreEntry));
    if (newEntry) {
        strncpy(newEntry->word, word, MAX_POSSIBLE_WORDLENGTH + 1);
        newEntry->count = 1;
        newEntry->pNext = NULL;
    }
    return newEntry;
}


void Put(TWordStore *ws, char *word) {

    if (ws->start != NULL) {
        WordStoreEntry *temp = ws->start;
        while (temp) {
            if (strcmp(word, temp->word) == 0) {
                temp->count += 1;
                return;
            }
            temp = temp->pNext;
        }

    } else {
        WordStoreEntry *new = createNewElement(word);
        if (new) {
            ws->start = new;
        }
        return;
    }

    WordStoreEntry *new = createNewElement(word);
    if (new) {
        new->pNext = ws->start;
        ws->start = new;
    }
    return;
}

int GetCount(TWordStore *ws, char *word) { // gibt zurück, wie oft das Wort 'word' hinzugefügt wurde

    if (ws->start != NULL) {
        WordStoreEntry *temp = ws->start;
        while (temp) {
            if (strcmp(word, temp->word) == 0) {
                return temp->count;
            }
            temp = temp->pNext;
        }
        return 0;
    } else {
        return 0;
    }

}

void Done(TWordStore *ws) {
    if (!ws->start) {
        return;
    }
    while (ws->start) {
        WordStoreEntry *temp = ws->start;
        ws->start = ws->start->pNext;
        free(temp);
    }
    return;
}

void readFromFile(TWordStore *ws) {
    FILE *file1;

    file1 = fopen("test.txt", "r"); // please work!

    if (file1 == NULL) {
        printf("Datei konnte nicht gefunden werden\n");
        return;
    } else {
        int next = -2;
        while (next != EOF) {
            char currentWord[MAX_POSSIBLE_WORDLENGTH + 1];
            for (int i = 0; i <= MAX_POSSIBLE_WORDLENGTH; i++) {
                currentWord[i] = '\0';
            }
            for (i = 0; i < MAX_POSSIBLE_WORDLENGTH; i++) {
                char currentChar = tolower(fgetc(file1));
                next = currentChar;
                if (currentChar == 32) break;

                if (currentChar < 'a') break;
                currentWord[i] = currentChar;
            }
            if (strcmp(currentWord,"") != 0) Put(ws, currentWord);
        }
        fclose(file1);
    }
}

void printAll(TWordStore *ws) {
    if (!ws) {
        printf("WordStore is empty\n");
        return;
    }
    WordStoreEntry *temp = ws->start;
    while(temp) {
        printf("Das Wort '%20s' ist %6dx aufgetreten.\n", temp->word, temp->count);
        temp = temp->pNext;
    }
    return;

}


int main() {
    printf("Hello, World!\n");
    TWordStore *ws = Init();
    Put(ws, "abc");
    Put(ws, "abc");
    Put(ws, "def");
    readFromFile(ws);
    printf("Anzahl 'lord' = %d\n", GetCount(ws, "lord"));
    printAll(ws);
    printf("Anzahl 'abc' = %i, Anzahl 'def' = %i, Anzahl 'ghi' = %i\n", GetCount(ws, "abc"), GetCount(ws, "def"),
           GetCount(ws, "ghi"));
    printf("Alles:\n");
    // ForEach (ws, PrintWordCount);
    Done(ws);

    return 0;
}
