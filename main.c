#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

//сравнение подстроки s (начиная с pos)
//с литеральной строкой pat без учёта регистра */
int icmp_at(const char *s, int pos, const char *pat) {
  int i = 0;
  while(pat[i] && s[pos+i]){
    char c1=(char)tolower((unsigned char)s[pos+i]);
    char c2=(char)tolower((unsigned char)pat[i]);
    if(c1!=c2)return 0;i++;
  }
  return pat[i]=='\0';//паттерн закончился->совпадение
}

//транслитерация одного слова/фрагмента английского текста
void translit_word(const char *src,char *dst){
  int i=0,len=(int)strlen(src);dst[0]='\0';

  while (i < len) {
    /* Многобуквенные сочетания – порядок важен (сначала длинные) */

    if (i + 1 < len && icmp_at(src, i, "th")) {
      strcat(dst, "с");
      i += 2;
    }
    else if (i + 1 < len && icmp_at(src, i, "sh")) {
      strcat(dst, "ш");
      i += 2;
    }
    else if (i + 1 < len && icmp_at(src, i, "ch")) {
      strcat(dst, "ч");
      i += 2;
    }
    else if (i + 2 < len && icmp_at(src, i, "sch")) {
      strcat(dst, "ск");
      i += 3;
    }
    else if (i + 1 < len && icmp_at(src, i, "ph")) {
      strcat(dst, "ф");
      i += 2;
    }
    /* NG -> нг */
    else if (i + 1 < len && icmp_at(src, i, "ng")) {
      strcat(dst, "нг");
      i += 2;
    }
    /* EA, EE, IE, EI – упрощённо */
    else if (i + 1 < len && icmp_at(src, i, "ea")) {
      strcat(dst, "и");
      i += 2;
    } else if (i + 1 < len && icmp_at(src, i, "ee")) {
      strcat(dst, "и");
      i += 2;
    } else if (i + 1 < len && icmp_at(src, i, "ie")) {
      strcat(dst, "и");
      i += 2;
    } else if (i + 1 < len && icmp_at(src, i, "ei")) {
      strcat(dst, "и");
      i += 2;
    }
    /* OW, OU, AU – грубые правила */
    else if (i + 1 < len && icmp_at(src, i, "ow")) {
      strcat(dst, "ау");
      i += 2;
    } else if (i + 1 < len && icmp_at(src, i, "ou")) {
      strcat(dst, "ау");
      i += 2;
    } else if (i + 1 < len && icmp_at(src, i, "au")) {
      strcat(dst, "о");
      i += 2;
    }
    /* SINGLE LETTERS */
    else {
      char c = src[i];
      char lc = (char)tolower((unsigned char)c);
      /* только буквы – остальное просто копируем */
      switch (lc) {
        case 'a': strcat(dst, "а"); break;
        case 'b': strcat(dst, "б"); break;
        case 'c': 
                  /* очень грубо: перед e,i,y – как с, иначе к */
                  if (i + 1 < len) {
                    char n = (char)tolower((unsigned char)src[i + 1]);
                    if (n == 'e' || n == 'i' || n == 'y')
                      strcat(dst, "с");
                    else
                      strcat(dst, "к");
                  } else {
                    strcat(dst, "к");
                  }
                  break;
        case 'd': strcat(dst, "д"); break;
        case 'e': strcat(dst, "э"); break;
        case 'f': strcat(dst, "ф"); break;
        case 'g':
                  /* перед e,i,y – как дж, иначе г */
                  if (i + 1 < len) {
                    char n2 = (char)tolower((unsigned char)src[i + 1]);
                    if (n2 == 'e' || n2 == 'i' || n2 == 'y')
                      strcat(dst, "дж");
                    else
                      strcat(dst, "г");
                  } else {
                    strcat(dst, "г");
                  }
                  break;
        case 'h': strcat(dst, "х"); break;
        case 'i': strcat(dst, "и"); break;
        case 'j': strcat(dst, "дж"); break;
        case 'k': strcat(dst, "к"); break;
        case 'l': strcat(dst, "л"); break;
        case 'm': strcat(dst, "м"); break;
        case 'n': strcat(dst, "н"); break;
        case 'o': strcat(dst, "о"); break;
        case 'p': strcat(dst, "п"); break;
        case 'q': strcat(dst, "к"); break; /* упрощённо */
        case 'r': strcat(dst, "р"); break;
        case 's': strcat(dst, "с"); break;
        case 't': strcat(dst, "т"); break;
        case 'u': strcat(dst, "у"); break;
        case 'v': strcat(dst, "в"); break;
        case 'w': strcat(dst, "в"); break;
        case 'x': strcat(dst, "кс"); break;
        case 'y': strcat(dst, "й"); break;
        case 'z': strcat(dst, "з"); break;
        default:
                  /* не латинская буква – просто копируем как есть */
                  {
                    char tmp[2] = {src[i], '\0'};
                    strcat(dst, tmp);
                  }
      }
      i++;
    }
  }
}

/* обработка одной строки: пословная транскрипция */
void process_line(const char *input, char *output) {
  int i = 0;
  char word[256];
  char translit[512];

  output[0] = '\0';

  while (input[i] != '\0') {
    /* если буква – накапливаем слово */
    if (isalpha((unsigned char)input[i])) {
      int wlen = 0;
      while (input[i] != '\0' && isalpha((unsigned char)input[i]) && wlen < 255) {
        word[wlen++] = input[i];
        i++;
      }
      word[wlen] = '\0';

      translit_word(word, translit);
      strcat(output, translit);
    } else {
      /* разделитель/знак препинания – копируем как есть */
      char tmp[2] = {input[i], '\0'};
      strcat(output, tmp);
      i++;
    }
  }
}

/* безопасный ввод строки (замена gets - разрешено заданием) */
int safe_gets(char *str, int max_len) {
  if (fgets(str, max_len, stdin) != NULL) {
    /* убираем символ новой строки, если есть */
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
      str[len-1] = '\0';
    }
    return 1;
  }
  return 0;
}

int main(){
  //line[MAX_LINE] is English //result[MAX_LINE*4] is russian
  //translite //usually it requires more letters
  char line[MAX_LINE],result[MAX_LINE * 4],answer[8];

  printf("СИМВОЛЬНАЯ ОБРАБОТКА: транскрипция английского текста русскими буквами\n");
  printf("Допускается ввод одной строки длиной до %d символов.\n", MAX_LINE - 1);

  while (1) {
    printf("\nВведите английский текст:\n");

    if (!safe_gets(line, MAX_LINE)) {
      printf("Ошибка ввода. Завершение.\n");
      break;
    }

    printf("\nИсходный текст: %s\n", line);

    process_line(line, result);

    printf("Преобразованный текст: %s\n", result);

    printf("\nПродолжить работу? (Y/n): ");
    if(!safe_gets(answer, 8)) break;
    if(tolower(answer[0]=='n'))break;
  }return 0;
}
