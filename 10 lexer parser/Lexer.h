/**
* Введенский Вячеслав, MADE-12
*
парсер строк состоящих из следующих токенов:
    имя переменной,    число,    операция,    символ равно, if else, while
Числа целые int состоит из символов от 0 до 9 (ограничим 2^31),
имя переменной - маленькие латинские
пробельные символы (пробел, табуляция, перевод строки)

Классу скармливаются строки.
Парсятся последовательно. Пользователь получает по одному токену за раз.
 * */
#ifndef _LEXER_H_
#define _LEXER_H_

#include <utility>
#include <string>
#include <limits>
#include <functional>
#include <vector>

/// типы токенов
enum TokenType {
    TOKEN_EOF,   // конец файла
    TOKEN_EOL,   // конец строки
    TOKEN_NAME,  // имя переменной
    TOKEN_VALUE, // число
    TOKEN_OPER,  // операция и скобки
    TOKEN_IF,    // если
    TOKEN_ELSE,  // иначе
    TOKEN_WHILE, // цикл
    TOKEN_ERROR  // ошибка
};

struct Token {
    Token(TokenType _type, int _value, std::string _str) :
            type(_type), value(_value), str(std::move(_str)) {}

    TokenType type;
    int value = 0;
    std::string str;

    std::string toString();
};

/// состоянии при чтении
enum ReadState {
    READ_EOF,        // конец файла
    READ_EOL,        // конец строки
    READ_SPACE,      // читаем пробелы
    READ_NAME,       // начинаем читать токен
    READ_VALUE,      // число
    READ_OPER,       // операция
    READ_ERROR       // ошибка
};

/// Парсер строк на токены.
class Lexer {
private:
    // Максимально допустимое целое число при парсинге строк.
    static const int MAX_VALUE = std::numeric_limits<int32_t>::max();

    // Промотать до окончания пробельных символов.
    void skipSpace();

    // Считать целочисленный токен из строки (с проверкой на превышение максимума).
    int readInt();

    // текущий символ.
    char current();

    // Считать строковый токен из строки.
    std::string readString();

    // Считать операцию из строки.
    std::string readOper();

    // текущее состояние
    ReadState checkState();

    // сдвинуть каретку
    void shift();

    // а не конец ли строки
    bool eol();

    // получить следующий токен.
    Token next();

    // запрещаем копирование класса
    Lexer(const Lexer &) = delete;

    Lexer(const Lexer &&) = delete;

    Lexer &operator=(const Lexer &) const = delete;

    Lexer &&operator=(const Lexer &&) = delete;


    // все строки, которые были переданы для парсинга
    std::vector<std::string> lines;
    // текущее состояние
    ReadState state = READ_SPACE;
    // сообщение об ошибке
    std::string errorMessage;
    // Текущее положение каретки в строке.
    unsigned int idx = 0;
    // Текущая строка.
    unsigned int idx_line = 0;
    // текущий токен.
    Token current_token = {TOKEN_EOF, 0, ""};
    // где был считан прошлый токен.
    unsigned int prev_idx = 0;
    unsigned int prev_idx_line = 0;
public:
    Lexer() = default;

    ~Lexer() = default;

    /// добавить строку к разбору
    void add(const std::string &line);

    /// сдвинуться к следующему токену
    void shiftToken();

    /// скипнуть перевод строки
    void skipEol();

    /// не является ли токен закрытой скобкой
    bool isClosingBrace();

    /// тип текущего токена
    TokenType tokenType();

    /// операция, если токен это операция
    std::string oper();

    /// строковое значение токена (например имя переменной)
    std::string tokenSValue();

    /// целочисленное значение токена
    int tokenValue();

    /// вывести в поток исходный текст.
    void printSource();

    /// вернуть позицию последнего считаного токена в виде строки "idx_line:idx"
    std::string lastPosition();

    /// а не конец ли данных
    bool eof();

    /// если ли ошибка при считывании
    bool isError();

    /// очистить состояние
    void clear();
};

#endif //_LEXER_H_
