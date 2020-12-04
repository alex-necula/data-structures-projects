#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* Key;
typedef int Value;
typedef long(*HashFunction)(Key, long);

typedef struct Element {
  Key key;
  Value value;
  struct Element *next;
} Element;

typedef struct HashTable {
  Element** elements;
  long size;
  HashFunction hashFunction;
} HashTable;

void initHashTable(HashTable **h, long size, HashFunction f) {
  (*h) = (HashTable*) malloc(sizeof(HashTable));
  (*h)->size = size;
  (*h)->elements = (Element**) malloc(sizeof(Element*) * size);
  for (int i = 0; i < size; i++) {
    (*h)->elements[i] = NULL;
  }
  (*h)->hashFunction = f;
}

int exists(HashTable *hashTable, Key key) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e = hashTable->elements[idx];
  while (e != NULL) {
    if (!strcmp(e->key, key)) {
      return 1;
    }
    e = e->next;
  }
  return 0;
}

Value get(HashTable *hashTable, Key key) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e = hashTable->elements[idx];
  while (e != NULL) {
    if (!strcmp(e->key, key)) {
      return e->value;
    }
    e = e->next;
  }
}

void put(HashTable *hashTable, Key key, Value value) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e = hashTable->elements[idx];
  while (e != NULL) {
    if (!strcmp(e->key, key)) {
      e->value = value;
      return;
    }
    e = e->next;
  }

  e = (Element*) malloc(sizeof(Element));
  e->key = (Key)strdup(key);
  e->value = value;

  e->next = hashTable->elements[idx];
  hashTable->elements[idx] = e;
}

void deleteKey(HashTable *hashTable, Key key) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e1 = hashTable->elements[idx];
  Element* e2 = e1->next;
  if (!strcmp(e1->key, key)) {
    hashTable->elements[idx] = e2;
    free(e1->key);
    free(e1);
  } else {
    while (strcmp(e2->key, key)) {
      e1 = e2;
      e2 = e2->next;
    }
    e1->next = e2->next;
    free(e2->key);
    free(e2);
  }
}

void print(HashTable *hashTable) {
  long idx;
  Element* e;
  printf("---START---\n");
  for (idx = 0; idx < hashTable->size; idx++) {
    e = hashTable->elements[idx];
    if (e != NULL) {
      printf("%ld:\n", idx);
      while (e != NULL) {
        printf("\t%s : %d\n", e->key, e->value);
        e = e->next;
      }
    }
  }
  printf("----END----\n");
}

void freeHashTable(HashTable *hashTable) {
  Element *e1, *e2;
  long idx;

  for (idx = 0; idx < hashTable->size; idx++) {
    e1 = hashTable->elements[idx];
    while (e1 != NULL) {
      e2 = e1->next;
      free(e1->key);
      free(e1);
      e1 = e2;
    }
  }
  free(hashTable->elements);
  free(hashTable);
}


long hash1(Key word, long size) {
  int i = 0;
  long hash = 0;
  while (word[i] != '\0') {
    hash = hash * 17 + word[i];
    i++;
  }
  return hash % size;
}

int main(int argc, char* argv[]) {
  HashTable *hashTable;
  FILE *f1, *f2;
  char word[256];
  long hashSize, common;

  hashSize = atoi(argv[1]);
  f1 = fopen(argv[2], "r");
  f2 = fopen(argv[3], "r");

  initHashTable(&hashTable, hashSize, &hash1);

  // Cerinta 3
  while (fscanf(f1, "%s", word) != EOF) {
    if (exists(hashTable, word)) {
      put(hashTable, word, get(hashTable, word) +1);
    } else {
      put(hashTable, word, 1);
    }
  }

  print(hashTable);

  // Cerinta 4
  common = 0;
  long cnt;

  while (fscanf(f2, "%s", word) != EOF) {
    if (exists(hashTable, word)) {
      common = common + 1;
      cnt = get(hashTable, word);
      if (cnt == 1) {
        deleteKey(hashTable, word);
      } else {
        put(hashTable, word, cnt - 1);
      }
    }
  }

  printf("Common words: %ld\n", common);

  fclose(f1);
  fclose(f2);
  return 0;
}
