/* 
 * @copyright (c) 2008, hedspi, hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    //printToken(lookAhead); 
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

/* done, dich khai bao hang so, line 187*/

void compileConstDecls(void) {
  while(lookAhead->tokenType == TK_IDENT)
    compileConstDecl(); 
}

void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON); 
}

/* done, dich khai bao kieu, line 225*/

void compileTypeDecls(void) {
  while(lookAhead->tokenType == TK_IDENT)
    compileTypeDecl(); 
}

void compileTypeDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON); 
}

/*done, khai bao bien, line 225 */

void compileVarDecls(void) {
  while(lookAhead->tokenType == TK_IDENT)
    compileVarDecl();
}

void compileVarDecl(void) {
  eat(TK_IDENT);
  while(lookAhead->tokenType == SB_COMMA){
    eat(SB_COMMA);
    eat(TK_IDENT);
  }
  eat(SB_COLON); 
  compileType();
  eat(SB_SEMICOLON); 
}

/*done, dich ham con, thu tuc con */

void compileSubDecls(void) {
  assert("Parsing subtoutines ...."); 
  if(lookAhead->tokenType == KW_FUNCTION){
    compileFuncDecl();
    compileSubDecls();
  } else if(lookAhead->tokenType == KW_PROCEDURE){
    compileProcDecl();
    compileSubDecls();
  }
  assert("Subtoutines parsed ....");
}

/*done, dich ham con, line 272 */

