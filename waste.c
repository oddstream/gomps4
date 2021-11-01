/* waste.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <lua.h>

#include "baize.h"
#include "pile.h"
#include "array.h"
#include "constraint.h"
#include "waste.h"

static struct PileVtable wasteVtable = {
    &WasteCanMoveTail,
    &WasteCanAcceptCard,
    &WasteCanAcceptTail,
    &PileGenericTapped,
    &WasteCollect,
    &WasteComplete,
    &WasteConformant,
    &WasteAccept,
    &WasteSetAccept,
    &WasteSetRecycles,
    &WasteCountSortedAndUnsorted,

    &PileUpdate,
    &PileDraw,
    &PileFree,
};

struct Waste* WasteNew(struct Baize *const baize, Vector2 slot, enum FanType fan)
{
    struct Waste* self = calloc(1, sizeof(struct Waste));
    if ( self ) {
        PileCtor(baize, (struct Pile*)self, "Waste", slot, fan);
        self->super.vtable = &wasteVtable;
    }
    return self;
}

_Bool WasteCanMoveTail(struct Array *const tail)
{
    if (ArrayLen(tail)>1) {
        struct Card *c = ArrayGet(tail, 0);
        struct Baize *baize = CardToBaize(c);
        BaizeSetError(baize, "(CSOL) Only a single card can be moved from Waste");
        return false;
    }
    return true;
}

_Bool WasteCanAcceptCard(struct Baize *const baize, struct Pile *const self, struct Card *const c)
{
    (void)baize;

    struct Array1 tail = Array1New(c);
    return CanTailBeAppended(self, (struct Array*)&tail);
    // don't need to free an Array1
}

_Bool WasteCanAcceptTail(struct Baize *const baize, struct Pile *const self, struct Array *const tail)
{
    if (ArrayLen(tail) == 1) {
        return WasteCanAcceptCard(baize, self, ArrayGet(tail, 0));
    }
    return CanTailBeAppended(self, tail);
}

void WasteTapped(struct Pile *const self, struct Array *const tail)
{
    (void)self;
    (void)tail;
}

int WasteCollect(struct Pile *const self)
{
    return PileGenericCollect(self);
}

_Bool WasteComplete(struct Pile *const self)
{
    return PileEmpty(self);
}

_Bool WasteConformant(struct Pile *const self)
{
    return PileEmpty(self);
}

enum CardOrdinal WasteAccept(struct Pile *const self)
{
    (void)self;
    return 0;
}

void WasteSetAccept(struct Pile *const self, enum CardOrdinal ord)
{
    // we don't do that here
    (void)self;
    (void)ord;
}

void WasteSetRecycles(struct Pile *const self, int r)
{
    // we don't do that here
    (void)self;
    (void)r;
}

void WasteCountSortedAndUnsorted(struct Pile *const self, int *sorted, int *unsorted)
{
    (void)sorted;
    *unsorted += ArrayLen(self->cards);
}
