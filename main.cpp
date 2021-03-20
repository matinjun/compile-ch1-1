#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include <direct.h>

// ----------------------------------------------------------------
// TODO: read file line by line
/**
 * @brief read file line by line
 * 
 * @param file: std::string 
 * @return std::vector<std::string> lines in file 
 */
std::vector<std::string> read_file_lines(const std::string & file) {
    std::fstream in_file(file, std::ios::in);
    std::vector<std::string> lines;
    std::string line;
    if(!in_file.is_open()) {
        std::cerr << "\n---------------------\n";
        std::cerr << "    file not found" << std::endl;
        std::cerr << "---------------------\n";
        char path[128];
        getcwd(path, 128);
        std::cerr << "the current working directory is " << path << std::endl;
        std::cerr << "---------------------\n";
        std::exit(1);
    }
    
    while(std::getline(in_file, line)) {
        if(line.length() > 0) {
            lines.push_back(line);
        }
    }

    in_file.close();
    return lines;
}

// TODO: preprocess
// TODO: 去掉注释，难以实现
std::string removeComments(std::string prgm) 
{ 
    int n = prgm.length(); 
    std::string res; 
  
    // Flags to indicate that single line and multpile line comments 
    // have started or not. 
    bool s_cmt = false; 
    bool m_cmt = false; 
  
  
    // 为了防止越界，添加了i + 1 < n的条件
    // Traverse the given program 
    for (int i=0; i<n; i++) 
    { 
        // If single line comment flag is on, then check for end of it 
        if (s_cmt == true && prgm[i] == '\n') 
            s_cmt = false; 
  
        // If multiple line comment is on, then check for end of it 
        else if  (i + 1 < n && m_cmt == true && prgm[i] == '*' && prgm[i+1] == '/') 
            m_cmt = false,  i++; 
  
        // If this character is in a comment, ignore it 
        else if (s_cmt || m_cmt) 
            continue; 
  
        // Check for beginning of comments and set the approproate flags 
        else if (i + 1 < n && prgm[i] == '/' && prgm[i+1] == '/') 
            s_cmt = true, i++; 
        else if (i + 1 < n && prgm[i] == '/' && prgm[i+1] == '*') 
            m_cmt = true,  i++; 
  
        // If current character is a non-comment character, append it to res 
        else  res += prgm[i]; 
    } 
    return res; 
} 

// ----------------------------------------------------------------
// TODO: 提取字符串常量，单边引号难以处理，双引号嵌套难以处理
// TODO: 提取形如'x'的字符常量，同上
// DONE
/**
 * @brief return string constants and char constants
 * 
 * @param line 
 * @param ch 
 * @return std::vector<std::string> 
 */
std::vector<std::string> remove_ch_string(std::string & line, char ch = '\"') {
    int i, j;
    std::vector<std::string> strings;
    std::string str;
    
    do {
        i = j = -1;
        int len = line.length();
        int k = 0;
        // 定位ch的位置，注意不是\"或\'
        for(; k < len; k++) {
            if(ch == line[k] && (0 == k || '\\' != line[k - 1])) {
                i = k;
                break;
            }
        }
        for(k = k+1; k < len; k++) {
            if(ch == line[k] && (0 == k || '\\' != line[k - 1])) {
                j = k;
                break;
            }
        }
        if(i != -1 && j != -1) {
            str = line.substr(i, j + 1 - i);
            strings.push_back(str);
            line.erase(i, j + 1 - i);
        }
    } while (i != -1 && j != -1);
    return strings;
}

// ---------------------------------------------------------------
// TODO: [ ] { } ( ) , 前后插入空格
// DONE
/**
 * @brief add stpace around '[', ']', '(', ')', '{', '}'
 * 
 * @param str 
 */
void preprocess_add_space(std::string & str) {

    std::string::iterator it = str.begin();
    while(it != str.end()) {
        switch(*it) {
            case '(': 
            case ')':
            case '[': 
            case ']':
            case '{':
            case '}':
                it = str.insert(it, ' ');
                it = str.insert(it + 2, ' ');
                break;
            default:
                it++;
        }
    }

}
// ----------------------------------------------------------------
// TODO: return words of sentence
// DONE
/**
 * @brief parse a string to an array of words
 * 
 * @param str 
 * @return std::vector<std::string> 
 */