void compileFuncDecl(void) {
  assert("Parsing a function ...."); 
  eat(KW_FUNCTION);
  eat(TK_IDENT); 
  //compileParams();
  if(lookAhead->tokenType != SB_LPAR){
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
  }
  
  while(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    if(lookAhead->tokenType != SB_RPAR){
      compileParam();
      compileParams2();
    } 
    eat(SB_RPAR);
  }
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

/*done, dich thu tuc con, line 272 */

void compileProcDecl(void) {
  assert("Parsing a procedure ...."); 
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  //compileParams();
  if(lookAhead->tokenType != SB_LPAR){
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
  }
  
  while(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    if(lookAhead->tokenType != SB_RPAR){
      compileParam();
      compileParams2();
    } 
    eat(SB_RPAR);
  }
  
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

/* dich bien hang */

void compileUnsignedConstant(void) {
  switch(lookAhead->tokenType){
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    error(ERR_INVALIDCHARCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

/* dich hang so:
   + neu la so thi + / - voi so duong
   + neu la ki tu thi an luon
*/

void compileConstant(void) {
  switch(lookAhead->tokenType){
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    compileConstant2();
    break;
  }
}

void compileConstant2(void) {
  switch(lookAhead->tokenType){
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

/* dich khai bao kieu du lieu moi:
   + so 
   + ki tu 
   + mang so, mang ki tu
*/

void compileType(void) {
  switch(lookAhead->tokenType){
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break; 
  default:
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileBasicType(void) {
  switch(lookAhead->tokenType){
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  default:
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

/*dich thong so cho ham
  + vi du : F(n:integer)
  + thi params la (n:integer)
*/

void compileParams(void) {
  
  while(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
      compileParam();
      compileParams2();      
      eat(SB_RPAR);
  }
}

void compileParams2(void) {
  while(lookAhead->tokenType == SB_SEMICOLON){
    eat(SB_SEMICOLON);
    compileParam(); 
  }
}

void compileParam(void) {
  if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  } else if(lookAhead->tokenType == KW_VAR){
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
  } else {
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
  }
}

/* dich cac dang cau lenh */

void compileStatements(void) {
  compileStatement();
  compileStatements2();  
}

void compileStatements2(void) {
  while(lookAhead->tokenType == SB_SEMICOLON){
    eat(SB_SEMICOLON);
    compileStatement();
  }
}

void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
  case KW_DO:
    compileDoSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE: 
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

/* done, dich cau lenh gan, line  */

void compileAssignSt(void) {
  assert("Parsing an assign statement ...."); 
  eat(TK_IDENT);
  compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
   
  if(lookAhead->tokenType == SB_EQ || lookAhead->tokenType == SB_NEQ ||
     lookAhead->tokenType == SB_LT || lookAhead->tokenType == SB_LE ||
     lookAhead->tokenType == SB_GT || lookAhead->tokenType == SB_GE) {

    switch(lookAhead->tokenType){
    case SB_EQ:
      eat(SB_EQ);
      compileExpression();
      break;
    case SB_NEQ:
      eat(SB_NEQ);
      compileExpression();
      break;
    case SB_LT:
      eat(SB_LT);
      compileExpression();
      break;
    case SB_LE:
      eat(SB_LE);
      compileExpression();
      break;
    case SB_GT:
      eat(SB_GT);
      compileExpression();
      break;
    case SB_GE:
      eat(SB_GE);
      compileExpression();
      break;
    } // het switch

    eat(SB_QUERY);
    compileExpression();
    eat(SB_COLON);
    compileExpression();
    
  }
  
  assert("Assign statement parsed ....");
}

/* done, dich cau lenh goi ham, line  */

void compileCallSt(void) {
  assert("Parsing a call statement ...."); 
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

/*done, dich nhom lenh, line */

void compileGroupSt(void) {
  assert("Parsing a group statement ...."); 
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

/*done, dich cau lenh if*/

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

/*done, dich cau lenh while */

void compileWhileSt(void) {
  assert("Parsing a while statement ...."); 
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement parsed ....");
}

/*
  done, dich cau lenh do ...  while 
*/

void compileDoSt(void) {
  assert("Parsing a do while statement ...."); 
  eat(KW_DO);
  compileStatement();  
  eat(KW_WHILE);
  compileCondition();
  assert("Do While statement parsed ....");
}



/*
  done, dich cau lenh for
  chap nhan dang lenh giong C:
  for(i; i < 1; i = i + 1)
*/

void compileForSt(void) {
  assert("Parsing a for statement ...."); 
  eat(KW_FOR);
  if(lookAhead->tokenType == TK_IDENT){
    eat(TK_IDENT);
    eat(SB_ASSIGN);
    compileExpression();
    eat(KW_TO);
    compileExpression();
    eat(KW_DO);
    compileStatement(); 
  } else if(lookAhead->tokenType == SB_LPAR){
    eat(SB_LPAR);
    compileExpression();
    if(lookAhead->tokenType == SB_EQ){
      eat(SB_EQ);
      compileExpression();  
    } 
    eat(SB_SEMICOLON);
    compileCondition();
    eat(SB_SEMICOLON);
    compileExpression();
    if(lookAhead->tokenType == SB_EQ){
      eat(SB_EQ);
      compileExpression();   
    } 
    eat(SB_RPAR);
    compileStatement(); 
  }
  
  
  assert("For statement parsed ....");
}

/*chi so trong ham*/

void compileArguments(void) {
  if(lookAhead->tokenType != SB_LPAR){
    error(ERR_INVALIDPARAM, lookAhead->lineNo, lookAhead->colNo);
  }
  
  switch(lookAhead->tokenType){
  case SB_LPAR:
    eat(SB_LPAR);
    if(lookAhead->tokenType != SB_RPAR){
      compileExpression();
	compileArguments2();
      } 
      eat(SB_RPAR); 
      break;
      // Follow (term2)
    case SB_TIMES:
    case SB_SLASH:
    case SB_PLUS:
    case SB_MINUS:
      // Follow expression (for)
    case KW_TO:
    case KW_DO:
    case SB_RPAR:
      // first args2
    case SB_COMMA:
      // Follow expression
    case SB_EQ:
    case SB_NEQ:
    case SB_LE:
    case SB_LT:
    case SB_GE:
    case SB_GT:
      // follow expression (indexes)
    case SB_RSEL:
      // First statement
    case SB_SEMICOLON: 
    case KW_END:
    case KW_ELSE:
      // Follow condition (if)
    case KW_THEN:
      //case SB_QUERY:
      break;
    default:
      error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    
    }
  }

void compileArguments2(void) {
  while(lookAhead->tokenType == SB_COMMA){
    eat(SB_COMMA);
    compileExpression(); 
  }
}

/*bieu thuc dieu kien */

void compileCondition(void) {
  compileExpression();
  compileCondition2();
}

void compileCondition2(void) {
  switch(lookAhead->tokenType){
  case SB_EQ:
    eat(SB_EQ);
    compileExpression();
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    compileExpression();
    break;
  case SB_LT:
    eat(SB_LT);
    compileExpression();
    break;
  case SB_LE:
    eat(SB_LE);
    compileExpression();
    break;
  case SB_GT:
    eat(SB_GT);
    compileExpression();
    break;
  case SB_GE:
    eat(SB_GE);
    compileExpression();
    break;
  default:
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

/* dich bieu thuc 
   = + bieu thuc
   = - bieu thuc
*/

void compileExpression(void) {
  assert("Parsing an expression"); 
  switch(lookAhead->tokenType){
  case SB_PLUS:
    eat(SB_PLUS);
    compileExpression2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileExpression2();
    break;
  default:
    compileExpression2();
    break;
  }
  assert("Expression parsed");
}

void compileExpression2(void) {
  compileTerm();
  compileExpression3();
}


void compileExpression3(void) {
  switch(lookAhead->tokenType){
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
    break;
    //TODO
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
  case KW_WHILE:
    // cho bai lam them
  case SB_ASSIGN:
  case SB_QUERY:
  case SB_COLON:
    break;
  default:
    error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
 
  }
}

/*bieu thuc chua nhan va chia */

void compileTerm(void) {
  compileFactor();
  compileTerm2();
}

void compileTerm2(void) {
  switch(lookAhead->tokenType){
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;
    //TODO
  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case SB_RPAR:
  case SB_COMMA:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_LT:
  case SB_GE:
  case SB_GT:
  case SB_RSEL:
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
  case KW_WHILE:
    // cho phan lam them
  case SB_ASSIGN:
  case SB_QUERY:
  case SB_COLON:
    break;
  default:
    error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);

  }
}

/* bieu thuc chua nhan va chia */

void compileFactor(void) {
  switch(lookAhead->tokenType){
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_CHAR: 
    eat(TK_CHAR);
    break;
  case TK_IDENT: 
    eat(TK_IDENT);
    switch(lookAhead->tokenType){
    case SB_LSEL:
      compileIndexes();
	break;
      case SB_LPAR:
	compileArguments();
	break;
      default:
	break;
      }
      break;
    
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();
      eat(SB_RPAR);
      break; 
    default: 
      error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    }
  }

  /* chi so cua mang */

  void compileIndexes(void) {
    if(lookAhead->tokenType == SB_LSEL){
      eat(SB_LSEL);
      compileExpression();
    
      while(lookAhead->tokenType == SB_COMMA){
	eat(SB_COMMA);
	compileExpression();
      }
    
      eat(SB_RSEL);
      compileIndexes();
    }
  }

  int compile(char *fileName) {
    if (openInputStream(fileName) == IO_ERROR)
      return IO_ERROR;

    currentToken = NULL;
    lookAhead = getValidToken();

    compileProgram();

    free(currentToken);
    free(lookAhead);
    closeInputStream();
    return IO_SUCCESS;

  }
