#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error;
} options;

void parser(int argc, char *argv[], options *cat_opt);
void reader(char *argv[], options *cat_opt);

int main(int argc, char *argv[]) {  //будем здесь всё запускать, char *
                                    //указатель на нулевой аргумент индекса
  // объявляем структуру, чтобы когда прошел парсер структура стригирила на 1
  if (argc > 1) {
    options cat_opt = {0};         // тут мы занулили
    parser(argc, argv, &cat_opt);  //будем в него передавать счетчик аргументов
    reader(argv, &cat_opt);
  }
  return 0;
}

void parser(int argc, char *argv[],
            options *cat_opt) {  //будем проверять через getlong stdin

  int opt;
  int index_opt = -1;
  const char *short_opt = "benstvTE";
  const struct option long_opt[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"show-nonprinting", no_argument, NULL, 'v'},
      {"show-ends", no_argument, NULL, 'E'},
      {"show-tabs", no_argument, NULL, 'T'},
      {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, short_opt, long_opt, &index_opt)) !=
         -1)  //возврвщает -1 пока не встречает
              //опции похожии их этой строчки
  {
    switch (opt) {
      case 'b':
        cat_opt->b = 1;  // если встретится b то заменится на 1
        break;
      case 'e':
        cat_opt->e = 1;
        cat_opt->v = 1;  // e ещё может подразумевать v
        break;
      case 'n':
        // cat_opt->n = !cat_opt->b;
        cat_opt->n = 1;
        break;
      case 's':
        cat_opt->s = 1;
        break;
      case 't':
        cat_opt->t = 1;
        cat_opt->v = 1;  // e ещё может подразумевать t
        break;
      case 'v':
        cat_opt->v = 1;
        break;
      case 'T':
        cat_opt->t = 1;
        break;
      case 'E':
        cat_opt->e = 1;
        break;
      default:
        fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
        cat_opt->error = 1;
        break;
    }
  }
}

void reader(char *argv[],
            options *cat_opt) {  // будем открывать файл и применять флаги
  FILE *fp;
  fp = fopen(argv[optind], "r");
  int current, prev = '\n';
  int temp = 0;
  int str_count = 1;
  if (cat_opt->error != 1) {
    if (fp) {
      while ((current = getc(fp)) != EOF)  // пока это не равно концу файла
      {
        if (cat_opt->s) {  // squeeze сжимает несколько смежных пустых строк
          if (current ==
              '\n') {  // равен следующему символу в файле и дальше не сжимаем
            temp++;
          }
          if (current != '\n') {
            temp = 0;
          }
          if (temp > 2) {
            continue;
          }
        }

        if (cat_opt->n && prev == '\n' &&
            !cat_opt->b) {  // number нумерует все выходные строки
          printf("%6d\t", str_count++);
        }

        if (cat_opt->b && prev == '\n' &&
            !(current ==
              '\n')) {  // number-nonblank нумерует только непустые строки
          printf("%6d\t", str_count++);
        }

        if (cat_opt->e &&
            current == '\n') {  // также отображает символы конца строки как $
          printf("$");
        }
        if (cat_opt->t && current == '\t') {  // также отображает табы как ^I
          printf("^I");
        } else {
          if (cat_opt->v) {
            if (current < 0) {
              printf("M-%c", current + 128);
            } else if (current == 9 || current == 10) {
              printf("%c", current);
            } else if (current < 32) {
              printf("^%c", current + 64);
            } else if (current < 127) {
              printf("%c", current);
            } else {
              printf("^?");
            }
          } else {
            printf("%c", current);
          }
        }
        prev = current;
      }
    } else {
      printf("No such file or directory");
    }
  } else {
  }
}
