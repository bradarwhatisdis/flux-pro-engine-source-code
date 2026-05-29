#pragma once

#include "Types.h"

struct CGPoint {
    double x;
    double y;
};

struct CGSize {
    double width;
    double height;
};

struct CGRect {
    CGPoint origin;
    CGSize size;
};

struct CCNode : Class {
    Field<0x2c8, CGSize> _contentSize;
    Field<0x2e0, ptr> _parent;

    CCNode(ptr instance = 0) : Class(instance), _contentSize(instance), _parent(instance) {}

    operator bool() { return instance && this->isInstanceOf("CCNode"); }
};

/* 0x3922f0c
void FUN_03922f0c(undefined8 *param_1,long param_2) {

void FUN_03921080(long *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8) {
    long lVar1;
    code *pcVar2;
    double dVar3;
    double dVar4;
    double adStack_78 [6];
    long local_48;
    
    lVar1 = tpidr_el0;
    local_48 = *(long *)(lVar1 + 0x28);
    dVar3 = (double)param_1[0x59];
    dVar4 = (double)param_1[0x5a];
    pcVar2 = (code *)FUN_038332d0(param_1,(long *)&PTR_s_nodeToParentTransform_04a9d280);
    (*pcVar2)(adStack_78,param_1,&PTR_s_nodeToParentTransform_04a9d280);
    FUN_038e2c4c(0.0,0.0,dVar3,dVar4,adStack_78);
    if (*(long *)(lVar1 + 0x28) == local_48) {
        return;
    }
    __stack_chk_fail();
}
 */