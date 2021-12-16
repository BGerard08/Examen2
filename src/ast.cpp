#include "ast.h"
#include <iostream>
#include <sstream>
#include <set>
#include "asm.h"

const char * floatTemps[] = {"$f0",
                            "$f1",
                            "$f2",
                            "$f3",
                            "$f4",
                            "$f5",
                            "$f6",
                            "$f7",
                            "$f8",
                            "$f9",
                            "$f10",
                            "$f11",
                            "$f12",
                            "$f13",
                            "$f14",
                            "$f15",
                            "$f16",
                            "$f17",
                            "$f18",
                            "$f19",
                            "$f20",
                            "$f21",
                            "$f22",
                            "$f23",
                            "$f24",
                            "$f25",
                            "$f26",
                            "$f27",
                            "$f28",
                            "$f29",
                            "$f30",
                            "$f31"
                        };

#define FLOAT_TEMP_COUNT 32
set<string> intTempMap;
set<string> floatTempMap;

extern Asm assemblyFile;

int globalStackPointer = 0;

string getFloatTemp(){
    for (int i = 0; i < FLOAT_TEMP_COUNT; i++)
    {
        if(floatTempMap.find(floatTemps[i]) == floatTempMap.end()){
            floatTempMap.insert(floatTemps[i]);
            return string(floatTemps[i]);
        }
    }
    cout<<"No more float registers!"<<endl;
    return "";
}

void releaseFloatTemp(string temp){
    floatTempMap.erase(temp);
}

void FloatExpr::genCode(Code &code){
    string floatTemp = getFloatTemp();
    code.place = floatTemp;
    stringstream ss;
    ss << "li.s " << floatTemp <<", " <<endl;
    code.code = ss.str();
}

void SubExpr::genCode(Code &code){
}

void DivExpr::genCode(Code &code){
}

void IdExpr::genCode(Code &code){
}

string ExprStatement::genCode(){
    return "Expr statement code generation\n";
}

string IfStatement::genCode(){
    Code exprCode;
    this->conditionalExpr->genCode(exprCode);
    stringstream code;
    code << exprCode.code << endl;
    code << "bc1f "<< endl;
    return code.str();
}

void MethodInvocationExpr::genCode(Code &code){
    
}

string AssignationStatement::genCode(){
    return "Assignation statement code generation\n";
}

void GteExpr::genCode(Code &code){
}

void LteExpr::genCode(Code &code){
    Code leftSideCode; 
    Code rightSideCode;
    stringstream ss;
    this->expr1->genCode(leftSideCode);
    this->expr2->genCode(rightSideCode);
    ss << leftSideCode.code <<endl<< rightSideCode.code<<endl;
  
    ss<< "c.le.s "<< leftSideCode.place<< ", "<< rightSideCode.place<<endl;
    
    code.code = ss.str();
}

void EqExpr::genCode(Code &code){
    Code leftSideCode; 
    Code rightSideCode;
    this->expr1->genCode(leftSideCode);
    this->expr2->genCode(rightSideCode);
    stringstream ss;
    ss << leftSideCode.code << endl
    << rightSideCode.code <<endl;
    code.code = ss.str();
}

void ReadFloatExpr::genCode(Code &code){
    
}

string PrintStatement::genCode(){
     Code exprCode;
    stringstream code;
    code<< exprCode.code<<endl;
     code << "mov.s $f12, "<< exprCode.place<<endl
     << "li $v0, 2"<<endl
     << "syscall"<<endl;
    
    return code.str();
}



string ReturnStatement::genCode(){
   Code exprCode;
    this->expr->genCode(exprCode);
    stringstream ss;
    ss << exprCode.code << endl;
    ss<< "move $v0, "<< exprCode.place <<endl;
    return ss.str();
}

string MethodDefinitionStatement::genCode(){
 
    int stackPointer = 4;
    globalStackPointer = 0;
    stringstream code;
    code << this->id<<": "<<endl;
    if(this->params.size() > 0){
        
        for(int i = 0; i< this->params.size(); i++){
            code << "sw $a"<<i<<", "<< stackPointer<<"($sp)"<<endl;
            stackPointer +=4;
            globalStackPointer +=4;
            i ++;
        }
    }
    stringstream sp;
    int currentStackPointer = globalStackPointer;
    sp << endl<<"addiu $sp, $sp, -"<<currentStackPointer<<endl;
    code << "addiu $sp, $sp, "<<currentStackPointer<<endl;
    code <<"jr $ra"<<endl;
  
    string result = code.str();
    result.insert(id.size() + 2, sp.str());
    return result;
}