#include <stdio.h>  
#include <stdlib.h>  
#include <ctype.h>  
#include <string.h> // Required for strlen()  

// Enum to define token types  
enum TokenType {  
    TOKEN_NUMBER,   // Represents numbers  
    TOKEN_PLUS,     // Represents '+' operator  
    TOKEN_MULTIPLY, // Represents '*' operator  
    TOKEN_EOF       // Represents the end of input  
};  

// Structure to represent a token  
struct Token {  
    enum TokenType type; // Token type (number, plus, multiply, EOF)  
    int value; // Stores the value for numbers  
};  

// Lexer structure for handling input  
struct Lexer {  
    const char *input; // Input string  
    size_t pos;        // Current position in input  
    char current_char; // Current character being processed  
};  

// Function to create a new lexer  
struct Lexer *create_lexer(const char *input) {  
    struct Lexer *lexer = malloc(sizeof(struct Lexer));  
    lexer->input = input;  
    lexer->pos = 0;  
    lexer->current_char = input[lexer->pos]; // Initialize with first character  
    return lexer;  
}  

// Function to move to the next character in the input  
void advance(struct Lexer *lexer) {  
    lexer->pos++;  
    if (lexer->pos >= strlen(lexer->input)) {  
        lexer->current_char = '\0'; // End of input  
    } else {  
        lexer->current_char = lexer->input[lexer->pos];  
    }  
}  

// Function to skip whitespace characters  
void skip_whitespace(struct Lexer *lexer) {  
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {  
        advance(lexer);  
    }  
}  

// Function to extract the next token from input  
struct Token *get_next_token(struct Lexer *lexer) {  
    while (lexer->current_char != '\0') {  
        if (isspace(lexer->current_char)) {  //skip whitespace characters
            skip_whitespace(lexer);  
            continue;  
        }  
        if (isdigit(lexer->current_char)) {  //parse numbers
            int value = 0;  
            while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {  
                value = value * 10 + (lexer->current_char - '0');       //convert char to int
                advance(lexer);  
            }  
            struct Token *token = malloc(sizeof(struct Token));  
            token->type = TOKEN_NUMBER;  
            token->value = value;  
            return token;  
        }  
        if (lexer->current_char == '+') {  
            advance(lexer);  
            struct Token *token = malloc(sizeof(struct Token));  
            token->type = TOKEN_PLUS;  
            token->value = 0;  
            return token;  
        }  
        if (lexer->current_char == '*') {  
            advance(lexer);  
            struct Token *token = malloc(sizeof(struct Token));  
            token->type = TOKEN_MULTIPLY;  
            token->value = 0;  
            return token;  
        }  
        fprintf(stderr, "Error: Unknown character '%c'\n", lexer->current_char);  
        exit(EXIT_FAILURE);  
    }  
    struct Token *token = malloc(sizeof(struct Token));  
    token->type = TOKEN_EOF;  
    token->value = 0;  
    return token;  
}  

// Parser structure to parse tokens  
struct Parser {  
    struct Lexer *lexer;          // Lexer for tokenizing input  
    struct Token *current_token;  // Current token being processed  
};  

// Function to create a new parser  
struct Parser *create_parser(struct Lexer *lexer) {  
    struct Parser *parser = malloc(sizeof(struct Parser));  
    parser->lexer = lexer;  
    parser->current_token = get_next_token(lexer); // Get first token  
    return parser;  
}  

// Function to consume a token and move to the next one  
void eat(struct Parser *parser, enum TokenType token_type) {  
    if (parser->current_token->type == token_type) {  
        free(parser->current_token); // Free current token  
        parser->current_token = get_next_token(parser->lexer); // Get next token  
    } else {  
        fprintf(stderr, "Error: Unexpected token type\n");  
        exit(EXIT_FAILURE);  
    }  
}  

// Function to parse a number (factor)  
int factor(struct Parser *parser) {  
    struct Token *token = parser->current_token;  
    eat(parser, TOKEN_NUMBER); // Consume the number  
    return token->value; // Return its value  
}  

// Function to parse multiplication terms  
int term(struct Parser *parser) {  
    int result = factor(parser);  
    while (parser->current_token->type == TOKEN_MULTIPLY) {  
        eat(parser, TOKEN_MULTIPLY);  
        result *= factor(parser); // Multiply by next factor  
    }  
    return result;  
}  

// Function to parse addition expressions  
int expr(struct Parser *parser) {  
    int result = term(parser);  
    while (parser->current_token->type == TOKEN_PLUS) {  
        eat(parser, TOKEN_PLUS);  
        result += term(parser); // Add next term  
    }  
    return result;  
}  


// Ensure that the required structures and functions are declared
struct Lexer;
struct Parser;

struct Lexer *create_lexer(const char *input);
struct Parser *create_parser(struct Lexer *lexer);
int expr(struct Parser *parser);

int main() {
    char input[256];  // Define a character array to store user input
    printf("Enter an expression: ");  
    fgets(input, sizeof(input), stdin);  // Read user input safely

    // Remove newline character from input if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // Create a lexer with the input expression
    struct Lexer *lexer = create_lexer(input);
    if (!lexer) {
        fprintf(stderr, "Error: Failed to create lexer\n");
        return EXIT_FAILURE;
    }

    // Create a parser using the lexer
    struct Parser *parser = create_parser(lexer);
    if (!parser) {
        fprintf(stderr, "Error: Failed to create parser\n");
        free(lexer);
        return EXIT_FAILURE;
    }

    // Evaluate the expression using the parser
    int result = expr(parser);
    printf("Result: %d\n", result);  // Print the result of the evaluation

    // Free allocated memory
    free(parser->current_token);
    free(parser);
    free(lexer);

    return 0;  // Return 0 indicating successful execution
}