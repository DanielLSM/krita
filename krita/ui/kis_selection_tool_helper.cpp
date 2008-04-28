/*
 *  Copyright (c) 2007 Sven Langkamp <sven.langkamp@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "kis_selection_tool_helper.h"


#include <QUndoCommand>

#include <KoShapeController.h>
#include <KoPathShape.h>

#include "kis_pixel_selection.h"
#include "kis_shape_selection.h"
#include "kis_image.h"
#include "canvas/kis_canvas2.h"
#include "kis_view2.h"
#include "kis_selection_manager.h"
#include "kis_selected_transaction.h"

KisSelectionToolHelper::KisSelectionToolHelper( KisCanvas2* canvas, KisLayerSP layer, const QString& name)
    : m_canvas(canvas)
    , m_layer(layer)
    , m_name(name)
{
    m_image = m_layer->image();
}

KisSelectionToolHelper::~KisSelectionToolHelper()
{
}

QUndoCommand* KisSelectionToolHelper::selectPixelSelection(KisPixelSelectionSP selection, selectionAction action)
{
    bool hasSelection = m_layer->selection();

    if(!hasSelection)
        m_image->setGlobalSelection();

    KisSelectedTransaction *t = new KisSelectedTransaction(m_name, m_layer);

    KisPixelSelectionSP getOrCreatePixelSelection = m_layer->selection()->getOrCreatePixelSelection();

    if(! hasSelection || action == SELECTION_REPLACE)
    {
        getOrCreatePixelSelection->clear();
        if(action==SELECTION_SUBTRACT)
            getOrCreatePixelSelection->invert();
    }
    getOrCreatePixelSelection->applySelection(selection, action);

    if(hasSelection && action != SELECTION_REPLACE && action != SELECTION_INTERSECT) {
        QRect rc = selection->selectedRect();
        getOrCreatePixelSelection->setDirty(rc);
        m_layer->selection()->updateProjection(rc);
        m_canvas->view()->selectionManager()->selectionChanged();
    } else {
        getOrCreatePixelSelection->setDirty(m_image->bounds());
        m_layer->selection()->updateProjection(m_image->bounds());
        m_canvas->view()->selectionManager()->selectionChanged();
    }
    return t;
}

QUndoCommand* KisSelectionToolHelper::addSelectionShape(KoShape* shape)
{
    bool hasSelection = m_layer->selection();

    if(!hasSelection)
        m_image->setGlobalSelection();

    KisSelectionSP selection = m_layer->selection();

    KisShapeSelection* shapeSelection;
    if(!selection->hasShapeSelection()) {
        shapeSelection = new KisShapeSelection(m_image, selection);
        QUndoCommand * cmd = m_canvas->shapeController()->addShape(shapeSelection);
        cmd->redo();
        selection->setShapeSelection(shapeSelection);
    }
    else {
        shapeSelection = static_cast<KisShapeSelection*>(selection->shapeSelection());
    }
    QUndoCommand * cmd = m_canvas->shapeController()->addShape(shape);
    shapeSelection->addChild(shape);
    return cmd;
}

