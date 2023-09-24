#define _GNU_SOURCE  // getline
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int e;  //Шаблон
  int i;  //Игнорирует регистр символов при сравнениях.
  int v;  //Выдает все строки, за исключением содержащих образец.
  int c;  //Выдает только количество строк, содержащих образец.
  int l;  //Выдает только имена файлов, содержащих сопоставившиеся строки, по
          //одному в строке. Если образец найден в нескольких строках файла, имя
          //файла не повторяется.
  int n;  //Выдает перед каждой строкой ее номер в файле (строки нумеруются с
          // 1).
  int h;  //Выводит совпадающие строки, не предваряя их именами файлов.
  int s;  //Подавляет сообщения об ошибках о несуществующих или нечитаемых
          //файлах.
  int f;  //Получает регулярные выражения из файла.
  int o;  //Печатает только совпадающие (непустые) части совпавшей строки.
  int error;
} options;

void parser(int argc, char *argv[], options *grep_opt);
void reader(int argc, char *argv[], options *grep_opt);
void no_flags(int argc, char *argv[]);

int main(int argc, char *argv[]) {  //будем здесь всё запускать, char *
                                    //указатель на нулевой аргумент индекса
  // объявляем структуру, чтобы когда прошел парсер структура стригирила на 1
  // if (argc == 2) {
  //     fprintf(stderr, "no pattern\n");
  //   }
  if (argc > 2) {
    options grep_opt = {0};  // тут мы занулили
    grep_opt.error = 1;
    parser(argc, argv, &grep_opt);  //будем в него передавать счетчик аргументов
    if (grep_opt.error == 0) {
      reader(argc, argv, &grep_opt);
    } else if (grep_opt.error == 1 && argc >= 3) {
      no_flags(argc, argv);
    }
  }

  return 0;
}

void parser(int argc, char *argv[],
            options *grep_opt) {  //будем проверять через getlong stdin
  int opt;
  int index_opt = -1;

  const char *short_opt = "e:f:ivclnhso";
  const struct option long_opt[] = {
      {"count", no_argument, NULL, 'c'},
      {"regexp", required_argument, NULL, 'e'},
      {"ignore-case", no_argument, NULL, 'i'},
      {"files-with-matches", no_argument, NULL, 'l'},
      {"line-number", no_argument, NULL, 'n'},
      {"invert-match", no_argument, NULL, 'v'},
      {"no-filename", no_argument, NULL, 'h'},
      {"no-messages", no_argument, NULL, 's'},
      {"file", required_argument, NULL, 'f'},
      {"only-matching", no_argument, NULL, 'o'},
      {NULL, no_argument, NULL, 0}};
  grep_opt->error = 1;
  while ((opt = getopt_long(argc, argv, short_opt, long_opt, &index_opt)) !=
         -1)  //возврвщает -1 пока не встречает
              //опции похожии их этой строчки
  {
    switch (opt) {
      case 'e':
        grep_opt->e = 1;
        grep_opt->error = 0;
        break;
      case 'i':
        grep_opt->i = 1;
        grep_opt->error = 0;
        break;
      case 'v':
        grep_opt->v = 1;
        grep_opt->error = 0;
        break;
      case 'c':
        grep_opt->c = 1;
        grep_opt->error = 0;
        break;
      case 'l':
        grep_opt->l = 1;
        grep_opt->error = 0;
        break;
      case 'n':
        grep_opt->n = 1;
        grep_opt->error = 0;
        break;
      case 'h':
        grep_opt->h = 1;
        grep_opt->error = 0;
        break;
      case 's':
        grep_opt->s = 1;
        grep_opt->error = 0;
        break;
      case 'f':
        grep_opt->f = 1;
        grep_opt->error = 0;
        break;
      case 'o':
        grep_opt->o = 1;
        grep_opt->error = 0;
        break;
      default:
        grep_opt->error = 2;
        fprintf(stderr,
                "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] "
                "[-B num] [-C[num]]\n");
        fprintf(
            stderr,
            "\t[-e pattern] [-f file] [--binary-files=value] [--color=when]\n");
        fprintf(stderr,
                "\t[--context[=num]] [--directories=action] [--label] "
                "[--line-buffered]\n");
        fprintf(stderr, "\t[--null] [pattern] [file ...]\n");
        break;
    }
  }
}

