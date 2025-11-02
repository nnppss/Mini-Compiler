#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./mini_compiler <source_file>" << std::endl;
        return 1;
    }

    // Read code from the file
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    file.close();

    try {
        // Lexical Analysis
        Lexer lexer(code);
        Queue<Token> tokens = lexer.generateTokens();

        // Parsing
        Parser parser(tokens);
        ASTNode* root = parser.parse();

        // Interpretation
        Interpreter interpreter(root);
        interpreter.interpret();

        // Clean up
        delete root;

    } catch (const std::exception& e) {
        // Print the error message to stderr
        std::cerr << "Error: " << e.what() << std::endl;
        // Optionally, you can return a non-zero exit code to indicate an error
        return 1;
    }
    return 0;
}
