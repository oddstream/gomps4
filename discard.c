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
#include "check.h"
#include "util.h"

static struct PileVtable discardVtable = {
    &DiscardCanAcceptCard,
    &DiscardCanAcceptTail,
    &DiscardCollect,
    &DiscardComplete,
    &DiscardConformant,
    &DiscardSetAccept,
    &DiscardSetRecycles,
    &DiscardCountSortedAndUnsorted,

    &PileUpdate,
    &DiscardDraw,
    &PileFree,
};

struct Discard* DiscardNew(Vector2 slot, enum FanType fan)
{
    struct Discard* self = calloc(1, sizeof(struct Discard));
    if ( self ) {
        PileCtor((struct Pile*)self, "Discard", slot, fan);
        self->super.vtable = &discardVtable;
    }
    return self;
}

bool DiscardCanAcceptCard(struct Baize *const baize, struct Pile *const self, struct Card *const c)
{
    BaizeResetError(baize);

    (void)self;
    (void)c;

    return false;
}

bool DiscardCanAcceptTail(struct Baize *const baize, struct Pile *const self, struct Array *const tail)
{
    BaizeResetError(baize);

    if ( !PileEmpty(self) ) {
        return false;
    }
    if ( ArrayLen(tail) != 13 ) {
        BaizeSetError(baize, "Can only move 13 cards to a Discard");
        return false;
    }
    return CheckDragTail(baize, tail);
}

int DiscardCollect(struct Pile *const self)
{
    (void)self;
    return 0;
}

bool DiscardComplete(struct Pile *const self)
{
    return PileLen(self) == 13;
}

bool DiscardConformant(struct Pile *const self)
{
    return PileEmpty(self) || PileLen(self) == 13;
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

    PileDraw(self);

    float fontSize = cardWidth / 2.0f;
    Vector2 pos = PileScreenPos(self);
    pos.x += cardWidth / 8.0f;
    pos.y += cardWidth / 16.0f;
    DrawTextEx(fontAcme, "=", pos, fontSize, 0, baizeHighlightColor);
}
