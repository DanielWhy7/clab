#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RECORDS 100
#define MAX_NAME_LEN 50
#define MAX_GROUP_LEN 20

// Структура для хранения данных о студенте
typedef struct {
  char surname[MAX_NAME_LEN];
  char name[MAX_NAME_LEN];
  char group[MAX_GROUP_LEN];
  int skips;
} Student;

// Глобальные переменные
Student records[MAX_RECORDS];
int recordCount = 0;
int currentIndex = 0;
char currentFileName[256] = "";
int modified = 0;

// Функции работы с файлами
int loadFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return 0;
  }
  recordCount = 0;
  while (recordCount < MAX_RECORDS && 
      fscanf(file, "%s %s %s %d", 
        records[recordCount].surname,
        records[recordCount].name,
        records[recordCount].group,
        &records[recordCount].skips) == 4) {
    recordCount++;
  }

  fclose(file);
  currentIndex = 0;
  modified = 0;
  strcpy(currentFileName, filename);
  return 1;
}

int saveToFile(const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Ошибка: не удалось открыть файл для записи!\n");
    return 0;
  }

  for (int i = 0; i < recordCount; i++) {
    fprintf(file, "%s %s %s %d\n",
        records[i].surname,
        records[i].name,
        records[i].group,
        records[i].skips);
  }

  fclose(file);
  modified = 0;
  return 1;
}

// Функция сравнения для qsort (сортировка по ФИО)
int compareStudents(const void *a, const void *b) {
  Student *s1 = (Student *)a;
  Student *s2 = (Student *)b;

  int cmp = strcmp(s1->surname, s2->surname);
  if (cmp != 0) return cmp;

  return strcmp(s1->name, s2->name);
}

// Функции работы с массивом записей
void sortByName() {
  qsort(records, recordCount, sizeof(Student), compareStudents);
  currentIndex = 0;
}

void displayRecord(int index) {
  if (index < 0 || index >= recordCount) {
    printf("Нет записей для отображения!\n");
    return;
  }

  printf("\n=== Запись %d/%d ===\n", index + 1, recordCount);
  printf("Фамилия: %s\n", records[index].surname);
  printf("Имя: %s\n", records[index].name);
  printf("Группа: %s\n", records[index].group);
  printf("Пропуски: %d\n", records[index].skips);
  printf("===================\n\n");
}

void nextRecord() {
  if (recordCount == 0) {
    printf("Массив записей пуст!\n");
    return;
  }
  currentIndex = (currentIndex + 1) % recordCount;
  displayRecord(currentIndex);
}

void previousRecord() {
  if (recordCount == 0) {
    printf("Массив записей пуст!\n");
    return;
  }
  currentIndex = (currentIndex - 1 + recordCount) % recordCount;
  displayRecord(currentIndex);
}

void addRecord() {
  if (recordCount >= MAX_RECORDS) {
    printf("Массив переполнен!\n");
    return;
  }

  printf("\n--- Добавление новой записи ---\n");
  printf("Фамилия: ");
  scanf("%s", records[recordCount].surname);

  printf("Имя: ");
  scanf("%s", records[recordCount].name);

  printf("Группа: ");
  scanf("%s", records[recordCount].group);

  printf("Количество пропусков: ");
  scanf("%d", &records[recordCount].skips);

  recordCount++;
  modified = 1;
  printf("Запись добавлена!\n\n");
}

void editRecord() {
  if (recordCount == 0) {
    printf("Массив записей пуст!\n");
    return;
  }

  displayRecord(currentIndex);

  printf("--- Редактирование записи ---\n");
  printf("Фамилия: ");
  scanf("%s", records[currentIndex].surname);

  printf("Имя: ");
  scanf("%s", records[currentIndex].name);

  printf("Группа: ");
  scanf("%s", records[currentIndex].group);

  printf("Количество пропусков: ");
  scanf("%d", &records[currentIndex].skips);

  modified = 1;
  printf("Запись отредактирована!\n\n");
}

void deleteRecord() {
  if (recordCount == 0) {
    printf("Массив записей пуст!\n");
    return;
  }

  displayRecord(currentIndex);

  printf("Вы уверены? (y/n): ");
  char confirm;
  scanf(" %c", &confirm);

  if (confirm == 'y' || confirm == 'Y') {
    for (int i = currentIndex; i < recordCount - 1; i++) {
      records[i] = records[i + 1];
    }
    recordCount--;
    if (currentIndex >= recordCount && currentIndex > 0) {
      currentIndex--;
    }
    modified = 1;
    printf("Запись удалена!\n\n");
  } else {
    printf("Отмена операции.\n\n");
  }
}

