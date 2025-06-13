#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

enum TokenType{
    Inc     ='+',
    Dec     = '-',
    Left    = '<',
    Right   = '>',
    Input   = ',',
    Output  = '.',
    Jump_If_Zero        = '[',
    Jump_If_Not_Zero    = ']',
};


struct Token{
    TokenType type;
    size_t val;
};



int main(int argc,char* argv[])
{
    std::string code,path=argv[1];
    {
        std::stringstream contents_stream;
        std::fstream input(path, std::ios::in);
        if (!input.is_open())
        {
            std::cerr << "Failed to open file   " << path << "\n";
            exit(EXIT_FAILURE);
        }
        contents_stream << input.rdbuf();
        code = contents_stream.str();
    }
    std::cout << code << "\n";
    std::vector<Token> tokens;
    int index=0;
    while(index<code.size())
    {
        char c=code[index++];
        size_t count=1;
        while(code[index]==c)
        {
            count++;
            index++;
        }
        switch(c)
        {
            case '+':tokens.push_back({TokenType::Inc,count});break;
            case '-':tokens.push_back({TokenType::Dec,count});break;
            case '<':tokens.push_back({TokenType::Left,count});break;
            case '>':tokens.push_back({TokenType::Right,count});break;
            case ',':tokens.push_back({TokenType::Input,count});break;
            case '.':tokens.push_back({TokenType::Output,count});break;
            case '[': tokens.push_back({TokenType::Jump_If_Zero,count});break;
            case ']':tokens.push_back({TokenType::Jump_If_Not_Zero,count});break;
        }
    }
    for (const Token& token : tokens)
    {
        std::cout << char(token.type) << " " << token.val << "\n";
    }
    
    return 0;
}