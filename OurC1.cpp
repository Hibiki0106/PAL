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

struct Information // �C��token����� 
{
  string token_name ;
  string token_type ; // id num double sign num_sign
};

struct Error // �O�����~����T 
{
  string error_name ;  
  string error_type ; // Unrecognized Unexpected Undefined
};

struct Reg // �Q�w�q��token��� ex: salary := 3000 
{
  string token_name ;
  string token_type ;
  double value ;
};

vector<Reg> gReg_line ;           // �s�w�q�Lident��T��vector
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

  double String_to_Double( string str ) // �r���W���N�� 
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

  void Dealwith_num_sign( vector<Information> &list ) // [sign] num �����p 
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

  int Priority( string op ) // �B�⤸���u���ŧO 
  {
    if ( op == "+" || op == "-"  ) return 1 ;
    else if ( op == "*" || op == "/" ) return 2 ;
    else return 0 ;
  } // Priority()

  vector<Information> In_To_Post( vector<Information> infix ) // ���� �� ��� 
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

        stack[++top] = infix[i]; // �s�J���| 
      } // else if
      else if ( infix[i].token_name == ")" )
      {
        while ( stack[top].token_name != "(" ) { // �J ) ��X�� ( 
          postfix.push_back( stack[top--] ) ;
        } // while 

        top--; // ����X (
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

  double Ident_value( Information token ) // ���o��id�N��num 
  {
    double re = -1 ;
    
    for ( int i = 0 ; i < gReg_line.size() ; i++ )
    {
      if ( gReg_line[i].token_name == token.token_name )
        re = gReg_line[i].value ;
    } // for

    return re ;
  } // Ident_value()

  double Calculate( vector<Information> infix ) // �p��Ӻ⦡������ 
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

  void Remove_White_Space() // ���L�Ҧ��Ů檽��Ū��D�Ů�r������
  {
    while ( cin.peek() == ' ' || cin.peek() == '\t' )
    {
      cin.get() ;
    } // while
  } // Remove_White_Space()

  void Skip_White_Space( char &ch ) // ���L�Ҧ��Ů檽��Ū��D�Ů�r������A�æ^�ǸӦr�� 
  {
    while ( ch == ' ' || ch == '\t' ) 
    {
      ch = cin.get() ;
    } // while 
  } // Skip_White_Space()
  
  Information Get_Token() { // �}�lŪ�J�r������token�æ^�� 
            // �p�G�O�r���}�Y �᭱�i�H�O�r���B�Ʀr�B���u
            // �p�G�O�Ʀr�}�Y ���᭱�i�H���Ʀr�M. .�}�Y+�Ʀr�]�����Ʀr 
            // �S��Ÿ� >= <= ���� 
            // ��l���h�����}�Y ������@token�B�z 
            // �C���X�@��token �N�iinterpret���q 
            
    char temp ;                  // �ȦsŪ�J���r��
    char peek ;                  // �Ȧs�U�ӭnŪ�J���r�� �s��P�_�ɻݭn�Ψ� ex: ����     
    bool decimal_point = false ; // �Ʀr�O�_�w�g��p���I �p���I�̦h�@�� ex: 1.2.3 (error)
    bool frist_isdot = false ;   // �Ĥ@�Ӧr����"." �᭱���Ʀr���� 0.xx ex: .9 == 0.9  
    bool frist_char = true ;     // �O�_���Ĥ@�Ӧr��
    bool finish_cut = false ;    // �O�_��������token
    Information token ;     
    
    while ( !finish_cut ) // ���X�@��token�N���� 
    {
      temp = cin.get() ;
      if ( frist_char ) Skip_White_Space( temp ) ;
          
      if ( frist_char ) // �|���M�w�Ĥ@�Ӧr�������� 
      {
        if ( isalpha( temp ) ) // ���r���r�� 
        {
          token.token_name = temp ;
          token.token_type = "id" ;
          frist_char = false ; 
        } // if
        else if ( isdigit( temp ) ) // ���r���Ʀr 
        {
          decimal_point = false ;
          token.token_name = temp ;
          token.token_type = "num" ;
          frist_char = false ;
        } // else if
        else // ���r���Ÿ� 
        {
          if ( temp == '/' ) // �i�ର���� peek�U�@�Ӧp�G�]�O'/'�N���88
          {
            peek = cin.peek() ;
            if ( peek == '/' ) // �T�w������ 88 
            {
              char command_temp[128] ;
              cin.getline( command_temp, 128 ) ;
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else
          } // if
          else if ( temp == '.' ) // �i�ର�p�� 
          {
            Remove_White_Space() ;
            peek = cin.peek() ;
            if ( isdigit( peek ) ) // �T�w���p�� 
            {
              token.token_name = ".";
              token.token_type = "double" ;
              frist_isdot = true ;
              frist_char = false ; 
            } // if
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == ':' ) // := �~�~���� 
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
            else // �@��Ÿ� 
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
            else // �@��Ÿ� 
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
            else // �@��Ÿ� 
            {
              token.token_name = temp ;
              token.token_type = "sign" ;
              finish_cut = true ;
            } // else 
          } // else if
          else if ( temp == '\n' )
          {
            
          } // else if
          else // �@��Ÿ� 
          {
            token.token_name = temp ;
            token.token_type = "sign" ;
            finish_cut = true ; 
          } // else
        } // else
      } // if
      else // �w�M�wtoken�Ĥ@�Ӧr�������� 
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
        else if ( token.token_type == "id" ) // token���r�� 
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
        else if ( token.token_type == "num" || token.token_type == "double" ) // token���Ʀr
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
  vector<Information> mToken_line ; // �stoken���������vector ( �qdata�Ө�      
  Error merror ;                    // �O�����~����T ( ���~��token & ���~�����O 
  string mImplement_type ;          // �n����������� ex: 1+2 -> math 
                                    // 1>0 -> boolean, a := 1 -> ident 
  bool mdivision ;                  // �W�@�ӹB�⤸�O���k�@���� /0�����p 

  Interpret()
  {
    gReg_line.clear() ;
  } // Interpret()

  bool Unrecognized_token( string s ) // ���k���~ ex : 2 + $ - 5
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

  void Unexpected_token( string str ) // �y�k���~ ex : 2 + * - 5 
  {                                   // Unexpected token : '*'
    if ( merror.error_name == "empty" )
    {
      char command_temp[128] ;
      cin.getline( command_temp, 128 ) ;
      merror.error_name = str ;
      merror.error_type = "Unexpected" ; 
    } // if
  } // Unexpected_token()

  bool Undefined_identifier( string str ) // �y�q���~ ex : bcd * 2 (�|���ŧibcd) 
  {                                       // Undefined identifier : 'bcd'
                                          // ��k�L���~

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

  void Print_error() // �L�X���~�T�� 
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
    if ( token.token_name == "quit" ) // ���q���u 
    {
      gQuit = true ;
    } // if
    else
    {
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
          {
            return false ;
          } // if
          else
          {
            if ( ArithExp( token ) )
            {
              if ( token.token_name == ";" ) return true ;
              else // ��k���~ 
              {
                Unexpected_token( token.token_name ) ; 
                return false ; 
              } // else 
            } // if
            else
            {
              Unexpected_token( token.token_name ) ; 
              return false ; // �y�k���~
            } // else
          } // else
        } // if
        else if ( IDlessArithExpOrBexp( token ) )  // <IDlessArithExpOrBexp> ';'
        {
          if ( token.token_name == ";" ) 
          {
            return true ;
          } // if
          else // ��k���~ 
          {
            Unexpected_token( token.token_name ) ; 
            return false ; 
          } // else
        } // else if
      } // else
    } // if
    else return false ; // ���O ident 

    return false ;
  } // Ident_command()

  bool Not_Ident_command( Information token ) // <NOT_IDStartArithExpOrBexp> ';'
  {  
    Data data ; 

    if ( NOT_ID_StartArithExpOrBexp( token ) )
    {
      if ( token.token_name == ";" ) return true ;
      else // ��k���~ 
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
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
            else // �y�k���~ 
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
        {   
          return false ;
        } // if       
        else
        {
          if ( ArithExp( token ) )
          {
            return true ;
          } // if
          else // �y�k���~ 
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
    else // �y�k���~ 
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "+" || token.token_name == "-" )    
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
    else // �y�k���~ 
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "*" || token.token_name == "/" )    
        {
          if ( token.token_name == "/" ) mdivision = true ;
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
    else // �y�k���~ 
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
          else // �y�k���~ 
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
      return false ; // �y�k���~
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "+" || token.token_name == "-" )    
        {
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
        {
          end_recursive = true ;
          return false ;
        } // if
        else if ( token.token_name == "*" || token.token_name == "/" )    
        {
          if ( token.token_name == "/" ) mdivision = true ;
          mToken_line.push_back( token ) ;
          token = data.Get_Token() ;
          if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
        if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
      if ( token.token_type == "sign" && Unrecognized_token( token.token_name ) ) // ���k���~
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
          else // �y�k���~ 
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
    else return false ; // �y�k���~ 

    return false ;
  } // Factor()

  bool Compare( double num1, double num2, string bool_sign ) // �����̬O�_����bool_sign���� 
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

  void Implemete() // ����æL�X���� 
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
