# include <iostream>
# include <cstring>
# include <string> 
# include <vector>
# include <cstdio>
# include <cstdlib>
# include <iomanip>
# include <stdexcept>
# include <cmath>
# include <map>
# include <iomanip>
# include <sstream>
# include <cctype>
# include <exception>
# include <string.h>

using namespace std;

struct Information // 每個token的資料 
{
  string token_name ;
  string token_type ; // id num double sign num_sign
};

struct Error // 記錄錯誤的資訊 
{
  string error_name ;  
  string error_type ; // Unrecognized Unexpected Undefined
};

struct Reg // 被定義的token資料 ex: salary := 3000 
{
  string token_name ;
  string token_type ;
  double value ;
};

vector<Reg> gReg_line ;           // 存定義過ident資訊的vector
bool gQuit = false ;

string Double_to_String( double num )
{
  stringstream ss;
  ss << num;
  string s( ss.str() );

  return s;
} // Double_to_String()

class Algorithm
{
public :

  double String_to_Double( string str ) // 字面上的意思 
  {
    double re ;  

    if ( str[0] == '-' )
    {
      str.erase( str.begin() ) ;
      re = atof( str.c_str() );
      re *= -1 ;
    } // if
    else
      re = atof( str.c_str() );

    return re ;
  } // String_to_Double()

  void Dealwith_num_sign( vector<Information> &list ) // [sign] num 的情況 
  {
    for ( int i = 0 ; i < list.size() ; i++ )
    {
      if ( list[i].token_type == "num_sign" )
      {
        if ( list[i].token_name == "-" )
          list[i+1].token_name = list[i].token_name + list[i+1].token_name ;

        list.erase( list.begin() + i ) ;
        i-- ;
      } // if
    } // for 
  } // Dealwith_num_sign()

  int Priority( string op ) // 運算元的優先級別 
  {
    if ( op == "+" || op == "-"  ) return 1 ;
    else if ( op == "*" || op == "/" ) return 2 ;
    else return 0 ;
  } // Priority()

  vector<Information> In_To_Post( vector<Information> infix ) // 中序 轉 後序 
  { 
    Information stack[80] ;
    vector<Information> postfix ;
    int i, top;
    for ( i = 0, top = 0 ; i < infix.size() ; i++ )
    { 
      if ( infix[i].token_name == "(" )
      {
        stack[++top] = infix[i];
      } // if
      else if ( infix[i].token_name == "+" || infix[i].token_name == "-" ||
                infix[i].token_name == "*" || infix[i].token_name == "/" )
      {
        while ( Priority( stack[top].token_name ) >= Priority( infix[i].token_name ) ) { 
          postfix.push_back( stack[top--] ) ;
        } // while 

        stack[++top] = infix[i]; // 存入堆疊 
      } // else if
      else if ( infix[i].token_name == ")" )
      {
        while ( stack[top].token_name != "(" ) { // 遇 ) 輸出至 ( 
          postfix.push_back( stack[top--] ) ;
        } // while 

        top--; // 不輸出 (
      } // else if
      else
      {
        postfix.push_back( infix[i] ) ;
      } // else    
    } // for

    while ( top > 0 ) 
    { 
      postfix.push_back( stack[top--] ) ;
    } // while

    return postfix ;
  } // In_To_Post()

  double Ident_value( Information token ) // 取得該id代表的num 
  {
    double re = -1 ;
    
    for ( int i = 0 ; i < gReg_line.size() ; i++ )
    {
      if ( gReg_line[i].token_name == token.token_name )
        re = gReg_line[i].value ;
    } // for

    return re ;
  } // Ident_value()

