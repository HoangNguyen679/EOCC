/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // done
  Type* tmp = (Type*)malloc(sizeof(Type));
  tmp->typeClass = type->typeClass; 
  tmp->arraySize = type->arraySize;
  tmp->elementType = type->elementType;
  return tmp;
}

int compareType(Type* type1, Type* type2) {
  // done
  if(type1->typeClass == type2->typeClass){
    if(type1->typeClass == TP_ARRAY){
      if(!(type1->arraySize == type2->arraySize
	   && compareType(type1->elementType,type2->elementType)))
	return 0;
    }
    return 1;
  }
  return 0;
}

void freeType(Type* type) {
  // done
  if(type->elementType != NULL)
    free(type->elementType);
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // done
  ConstantValue *v = (ConstantValue *)malloc(sizeof(ConstantValue));
  v->type = TP_INT;
  v->intValue = i;
  return v;
}

ConstantValue* makeCharConstant(char ch) {
  // done
  ConstantValue *v = (ConstantValue *)malloc(sizeof(ConstantValue));
  v->type = TP_CHAR;
  v->charValue = ch;
  return v;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // done
  ConstantValue *tmp = (ConstantValue *)malloc(sizeof(ConstantValue));
  tmp->type = v->type;
  if(tmp->type == TP_INT)
    tmp->intValue = v->intValue;
  else
    tmp->charValue = v->charValue;
  return tmp;
}

/******************* Object utilities ******************************/

Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* createConstantObject(char *name) {
  // done
  Object* constant = (Object*)malloc(sizeof(Object));
  strcpy(constant->name, name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  constant->constAttrs->value = NULL;
  return constant;
}

Object* createTypeObject(char *name) {
  // done
  Object* type = (Object*)malloc(sizeof(Object));
  strcpy(type->name, name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  type->typeAttrs->actualType = NULL;
  return type;
}

Object* createVariableObject(char *name) {
  // done
  Object* variable = (Object*)malloc(sizeof(Object));
  strcpy(variable->name, name);
  variable->kind = OBJ_VARIABLE;
  variable->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  variable->varAttrs->type = NULL; 
  variable->varAttrs->scope = symtab->currentScope;
  return variable;
}

Object* createFunctionObject(char *name) {
  // done
  Object* function = (Object*)malloc(sizeof(Object));
  strcpy(function->name, name);
  function->kind = OBJ_FUNCTION;
  function->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  function->funcAttrs->paramList = NULL;
  function->funcAttrs->returnType = NULL;
  function->funcAttrs->scope = createScope(function, symtab->currentScope);
  return function;
}

Object* createProcedureObject(char *name) {
  // done
  Object* procedure = (Object*)malloc(sizeof(Object));
  strcpy(procedure->name, name);
  procedure->kind = OBJ_PROCEDURE;
  procedure->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  procedure->procAttrs->paramList = NULL;
  procedure->procAttrs->scope = createScope(procedure, symtab->currentScope);
  return procedure;
}

Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // done
  Object* parameter = (Object*)malloc(sizeof(Object));
  strcpy(parameter->name, name);
  parameter->kind = OBJ_PARAMETER;
  parameter->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  parameter->paramAttrs->kind = kind;
  parameter->paramAttrs->type = NULL;
  parameter->paramAttrs->function = owner;
  return parameter;
}

void freeObject(Object* obj) {
  // done
  switch (obj->kind) {
  case OBJ_CONSTANT:
    free(obj->constAttrs->value);
    free(obj->constAttrs);
    break;
  case OBJ_TYPE:
    free(obj->typeAttrs->actualType);
    free(obj->typeAttrs);
    break;
  case OBJ_VARIABLE:
    free(obj->varAttrs->type);
    free(obj->varAttrs);
    break;
  case OBJ_FUNCTION:
    freeReferenceList(obj->funcAttrs->paramList);
    freeType(obj->funcAttrs->returnType);
    freeScope(obj->funcAttrs->scope);
    free(obj->funcAttrs);
    break;
  case OBJ_PROCEDURE:
    freeReferenceList(obj->procAttrs->paramList);
    freeScope(obj->procAttrs->scope);
    free(obj->procAttrs);
    break;
  case OBJ_PROGRAM:
    freeScope(obj->progAttrs->scope);
    free(obj->progAttrs);
    break;
  case OBJ_PARAMETER:
    freeType(obj->paramAttrs->type);
    free(obj->paramAttrs);
  }
  free(obj);
}

void freeScope(Scope* scope) {
  // done
  freeObjectList(scope->objList);
  free(scope);
}

void freeObjectList(ObjectNode *objList) {
  // done
  ObjectNode* list = objList;
  while (list != NULL) {
    ObjectNode* node = list;
    list = list->next;
    freeObject(node->object);
    free(node);
  }
}

void freeReferenceList(ObjectNode *objList) {
  // done
  ObjectNode* list = objList;
  while (list != NULL) {
    ObjectNode* node = list;
    list = list->next;
    free(node);
  }
}

void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
      n->next = node;
    }
  }

Object* findObject(ObjectNode *objList, char *name) {
  // done
  while (objList != NULL) {
    if (strcmp(objList->object->name, name) == 0) 
      return objList->object;
    else objList = objList->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
	break;
      case OBJ_PROCEDURE:
	addObject(&(owner->procAttrs->paramList), obj);
	break;
      default:
	break;
      }
    }
 
    addObject(&(symtab->currentScope->objList), obj);
  }


