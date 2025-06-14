#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>

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
    bool debug=std::string(argv[2])=="debug";
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
    std::vector<Token> tokens;
    std::stack<int> addr;
    std::string ops="+-<>.,[]";
    size_t index=0;
    while(index<code.size())
    {
        char c=code[index++];
        if(c=='[')
        {
            addr.push(tokens.size());
            tokens.push_back({TokenType::Jump_If_Zero,0});
        }
        else if(c == ']')
        {
            if(addr.empty())
            {
                std::cerr << "Unbalanced loops ] at  " << index << "\n"; 
                exit(EXIT_FAILURE);
            }
            size_t val=addr.top();
            addr.pop();
            tokens[val].val=tokens.size()+1;
            tokens.push_back({TokenType::Jump_If_Not_Zero,val+1});
        }
        else 
        {
            size_t count=1;
            while(code[index]==c)
            {
                count++;
                index++;
                while(index < code.size() && ops.find(code[index])==std::string::npos){index++;}
            }
            switch(c)
            {
                case '+':tokens.push_back({TokenType::Inc,count});break;
                case '-':tokens.push_back({TokenType::Dec,count});break;
                case '<':tokens.push_back({TokenType::Left,count});break;
                case '>':tokens.push_back({TokenType::Right,count});break;
                case ',':tokens.push_back({TokenType::Input,count});break;
                case '.':tokens.push_back({TokenType::Output,count});break;
            }
        }
    }
    if(debug)
    {
        int i=0;
        for (const Token& token : tokens)
        {
            std::cout <<  i++ << " " << char(token.type) << " " << token.val << "\n";
        }
        std::cout << code << "\n";
    }
    int memory[100]={0};
    int ip=0;
    int head=0;

    while(ip<tokens.size())
    {
        Token token=tokens[ip++];
        switch(token.type)
        {
            case Inc: memory[head]+=token.val;break;
            case Dec: memory[head]-=token.val;break;
            case Right:head+=token.val;break;
            case Left:head-=token.val;break;
            case Input: exit(EXIT_FAILURE);
            case Output: while(token.val--)std::cout << char(memory[head]);break;
            case Jump_If_Zero: if(memory[head]==0)ip=token.val;break;
            case Jump_If_Not_Zero:if(memory[head]!=0)ip=token.val;break;
        }
    }
    
    return 0;
}