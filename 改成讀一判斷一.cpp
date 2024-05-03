#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

struct tokenString {
  string token;
  string type;
};

class SyntacticalError {
  public :
    string errorString;
    SyntacticalError ( string s ) {
      string temp;
      getline(cin, temp);
      errorString = s;
      cout << "SyntacticalError : " << errorString << endl;
    }


};

class data {
  public :
    char ch, peekch;
    tokenString in;
    bool quit = false, alreadyfloat = false;
    string fw;
    vector<tokenString> inputData;

    void skipwhitespace ( char & ch ) {
      while ( ch == ' ' || ch == '\t' || ch == '\n' ) {
        ch = cin.get() ;
      }
    }

    bool iswhitespace ( char ch ) {
      if ( ch == ' ' || ch == '\t' || ch == '\n' ) {
        return true;
      } else return false;
    }

    bool iscomment ( char ch ) {
      char pp;
      string comment;
      pp = cin.peek();
      //cout << pp <<endl;
      while ( iswhitespace( pp ) ) {
        pp = cin.get();
        pp = cin.peek();
      } // while ()


      if ( pp == '/' ) {
        getline(cin , comment );
        return true;
      } // if ()
      else return false;
    }

    bool issign ( char ch ) {
      if ( ch == '+' || ch == '-' || ch == '*' || ch == ':' || ch == ';' ||
           ch == '=' || ch == '>' || ch == '<' || ch == '(' || ch == ')' ||
           ch == '[' || ch == ']' ) {
        return true;
      } else if ( ch == '/' ) {
        if ( iscomment( ch ) ) return false ;
        else return true;
      } else return false;
    }

    bool isdigit ( char ch ) {
      if ( ch == '.' ) return true;
      if ( ch >= '0' && ch <= '9' ) return true;

      return false;
    }


    void signTostring ( char ch, tokenString &in ) {
      in.token = ch;
      if ( ch == '+' ) in.type = "add";
      else if ( ch == '-' ) in.type = "sub";
      else if ( ch == '*' ) in.type = "mul";
      else if ( ch == '/' ) in.type = "div";
      else if ( ch == ':' ) in.type = "col";
      else if ( ch == ';' ) in.type = "sem";
      else if ( ch == '=' ) in.type = "eq";
      else if ( ch == '>' ) in.type = "big";
      else if ( ch == '<' ) in.type = "les";

    }


    void clearstruct () {
      in.token = "\0";
      in.type = "\0";
    }

    void dealfloat( char ch, tokenString & in, bool & hadDot ) { // 這裡的ch 是peek得到的
      for ( int a = 0 ; a < in.token.size() ; a++ ) {
        if ( in.token[a] == '.' ) {
          hadDot = true;
        } // if ()
      }

      if ( !hadDot ) {
        cout << "有來";
        ch = cin.get();
        in.token = in.token + ch;
      }

    } // 有個小問題是 型別為id時 能不能接'.'


    /*
    void dealtype( tokenString & in ) {
      if ( in.token == "int" ) in.type == "INT";
    }
    */


    void user_input() { // ( definition | statement ) { definition | statement }
      gettoken();
      cout << "user_input gettoken : " << in.token << ".." << in.type << endl;
      if ( in.token == "void" || in.token == "int" || in.token == "char" ||
           in.token == "float" || in.token == "string" || in.token == "bool" ) {

        definition();
      } // if definition路線
      else {
        ;
      } // else statement路線

    } // user_input()

    bool definition() {
      if ( in.token == "void" ) {
      	inputData.push_back( in );
      	gettoken();
      	
      	if ( in.type == "id" ) {
          inputData.push_back( in );
          gettoken();

          function_definition_without_ID();

        } // if
        else {
          throw SyntacticalError( in.token );
        } // else
      	
	  } // if VOID Identifier function_definition_without_ID
      else {

        type_specifier();

        gettoken();
        cout << "definition gettoken : " << in.token << ".." << in.type << endl;
        if ( in.type == "id" ) {
          inputData.push_back( in );
          gettoken();

          function_definition_or_declarators();

        } // if
        else {
          //cout << "要來這裡";
          throw SyntacticalError( in.token );
        } // else
      } // else   type_specifier Identifier function_definition_or_declarators

    }


