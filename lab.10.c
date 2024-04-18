#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie {
  struct Trie *children[ALPHABET_SIZE];
  int isEndOfWord;
  int count;
};
// Initializes a trie structure
struct Trie *createTrie() {
  struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
  if (pTrie) {
    pTrie->isEndOfWord = 0;
    pTrie->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      pTrie->children[i] = NULL;
    }
  }
  return pTrie;
}
// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
  struct Trie *current = pTrie;
  int len = strlen(word);
  for (int i = 0; i < len; i++) {
    int index = word[i] - 'a';
    if (!current->children[index])
      current->children[index] = createTrie();
    current = current->children[index];
  }
  if (current) {
    current->isEndOfWord = 1;
    current->count++;
  }
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie *pTrie, char *word) {
  struct Trie *current = pTrie;
  int len = strlen(word);
  for (int i = 0; i < len; i++) {
    int index = word[i] - 'a';
    if (!current || !current->children[index])
      return 0;
    current = current->children[index];
  }
  return (current && current->isEndOfWord) ? current->count : 0;
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
  if (pTrie == NULL)
    return NULL;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    deallocateTrie(pTrie->children[i]);
  }
  free(pTrie);
  return NULL;
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  int count = 0;
  char word[256];
  while (fscanf(file, "%s", word) == 1) {
    pInWords[count] = strdup(word);
    count++;
  }

  fclose(file);
  return count;
}

int main(void) {
  char *inWords[256];

  // Read the number of words in the dictionary
  int numWords = readDictionary("dictionary.txt", inWords);
  for (int i = 0; i < numWords; ++i) {
    printf("%s\n", inWords[i]);
  }

  struct Trie *pTrie = createTrie();
  for (int i = 0; i < numWords; i++) {
    insert(pTrie, inWords[i]);
  }

  // Words to check
  char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
  for (int i = 0; i < 5; i++) {
    printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
  }

  pTrie = deallocateTrie(pTrie);
  if (pTrie != NULL)
    printf("There is an error in this program\n");

  return 0;
}
