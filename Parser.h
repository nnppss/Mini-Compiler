#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include <vector>
#include <stdexcept>
#include <string>

// AST Node Types
enum NodeType {
    N_NUMBER,
    N_VARIABLE,
    N_BIN_OP,
    N_ASSIGN,
    N_PRINT,
    N_IF,
    N_WHILE,
    N_BLOCK
};

// Base AST Node
class ASTNode {
public:
    NodeType type;
    int lineNumber;

    ASTNode(NodeType type, int lineNumber) : type(type), lineNumber(lineNumber) {}
    virtual ~ASTNode() {}
};

// Number Node
class NumberNode : public ASTNode {
public:
    int value;
    NumberNode(int value, int lineNumber) : ASTNode(N_NUMBER, lineNumber), value(value) {}
};

// Variable Node
class VariableNode : public ASTNode {
public:
    std::string name;
    VariableNode(const std::string& name, int lineNumber) : ASTNode(N_VARIABLE, lineNumber), name(name) {}
};

// Binary Operation Node
class BinOpNode : public ASTNode {
public:
    ASTNode* left;
    std::string op;
    ASTNode* right;

    BinOpNode(ASTNode* left, const std::string& op, ASTNode* right, int lineNumber)
        : ASTNode(N_BIN_OP, lineNumber), left(left), op(op), right(right) {}
    ~BinOpNode() {
        delete left;
        delete right;
    }
};

// Assignment Node
class AssignNode : public ASTNode {
public:
    std::string name;
    ASTNode* value;

    AssignNode(const std::string& name, ASTNode* value, int lineNumber)
        : ASTNode(N_ASSIGN, lineNumber), name(name), value(value) {}
    ~AssignNode() {
        delete value;
    }
};

// Print Node
class PrintNode : public ASTNode {
public:
    ASTNode* expression;
    PrintNode(ASTNode* expr, int lineNumber) : ASTNode(N_PRINT, lineNumber), expression(expr) {}
    ~PrintNode() {
        delete expression;
    }
};

// If Statement Node
class IfNode : public ASTNode {
public:
    ASTNode* condition;
    ASTNode* trueBlock;
    ASTNode* falseBlock; // Can be nullptr

    IfNode(ASTNode* cond, ASTNode* tBlock, ASTNode* fBlock, int lineNumber)
        : ASTNode(N_IF, lineNumber), condition(cond), trueBlock(tBlock), falseBlock(fBlock) {}
    ~IfNode() {
        delete condition;
        delete trueBlock;
        if (falseBlock)
            delete falseBlock;
    }
};

// While Loop Node
class WhileNode : public ASTNode {
public:
    ASTNode* condition;
    ASTNode* block;

    WhileNode(ASTNode* cond, ASTNode* blk, int lineNumber)
        : ASTNode(N_WHILE, lineNumber), condition(cond), block(blk) {}
    ~WhileNode() {
        delete condition;
        delete block;
    }
};

// Block Node
class BlockNode : public ASTNode {
public:
    std::vector<ASTNode*> statements;

    BlockNode(int lineNumber) : ASTNode(N_BLOCK, lineNumber) {}
    ~BlockNode() {
        for (ASTNode* stmt : statements)
            delete stmt;
    }
};

class Parser {
private:
    Queue<Token> tokens;
    Token currentToken;
    Token prevToken;

    void advance() {
        prevToken = currentToken;
        if (!tokens.isEmpty())
            currentToken = tokens.dequeue();
    }

    void expect(TokenType type) {
        if (currentToken.type == type)
            advance();
        else
            throw std::runtime_error("Expected token '" + tokenTypeToString(type) + "' at line " + std::to_string(currentToken.lineNumber));
    }

