/*
# ---------------------------------------------------------------------------
#
#       L-Py: L-systems in Python
#
#       Copyright 2003-2008 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
#
#       File author(s): F. Boudon (frederic.boudon@cirad.fr)
#
# ---------------------------------------------------------------------------
#
#                      GNU General Public Licence
#
#       This program is free software; you can redistribute it and/or
#       modify it under the terms of the GNU General Public License as
#       published by the Free Software Foundation; either version 2 of
#       the License, or (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public
#       License along with this program; see the file COPYING. If not,
#       write to the Free Software Foundation, Inc., 59
#       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ---------------------------------------------------------------------------
*/

#ifndef LPY_NO_PLANTGL_INTERPRETATION

#include "plot.h"
#include "axialtree.h"
#include "interpretation.h"
#include "lsyscontext.h"
#include "../plantgl/gui/viewer/pglapplication.h"


PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE
LPY_USING_NAMESPACE

/*---------------------------------------------------------------------------*/

static PlotFunction PLOT = &PGLViewerApplication::display;

void LPY::registerPglPlotFunction(PlotFunction func)
{
    PLOT = func;
}

void LPY::cleanPglPlotFunction()
{
    PLOT = &PGLViewerApplication::display;
}

void LPY::plot(const PGL(ScenePtr)& s)
{
    PLOT(s);
}

/*---------------------------------------------------------------------------*/

static GetSelectFunction GETSELECT = &ViewerApplication::getSelection;

void LPY::registerGetSelectionFunction(GetSelectFunction func)
{
    GETSELECT = func;
}

void LPY::cleanGetSelectionFunction()
{
    GETSELECT = &PGLViewerApplication::getSelection;
}

std::vector<uint_t> LPY::getSelection()
{
    return GETSELECT();
}

/*---------------------------------------------------------------------------*/

static WaitSelectFunction DEFAULTWAITSELECT =
#if PGL_VERSION >= 0x020B01
	&ViewerApplication::waitSelection;
#else
	NULL;
#endif

static WaitSelectFunction WAITSELECT = DEFAULTWAITSELECT;

void LPY::registerWaitSelectionFunction(WaitSelectFunction func)
{
    WAITSELECT = func;
}

void LPY::cleanWaitSelectionFunction()
{
	WAITSELECT = DEFAULTWAITSELECT;
}

uint_t LPY::waitSelection(const std::string& txt)
{
#if PGL_VERSION >= 0x020B01
	if (WAITSELECT != NULL)
#endif
		return WAITSELECT(txt);
#if PGL_VERSION >= 0x020B01
	else return UINT32_MAX;
#endif
}

/*---------------------------------------------------------------------------*/



static void DEFAULTDISPLAYMESSAGE(const std::string& txt) { ViewerApplication::showMessage(txt); }

static DisplayMessageFunction DISPLAYMESSAGE = &DEFAULTDISPLAYMESSAGE;

void LPY::registerDisplayMessageFunction(DisplayMessageFunction func)
{
    DISPLAYMESSAGE = func;
}

void LPY::cleanDisplayMessageFunction()
{
	DISPLAYMESSAGE = &DEFAULTDISPLAYMESSAGE;
}

void LPY::displayMessage(const std::string& txt)
{
	DISPLAYMESSAGE(txt);
}

/*---------------------------------------------------------------------------*/

static void pglSaveImage(const std::string& fname, const std::string& format)
{ ViewerApplication::saveImage(fname,format); }

static SaveImageFunction SAVEIMAGE = &pglSaveImage;

void LPY::registerSaveImageFunction(SaveImageFunction func)
{
    SAVEIMAGE = func;
}

void LPY::cleanSaveImageFunction()
{
    SAVEIMAGE = &pglSaveImage;
}

void LPY::saveImage(const std::string& fname, const std::string& format)
{
    return SAVEIMAGE(fname,format);
}

/*---------------------------------------------------------------------------*/

void LPY::plot(AxialTree& tree){
  plot(tree,LsysContext::currentContext()->turtle);
}

void LPY::plot(AxialTree& tree, PglTurtle& turtle){
  // PGLViewerApplication::display(LPY::scene(tree,turtle));
  PLOT(LPY::scene(tree,turtle));
}

void LPY::plot(AxialTree& tree,StringMatching& matching){
  plot(tree,matching,LsysContext::currentContext()->turtle);
}

void LPY::plot(AxialTree& tree,StringMatching& matching, PglTurtle& turtle){
  // PGLViewerApplication::display(LPY::scene(tree,turtle));
  PLOT(LPY::scene(tree,matching,turtle));
}

/*---------------------------------------------------------------------------*/

ScenePtr LPY::scene(AxialTree& tree){
  return scene(tree,LsysContext::currentContext()->turtle);
}

ScenePtr LPY::scene(AxialTree& tree, PglTurtle& turtle){
  turtle_interpretation(tree,turtle);
  return turtle.getScene();
}

ScenePtr LPY::scene(AxialTree& tree,StringMatching& matching){
  return scene(tree,matching,LsysContext::currentContext()->turtle);
}

ScenePtr LPY::scene(AxialTree& tree,StringMatching& matching, PglTurtle& turtle){
  turtle_interpretation(tree,turtle,matching);
  return turtle.getScene();
}

/*---------------------------------------------------------------------------*/

#endif