    void type_specifier() {
      if ( in.token == "int" || in.token == "char" || in.token == "float" || in.token == "string" || in.token == "bool" ) {
        inputData.push_back( in );
      } // if
      else {
        throw SyntacticalError( in.token ) ;
      } // else
    } // type_specifier()


    void function_definition_or_declarators() {
      if ( in.token == "(" ) {
        function_definition_without_ID() ;
      } // if     走 function_definition_without_ID 路線
      else {
        rest_of_declarators();
      } // else
    } // function_definition_or_declarators()


    void function_definition_without_ID() {
      // '(' [ VOID | formal_parameter_list ] ')' compound_statement
      if ( in.token == "(" ) {
        inputData.push_back( in );
        gettoken();

        if ( in.token != ")" ) {

          if ( in.token == "void" ) {
            inputData.push_back( in );
            gettoken();
          } // if
          else {

            formal_parameter_list();

          } //else

        } // if 代表可能是 [ VOID | formal_parameter_list ] 或文法錯誤

        if ( in.token == ")" ) {
          gettoken();
          inputData.push_back( in );
        } // if
        else {
          throw SyntacticalError( in.token ) ;
        } // else

      } // if
      else {
      	throw SyntacticalError( in.token ) ;
	  } // else
      
      compound_statement(); // 往前13行有gettoken過一次了 
      
    } // function_definition_without_ID()



    void rest_of_declarators() {
      // [ '[' Constant ']' ]
      // { ',' Identifier [ '[' Constant ']' ] } ';'

      if ( in.token == "[" ) {
        inputData.push_back( in );
        gettoken();
        if ( in.type == "num" ) {
          inputData.push_back( in );
          gettoken();
        } // if
        else {
          throw SyntacticalError( in.token ) ;
        } // else

        if ( in.token == "]" ) {
          inputData.push_back( in );
          gettoken();
        } // if
        else {
          throw SyntacticalError( in.token ) ;
        } // else

      } // if

      while ( in.token == "," ) {
        inputData.push_back( in );
        gettoken();
        if ( in.type == "id" ) {
          inputData.push_back( in );
          gettoken();
        } // if
        else {
          throw SyntacticalError( in.token ) ;
        } // else

        if ( in.token == "[" ) {
          inputData.push_back( in );
          gettoken();
          if ( in.type == "num" ) {
            inputData.push_back( in );
            gettoken();
          } // if
          else {
            throw SyntacticalError( in.token ) ;
          } // else

          if ( in.token == "]" ) {
            inputData.push_back( in );
            gettoken();
          } // if
          else {
            throw SyntacticalError( in.token ) ;
          } // else

        } // if

      } // while

      if ( in.token == ";" ){
      	inputData.push_back( in );
      	gettoken();
	  } // if
	  else {
	  	throw SyntacticalError( in.token ) ;
	  } // else
    } // rest_of_declarators()

    void formal_parameter_list() {

      type_specifier();

      gettoken();

      if ( in.token == "&" ) {
        inputData.push_back( in );
        gettoken();
      } // if [ '&' ] 可以有& 也可以不用有

      if ( in.type == "id" ) {
        inputData.push_back( in );
        gettoken();
      } // if
      else {
        throw SyntacticalError( in.token ) ;
      } // else

      if ( in.token == "[" ) {
        inputData.push_back( in );
        gettoken();

        if ( in.type == "num" ) {
          inputData.push_back( in );
          gettoken();

          if ( in.type == "]" ) {
            inputData.push_back( in );
            gettoken();
          } // if
          else {
            throw SyntacticalError( in.token ) ;
          } // else

        } // if
        else {
          throw SyntacticalError( in.token ) ;
        } // else

      } // if 有設定大小 [100] 之類的

      if ( in.token == "," ) {
        inputData.push_back( in );
        gettoken();
        formal_parameter_list();
      } // if

      // 可是這樣出function 如果後面沒接 "," 代表我提前讀入下一字元
      // 可能要修改成用peek
    } // formal_parameter_list