void reader(int argc, char *argv[],
            options *grep_opt) {  // будем открывать файл и применять флаги
                                  //  char current;

  FILE *file = NULL;
  char **pattern = &argv[1];
  char **end = &argv[argc];
  regex_t preg;
  char *line = NULL;
  size_t length = 0;
  regmatch_t match;
  if (grep_opt->i || grep_opt->v || grep_opt->c || grep_opt->l || grep_opt->n ||
      grep_opt->h || grep_opt->s || grep_opt->f || grep_opt->o) {
    grep_opt->e = 0;
  }
  for (; pattern != end && pattern[0][0] == '-'; ++pattern)
    ;
  // if (pattern == end) {
  //   fprintf(stderr, "no pattern\n");
  //   exit(1);
  // }

  if (grep_opt->i) {
    regcomp(&preg, *pattern, REG_ICASE);
  } else {
    regcomp(&preg, *pattern, 0);
  }
  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;

    int count_str = 1;
    int count = 0;

    file = fopen(*filename, "rb");
    if (file) {
      while (getline(&line, &length, file) > 0) {
        // current = getc(file);
        int no_match = regexec(&preg, line, 1, &match, 0);
        if (grep_opt->i) {
          if (argc > 4) {
            printf("%s:", *filename);
          }
          if (grep_opt->v) {  //Инвертирует смысл поиска соответствий.
            if (no_match) {
              printf("%s", line);
            }
          } else if (!no_match) {
            if (grep_opt->h) {  //Выводит совпадающие строки
              printf("%s", line);
            }

            if (grep_opt->l) {  //Выводит только совпадающие файлы.
              printf("%s\n", *filename);
              break;
            }

            if (grep_opt->n) {  //Предваряет каждую строку вывода номером

              printf("%d:%s", count_str, line);
            }

            if (grep_opt->e == 1 || grep_opt->s)  //Шаблон
            {
              printf("%s", line);
            }

            if (grep_opt->c) {
              ++count;
            }
            if (grep_opt->o)  //Печатает только совпадающие (непустые) части
                              //совпавшей строки.
            {
              printf("%s\n", *pattern);
            }
            if (!grep_opt->v && !grep_opt->h && !grep_opt->l && !grep_opt->n &&
                !grep_opt->e && !grep_opt->c && !grep_opt->o) {
              printf("%s", line);
            }
          }
        }
        if (grep_opt->v &&
            !grep_opt->i) {  //Инвертирует смысл поиска соответствий.
          if (no_match) {
            if (argc > 4) {
              printf("%s:", *filename);
            }
            printf("%s", line);
          }
        } else {
          if (!no_match) {
            if (grep_opt->h && !grep_opt->i) {  //Выводит совпадающие строки
              if (argc > 4) {
                printf("%s:", *filename);
              }
              printf("%s", line);
            }

            if (grep_opt->l &&
                !grep_opt->i) {  //Выводит только совпадающие файлы.
              printf("%s\n", *filename);
              break;
            }

            if (grep_opt->n &&
                !grep_opt->i) {  //Предваряет каждую строку вывода номером
              if (argc > 4) {
                printf("%s:", *filename);
              }
              printf("%d:%s", count_str, line);
            }

            if ((grep_opt->e == 1 || grep_opt->s) && !grep_opt->i)  //Шаблон
            {
              if (argc > 4) {
                printf("%s:", *filename);
              }
              printf("%s", line);
            }

            if (grep_opt->f) {  //Получает регулярные выражения из файла.
            }
            if (grep_opt->c && !grep_opt->i) {
              ++count;
            }
            if (grep_opt->o &&
                !grep_opt->i)  //Печатает только совпадающие (непустые) части
                               //совпавшей строки.
            {
              if (argc > 4) {
                printf("%s:", *filename);
              }
              printf("%s\n", *pattern);
            }
          }
        }
        count_str++;
      }
      if (grep_opt->c) {  //Выводит только количество совпадающих строк.
        if (argc > 4) {
          printf("%s:", *filename);
        }
        printf("%i\n", count);
      }
      fclose(file);

    } else if (!grep_opt->s) {
      if (argc > 4) {
        printf("%s:", *filename);
      }
      printf("No such file or directory\n");
    }
  }
  free(line);
  regfree(&preg);
}

void no_flags(int argc, char *argv[]) {
  FILE *file = NULL;
  char **pattern = &argv[1];
  char **end = &argv[argc];
  regex_t preg;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  regcomp(&preg, *pattern, 0);
  for (char **filename = pattern + 1; filename != end; ++filename) {
    file = fopen(*filename, "rb");
    if (file) {
      while (getline(&line, &length, file) > 0) {
        if (!regexec(&preg, line, 1, &match, 0)) {
          if (argc > 3) {
            printf("%s:", *filename);
          }
          printf("%s", line);
        }
      }
      fclose(file);
    } else {
      if (argc > 3) {
        printf("%s:", *filename);
      }
      printf("No such file or directory\n");
    }
  }
  free(line);
  regfree(&preg);
}