/* widget.c */

#include "ui.h"

void WidgetCtor(struct Widget *const self, struct Container* parent, int align)
{
    self->parent = parent;
    self->align = align;
}

void WidgetUpdate(struct Widget *const self)
{
    (void)self;
}

void WidgetDraw(struct Widget *const self)
{
    (void)self; // all drawing done by subclasses
}

void WidgetFree(struct Widget *const self)
{
    if ( self ) {
        free(self);
    }
}