std::vector<std::string> read_words(const std::string & str) {
    std::vector<std::string> words;
    std::string word;
    int i = 0;
    do {
        while(std::isspace(str[i]) && i < str.length()) ++i;
        while(!std::isspace(str[i]) && i < str.length()) {
            word += str[i++];
        }
        if(word.length() != 0) words.push_back(word);
        word.clear();
    } while(i < str.length());
    return words;
}

// ----------------------------------------------------------------
// TODO: split a word into a list of varibles, keywords, operators
// 在预处理之后，该程序可以基本保证正确性
/**
 * @brief return tokens of a word
 * 
 * @param word 
 * @return std::vector<std::string> 
 */
std::vector<std::string> split_word(const std::string & word) {
    std::vector<std::string> tokens;
    std::string token;
    int i = 0;
    while(i < word.length()) {
        while((std::isalnum(word[i]) || '_' == word[i]) && i < word.length()) {
            token += word[i++];
        }
        if(token.length()) {
            tokens.push_back(token);
            token.clear();
        }

        while((!std::isalnum(word[i]) && '_' != word[i]) && i < word.length()) {
            token += word[i++];
        }
        if(token.length()) {
            tokens.push_back(token);
            token.clear();
        }
    }
    return tokens;
}

// ----------------------------------------------------------------
// TODO: 词素种类
enum TokenType {
    ALGORITHM_OPERATOR,
    ASSIGN_OPERATOR,
    BITS_OPERATOR,
    LOGIC_OPERATOR,
    POINTER_OPERATOR,
    LEFT_PARENTHES,
    RIGHT_PARENTHES,
    LEFT_SQUARE_OPERATOR,
    RIGHT_SQUARE_OPERATOR,
    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,
    IDENTIFIER,
    NUM,
    STRING_TYPE,
    CHAR_TYPE,
    MACRO_TOKEN,
    DELIMITER,
    KEYWORDS
};
// TODO: 判断类型
/**
 * @brief 实例whitchType判断词素类型
 * 
 * 通过构造函数会索引关键词文件
 */
