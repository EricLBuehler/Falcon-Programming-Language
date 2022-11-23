struct _tok_data {
    string data;
    token_type type;
} ;

class Position{
    public:
        int col;
        int line;
        string infile;
        int index;
        Position(){}

        Position(string infile_){
            this->index=0;
            this->col=0;
            this->line=0;
            this->infile=infile_;
        }

        Position(string infile_, int index_, int col_, int line_){
            this->index=index_;
            this->col=col_;
            this->line=line_;
            this->infile=infile_;
        }

        void advance(char chr){
            this->index++;

            if (chr=='\n'){
                this->line++;
                this->col=0;
            } else{
                this->col++;
            }
        }

        void advance(){
            this->index++;
            this->col++;
        }

        Position copy(){
            return Position(this->infile,this->index,this->col,this->line);
        }
        
};

class Token{
    friend std::ostream& operator<<(std::ostream &s, const Token &tok);

    public:
        string data="";
        token_type type;
        Position start;
        Position end;
        Token(){}
        Token(string data_, token_type type_, Position start_, Position end_){
            data=data_;
            type=type_;
            start=start_;
            end=end_;
        }
    
};

std::ostream& operator<<(std::ostream &s, const Token &tok) {
    if (tok.data.length()==0){
        return s << token_type_to_str(tok.type);
    }
    return s << token_type_to_str(tok.type) << ":" << tok.data;
}


class Lexer{
    public:
        unordered_map<char, char> escape_chars = {
                { 'n', '\n' },
                { 't', '\t' },
                { '"', '"' },
                { '\'', '\'' },
                { '\\', '\\' },
                { '?', '\?' },
                { 'a', '\a' },
                { 'b', '\b' },
                { 'f', '\f'},
                { 'r', '\r' },
                { 'v', '\v' },
                { '0', '\0' },
        };

        string text;
        string file;
    
        vector<Token> tokens;

        Position pos;

        vector<string> KEYWORDS;

        Lexer(){}

        Lexer(string text_, vector<string> kwds){
            this->text=text_;
            this->chr=text[0];
            this->KEYWORDS=kwds;
        }

        void advance(){
            this->pos.advance(this->chr);
            if (this->pos.index > this->text.length()){
                this->chr='\0';
                return;
            }
            this->chr=this->text[this->pos.index];
        }

        char get_next(){
            if (this->pos.index+1 >= this->text.size()){
                return '\0';
            }
            return this->text[this->pos.index+1];
        }

