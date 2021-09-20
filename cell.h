/* cell.h */

#ifndef CELL_H
#define CELL_H

#include <stdlib.h>
#include <raylib.h>
#include <lua.h>
#include "array.h"
#include "card.h"
#include "pile.h"

struct Cell {
    struct Pile super;
};

struct Cell* CellNew(Vector2 pos, enum FanType fan, enum DragType drag, const char* buildfunc, const char* dragfunc);
bool CellCardTapped(struct Card *c);
bool CellPileTapped(struct Pile *p);
bool CellCanAcceptTail(struct Pile *const self, lua_State *L, struct Array *const tail);
void CellSetAccept(struct Pile *const self, enum CardOrdinal ord);
void CellSetRecycles(struct Pile *const self, int r);
void CellDraw(struct Pile *const self);

#endif