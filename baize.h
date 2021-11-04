/* baize.h */

#ifndef BAIZE_H
#define BAIZE_H

#include <lua.h>

#include "array.h"
#include "pile.h"

struct Baize {
    unsigned magic;

    char variantName[64];

    size_t numberOfCardsInLibrary;
    struct Card *cardLibrary;

    lua_State *L;

    struct Array *piles;
    struct Pile *stock;         // a struct Stock*
    struct Pile *waste;         // a struct Waste*, may be NULL, used by status bar
    struct Array *foundations;  // used by collect
    struct Array *tableaux;

    struct Array *tail;
    struct Array *undoStack;
    size_t savedPosition;
    Vector2 lastTouch;
    struct Pile *touchedPile;

    struct Widget *touchedWidget;

    _Bool dragging;
    Vector2 dragOffset;

    _Bool powerMoves;    // set from POWER_MOVES variable in variant.lua

    struct UI* ui;

    char* errorString;
};

struct Baize* BaizeNew(const char*);
_Bool BaizeValid(struct Baize *const self);
unsigned BaizeCRC(struct Baize *const self);
void BaizeResetError(struct Baize *const self);
void BaizeSetError(struct Baize *const self, const char *str);
void BaizeOpenLua(struct Baize *const self);
void BaizeCloseLua(struct Baize *const self);
void BaizeCreatePiles(struct Baize *const self);
void BaizeResetState(struct Baize *const self, struct Array *undoStack);
void BaizePositionPiles(struct Baize *const self, const int windowWidth);
void BaizeNewDealCommand(struct Baize *const self, void* param);
struct Pile* BaizeFindPile(struct Baize* self, const char *category, int n);
int BaizeCountPiles(struct Baize* self, const char* category);
_Bool BaizeMakeTail(struct Baize *const self, struct Card *const cFirst);
_Bool BaizeDragging(struct Baize *const self);
void BaizeStartDrag(struct Baize *const self);
void BaizeDragBy(struct Baize *const self, Vector2 delta);
void BaizeStopDrag(struct Baize *const self);
void BaizeTouchStart(struct Baize *const self, Vector2 touchPosition);
void BaizeTouchMove(struct Baize *const self, Vector2 touchPosition);
void BaizeTouchStop(struct Baize *const self, Vector2 touchPosition);
void BaizeCollectCommand(struct Baize *const self, void* param);
_Bool BaizeComplete(struct Baize *const self);
_Bool BaizeConformant(struct Baize *const self);
void BaizeAfterUserMove(struct Baize *const self);
void BaizeLayout(struct Baize *const self, const int width, const int height);
void BaizeUpdate(struct Baize *const self);
void BaizeDraw(struct Baize *const self);
void BaizeFree(struct Baize *const self);

void BaizeGetLuaGlobals(struct Baize *const self);
void BaizeStartGame(struct Baize *const self);
void BaizeTailTapped(struct Baize *const self);
void BaizePileTapped(struct Baize *const self, struct Pile *const c);
void BaizeToggleNavDrawerCommand(struct Baize *const self, void* param);
void BaizeToggleVariantDrawerCommand(struct Baize *const self, void* param);
void BaizeFindGameCommand(struct Baize *const self, void* param);
void BaizeReloadVariantCommand(struct Baize *const self, void* param);
void BaizeChangeVariantCommand(struct Baize *const self, void* param);

#endif