        Position tokenize(){
            while (this->chr!='\0') {
                if ( (isalpha(this->chr) || this->chr=='_' ) ){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_identifier();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                    continue;
                }

                else if (isdigit(this->chr)){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_number();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                    continue;
                }

                else if (this->chr=='#'){
                    make_comment();
                    continue;
                }

                else if (this->chr=='\n' || this->chr==';'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("",T_NEWLINE,start,end);
                    tokens.push_back(t);
                }
                else if (this->chr=='"'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_string();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }
                else if (this->chr=='\''){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_string_single();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='='){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_equals();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                    continue;
                }

                else if (this->chr=='.'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_dot();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                    continue;
                }

                else if (this->chr==':'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t(":",T_COLON,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='+'){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_plus();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='-'){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_sub();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='*'){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_mul();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='/'){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_div();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='%'){
                    Position start=this->pos.copy();
                    struct _tok_data res =make_percent();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='('){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("(",T_LPAREN,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr==')'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t(")",T_RPAREN,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='>'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_greater();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }
                
                else if (this->chr=='<'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_less();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='!'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_notequals();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='{'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("{",T_LCURLY,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='}'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("}",T_RCURLY,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr==','){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t(",",T_COMMA,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='['){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("[",T_LSQUARE,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr==']'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("]",T_RSQUARE,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='~'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("~",T_TILDE,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='&'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_amp();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='|'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_vbar();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='\\'){
                    while (this->chr!='\n'){
                        this->advance();
                    }
                }

                else if (this->chr=='?'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("?",T_QMARK,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='@'){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t("@",T_AT,start,end);
                    tokens.push_back(t);
                }

                else if (this->chr=='^'){
                    Position start=this->pos.copy();
                    struct _tok_data res = make_caret();
                    Position end=this->pos.copy();
                    Token t(res.data,res.type,start,end);
                    tokens.push_back(t);
                }

                else if (!isspace(this->chr)){
                    Position start=this->pos.copy();
                    Position end=this->pos.copy();
                    end.advance();
                    Token t(string(1,this->chr),T_UNKNOWN,start,end);
                    tokens.push_back(t);
                }

                this->advance();
            }

            Position start=this->pos.copy();
            Position end=this->pos.copy();
            end.advance();
            Token t("",T_EOF,start,end);
            tokens.push_back(t);
            return this->pos.copy();
        }

    private:
        char chr;

        _tok_data make_greater(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_GT;
            if (this->get_next()=='='){
                this->advance();
                res.data=">=";
                res.type=T_GTE;
            }
            if (this->get_next()=='>'){
                this->advance();
                res.data=">>";
                res.type=T_RSHIFT;
                if (this->get_next()=='='){
                    this->advance();
                    res.data=">>=";
                    res.type=T_IRSH;
                }
            }
            return res;
        }

        _tok_data make_less(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_LT;
            if (this->get_next()=='='){
                this->advance();
                res.data="<=";
                res.type=T_LTE;
            }
            if (this->get_next()=='<'){
                this->advance();
                res.data="<<";
                res.type=T_LSHIFT;
                if (this->get_next()=='='){
                    this->advance();
                    res.data="<<=";
                    res.type=T_ILSH;
                }
            }
            return res;
        }

        _tok_data make_notequals(){
            _tok_data res;
            res.type=T_ERR;
            if (this->get_next()=='='){
                this->advance();
                res.data="!=";
                res.type=T_NE;
                return res;
            }
            this->advance();
            return res;
        }

        _tok_data make_equals(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_EQ;
            this->advance();
            if (this->chr=='='){
                this->advance();
                res.data="==";
                res.type=T_EE;
            }
            return res;
        }

        _tok_data make_binary_number(){
            string output;

            while (this->chr!='\0') {
                if (this->chr=='_'){
                    this->advance();
                    continue;
                }
                else{
                    output.push_back(this->chr);
                }
                this->advance();
            }

            _tok_data res;
            res.data=output;
            res.type=T_BIN;
            return res;
        }

        _tok_data make_hex_number(){
            string output;

            while (this->chr!='\0') {
                if (this->chr=='_'){
                    this->advance();
                    continue;
                }
                else{
                    output.push_back(this->chr);
                }
                this->advance();
            }

            _tok_data res;
            res.data=output;
            res.type=T_BIN;
            return res;
        }

        _tok_data make_octal_number(){
            string output;

            while (this->chr!='\0') {
                if (this->chr=='_'){
                    this->advance();
                    continue;
                }
                else{
                    output.push_back(this->chr);
                }
                this->advance();
            }

            _tok_data res;
            res.data=output;
            res.type=T_OCTAL;
            return res;
        }

        _tok_data make_number(){
            int dotcount = 0;
            int expcount = 0;

            string output;

            while (this->chr!='\0') {
                if (this->chr=='.'){
                    if (dotcount==1){
                        break;
                    }
                    dotcount++;
                    
                    output.push_back(this->chr);
                }
                else if (this->chr=='e'){
                    if (expcount==1){
                        break;
                    }
                    expcount++;
                    
                    output.push_back(this->chr);
                }
                else if (this->chr=='b' && output.size()==1 && output[0]=='0'){
                    this->advance();
                    return make_binary_number();
                }
                else if (this->chr=='x' && output.size()==1 && output[0]=='0'){
                    this->advance();
                    return make_hex_number();
                }
                else if (this->chr=='o' && output.size()==1 && output[0]=='0'){
                    this->advance();
                    return make_octal_number();
                }
                else if ( (this->chr=='-' || this->chr=='+') && expcount==1){
                    output.push_back(this->chr);
                }
                else if (isdigit(this->chr)){
                    output.push_back(this->chr);
                }
                else if (this->chr=='_'){
                    this->advance();
                    continue;
                }
                else{
                    break;
                }

                this->advance();
            }

            _tok_data res;
            res.data=output;

            if (dotcount==0){
                res.type=T_INT;
            }
            if (dotcount==1 || expcount==1){
                res.type=T_FLOAT;
            }
            return res;
        }

        _tok_data make_identifier(){
            string output;

            while (this->chr!='\0' && (isalpha(this->chr) || this->chr=='_'  || isdigit(this->chr)) && this->chr!='=' \
            && this->chr!='*' && this->chr!='-' && this->chr!='/' && this->chr!='+' && this->chr!=')'\
            && this->chr!='(' && this->chr!='{' && this->chr!='}' ) {
                output.push_back(this->chr);

                this->advance();
            }

            
            for (string kwd:KEYWORDS){
                if (output==kwd){
                    _tok_data res;
                    res.data=output;
                    res.type=T_KWD;

                    return res;
                }
            }
            if (output=="and"){
                _tok_data res;
                res.data=output;
                res.type=T_AND;

                return res;
            }
            if (output=="or"){
                _tok_data res;
                res.data=output;
                res.type=T_OR;

                return res;
            }
            if (output=="not"){
                _tok_data res;
                res.data=output;
                res.type=T_NOT;

                return res;
            }
            if (output=="in"){
                _tok_data res;
                res.data=output;
                res.type=T_IN;

                return res;
            }
            if (output=="True"){
                _tok_data res;
                res.data=output;
                res.type=T_TRUE;

                return res;
            }
            if (output=="False"){
                _tok_data res;
                res.data=output;
                res.type=T_FALSE;

                return res;
            }
            if (output=="None"){
                _tok_data res;
                res.data=output;
                res.type=T_NONE;

                return res;
            }
            if (output=="is"){
                _tok_data res;
                res.data=output;
                res.type=T_IS;

                return res;
            }

            _tok_data res;
            res.data=output;
            res.type=T_IDENTIFIER;

            return res;

            
        }

        _tok_data make_string(){
            string output="";
            this->advance();
            bool escape=false;

            while (this->chr!='\0' && (this->chr!='"' || escape) ) {
                if (escape){
                    if (this->chr=='u'){ //Unicode literal
                        escape=false;
                        this->advance();
                                    
                        string unicode;

                        while (this->chr!='\0' && this->chr!='"') {
                            if (this->chr=='_'){
                                this->advance();
                                continue;
                            }
                            else if ( (!isdigit(this->chr) && tolower(this->chr)!='a' && tolower(this->chr)!='b' &&\
                                                            tolower(this->chr)!='c' && tolower(this->chr)!='d' &&\
                                                            tolower(this->chr)!='e' && tolower(this->chr)!='f') ||\
                                                            unicode.size()==4){                                
                                _tok_data res;
                                res.data=to_string(unicode.size());
                                res.type=T_ERROR_UNICODE;
                                return res;
                            }                                                              
                            else{
                                unicode.push_back(this->chr);
                            }
                            this->advance();
                        }

                        uint32_t num = (uint32_t)strtol(unicode.c_str(), NULL, 16);
                        output+=codept_to_str(num);

                        if (this->chr=='"'){
                            _tok_data res;
                            res.data=output;
                            res.type=T_STR;
                            return res;
                        }
                        this->advance();
                        continue;
                    }
                    if (escape_chars.find(this->chr)==escape_chars.end()){
                        output.push_back('\\');
                        output.push_back(this->chr);
                        escape=false;
                    }
                    else{
                        output.push_back(escape_chars[this->chr]);
                        escape=false;
                    }
                }
                else{
                    if (this->chr=='\\'){
                        escape=true;
                    }
                    else{
                        output+=this->chr;
                    }
                }

                this->advance();
            }
            _tok_data res;
            res.data=output;
            res.type=T_STR;
            if (this->chr=='\0'){
                res.type=T_ERROR_EOF;
            }
            
            return res;

            
        }

        _tok_data make_string_single(){
            string output="";
            this->advance();
            bool escape=false;

            while (this->chr!='\0' && (this->chr!='\'' || escape) ) {                
                if (escape){
                    if (this->chr=='u'){ //Unicode literal
                        escape=false;
                        this->advance();
                                    
                        string unicode;

                        while (this->chr!='\0' && this->chr!='"') {
                            if (this->chr=='_'){
                                this->advance();
                                continue;
                            }
                            else if ( (!isdigit(this->chr) && tolower(this->chr)!='a' && tolower(this->chr)!='b' &&\
                                                            tolower(this->chr)!='c' && tolower(this->chr)!='d' &&\
                                                            tolower(this->chr)!='e' && tolower(this->chr)!='f') ||\
                                                            unicode.size()==4){                                
                                _tok_data res;
                                res.data=to_string(unicode.size());
                                res.type=T_ERROR_UNICODE;
                                return res;
                            }                                                              
                            else{
                                unicode.push_back(this->chr);
                            }
                            this->advance();
                        }

                        uint32_t num = (uint32_t)strtol(unicode.c_str(), NULL, 16);
                        output+=codept_to_str(num);

                        if (this->chr=='"'){
                            _tok_data res;
                            res.data=output;
                            res.type=T_STR;
                            return res;
                        }
                        this->advance();
                        continue;
                    }
                    if (escape_chars.find(this->chr)==escape_chars.end()){
                        output.push_back('\\');
                        output.push_back(this->chr);
                        escape=false;
                    }
                    output.push_back(escape_chars[this->chr]);
                    escape=false;
                }
                else{
                    if (this->chr=='\\'){
                        escape=true;
                    }
                    else{
                        output+=this->chr;
                    }
                }

                this->advance();
            }
            _tok_data res;
            res.data=output;
            res.type=T_STR;
            if (this->chr=='\0'){
                res.type=T_ERROR_EOF;
            }
            
            return res;

            
        }

        _tok_data make_mul(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_MUL;
            if (this->get_next()=='*'){
                res.type=T_POW;
                res.data="**";
                this->advance();
                if (this->get_next()=='='){
                    res.type=T_IPOW;
                    res.data="**=";
                    this->advance();
                }
            }
            if (this->get_next()=='='){
                this->advance();
                res.data="*=";
                res.type=T_IMUL;
            }
            return res;
        }

        _tok_data make_plus(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_PLUS;
            if (this->get_next()=='='){
                this->advance();
                res.data="+=";
                res.type=T_IADD;
            }
            return res;
        }

        _tok_data make_sub(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_MINUS;
            if (this->get_next()=='='){
                this->advance();
                res.data="-=";
                res.type=T_ISUB;
            }
            return res;
        }

        _tok_data make_div(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_DIV;
            if (this->get_next()=='='){
                this->advance();
                res.data="/=";
                res.type=T_IDIV;
            }
            if (this->get_next()=='/'){
                this->advance();
                res.data="//";
                res.type=T_FLDIV;
                if (this->get_next()=='='){
                    this->advance();
                    res.data="//=";
                    res.type=T_IFLDIV;
                }
            }
            return res;
        }

        _tok_data make_percent(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_PERCENT;
            if (this->get_next()=='='){
                this->advance();
                res.data="%=";
                res.type=T_IMOD;
            }
            return res;
        }

        _tok_data make_dot(){
            this->advance();
            if (this->chr!='.'){
                int dotcount = 1;
                
                if (isdigit(this->chr)){
                    string output="0.";

                    while (this->chr!='\0') {
                        if (this->chr=='.'){
                            if (dotcount==1){
                                break;
                            }
                            dotcount++;
                            
                            output.push_back(this->chr);
                        }
                        else if (isdigit(this->chr)){
                            output.push_back(this->chr);
                        }
                        else{
                            break;
                        }

                        this->advance();
                    }
                    
                    _tok_data res;
                    res.data=output;
                    res.type=T_FLOAT;
                    return res;
                }

                for (int i=0; i<1; i++){
                    _tok_data res=make_identifier();
                    if (res.data==""){
                        break;
                    }
                    res.type=T_DOTIDENT;
                    return res;
                }
                

            }
            _tok_data res;
            res.type=T_ERR;
            return res;
        }

        _tok_data make_amp(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_AMPERSAND;
            if (this->get_next()=='='){
                this->advance();
                res.data="&=";
                res.type=T_IAMP;
            }
            return res;
        }

        _tok_data make_vbar(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_VBAR;
            if (this->get_next()=='='){
                this->advance();
                res.data="|=";
                res.type=T_IVBAR;
            }
            return res;
        }

        _tok_data make_caret(){
            _tok_data res;
            res.data=this->chr;
            res.type=T_CARET;
            if (this->get_next()=='='){
                this->advance();
                res.data="^=";
                res.type=T_IXOR;
            }
            return res;
        }

        void make_comment(){
            while (this->chr!='\0' && this->chr!='\n' ) {
                this->advance();
            }
        }

};