    void compound_statement() {
    	
    	
      
	} // compound_statement
	
	
	
    void get_remain_letter ( tokenString & in, char ch ) {
      char temp_ch;
      in.token = ch;
      temp_ch = cin.peek();
      bool interrupt = false;
      while ( !iswhitespace( temp_ch ) && temp_ch != ';' && !interrupt ) {
        if ( isalpha( temp_ch ) || isdigit( temp_ch ) || temp_ch == '_' ) {
          ch = cin.get();
          in.token = in.token + ch;

        } // if ()
        else interrupt = true;

        //cout << "omo";
        temp_ch = cin.peek();
      } // while ()
    }



    void get_remain_num ( tokenString & in, char ch ) { // ch 是 . 和 數字都會進來 目的是讀完剩下的
      char temp_ch;
      bool hadDot = false;
      in.token = ch;
      temp_ch = cin.peek();
      while ( isdigit( temp_ch ) && !hadDot ) { // 0.12.3
        if ( temp_ch == '.' ) dealfloat( temp_ch, in, hadDot ) ;
        else {
          ch = cin.get();
          in.token = in.token + ch;
        } // else

        temp_ch = cin.peek();
      } // while

    }





    void gettoken () {
      ch = cin.get() ;
      skipwhitespace( ch );
      //cout << ch << endl;
      if ( isalpha( ch ) || ch == '_' ) {
        /*
        if ( in.type == "num" ) {
          inputData.push_back( in );
          clearstruct( in );
        } // if ()
        */


        get_remain_letter( in, ch );
        in.type = "id";
        //dealtype( in );

        //in.token = in.token + ch;

      } // if ( isalpha(ch) )
      else if ( isdigit( ch ) || ch == '.' ) {
        in.type = "num";
        cout << " go digital ";
        get_remain_num( in, ch );
        /*
        if ( ch == '.' ) dealfloat( ch, in, inputData );
        else if ( in.type == "id" ) in.token = in.token + ch;
        else {
         in.token = in.token + ch;
         in.type = "num";
        }
        */

      } // else if
      else if ( issign( ch ) ) {
        /*
        	if ( in.token != "\0" ) inputData.push_back( in ) ;
          in.token = ch;
          in.type = "sign";
          inputData.push_back( in ) ;
          clearstruct( in );
          */
        in.token = ch;
        in.type = "sign";

      } else if ( ch == '"' ) {
        in.token = ch;
        ch = cin.get();
        while ( ch != '"' ) {
          in.token = in.token + ch;
          ch = cin.get();
        } // while

        in.token = in.token + ch;
        in.type = "id";
      } else if ( ch == '/' ) {
        cout << "omo";
        gettoken ();
      }

      //if ( in.token == "\0" )
    } // gettoken

    void peekChar() {
      peekch = cin.peek();
      skipwhitespace ( peekch );
    } //
};

int main() {

  data data;

  while ( !data.quit ) {

    //getInput
    try {
      data.user_input();

    } catch ( SyntacticalError syntax ) {
      cout << "有catch到喔OUO"<<endl;
    }


    //cout << "token : " << data.in.token << endl << "type : " << data.in.type << endl;


    /*

    if ( !isalpha( ch ) &&  !isdigit( ch ) && !issign(ch) && !iswhitespace(ch) && ch != '.' && ch != '_' && ch != '.' && ch != '/' ) {
      cout << "Unrecognized" << endl;
      getline( cin, fw );
      clearstruct( in );
      inputData.clear();
    }


    if ( ch == ';' ) calculate(inputData);
    */

    if ( data.in.token == "done()" && data.inputData.size() == 0 ) {
      cout << "偵測到第一個是quit 所以跳掉" << endl;
      data.quit = true;
    }

    data.clearstruct();

  }

}

// 試著依樣判斷; 但會標記enter 遇到問題只刪到該行的enter
