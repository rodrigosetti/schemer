#include "evaluation_result.h"

using namespace std;

EvaluationResult::EvaluationResult( Expression *expression ) : GarbageCollectable() {
    data.expression = expression;
    tailCall = false;
}

EvaluationResult::EvaluationResult( Procedure *procedure, Environment *environment )
    : GarbageCollectable() {
    data.tailCall.procedure = procedure;
    data.tailCall.environment = environment;
    tailCall = true;
}

void EvaluationResult::deepReach() {
    GarbageCollectable::deepReach();
    if (tailCall) {
        data.tailCall.procedure->deepReach();
        data.tailCall.environment->deepReach();
    } else {
        data.expression->deepReach();
    }
}

