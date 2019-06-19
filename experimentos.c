#include "SuffixTree.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
char adn_ready[8388700];
char english_ready[8388700];

int random_number(int lower, int upper) {
  srand(time(0));
  int num = (rand() % (upper - lower + 1)) + lower;
  return num;
}

int is_char_good(char c){
  if (c == '.' || c == ',' || c == ';' || c == ':' || c == ' '
        || c == '\n' || c == '\t') {
    return 0;
  }
  else {
    return 1;
  }
}

void preprocess_files(char* filename, int sizetext) {
  char source[sizetext + 1];
  FILE *fp = fopen(filename, "r");
  if (fp != NULL) {
    int i = 0;
    while((symbol = getc(fp)) != EOF && i < sizetext) {
      char tolower_char = tolower(symbol);
      int add_char = is_char_good(tolower_char);
      if (add_char) {
        strcat(source, tolower_char);
        i++;
      }
    }
    if (filename == "dna.50MB.txt") {
      adn_ready = source;
    }
    else if (filename == "english.50MB.txt") {
      english_ready = source;
    }

    fclose(fp);
  }
}

double experiment_count_adn(int sizetext, char* patron){
  //procesar source como string
  clock_t t;
  t = clock();
  int contador = count(adn_ready, patron);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; //en segundos
}

double experiment_count_english(int sizetext, char* patron){
  //procesar source como string
  clock_t t;
  t = clock();
  int contador = count(english_ready, patron);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; //en segundos
}

int int main(int argc, char const *argv[]) {
  int* n_largos = [1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
                1048576, 2097152, 4194304, 8388608];
  preprocess_files("adn.50MB.txt", n_largos[0]);
  preprocess_files("english.50MB.txt", n_largos[0]);
  //Primera coleccion: texto en inglés
  //  Count, patrón tipo palabra aleatorio, english text

  //double time1 = experiment_count(n_largos[0], );

  // Count, patrón tamaño m, ADN, n/10 strings
  int* m = [8, 16, 32, 64];
  for (int i = 0; i < 14; i++) {  //Iteracion en n_largos
    for (int j = 0; j < 4; j++) { //Iteracion en tamaño del patron
      int random = random_number(0, n_largos[i] - m[j]);
      char patron[m[j] + 1];
      strncpy(patron, adn_ready[8388700], m[j]);
      patron[m[j]] = '\0';
      double time2 = experiment_count_adn(n_largos[i], patron);
      printf("Tiempo %f\n", time2);
    }
  }
  return 0;
}