  double Calculate( vector<Information> infix ) // 計算該算式的答案 
  {
    Dealwith_num_sign( infix ) ;
    vector<Information> postfix = In_To_Post( infix );
    vector<double> num_stack ;
    double temp1, temp2 ; 
  
    for ( int i = 0 ; i < postfix.size() ; i++ )
    {
      if ( postfix[i].token_name == "+" )
      {
        temp1 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        temp2 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        
        temp2 += temp1 ;
        num_stack.push_back( temp2 ) ;
      } // if
      else if ( postfix[i].token_name == "-" )
      {
        temp1 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        temp2 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        
        temp2 -= temp1 ;
        num_stack.push_back( temp2 ) ;
      } // else if
      else if ( postfix[i].token_name == "*" )
      {
        temp1 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        temp2 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        
        temp2 *= temp1 ;
        num_stack.push_back( temp2 ) ;
      } // else if
      else if ( postfix[i].token_name == "/" )
      {
        temp1 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        temp2 = num_stack[num_stack.size()-1] ;
        num_stack.pop_back();
        
        temp2 /= temp1 ;
        num_stack.push_back( temp2 ) ;
      } // else if
      else
      {
        if ( postfix[i].token_type == "id" )
        {
          temp1 = Ident_value( postfix[i] ) ;
        } // if
        else
        {
          temp1 = String_to_Double( postfix[i].token_name ) ;
        } // else            

        num_stack.push_back( temp1 ) ;
      } // else
    } // for   

    return num_stack[0] ;
  } // Calculate()

} ;
class Data 
{
public:

  void Remove_White_Space() // 跳過所有空格直到讀到非空格字元為止
  {
    while ( cin.peek() == ' ' || cin.peek() == '\t' )
    {
      cin.get() ;
    } // while
  } // Remove_White_Space()

  void Skip_White_Space( char &ch ) // 跳過所有空格直到讀到非空格字元為止，並回傳該字元 
  {
    while ( ch == ' ' || ch == '\t' ) 
    {
      ch = cin.get() ;
    } // while 
  } // Skip_White_Space()
  