    std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case T_IDENTIFIER: return "identifier";
            case T_NUMBER: return "number";
            case T_OPERATOR: return "operator";
            case T_ASSIGN: return "=";
            case T_SEMICOLON: return ";";
            case T_LPAREN: return "(";
            case T_RPAREN: return ")";
            case T_LBRACE: return "{";
            case T_RBRACE: return "}";
            case T_PRINT: return "print";
            case T_IF: return "if";
            case T_ELSE: return "else";
            case T_WHILE: return "while";
            default: return "unknown";
        }
    }

    // Parsing functions
    ASTNode* program() {
        BlockNode* root = new BlockNode(currentToken.lineNumber);
        while (currentToken.type != T_EOF) {
            root->statements.push_back(statement());
        }
        return root;
    }

    ASTNode* statement() {
        if (currentToken.type == T_IDENTIFIER) {
            // Variable assignment
            return assignmentStatement();
        } else if (currentToken.type == T_PRINT) {
            // Print statement
            return printStatement();
        } else if (currentToken.type == T_IF) {
            // If statement
            return ifStatement();
        } else if (currentToken.type == T_WHILE) {
            // While loop
            return whileStatement();
        } else if (currentToken.type == T_LBRACE) {
            // Block
            return block();
        } else {
            throw std::runtime_error("Unexpected token '" + currentToken.value + "' at line " + std::to_string(currentToken.lineNumber));
        }
    }

    ASTNode* assignmentStatement() {
        std::string varName = currentToken.value;
        int lineNumber = currentToken.lineNumber;
        advance();
        expect(T_ASSIGN);
        ASTNode* expr = expression();
        expect(T_SEMICOLON);
        return new AssignNode(varName, expr, lineNumber);
    }

    ASTNode* printStatement() {
        int lineNumber = currentToken.lineNumber;
        expect(T_PRINT);
        expect(T_LPAREN);
        ASTNode* expr = expression();
        expect(T_RPAREN);
        expect(T_SEMICOLON);
        return new PrintNode(expr, lineNumber);
    }

    ASTNode* ifStatement() {
        int lineNumber = currentToken.lineNumber;
        expect(T_IF);
        expect(T_LPAREN);
        ASTNode* condition = expression();
        expect(T_RPAREN);
        ASTNode* trueBlock = statement();
        ASTNode* falseBlock = nullptr;
        if (currentToken.type == T_ELSE) {
            advance();
            falseBlock = statement();
        }
        return new IfNode(condition, trueBlock, falseBlock, lineNumber);
    }

    ASTNode* whileStatement() {
        int lineNumber = currentToken.lineNumber;
        expect(T_WHILE);
        expect(T_LPAREN);
        ASTNode* condition = expression();
        expect(T_RPAREN);
        ASTNode* loopBlock = statement();
        return new WhileNode(condition, loopBlock, lineNumber);
    }

    ASTNode* block() {
        int lineNumber = currentToken.lineNumber;
        expect(T_LBRACE);
        BlockNode* blockNode = new BlockNode(lineNumber);
        while (currentToken.type != T_RBRACE && currentToken.type != T_EOF) {
            blockNode->statements.push_back(statement());
        }
        expect(T_RBRACE);
        return blockNode;
    }

    ASTNode* expression() {
        ASTNode* node = equality();
        return node;
    }

    ASTNode* equality() {
        ASTNode* node = comparison();
        while (currentToken.type == T_OPERATOR && (currentToken.value == "==" || currentToken.value == "!=")) {
            std::string op = currentToken.value;
            int lineNumber = currentToken.lineNumber;
            advance();
            node = new BinOpNode(node, op, comparison(), lineNumber);
        }
        return node;
    }

    ASTNode* comparison() {
        ASTNode* node = term();
        while (currentToken.type == T_OPERATOR && (currentToken.value == "<" || currentToken.value == "<=" ||
                                                   currentToken.value == ">" || currentToken.value == ">=")) {
            std::string op = currentToken.value;
            int lineNumber = currentToken.lineNumber;
            advance();
            node = new BinOpNode(node, op, term(), lineNumber);
        }
        return node;
    }

    ASTNode* term() {
        ASTNode* node = factor();
        while (currentToken.type == T_OPERATOR && (currentToken.value == "+" || currentToken.value == "-")) {
            std::string op = currentToken.value;
            int lineNumber = currentToken.lineNumber;
            advance();
            node = new BinOpNode(node, op, factor(), lineNumber);
        }
        return node;
    }

    ASTNode* factor() {
        ASTNode* node = unary();
        while (currentToken.type == T_OPERATOR && (currentToken.value == "*" || currentToken.value == "/" || currentToken.value == "%")) {
            std::string op = currentToken.value;
            int lineNumber = currentToken.lineNumber;
            advance();
            node = new BinOpNode(node, op, unary(), lineNumber);
        }
        return node;
    }

    ASTNode* unary() {
        if (currentToken.type == T_OPERATOR && (currentToken.value == "+" || currentToken.value == "-" || currentToken.value == "!")) {
            std::string op = currentToken.value;
            int lineNumber = currentToken.lineNumber;
            advance();
            return new BinOpNode(new NumberNode(0, lineNumber), op, unary(), lineNumber);
        }
        return primary();
    }

    ASTNode* primary() {
        Token token = currentToken;
        if (token.type == T_NUMBER) {
            advance();
            return new NumberNode(std::stoi(token.value), token.lineNumber);
        } else if (token.type == T_IDENTIFIER) {
            advance();
            return new VariableNode(token.value, token.lineNumber);
        } else if (token.type == T_LPAREN) {
            advance();
            ASTNode* node = expression();
            expect(T_RPAREN);
            return node;
        } else {
            throw std::runtime_error("Unexpected token '" + token.value + "' at line " + std::to_string(token.lineNumber));
        }
    }

public:
    Parser(Queue<Token>& tokens) : tokens(tokens) {
        advance();
    }

    ASTNode* parse() {
        return program();
    }
};

#endif // PARSER_H
