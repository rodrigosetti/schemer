#ifndef __EVALUATION_RESULT_H__
#define __EVALUATION_RESULT_H__

#include "environment.h"
#include "procedure.h"

class EvaluationResult: public GarbageCollectable {

    public:

        bool tailCall;

        union {
            Expression *expression;
            struct {
                Procedure *procedure;
                Environment *environment;
            }
            tailCall;
        }
        data;

        void deepReach();

        EvaluationResult( Expression *expression );
        EvaluationResult( Procedure *procedure, Environment *environment );
};

std::ostream & operator << (std::ostream &output, const EvaluationResult *evaluationResult);

#endif

