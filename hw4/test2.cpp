#define DEBUG_TYPE "hello"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
using namespace llvm;
namespace {
    struct Hello : public FunctionPass {
        static char ID; // Pass identification, replacement for typeid
        Hello() : FunctionPass(ID) {}
        int step = 0;
        virtual bool runOnFunction(Function &F) {
            Function *tmp = &F;
            for (Function::iterator bb = tmp->begin(); bb != tmp->end(); ++bb) {
                for (BasicBlock::iterator inst = bb->begin(); inst != bb->end(); ++inst) {
                    if (inst->getOpcode() == Instruction::Load) {
                        if(step==1){step++;}else{step=0;}
                    }else if(inst->getOpcode() == Instruction::BitCast) {
                        if(step==0){step++;}else{step=0;}
                    }else if (inst->getOpcode() == Instruction::GetElementPtr) {
                        if(step==2){
                            errs() <<"在"<<F.getName()<<"函数中"<< "发现虚函数调用点\n";
                            GetElementPtrInst* sinst = dyn_cast<GetElementPtrInst>(inst);
							Value* vtable_addr = sinst->getPointerOperand();
                            errs() <<"捕获到操作数getPointerOperand："<<vtable_addr<<"\n";
                            IRBuilder<> builder(inst->getNextNode());
                            Type *ltype = vtable_addr->getType();
                            std::vector<Type*> paramTypes = {ltype};
                            Type *retType = Type::getVoidTy(F.getParent()->getContext());
                            FunctionType *traceType = FunctionType::get(retType, paramTypes, true);
                            FunctionCallee traceFunc = F.getParent()->getOrInsertFunction("trace",traceType);
                            std::vector<Value*> args = {vtable_addr};
                            builder.CreateCall(traceFunc, args);
                        }else{step=0;}   
                    }else{step=0;}
                }
            }
            return false;
        }
    };
}
char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass");