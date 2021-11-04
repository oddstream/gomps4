/* discard.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <lua.h>

#include "baize.h"
#include "pile.h"
#include "array.h"
#include "discard.h"
#include "constraint.h"
#include "util.h"

static struct PileVtable discardVtable = {
    &DiscardCanMoveTail,
    &DiscardCanAcceptCard,
    &DiscardCanAcceptTail,
    &DiscardTapped,
    &DiscardCollect,
    &DiscardComplete,
    &DiscardConformant,
    &DiscardAccept,
    &DiscardSetAccept,
    &DiscardSetRecycles,
    &DiscardCountSortedAndUnsorted,

    &PileUpdate,
    &DiscardDraw,
    &PileFree,
};

struct Discard* DiscardNew(struct Baize *const baize, Vector2 slot, enum FanType fan)
{
    struct Discard* self = calloc(1, sizeof(struct Discard));
    if ( self ) {
        PileCtor(baize, (struct Pile*)self, "Discard", slot, fan);
        self->super.vtable = &discardVtable;
    }
    return self;
}

_Bool DiscardCanMoveTail(struct Array *const tail)
{
    struct Card *c = ArrayGet(tail, 0);
    struct Baize* baize = PileOwner(CardOwner(c));
    BaizeSetError(baize, "(CSOL) Cannot move cards from a Discard");
    return 0;
}

_Bool DiscardCanAcceptCard(struct Baize *const baize, struct Pile *const self, struct Card *const c)
{
    (void)baize;
    (void)self;
    (void)c;

    return 0;
}

_Bool DiscardCanAcceptTail(struct Baize *const baize, struct Pile *const self, struct Array *const tail)
{
    if ( !PileEmpty(self) ) {
        BaizeSetError(baize, "(CSOL) Can only move cards to an empty Discard");
        return 0;
    }
    int ndiscards = BaizeCountPiles(baize, self->category);
    if (ndiscards) {
        if ( ArrayLen(tail) != baize->numberOfCardsInLibrary / ndiscards ) {
            BaizeSetError(baize, "(CSOL) Can only move a full set of cards to a Discard");
            return 0;
        }
    }
    return CanTailBeAppended(self, tail);
}

void DiscardTapped(struct Pile *const self, struct Array *const tail)
{
    (void)self;
    (void)tail;
}

int DiscardCollect(struct Pile *const self)
{
    (void)self;
    return 0;
}

_Bool DiscardComplete(struct Pile *const self)
{
    struct Baize *baize = self->owner;
    int ndiscards = BaizeCountPiles(baize, self->category);
    if (ndiscards) {
        return PileLen(self) == baize->numberOfCardsInLibrary / ndiscards;
    }
    return 0;
}

_Bool DiscardConformant(struct Pile *const self)
{
    struct Baize *baize = self->owner;
    int ndiscards = BaizeCountPiles(baize, self->category);
    if (ndiscards) {
        return PileEmpty(self) || PileLen(self) == baize->numberOfCardsInLibrary / ndiscards;
    }
    return 0;
}

enum CardOrdinal DiscardAccept(struct Pile *const self)
{
    (void)self;
    return 0;
}

void DiscardSetAccept(struct Pile *const self, enum CardOrdinal ord)
{
    (void)self;
    (void)ord;
}

void DiscardSetRecycles(struct Pile *const self, int r)
{
    (void)self;
    (void)r;
}

void DiscardCountSortedAndUnsorted(struct Pile *const self, int *sorted, int *unsorted)
{
    (void)unsorted;
    *sorted += ArrayLen(self->cards);
}

void DiscardDraw(struct Pile *const self)
{
    extern Color baizeHighlightColor;
    extern Font fontAcme;
    extern float cardWidth;

    float fontSize = cardWidth / 2.0f;
    Vector2 pos = PileScreenPos(self);
    pos.x += cardWidth / 8.0f;
    pos.y += cardWidth / 16.0f;
    DrawTextEx(fontAcme, "=", pos, fontSize, 0, baizeHighlightColor);

    PileDraw(self);
}