  Information Get_Token() { // 開始讀入字元分類token並回傳 
            // 如果是字母開頭 後面可以是字母、數字、底線
            // 如果是數字開頭 那後面可以接數字和. .開頭+數字也視為數字 
            // 特殊符號 >= <= 等等 
            // 其餘的則不能當開頭 視為單一token處理 
            // 每切出一個token 就進interpret階段 
            
    char temp ;                  // 暫存讀入的字元
    char peek ;                  // 暫存下個要讀入的字元 連續判斷時需要用到 ex: 註解     
    bool decimal_point = false ; // 數字是否已經函小數點 小數點最多一個 ex: 1.2.3 (error)
    bool frist_isdot = false ;   // 第一個字元為"." 後面接數字視為 0.xx ex: .9 == 0.9  
    bool frist_char = true ;     // 是否為第一個字元
    bool finish_cut = false ;    // 是否切完此次token
    Information token ;     
    
    while ( !finish_cut ) // 切出一個token就停止 
    {
      temp = cin.get() ;
      if ( frist_char ) Skip_White_Space( temp ) ;
          
      if ( frist_char ) // 尚未決定第一個字元之種類 
      {
        if ( isalpha( temp ) ) // 首字為字母 
        {
          token.token_name = temp ;
          token.token_type = "id" ;
          frist_char = false ; 
        } // if
        else if ( isdigit( temp ) ) // 首字為數字 
        {
          decimal_point = false ;
          token.token_name = temp ;
          token.token_type = "num" ;
          frist_char = false ;
        } // else if
        else // 首字為符號 
        {
          if ( temp == '/' ) // 可能為註解 peek下一個如果也是'/'就整行88
          {
            peek = cin.peek() ;
            if ( peek == '/' ) // 確定為註解 88 
            {
              char command_temp[128] ;
              cin.getline( command_temp, 128 ) ;
            } // if
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else
          } // if
          else if ( temp == '.' ) // 可能為小數 
          {
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( isdigit( peek ) ) // 確定為小數 
            {
              token.token_name = ".";
              token.token_type = "double" ;
              frist_isdot = true ;
              frist_char = false ; 
            } // if
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == ':' ) // := ㄏㄏㄐㄐ 
          {
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( peek == '=' ) // ':=' 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '<' ) // <= || <>  
          {
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( peek == '=' || peek == '>' ) // '<=' || '<>' 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              Remove_White_Space() ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '>' ) // >=  
          {
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( peek == '=' ) // '>=' 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              Remove_White_Space() ;
              temp = cin.get() ;
              token.token_name += temp ;
              finish_cut = true ; 
            } // if
            else // 一般符號 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '\n' )
          {
            
          } // else if
          else // 一般符號 
          {
            token.token_name = temp ;
            token.token_type = "sign" ;
            finish_cut = true ; 
          } // else
        } // else
      } // if
      else // 已決定token第一個字元的種類 
      {
        if ( temp == '\n' )
        {
          cin.putback( temp ) ;
          finish_cut = true ;
        } // if
        else if ( frist_isdot ) 
        {
          if ( isdigit( temp ) )
          {
            token.token_name += temp ; 
          } // if
          else
          {
            cin.putback( temp ) ;
            frist_isdot = false ;
            finish_cut = true ;
          } // else
        } // else if
        else if ( token.token_type == "id" ) // token為字母 
        {
          if ( isalpha( temp ) || isdigit( temp ) || temp == '_' )
          {
            token.token_name += temp ;
          } // if 
          else 
          {
            cin.putback( temp ) ;
            finish_cut = true ;
          } // else
        } // else if
        else if ( token.token_type == "num" || token.token_type == "double" ) // token為數字
        {
          if ( isdigit( temp ) || ( temp == '.' && !decimal_point ) )
          {
            if ( temp == '.' ) 
            {
              decimal_point = true ;
              token.token_type = "double" ;
            } // if

            token.token_name += temp ;
          } // if
          else 
          {
            cin.putback( temp ) ;
            finish_cut = true ;
          } // else
        } // else if
      } // else

        
    } // while
   
    return token ; 
  } // Get_Token()

};

class Interpret
{
public:
  vector<Information> mToken_line ; // 存token直到分號的vector ( 從data而來      
  Error merror ;                    // 記錄錯誤的資訊 ( 錯誤的token & 錯誤的型別 
  string mImplement_type ;          // 要執行哪種類型 ex: 1+2 -> math 
                                    // 1>0 -> boolean, a := 1 -> ident 
  bool mdivision ;                  // 上一個運算元是除法　防止 /0的情況 

  Interpret()
  {
    gReg_line.clear() ;
  } // Interpret()

  bool Unrecognized_token( string s ) // 詞法錯誤 ex : 2 + $ - 5
  {                                   // Unrecognized token with first char : '$'
    if ( s == "+" || s == "-" || s == "*" || s == "/" || s == "=" || s == "(" ||
         s == ")" || s == "<" || s == ">" || s == ":=" || s == "<>" || s == ">=" ||
         s == "<=" || s == ";" )
    {
      return false ;
    } // if
    else 
    {
      if ( merror.error_name == "empty" )
      {
        char command_temp[128] ;
        cin.getline( command_temp, 128 ) ;
        merror.error_name = s ;
        merror.error_type = "Unrecognized" ;
      } // if

      return true ;
    } // else

  } // Unrecognized_token()

  void Unexpected_token( string str ) // 語法錯誤 ex : 2 + * - 5 
  {                                   // Unexpected token : '*'
    if ( merror.error_name == "empty" )
    {
      char command_temp[128] ;
      cin.getline( command_temp, 128 ) ;
      merror.error_name = str ;
      merror.error_type = "Unexpected" ; 
    } // if
  } // Unexpected_token()

  bool Undefined_identifier( string str ) // 語義錯誤 ex : bcd * 2 (尚未宣告bcd) 
  {                                       // Undefined identifier : 'bcd'
                                          // 文法無錯誤

    bool undefined = true ;
    for ( int i = 0 ; i < gReg_line.size() ; i++ )
    {
      if ( gReg_line[i].token_name == str ) undefined = false ;
    } // for

    if ( !undefined )
    {
      return false ;
    } // if
    else {
      if ( merror.error_name == "empty" )
      {
        char command_temp[128] ;
        cin.getline( command_temp, 128 ) ;
        merror.error_name = str ;
        merror.error_type = "Undefined" ;  
      } // if

      return true ;
    } // else

  } // Undefined_identifier()

  void Print_error() // 印出錯誤訊息 
  {
    if ( merror.error_type == "Unrecognized" )
    {
      cout << "> Unrecognized token with first char : '" << merror.error_name << "'" << endl ;
    } // if
    else if ( merror.error_type == "Unexpected" )
    {
      cout << "> Unexpected token : '" << merror.error_name << "'" << endl ;
    } // else if
    else if ( merror.error_type == "Undefined" )
    {
      cout << "> Undefined identifier : '" << merror.error_name << "'" << endl ;
    } // else if
  } // Print_error()

  void Command()
  {
    Data data ;
    Reg reg ;
    Information token ;
    mToken_line.clear() ;
    merror.error_name = "empty" ;
    mImplement_type = "math" ;
    mdivision = false ; 

    token = data.Get_Token() ;
    if ( token.token_name == "quit" ) // 高歌離席 
    {
      gQuit = true ;
    } // if
    else
    {
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
      {
   
      } // if
      else
      {
        if ( Ident_command( token ) ) // IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
        {
          if ( mImplement_type == "math" || mImplement_type == "boolean" )
          {
            if ( Undefined_identifier( mToken_line[0].token_name ) ) 
            {
  
            } // if
            else
            {
              Implemete() ;
            } // else
          } // if
          else
          {
            Implemete() ;
          } // else
        } // if
        else if ( Not_Ident_command( token ) ) // <NOT_IDStartArithExpOrBexp> ';'
        {
          /*
          cout << mImplement_type << endl ;
          for ( int i = 0 ; i < mToken_line.size() ; i++ )
          {
            cout << mToken_line[i].token_name << " " ;
          } // for

          cout << endl ;
          */
          
          Implemete() ;
        } // else if
      } // else 

      if ( merror.error_name != "empty" )
      {
        Print_error() ;
      } // if     
    } // else
  } // Command() 

  bool Ident_command( Information token ) // IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
  {  
    Data data ;
 
    if ( token.token_type == "id" )
    {
      mToken_line.push_back( token ) ;
      token = data.Get_Token() ;
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
      {
        return false ;
      } // if
      else
      {
        if ( token.token_name == ":=" ) // ':=' <ArithExp> ';'
        {
          mImplement_type = "ident" ;
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            return false ;
          } // if
          else
          {
            if ( ArithExp( token ) )
            {
              if ( token.token_name == ";" ) return true ;
              else // 文法錯誤 
              {
                Unexpected_token( token.token_name ) ; 
                return false ; 
              } // else 
            } // if
            else
            {
              Unexpected_token( token.token_name ) ; 
              return false ; // 語法錯誤
            } // else
          } // else
        } // if
        else if ( IDlessArithExpOrBexp( token ) )  // <IDlessArithExpOrBexp> ';'
        {
          if ( token.token_name == ";" ) 
          {
            return true ;
          } // if
          else // 文法錯誤 
          {
            Unexpected_token( token.token_name ) ; 
            return false ; 
          } // else
        } // else if
      } // else
    } // if
    else return false ; // 不是 ident 

    return false ;
  } // Ident_command()

  bool Not_Ident_command( Information token ) // <NOT_IDStartArithExpOrBexp> ';'
  {  
    Data data ; 

    if ( NOT_ID_StartArithExpOrBexp( token ) )
    {
      if ( token.token_name == ";" ) return true ;
      else // 文法錯誤 
      {
      	cout << "ouo4";
        Unexpected_token( token.token_name ) ; 
        return false ; 
      } // else
    } // if
    else {
      Unexpected_token( token.token_name ) ;
      return false ; 
    } // else
  } // Not_Ident_command()

  bool IDlessArithExpOrBexp( Information &token )
  {
    // {   '+' <Term>   | '-' <Term> | '*' <Factor> | '/' <Factor> }
    // [ <BooleanOperator> <ArithExp> ]

    Data data ;
    bool end_recursive = false ;

    while ( !end_recursive )
    {
      if ( token.token_name == "+" || token.token_name == "-" || token.token_name == "*" 
           || token.token_name == "/" || BooleanOperator( token )  )
      {
        if ( Undefined_identifier( mToken_line[0].token_name ) )
        {
          return false ;
        } // if
        else if ( token.token_name == "+" || token.token_name == "-" )
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            end_recursive = true ;
            return false ;
          } // if
          else
          {
            if ( !Term( token ) )
            {
              end_recursive = true ;
              return false ;
            } // if
          } // else
        } // else if
        else if ( token.token_name == "*" || token.token_name == "/" )
        {
          if ( token.token_name == "/" ) mdivision = true ;
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            end_recursive = true ;
            return false ;
          } // if
          else
          {
            if ( !Factor( token ) )
            {
              end_recursive = true ;
              return false ;
            } // if
          } // else 
        } // else if
        else if ( BooleanOperator( token ) )
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {   
            end_recursive = true ;
            return false ;
          } // if       
          else
          {
            if ( ArithExp( token ) )
            {
              end_recursive = true ;
              return true ;
            } // if
            else // 語法錯誤 
            {
              Unexpected_token( token.token_name ) ;
              end_recursive = true ;
              return false ; 
            } // else
          } // else
        } // else if
      } // if
      else 
      {
        end_recursive = true ; 
      } // else 
    } // while

    return true ;
  } // IDlessArithExpOrBexp()
  
  bool BooleanOperator( Information token ) // '=' | '<>' | '>' | '<' | '>=' | '<='
  {
    Data data ;  

    if ( token.token_name == "=" || token.token_name == "<>" || token.token_name == ">" ||
         token.token_name == "<" || token.token_name == ">=" || token.token_name == "<="  )
    {
      mImplement_type = "boolean" ;
      return true ;
    } // if
    else
    {
      return false ;
    } // else
  } // BooleanOperator()

  bool NOT_ID_StartArithExpOrBexp( Information &token ) 
  { // <NOT_ID_StartArithExp> [ <BooleanOperator> <ArithExp> ]
    Data data ;

    if ( NOT_ID_StartArithExp( token ) )
    {
      if ( BooleanOperator( token ) )
      {
        mToken_line.push_back( token ) ;
        token = data.Get_Token() ;
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {   
          return false ;
        } // if       
        else
        {
          if ( ArithExp( token ) )
          {
            return true ;
          } // if
          else // 語法錯誤 
          {
            Unexpected_token( token.token_name ) ;
            return false ; 
          } // else
        } // else
      } // if
      else
      {
      	cout << "ouo3";
        return true ;
      } // else
    } // if
    else // 語法錯誤 
    {
      return false ; 
    } // else
  } // NOT_ID_StartArithExpOrBexp()

  bool NOT_ID_StartArithExp( Information &token ) // <NOT_ID_StartTerm> { '+' <Term> | '-' <Term> }
  {                           
    Data data ;
    
    if ( NOT_ID_StartTerm( token ) )
    {
      bool end_recursive = false ;
      //cout << "ouo";
      while ( !end_recursive )
      {        
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "+" || token.token_name == "-" )    
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            end_recursive = true ;
            return false ;
          } // if
          else
          {
            if ( Term( token ) )
            {
  
            } // if
            else
            {
              end_recursive = true ;
              return false ;
            } // else
          } // else
        } // else if
        else
        {
          cout << "ouo2";
          end_recursive = true ;
          return true ;
        } // else 
      } // while
    } // if
    else // 語法錯誤 
    {
      return false ; 
    } // else

    return false ;
  } // NOT_ID_StartArithExp()

  bool NOT_ID_StartTerm( Information &token ) // <NOT_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
  {
    Data data ;  

    if ( NOT_ID_StartFactor( token ) )
    {
      bool end_recursive = false ;
      while ( !end_recursive )
      {      
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "*" || token.token_name == "/" )    
        {
          if ( token.token_name == "/" ) mdivision = true ;
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            end_recursive = true ;
            return false ;
          } // if
          else
          {
            if ( !Factor( token ) )
            {
              end_recursive = true ;
              return false ;
            } // if
          } // else
        } // else if
        else
        {
          end_recursive = true ;
          return true ;
        } // else 
      } // while
    } // if
    else // 語法錯誤 
    {
      return false ;
    } // else

    return false ;
  } // NOT_ID_StartTerm()

  bool NOT_ID_StartFactor( Information &token ) // [ SIGN ] NUM | '(' <ArithExp> ')' 
  {
    Data data ;
  
    if ( token.token_name == "+" || token.token_name == "-" || token.token_type == "num" ||
         token.token_type == "double" ) // [ SIGN ] NUM
    {
      if ( token.token_name == "+" || token.token_name == "-" ) // [ SIGN ] NUM  
      {
        token.token_type = "num_sign" ;
        mToken_line.push_back( token ) ;
        token = data.Get_Token() ;
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {
          return false ;
        } // if
        else
        {
          if ( token.token_type == "num" || token.token_type == "double" )
          {
            mToken_line.push_back( token ) ;
            token = data.Get_Token() ;
            return true ;
          } // if
          else
          {
            Unexpected_token( token.token_name ) ;
            return false ;
          } // else
        } // else
      } // if
      else // NUM
      {
        mToken_line.push_back( token ) ;
        token = data.Get_Token() ;
        return true ;
      } // else
    } // if
    else if ( token.token_name == "(" ) // '(' <ArithExp> ')'
    {
      mToken_line.push_back( token ) ;
      token = data.Get_Token() ;
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
      {
        return false ;
      } // if
      else
      {
        if ( ArithExp( token ) )
        {
          if ( token.token_name == ")" )
          {
            mToken_line.push_back( token ) ;
            token = data.Get_Token() ;
            return true ;
          } // if
          else // 語法錯誤 
          {
            Unexpected_token( token.token_name ) ;
            return false ;
          } // else
        } // if
        else
        {
          return false ;
        } // else
      } // else
    } // else if
    else {
      return false ; // 語法錯誤
    } // else

    return false ;
  } // NOT_ID_StartFactor()

  bool ArithExp( Information &token ) // <Term> { '+' <Term> | '-' <Term> }
  {
    Data data ;    

    if ( Term( token ) )
    {
      bool end_recursive = false ;
      while ( !end_recursive )
      {        
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "+" || token.token_name == "-" )    
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            end_recursive = true ;
            return false ;
          } // if
          else
          {
            if ( Term( token ) )
            {
  
            } // if
            else
            {
              end_recursive = true ;
              return false ;
            } // else
          } // else
        } // else if
        else
        {
          //cout << "omo" ; 
          end_recursive = true ;
          return true ;
        } // else 
      } // while
    } // if
    else return false ;

    return false ;
  } // ArithExp()

  bool Term( Information &token ) // <Factor> { '*' <Factor> | '/' <Factor> }
  {
    Data data ;   
    if ( Factor( token ) )
    {
      bool end_recursive = false ;
      while ( !end_recursive )
      {        
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "*" || token.token_name == "/" )    
        {
          if ( token.token_name == "/" ) mdivision = true ;
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
          {
            end_recursive = true ;
            return false ;
          } // if
          else
          {
            if ( !Factor( token ) )
            {
              end_recursive = true ;
              return false ;
            } // if
          } // else
        } // else if
        else
        {
          end_recursive = true ;
          return true ;
        } // else 
      } // while
    } // if
    else 
    {
      return false ; 
    } // else

    return false ;
  } // Term()

  bool Factor( Information &token ) // IDENT | [ SIGN ] NUM | '(' <ArithExp> ')'
  {
    Data data ;
    Algorithm algorithm ;

    if ( token.token_type == "id" ) // IDENT 
    {
      if ( Undefined_identifier( token.token_name ) )
      {
        return false ;
      } // if
      else if ( mdivision && algorithm.Ident_value( token ) == 0 )
      {
        merror.error_name = "error" ;
        merror.error_type = "error" ;
        cout << "> Error" << endl ;
        char command_temp[128] ;
        cin.getline( command_temp, 128 ) ;
        return false ;
      } // else if
      else
      {
        mToken_line.push_back( token ) ;
        token = data.Get_Token() ;
        mdivision = false ;
        return true ;
      } // else    
    } // if
    else if ( token.token_name == "+" || token.token_name == "-" || token.token_type == "num"  ||
              token.token_type == "double" ) // [ SIGN ] NUM
    { 
      if ( token.token_name == "+" || token.token_name == "-" ) // [ SIGN ] NUM  
      {
        token.token_type = "num_sign" ;
        mToken_line.push_back( token ) ;
        token = data.Get_Token() ;
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
        {
          return false ;
        } // if
        else
        {
          if ( token.token_type == "num" || token.token_type == "double" )
          {
            if ( mdivision && algorithm.String_to_Double( token.token_name ) == 0 )
            {
              merror.error_name = "error" ;
              merror.error_type = "error" ;
              cout << "> Error" << endl ;
              char command_temp[128] ;
              cin.getline( command_temp, 128 ) ;
              return false ;
            } // if
            else
            {
              mToken_line.push_back( token ) ;
              token = data.Get_Token() ;
              mdivision = false ;
              return true ;
            } // else
          } // if
          else
          {
            Unexpected_token( token.token_name ) ;
            return false ;
          } // else
        } // else
      } // if
      else // NUM
      {
        if ( mdivision && algorithm.String_to_Double( token.token_name ) == 0 )
        {
          merror.error_name = "error" ;
          merror.error_type = "error" ;
          cout << "> Error" << endl ;
          char command_temp[128] ;
          cin.getline( command_temp, 128 ) ;
          return false ;
        } // if
        else
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          mdivision = false ;
          return true ;
        } // else
      } // else
    } // else if
    else if ( token.token_name == "(" ) // '(' <ArithExp> ')'
    {
      mdivision = false ;
      mToken_line.push_back( token ) ;
      token = data.Get_Token() ;
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // 詞法錯誤
      {
        return false ;
      } // if
      else
      {
        if ( ArithExp( token ) )
        {
          if ( token.token_name == ")" )
          {
            mToken_line.push_back( token ) ;
            token = data.Get_Token() ;
            return true ;
          } // if
          else // 語法錯誤 
          {
            Unexpected_token( token.token_name ) ;
            return false ;
          } // else
        } // if
        else
        {
          return false ;
        } // else
      } // else
    } // else if
    else return false ; // 語法錯誤 

    return false ;
  } // Factor()

  bool Compare( double num1, double num2, string bool_sign ) // 比較兩者是否滿足bool_sign條件 
  {
    if ( bool_sign == "=" )
    {
      if ( num1 == num2 ) return true ;
      else return false ;
    } // if 
    else if ( bool_sign == "<>" )
    {
      if ( num1 != num2 ) return true ;
      else return false ;
    } // else if 
    else if ( bool_sign == ">" )
    {
      if ( num1 > num2 ) return true ;
      else return false ;
    } // else if 
    else if ( bool_sign == "<" )
    {
      if ( num1 < num2 ) return true ;
      else return false ;
    } // else if
    else if ( bool_sign == ">=" )
    {
      if ( num1 >= num2 ) return true ;
      else return false ;
    } // else if
    else if ( bool_sign == "<=" )
    {
      if ( num1 <= num2 ) return true ;
      else return false ;
    } // else if

    return false ;   
  } // Compare()

  void Implemete() // 執行並印出答案 
  {
    Algorithm algorithm ;
    Reg reg ;

    if ( mImplement_type == "math" )
    {
      bool has_double = false ;
      for ( int i = 0 ; i < mToken_line.size() ; i++ )
      {
        if ( mToken_line[i].token_type == "double" ) has_double = true ;
        else if ( mToken_line[i].token_type == "id" ) 
        {
          // cout << mToken_line[i].token_name  << endl ;
          // system("pause") ;
          for ( int j = 0 ; j < gReg_line.size() ; j++ )
          {
            // cout << gReg_line[j].token_name << " " << gReg_line[j].token_type ;
            // system("pause") ;
            if ( gReg_line[j].token_name == mToken_line[i].token_name ) {
              if ( gReg_line[j].token_type == "double" ) has_double = true ;
            } // if
          } // for

        } // else if
      } // for

      double ans = algorithm.Calculate( mToken_line ) ;
      ans = round( ans * 1000.0 ) / 1000.0;

      if ( ans == ( int ) ans && has_double )
      {
        string str = Double_to_String( ans ) ;
        str = str + ".000" ;
        cout << "> " << str << endl ;
      } // if
      else
        cout << "> " << ans << endl ;
    } // if
    else if ( mImplement_type == "ident" )
    {
      reg.token_name = mToken_line[0].token_name ;
      mToken_line.erase( mToken_line.begin(), mToken_line.begin()+2 ) ;
      bool has_double = false ;
      for ( int i = 0 ; i < mToken_line.size() ; i++ )
      {
        if ( mToken_line[i].token_type == "double" ) has_double = true ;
        else if ( mToken_line[i].token_type == "id" ) 
        {
          for ( int j = 0 ; j < gReg_line.size() ; j++ )
          {
            if ( gReg_line[j].token_name == mToken_line[i].token_name ) {
              if ( gReg_line[j].token_type == "double" ) has_double = true ;
            } // if
          } // for

        } // else if
      } // for

      reg.value = algorithm.Calculate( mToken_line ) ;
      reg.value = round( reg.value * 1000.0 ) / 1000.0;

      bool undefined = true ;
      int defined_num = 0 ;
      for ( int i = 0 ; i < gReg_line.size() ; i++ )
      {
        if ( gReg_line[i].token_name == reg.token_name ) {
          defined_num = i ;
          undefined = false ;
        } // if
      } // for
 
      if ( has_double ) reg.token_type = "double" ;
      else reg.token_type = "num" ;

      if ( undefined ) gReg_line.push_back( reg ) ;
      else
      {
        gReg_line[defined_num].token_type = reg.token_type ;
        gReg_line[defined_num].value = reg.value ;
      } // else
      
      if ( reg.value == ( int ) reg.value && has_double )
      {
        string str = Double_to_String( reg.value ) ;
        str = str + ".000" ;
        cout << "> " << str << endl ;
      } // if
      else
        cout << "> " << reg.value << endl ;
    } // else if
    else if ( mImplement_type == "boolean" )
    {
      vector<Information> temp1, temp2 ;
      string bool_sign ;
      for ( int i = 0 ; i < mToken_line.size() ; i++ )
      {
        if ( mToken_line[i].token_name == "=" || mToken_line[i].token_name == "<>" || 
             mToken_line[i].token_name == ">" || mToken_line[i].token_name == "<" || 
             mToken_line[i].token_name == ">=" || mToken_line[i].token_name == "<="  )
        {
          bool_sign = mToken_line[i].token_name ;
          for ( int j = 0 ; j < i ; j++ )
          {
            temp1.push_back( mToken_line[j] ) ;
          } // for

          for ( int j = i+1 ; j < mToken_line.size() ; j++ )
          {
            temp2.push_back( mToken_line[j] ) ; 
          } // for
        } // if
      } // for

      double num_a = algorithm.Calculate( temp1 ) ;
      double num_b = algorithm.Calculate( temp2 ) ;

      if ( num_a > num_b && num_a - num_b <= 0.0001 )
      {
        num_a = num_b ;
      } // if
      else if ( num_b > num_a && num_b - num_a <= 0.0001 )
      {
        num_a = num_b ;
      } // else if   
   

      if ( Compare( num_a, num_b, bool_sign ) ) 
        cout << "> true" << endl ;
      else 
        cout << "> false" << endl ;
    } // else if
  } // Implemete()
};

int main() 
{
  int testnum ;
  cin >> testnum ;
  
  cout << "Program starts..." << endl ;
  Data data ;
  Interpret interpret ;
  gReg_line.clear() ;  
 
  while ( !gQuit )
  {
    interpret.Command() ;
  } // while
  
  cout << "> Program exits..." ;


} // main() 
