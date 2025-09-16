#include "scanner.h"
#include "utils/tokens.hpp"
#include "utils/errors.hpp"
#include <string>

Scanner::Scanner(const std::string& source)
{
    m_Source = source;

    m_Start = 0;
    m_Current = 0;
    m_Line = 1;
}

Tokens& Scanner::scan_tokens()
{
    while(!is_at_end())
    {
        m_Start = m_Current;
        scan_token();
    }

    m_Tokens.push_back(Token(TokenType::END_OF_FILE, "", m_Line));
    return m_Tokens;
}

void Scanner::scan_token()
{
    char c = advance();
    switch (c) {
        case '(' : add_token(TokenType::LEFT_PAREN); break;
        case ')' : add_token(TokenType::RIGHT_PAREN); break;
        case '{' : add_token(TokenType::LEFT_BRACE); break;
        case '}' : add_token(TokenType::RIGHT_BRACE); break;
        case ',' : add_token(TokenType::COMMA); break;
        case '.' : add_token(TokenType::DOT); break;
        case '-' : add_token(TokenType::MINUS); break;
        case '+' : add_token(TokenType::PLUS); break;
        case ';' : add_token(TokenType::SEMICOLON); break;
        case '*' : add_token(TokenType::STAR); break;
        case '!':
            add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if(match('/')) {
                while(!is_at_end() && peek() != '\n') advance();
            } else {
                add_token(TokenType::SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            m_Line++;
            break;
        case '"': string(); break;
        default:
            if (is_digit(c)) 
            {
                number();
            }
            else if(is_alpha(c))
            {
                identifier();
            }
            else 
            {
                error(m_Line, "Unexpected character.");
                break;
            }
    }
}

char Scanner::advance()
{
    return m_Source[m_Current++];
}

void Scanner::add_token(TokenType type)
{
    std::string text = m_Source.substr(m_Start, m_Current);
    m_Tokens.push_back(Token(type, text, m_Line));
}

void Scanner::add_token(TokenType type, Object literal)
{
    std::string text = m_Source.substr(m_Start, m_Current);
    m_Tokens.push_back(Token(type, text, m_Line, literal));
}

bool Scanner::match(char c)
{
    if(is_at_end()) return false;
    if(m_Source[m_Current] != c) return false;

    m_Current++;
    return true;
}

char Scanner::peek()
{
    if(is_at_end()) return '\0';
    return m_Source[m_Current];
}

void Scanner::string()
{
    while(!is_at_end() && peek() != '"')
    {
        if (peek() == '\n') m_Line++;
        advance();
    }   

    if(is_at_end())
    {
        error(m_Line, "Unterminated string");
        return;
    }

    advance(); // Closing "

    std::string value = m_Source.substr(m_Start + 1, m_Current - 1);
    add_token(TokenType::STRING, Object{value});
}

bool Scanner::is_digit(char c)
{
    return c >= '0' && c <= '9';
}

void Scanner::number()
{
    while(is_digit(peek())) advance();

    if(peek() == '.' && is_digit(peek_next()))
    {
        // Consume .
        advance();
        while(is_digit(peek())) advance();
    }

    add_token(TokenType::NUMBER, Object{std::stod(m_Source.substr(m_Start, m_Current))});
}

char Scanner::peek_next()
{
    if(m_Current + 1 >= m_Source.size()) return '\0';
    return m_Source[m_Current + 1];
}

bool Scanner::is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

void Scanner::identifier()
{
    while(is_alpha_numeric(peek())) advance();

    std::string text = m_Source.substr(m_Start, m_Current);

    TokenType type = TokenType::IDENTIFIER;
    if (keywords.find(text) != keywords.end()) {
        type = keywords[text];
    }

    add_token(type);
}

bool Scanner::is_alpha_numeric(char c)
{
    return is_alpha(c) || is_digit(c);
}

bool Scanner::is_at_end()
{
    return m_Current >= m_Source.size();
}

