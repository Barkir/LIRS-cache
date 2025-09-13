#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

if [ ! -f "./build/lru_demo" ]; then
    echo -e "${RED}Ошибка: программа lru_demo не найдена${NC}"
    echo "Скомпилируйте программу"
    exit 1
fi

if [ ! -d "./tests" ]; then
    echo -e "${YELLOW}Директория tests не найдена, создаем...${NC}"
    mkdir -p tests
    echo -e "${YELLOW}Создайте тестовые файлы в директории tests/${NC}"
    exit 1
fi

if [ -z "$(ls -A tests/*.txt 2>/dev/null)" ]; then
    echo -e "${YELLOW}В директории tests нет .txt файлов${NC}"
    echo "Пример тестового файла:"
    echo "4"
    echo "10"
    echo "2 3 4 5 6 2 3 1 5 8"
    exit 1
fi

echo -e "${BLUE}=== Запуск тестов LRU Cache ===${NC}"
echo "Найдены тестовые файлы:"
ls -la tests/*.txt

# Создаем директорию для результатов
mkdir -p test_results
rm -f test_results/*.log

total_tests=0
passed_tests=0
failed_tests=0

# Функция для запуска одного теста
run_test() {
    local test_file=$1
    local test_name=$(basename "$test_file" .txt)

    echo -e "\n${BLUE}--- Запуск теста: $test_name ---${NC}"

    # Запускаем тест и сохраняем вывод
    ./build/lru_demo < "$test_file" > "test_results/${test_name}.log" 2>&1

    # Проверяем код возврата
    local exit_code=$?

    # Считаем количество строк вывода
    local line_count=$(wc -l < "test_results/${test_name}.log")

    if [ $exit_code -eq 0 ]; then
        echo -e "${GREEN}✓ Тест $test_name завершен успешно${NC}"
        echo -e "  Вывод: $line_count строк"
        echo -e "  См. подробности: test_results/${test_name}.log"
        ((passed_tests++))
    else
        echo -e "${RED}✗ Тест $test_name завершен с ошибкой (код: $exit_code)${NC}"
        echo -e "  Вывод: $line_count строк"
        echo -e "  См. ошибки: test_results/${test_name}.log"
        ((failed_tests++))
    fi

    ((total_tests++))
}

for test_file in tests/*.txt; do
    run_test "$test_file"
done

echo -e "\n${BLUE}=== ИТОГИ ===${NC}"
echo -e "Всего тестов: $total_tests"
echo -e "${GREEN}Пройдено: $passed_tests${NC}"
echo -e "${RED}Провалено: $failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}🎉 Все тесты пройдены успешно!${NC}"
else
    echo -e "${RED}❌ Есть проваленные тесты${NC}"
fi

# Показываем последний лог для быстрой проверки
if [ $total_tests -gt 0 ]; then
    last_test=$(ls tests/*.txt | tail -n1)
    last_test_name=$(basename "$last_test" .txt)
    echo -e "\n${YELLOW}Последние строки из ${last_test_name}.log:${NC}"
    tail -5 "test_results/${last_test_name}.log"
fi
