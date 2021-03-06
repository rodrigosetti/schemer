#ifndef __GC_H__
#define __GC_H__

class GarbageCollectable {

    public:

        bool markForDeletion;

        GarbageCollectable();

        void reach();
        virtual void deepReach() {}
};

unsigned long int gc_run(GarbageCollectable *saved);
unsigned long int gc_delete_all();

#endif