// Обработка данных: фильтрация по количеству пропусков
void processData(int k1, int k2, int k3) {
  // Сортируем перед обработкой
  sortByName();

  // Файл 1: вызов в деканат (от К1 до К2)
  FILE *file1 = fopen("result/dean_call.txt", "w");
  if (file1 == NULL) {
    printf("Ошибка: не удалось создать файл dean_call.txt\n");
    return;
  }

  // Файл 2: выговор (от К2 до К3)
  FILE *file2 = fopen("result/reprimand.txt", "w");
  if (file2 == NULL) {
    printf("Ошибка: не удалось создать файл reprimand.txt\n");
    fclose(file1);
    return;
  }

  // Файл 3: отчисление (больше К3)
  FILE *file3 = fopen("result/expulsion.txt", "w");
  if (file3 == NULL) {
    printf("Ошибка: не удалось создать файл expulsion.txt\n");
    fclose(file1);
    fclose(file2);
    return;
  }

  // Распределяем студентов по категориям
  for (int i = 0; i < recordCount; i++) {
    char buffer[256];
    sprintf(buffer, "%s %s %s %d\n",
        records[i].surname,
        records[i].name,
        records[i].group,
        records[i].skips);

    if (records[i].skips >= k1 && records[i].skips <= k2) {
      fprintf(file1, "%s", buffer);
    } else if (records[i].skips > k2 && records[i].skips <= k3) {
      fprintf(file2, "%s", buffer);
    } else if (records[i].skips > k3) {
      fprintf(file3, "%s", buffer);
    }
  }

  fclose(file1);
  fclose(file2);
  fclose(file3);

  printf("\nОбработка данных завершена!\n");
  printf("Созданы файлы:\n");
  printf("- dean_call.txt (вызов в деканат)\n");
  printf("- reprimand.txt (выговор)\n");
  printf("- expulsion.txt (отчисление)\n\n");
}

// Меню второго уровня (работа с массивом)
void menuLevel2() {
  int choice;

  while (1) {
    printf("\n========== МЕНЮ РАБОТЫ С ДАННЫМИ ==========\n");
    printf("Всего записей: %d\n", recordCount);
    if (recordCount > 0) {
      printf("Текущая позиция: %d/%d\n", currentIndex + 1, recordCount);
      displayRecord(currentIndex);
    }
    printf("==========================================\n");
    printf("1. Предыдущая запись\n");
    printf("2. Следующая запись\n");
    printf("3. Добавить запись\n");
    printf("4. Удалить запись\n");
    printf("5. Изменить запись\n");
    printf("6. Назад в главное меню\n");
    printf("Выберите пункт: ");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        previousRecord();
        break;
      case 2:
        nextRecord();
        break;
      case 3:
        addRecord();
        break;
      case 4:
        deleteRecord();
        break;
      case 5:
        editRecord();
        break;
      case 6:
        return;
      default:
        printf("Неверный выбор!\n");
    }
  }
}

// Меню первого уровня (работа с файлом)
void menuLevel1() {
  int choice;

  while (1) {
    printf("\n============ ГЛАВНОЕ МЕНЮ ============\n");
    printf("Текущий файл: %s\n", currentFileName[0] ? currentFileName : "не открыт");
    printf("Записей в памяти: %d\n", recordCount);
    printf("Статус: %s\n", modified ? "ИЗМЕНЕНО" : "без изменений");
    printf("=====================================\n");
    printf("1. Открыть файл данных\n");
    printf("2. Работа с данными\n");
    printf("3. Сохранить файл данных\n");
    printf("4. Обработать данные и создать результаты\n");
    printf("5. Выход\n");
    printf("Выберите пункт: ");
    scanf("%d", &choice);

    switch (choice) {
      case 1: {
                printf("Введите имя файла: ");
                scanf("%s", currentFileName);
                if (loadFromFile(currentFileName)) {
                  printf("Файл успешно загружен! Записей: %d\n", recordCount);
                } else {
                  printf("Файл не найден. Создать новый? (y/n): ");
                  char confirm;
                  scanf(" %c", &confirm);
                  if (confirm == 'y' || confirm == 'Y') {
                    recordCount = 0;
                    modified = 0;
                    printf("Создан новый файл: %s\n", currentFileName);
                  } else {
                    currentFileName[0] = '\0';
                  }
                }
                break;
              }
      case 2: {
                if (currentFileName[0] == '\0') {
                  printf("Сначала откройте или создайте файл!\n");
                } else {
                  menuLevel2();
                }
                break;
              }
      case 3: {
                if (currentFileName[0] == '\0') {
                  printf("Сначала откройте или создайте файл!\n");
                } else {
                  if (saveToFile(currentFileName)) {
                    printf("Файл успешно сохранен!\n");
                  }
                }
                break;
              }
      case 4: {
                if (recordCount == 0) {
                  printf("Нет данных для обработки!\n");
                } else {
                  int k1, k2, k3;
                  printf("\nВведите пороги пропусков:\n");
                  printf("К1 (вызов в деканат): ");
                  scanf("%d", &k1);
                  printf("К2 (выговор): ");
                  scanf("%d", &k2);
                  printf("К3 (отчисление): ");
                  scanf("%d", &k3);

                  if (k1 < k2 && k2 < k3) {
                    processData(k1, k2, k3);
                  } else {
                    printf("Ошибка: К1 < К2 < К3!\n");
                  }
                }
                break;
              }
      case 5: {
                if (modified) {
                  printf("Данные были изменены. Сохранить? (y/n): ");
                  char confirm;
                  scanf(" %c", &confirm);
                  if (confirm == 'y' || confirm == 'Y') {
                    if (currentFileName[0] != '\0') {
                      saveToFile(currentFileName);
                    }
                  }
                }
                printf("До свидания!\n");
                return;
              }
      default:
              printf("Неверный выбор!\n");
    }
  }
}

int main() {
  printf("╔════════════════════════════════════════════════════╗\n"
       "║   УПРАВЛЕНИЕ ДАННЫМИ О СТУДЕНТАХ                   ║\n"
       "║   версия 1.0                                       ║\n"
       "╚════════════════════════════════════════════════════╝\n");

  menuLevel1();
  return 0;
}
