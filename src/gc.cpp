#include "gc.h"
#include <set>

using namespace std;

set<GarbageCollectable*> gc_objects;

GarbageCollectable::GarbageCollectable() {
    markForDeletion = false;
    gc_objects.insert( this );
}

void GarbageCollectable::reach() {
    if (markForDeletion) {
        markForDeletion = false;
        deepReach();
    }
}

unsigned long int gc_mark_all_for_deletion() {
    unsigned long int total = gc_objects.size();
    for (set<GarbageCollectable*>::iterator i = gc_objects.begin();
         i != gc_objects.end();
         i ++) {
        (*i)->markForDeletion = true;
    }
    return total;
}

unsigned long int gc_delete_all_marked() {
    unsigned long int total = 0;

    for (set<GarbageCollectable*>::iterator i = gc_objects.begin();
         i != gc_objects.end();) {
        if ( (*i)->markForDeletion ) {
            delete *i;
            gc_objects.erase(i++);
            total ++;
        }
        else {
            ++ i;
        }
    }

    return total;
}

unsigned long int gc_delete_all() {
    unsigned long int total = gc_objects.size();
    for (set<GarbageCollectable*>::iterator i = gc_objects.begin();
         i != gc_objects.end();
         i ++) {
        delete *i;
    }
    return total;
}

unsigned long int gc_run(GarbageCollectable *saved) {

    gc_mark_all_for_deletion();
    saved->reach();
    return gc_delete_all_marked();
}

