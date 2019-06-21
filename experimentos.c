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

void preprocess_english_file(char* filename, int sizetext) {
  char source[sizetext + 1];
  FILE *fp = fopen("english50MB.txt", "r");
  if (fp != NULL) {
    int i = 0;
    char symbol = fgetc(fp);
    while(symbol != EOF && i < sizetext) {
      char tolower_char = tolower(symbol);
      int add_char = is_char_good(tolower_char);
      if (add_char) {
        printf("%c\n", tolower_char);
        char *pChar = malloc(sizeof(char));
        *pChar = tolower_char;
        strcat(source, pChar);
        i++;
      }
      symbol = fgetc(fp);
    }
    source[sizetext] = '\0';
    if (strncmp(filename, "dna50MB.txt", 12)) {
      strcpy(adn_ready, source);
    }
    else if (strncmp(filename,"english50MB.txt", 12)) {
      strcpy(english_ready, source);
    }

    fclose(fp);
  }
}

void preprocess_dna_file(int sizetext) {
  char source[sizetext + 1];
  FILE *fp = fopen("dna50MB.txt", "r");
  if (fp != NULL) {
    int i = 0;
    char symbol = fgetc(fp);
    while(symbol != EOF && i < sizetext) {
      char tolower_char = tolower(symbol);
      int add_char = is_char_good(tolower_char);
      if (add_char) {
        /*printf("%c\n", tolower_char);*/
        char *pChar = malloc(sizeof(char));
        *pChar = tolower_char;
        strncat(adn_ready, pChar, 1);
        i++;
      }
      symbol = fgetc(fp);
    }
    adn_ready[sizetext] = '\0';
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
  return time_taken;
}

double experiment_locate_adn(int sizetext, char* patron){
  //procesar source como string
  clock_t t;
  t = clock();
  locate(adn_ready, patron);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; //en segundos
  return time_taken;
}

double experiment_construction_adn(char* T, int max){
  strncpy(text, T, max);
  strncat(text,"$", 1);
  clock_t t = clock();
  buildSuffixTree();
  t= clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; //en segundos
  return time_taken;
}

double experiment_count_english(int sizetext, char* patron){
  //procesar source como string
  clock_t t;
  t = clock();
  int contador = count(english_ready, patron);
  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; //en segundos
  return time_taken;
}

int main(int argc, char const *argv[]) {
  int n_largos[] = {1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288,
                1048576, 2097152, 4194304, 8388608};
  preprocess_dna_file(n_largos[0]);
  //preprocess_english_file(n_largos[0]);
  //Primera coleccion: texto en inglés
  //  Count, patrón tipo palabra aleatorio, english text


  //double time1 = experiment_count(n_largos[0], );

  // Segunda coleccion: cadenas de ADN
  // Count, patrón tamaño m, ADN, n/10 strings
  FILE* file_count_adn = fopen("file_count_adn.txt", "ab+");
  FILE* file_locate_adn = fopen("file_locate_adn.txt", "ab+");
  fprintf(file_count_adn, "%s\n", "New file for COUNT ADN file");
  fprintf(file_locate_adn, "%s\n", "New file for LOCATE ADN file");
  double time1 = experiment_construction_adn(adn_ready, 8388700);
  fprintf(file_count_adn, "Tiempo construccion: %f\n", time1);
  int m[] = {8, 16, 32, 64};
  fprintf(file_count_adn, "TEST ADN\n");
  fprintf(file_locate_adn, "TEST ADN\n");
  fprintf(file_count_adn, "-> TEST COUNT\n");
  fprintf(file_locate_adn, "-> TEST LOCATE\n");
  printf("%s\n", adn_ready);
  for (int i = 0; i < 1; i++) {  //Iteracion en n_largos
    fprintf(file_count_adn, "\n---------------------\n");
    fprintf(file_locate_adn, "\n---------------------\n");
    fprintf(file_count_adn, "n_largos: %d\n", n_largos[i]);
    fprintf(file_locate_adn, "n_largos: %d\n", n_largos[i]);
    for (int j = 0; j < 4; j++) { //Iteracion en tamaño del patron
      fprintf(file_count_adn, "Tamaño patron: %d\n", m[j]);
      fprintf(file_locate_adn, "Tamaño patron: %d\n", m[j]);
      for (int z = 0; z < (n_largos[i]/40); z++) { //N/40 experimentos por cada m
        int random = random_number(0, n_largos[i] - m[j]);
        char patron[m[j] + 1];
        strncpy(patron, adn_ready + random, m[j]);
        //patron[m[j]] = '\0';
        printf("%s\n", patron);
        double time2 = experiment_count_adn(n_largos[i], patron);
        fprintf(file_count_adn, "%f\n", time2);
        double timeloc = experiment_locate_adn(n_largos[i], patron);
        fprintf(file_locate_adn, "%f\n", timeloc);
      }
      fprintf(file_count_adn, "......\n" );
      fprintf(file_locate_adn, "......\n" );
    }
  }


  return 0;
}