class WhitchType {
    std::set<std::string> algorithmOperator;
    std::set<std::string> assignmentOperator;
    std::set<std::string> bitsOperator;
    std::set<std::string> logicalOperator;
    std::set<std::string> pointerOperator;
    std::set<std::string> keywords;

public:
    WhitchType(const std::string & file) {
        algorithmOperator = std::set<std::string>{"+", "-", "*", "/", "%", "++", "--", ","};
        assignmentOperator = std::set<std::string> {"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "|=", "^="};
        bitsOperator = std::set<std::string> {">>", "<<", "&", "^", "|", "~"};
        logicalOperator = std::set<std::string> {"!", "&&", "||", "!=", ">=", "<=", "==", ">", "<", "?", ":"};
        pointerOperator = std::set<std::string> {"*", "&", ".", "->"};

        std::fstream in(file);
        if(!in.is_open()) {
            std::cerr << "\n---------------------\n";
            std::cerr << "    file not found" << std::endl;
            std::cerr << "---------------------\n";
            char path[128];
            getcwd(path, 128);
            std::cerr << "the current working directory is " << path << std::endl;
            std::cerr << "---------------------\n";
            std::exit(1);
        }
        std::string str;
        while(in >> str) {
            keywords.insert(str);
        }
        in.close();
    }
    int operator()(const std::string & token) {

        if(";" == token) {
            return DELIMITER;
        } else if("(" == token) {
            return LEFT_PARENTHES; 
        } else if(")" == token) {
            return RIGHT_PARENTHES; 
        } else if("[" == token) {
            return LEFT_SQUARE_OPERATOR; 
        } else if("]" == token) {
            return RIGHT_SQUARE_OPERATOR; 
        } else if("{" == token) {
            return LEFT_CURLY_BRACKET;
        } else if("}" == token) {
            return RIGHT_CURLY_BRACKET;
        } else if("#" == token) {
            return MACRO_TOKEN;
        }

        if(algorithmOperator.count(token)) {
            return ALGORITHM_OPERATOR;
        } else if(assignmentOperator.count(token)) {
            return ASSIGN_OPERATOR;
        } else if(bitsOperator.count(token)) {
            return BITS_OPERATOR;
        } else if(logicalOperator.count(token)) {
            return LOGIC_OPERATOR;
        } else if(pointerOperator.count(token)) {
            return POINTER_OPERATOR;
        } else if(keywords.count(token)) {
            return KEYWORDS;
        }
        
        switch(token[0]) {
            case '\'':
                return CHAR_TYPE;
                break;
            case '\"':
                return STRING_TYPE;
                break;
        }

        if(!token.empty() && token.end() == 
            std::find_if(token.begin(), token.end(), 
                [](unsigned char ch){return !std::isdigit(ch);})) {
            return NUM;
        }

        return IDENTIFIER;
    }
} whitchType("../token-keywords");

// ---------------------------------------------------------------
// TODO: product lexemes with set
// read file and parse it
class Types {
    std::set<std::string> algorithmOperator;
    std::set<std::string> assignmentOperator;
    std::set<std::string> bitsOperator;
    std::set<std::string> logicalOperator;
    std::set<std::string> pointerOperator;
    std::set<std::string> leftParenthesis;
    std::set<std::string> rightParenthesis;
    std::set<std::string> leftSquareBracket;
    std::set<std::string> rightSquareBracket;
    std::set<std::string> leftCurlyBracket;
    std::set<std::string> rightCurlyBracket;
    std::set<std::string> identifier;
    std::set<std::string> num;
    std::set<std::string> stringType;
    std::set<std::string> charType;
    std::set<std::string> macroToken;
    std::set<std::string> delimiter;
    std::set<std::string> keywords;
    
    
public:
    void parse(const std::string & file) {
        auto lines = read_file_lines(file);
        for(auto line : lines) {
            auto chs = remove_ch_string(line, '\'');
            auto strs = remove_ch_string(line, '\"');
            for(auto ch: chs) {
                charType.insert(ch);
            }
            for(auto str: strs) {
                stringType.insert(str);
            }

            preprocess_add_space(line);
            auto words = read_words(line);
            for(auto word : words) {
                auto tokens = split_word(word);
                for(auto token : tokens) {
                    this->add(token);
                }
            }
        }
    }
    // 输出各个类型到文件
    friend std::ostream& operator<<(std::ostream&out, const Types& types) {
        auto fn = [&out](const std::set<std::string>& myset, const std::string& str) {
            for(auto element : myset) {
                out << "(" << str << ", " << element << ")" << std::endl;
            }
        };
        fn(types.algorithmOperator, "algorithmOperator");
        fn(types.assignmentOperator, "assignmentOperator");
        fn(types.bitsOperator, "bitsOperator");
        fn(types.logicalOperator, "logicalOperator");
        fn(types.pointerOperator, "pointerOperator");
        fn(types.leftParenthesis, "leftParenthesis");
        fn(types.rightParenthesis, "rightParenthesis");
        fn(types.leftSquareBracket, "leftSquareBracket");
        fn(types.rightSquareBracket, "rightSquareBracket");
        fn(types.leftCurlyBracket, "leftCurlyBracket");
        fn(types.rightCurlyBracket, "rightCurlyBracket");
        fn(types.identifier, "identifier");
        fn(types.num, "num");
        fn(types.stringType, "stringType");
        fn(types.charType, "charType");
        fn(types.macroToken, "macroToken");
        fn(types.delimiter, "delimiter");
        fn(types.keywords, "keywords");

        return out;
    }
private:
    void add(const std::string& token) {
        switch(whitchType(token)) {
            case ALGORITHM_OPERATOR:
                algorithmOperator.insert(token);
                break;
            case ASSIGN_OPERATOR:
                assignmentOperator.insert(token);
                break;
            case BITS_OPERATOR:
                bitsOperator.insert(token);
                break;
            case LOGIC_OPERATOR:
                logicalOperator.insert(token);
                break;
            case POINTER_OPERATOR:
                pointerOperator.insert(token);
                break;
            case LEFT_PARENTHES:
                leftParenthesis.insert(token);
                break;
            case RIGHT_PARENTHES:
                rightParenthesis.insert(token);
                break;
            case LEFT_SQUARE_OPERATOR:
                leftSquareBracket.insert(token);
                break;
            case RIGHT_SQUARE_OPERATOR:
                rightSquareBracket.insert(token);
                break;
            case LEFT_CURLY_BRACKET:
                leftCurlyBracket.insert(token);
                break;
            case RIGHT_CURLY_BRACKET:
                rightCurlyBracket.insert(token);
                break;
            case IDENTIFIER:
                identifier.insert(token);
                break;
            case NUM:
                num.insert(token);
                break;
            case STRING_TYPE:
                stringType.insert(token);
                break;
            case CHAR_TYPE:
                charType.insert(token);
                break;
            case MACRO_TOKEN:
                macroToken.insert(token);
                break;
            case DELIMITER:
                delimiter.insert(token);
                break;
            case KEYWORDS:
                keywords.insert(token);
                break;
        }

    }
};


// ---------------------------------------------------------------
// TODO: sumarize above work, and organize them into a class

int main(int, char**) {
#if 0
    std::cout << std::isspace('\0') << std::endl;
    std::cout << std::isspace(' ') << std::endl;
#endif
#define PRE_PROCESS_STRING 0
#if PRE_PROCESS_STRING
    std::string str = "while(str[i] == ' ' \"tfasd asdf\" || str[i] == '\\0') \"hello world\" ++i;";
    // std::string str = " ";
    // std::cout << "before preprocessing: " << str << std::endl;
    auto strings = remove_ch_string(str, '\'');
    std::cout << "\nafter removing char:" << std::endl;
    for(auto ch : strings) {
        std::cout << ch << std::endl;
    }
    strings = remove_ch_string(str);
    std::cout << "\nafter removing string:" << std::endl;
    for(auto ch : strings) {
        std::cout << ch << std::endl;
    }

    preprocess_add_space(str);
    std::cout << "after preprocessing: " << str << std::endl;
    auto words = read_words(str);
    for(auto word: words) {
        auto tokens = split_word(word);
        for(auto token:tokens) {
            std::cout << token << std::endl;
        }
    }
#endif

#define PARSE_FILE 1
#if PARSE_FILE
    Types parsefile;
    parsefile.parse("../shape.c");
    std::cout << parsefile;
    std::fstream out("../parse-shape-c.txt", std::ios::out);
    if(!out.is_open()) {
        std::cerr << "\n---------------------\n";
        std::cerr << "    file not found" << std::endl;
        std::cerr << "---------------------\n";
        char path[128];
        getcwd(path, 128);
        std::cerr << "the current working directory is " << path << std::endl;
        std::cerr << "---------------------\n";
        std::exit(1);   
    }
    out << parsefile;
    out.close();
#endif

#define TEST_FILE 0
#if TEST_FILE
    std::cout << "\nafter read file: " << std::endl;
    auto lines = read_file_lines("../token-operators");
    for(auto line: lines) {
        std::cout << line << std::endl;
    }
#endif
#if 0
    std::string sentence;
    std::cout << "the length of sentence is " << sentence.length() << std::endl;
    sentence += 'h';
    std::cout << "after modfying, the length of sentence is " << sentence.length() << std::endl;
    std::cout << sentence << std::endl;
#endif
    return 0;
}

#if 0
// 该函数为简易版
std::string read_word(const std::string & str) {
    std::string word;
    int i = 0;
    while(str[i] == ' ' || str[i] == '\0') ++i;
    while(str[i] != ' ' && str[i] != '\0')  {
        word.push_back(str[i]);
        i++;
    }
    word.push_back('\0');
    return word;
}
#endif