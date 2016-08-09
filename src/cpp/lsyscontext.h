/* ---------------------------------------------------------------------------
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

#pragma once

#include "lstringmatcher.h"
#include "axialtree.h"
#include "lsysoptions.h"
#include "paramproduction.h"
#include "../plantgl/tool/util_hashset.h"
#include <QtCore/QReadWriteLock>

LPY_BEGIN_NAMESPACE

#ifndef LPY_NO_PLANTGL_INTERPRETATION
#include "../plantgl/algo/modelling/pglturtle.h"
#endif

/*---------------------------------------------------------------------------*/

class LPY_API LsysContext {
public:
  friend class Lsystem;
  friend class ModuleVTable;

  /** string value of python variable containing lsystem informations. */
  static const std::string InitialisationFunctionName;
  static const std::string AxiomVariable;
  static const std::string DerivationLengthVariable;
  static const std::string DecompositionMaxDepthVariable;
  static const std::string HomomorphismMaxDepthVariable;
  static double DefaultAnimationTimeStep;

  /** Constructor */
  LsysContext();
  LsysContext(const boost::python::dict& locals);

  /** Destructor */
  virtual ~LsysContext();

  /** clear context. Set python namespace to default. Keep __builtin__, lpy and __filename__ object */
  void clear();
  /** Test whether namespace is empty */
  bool empty() const;

  /** string representation */
  std::string str() const ;

  /** The Start, End, StartEach, EndEach and PostDraw execution */
  boost::python::object start();
  boost::python::object start(AxialTree&);

  boost::python::object startEach();
  boost::python::object startEach(AxialTree&);

  boost::python::object end();
  boost::python::object end(AxialTree&);
#ifndef LPY_NO_PLANTGL_INTERPRETATION
  boost::python::object end(AxialTree&, const PGL::ScenePtr&);
#endif

  boost::python::object endEach();
  boost::python::object endEach(AxialTree&);
#ifndef LPY_NO_PLANTGL_INTERPRETATION
  boost::python::object endEach(AxialTree&, const PGL::ScenePtr&);
#endif

  void postDraw();

  AxialTree startInterpretation();
  AxialTree endInterpretation();

  inline bool hasStartFunction() const { return hasObject("Start"); }
  inline bool hasStartEachFunction() const { return hasObject("StartEach"); }
  inline bool hasEndFunction() const { return hasObject("End"); }
  inline bool hasEndEachFunction() const { return hasObject("EndEach"); }
  inline bool hasPostDrawFunction() const { return hasObject("PostDraw"); }
  inline bool hasStartInterpretationFunction() const { return hasObject("StartInterpretation"); }
  inline bool hasEndInterpretationFunction() const { return hasObject("EndInterpretation"); }

  /** The Start, End, StartEach, EndEach and PostDraw initialisation */
  void setStart(boost::python::object func);
  void setEnd(boost::python::object func);
  void setStartEach(boost::python::object func);
  void setEndEach(boost::python::object func);
  void setPostDraw(boost::python::object func);
  void setStartInterpretation(boost::python::object func);
  void setEndInterpretation(boost::python::object func);

  void check_init_functions();
  inline size_t getEndEachNbArgs() const { return  m_nbargs_of_endeach; }
  inline size_t getEndNbArgs() const { return  m_nbargs_of_end; }
  inline size_t getStartEachNbArgs() const { return  m_nbargs_of_starteach; }
  inline size_t getStartNbArgs() const { return  m_nbargs_of_start; }


  // initialise context using python function in namespace.
  bool initialise();
  size_t initialiseFrom(const std::string& lcode);

  /** compilation of code into the python namespace */
  void compile(const std::string&)  ;

  boost::python::object  evaluate(const std::string&)  ;
  boost::python::object  try_evaluate(const std::string&)  ;

  virtual boost::python::object compile(const std::string& name, const std::string& code)  ;

  /** application of a function */
  boost::python::object func(const std::string& funcname);

  /** access to value of the python namespace */
  int readInt(const std::string&) ;
  float readReal(const std::string&)  ;

  /** python namespace management */
  virtual void clearNamespace();
  virtual void updateNamespace(const boost::python::dict&);
  virtual void getNamespace(boost::python::dict&) const;
  virtual void updateFromContextNamespace(const LsysContext&);

  /** access to python object of the namespace */
  virtual bool hasObject(const std::string& name) const;

  virtual boost::python::object getObject(const std::string& name, const boost::python::object& defaultvalue = boost::python::object()) const;
  virtual void delObject(const std::string& name) ;

  virtual void setObject(const std::string& name, const boost::python::object&);
  virtual void setObjectToGlobals(const std::string& name, const boost::python::object&);

  bool copyObject(const std::string& name, LsysContext * sourceContext) ;
  bool copyObjectToGlobals(const std::string& name, LsysContext * sourceContext) ;

  /// protected access to python namespace. To be redefined.
  virtual boost::python::dict locals()  const { return m_locals; };
  virtual PyObject * globals()  const { return NULL; };
  
  /** make current or disable a context */
  void makeCurrent();
  bool isCurrent() const ;
  void done() ;

  /** static functions to access context */
  static inline LsysContext * currentContext() { return current(); }
  static LsysContext * current();
  static LsysContext * globalContext(); 

  // { return global(); }
  // static LsysContext * global();

  static LsysContext * defaultContext() ;
  static void cleanContexts();

  /** control of the direction of next iteration */
  inline void backward() { m_direction = eBackward; }
  inline void forward() { m_direction = eForward; }
  inline bool isForward() { return m_direction == eForward; }
  inline eDirection getDirection() const { return m_direction; }

  /** selection of group of rules */
  inline void useGroup(size_t gid) { m_group = gid; }
  inline size_t getGroup() const  { return m_group; }

  /** control of frame display */
  inline void frameDisplay(bool enabled) { m_frameDisplay = enabled; }
  inline bool isFrameDisplayed() const  { return m_frameDisplay; }

  /** iterative production */
  inline void nproduce(const AxialTree& prod)
  { m_nproduction.append(prod); }

  inline void nproduce(const boost::python::list& prod)
  { m_nproduction.append(AxialTree(prod)); }

  inline void reset_nproduction() { m_nproduction.clear(); }
  inline AxialTree get_nproduction() const { return m_nproduction; }
  inline void set_nproduction(const AxialTree& prod) { m_nproduction = prod; }

  /** parametric production */
  inline void add_pproduction(const ParametricProductionPtr pprod)
  { m_paramproductions.push_back(pprod); }

  inline void add_pproductions(const ParametricProductionList& pprod)
  { m_paramproductions.insert(m_paramproductions.end(),pprod.begin(),pprod.end()); }

  inline const ParametricProductionList& get_pproductions() const 
  { return m_paramproductions; }
  

  inline AxialTree generate(size_t pprod_id, const bp::list& args)
  {  return ParametricProduction::get(pprod_id)->generate(args); }


  inline AxialTree generate(const bp::tuple& args)
  { size_t pprod_id = bp::extract<size_t>(args[0])();    
	return ParametricProduction::get(pprod_id)->generate(args); }

  inline void pproduce(size_t pprod_id, const bp::list& args)
  { m_nproduction.append(generate(pprod_id,args)); }

  inline void pproduce(const bp::tuple& args)
  { m_nproduction.append(generate(args)); }

  /** animation time step property */
  double get_animation_timestep();
  void set_animation_timestep(double value);
  bool is_animation_timestep_to_default();

  inline bool isAnimationEnabled() const { return m_animation_enabled; }
  inline void setAnimationEnabled(bool enabled) { m_animation_enabled = enabled; }

  /** Specify if the selection check is required */
  bool isSelectionAlwaysRequired() const;
  void setSelectionAlwaysRequired(bool enabled);

  inline void requestSelection(const std::string& message) {
	m_selection_requested = true;
	m_selection_message = message;
	frameDisplay(true);
  }
  inline bool isSelectionRequested() const { return m_selection_requested; }
  inline const std::string& getSelectionMessage() const { return m_selection_message; }

  inline void selectionAquired() {
	m_selection_requested = false;
  }

  /// Specify whether a warning should be made if found sharp module
  inline bool warnWithSharpModule() const { return m_warn_with_sharp_module; }
  void setWarnWithSharpModule(bool);

#ifndef LPY_NO_PLANTGL_INTERPRETATION
  /** Turtles and interpretation structures */
  PGL(PglTurtle) turtle;
  PGL(Turtle)    envturtle;
#endif

  // initialise context using python function in namespace.
  bool m_initialise();
  size_t m_initialiseFrom(const std::string& lcode);

  /** Context options */
  LsysOptions options;

  /** module declaration. */
  void declare(const std::string& modules);
  inline void declare(ModuleClassPtr module)
  { m_modules.push_back(module); }

  void undeclare(const std::string& modules);
  void undeclare(ModuleClassPtr module);
  bool isDeclared(const std::string& module);
  bool isDeclared(ModuleClassPtr module);
  ModuleClassList declaredModules() const { return m_modules; }
  void declareModules(const ModuleClassList& other);

  void declareAlias(const std::string& aliasName, ModuleClassPtr module);

  void setModuleScale(const std::string& modules, int scale);


  /// return if no matching
  bool return_if_no_matching;
  inline void setReturnIfNoMatching(bool enabled) { return_if_no_matching = enabled; }

  /// optimization level
  static const int DEFAULT_OPTIMIZATION_LEVEL;
  int optimizationLevel;
  inline void setOptimizationLevel(int level) { optimizationLevel = level; }

  /** early return */
  void enableEarlyReturn(bool val) ;
  bool isEarlyReturnEnabled() ;
  inline void stop() { enableEarlyReturn(true); }

  void importContext(const LsysContext& other);
  
  void registerLstringMatcher(const LstringMatcherPtr lstringmatcher = LstringMatcherPtr())
  { m_lstringmatcher = lstringmatcher; }

  bool pInLeftContext(size_t, boost::python::dict& res);
  bool inLeftContext(const PatternString& pattern, boost::python::dict& res);

  bool pInRightContext(size_t, boost::python::dict& res);
  bool inRightContext(const PatternString& pattern, boost::python::dict& res);

  /** Iteration number property. Only set by Lsystem. Access by all other. */
public:
  size_t getIterationNb();
protected:
  void setIterationNb(size_t) ;

protected:
  boost::python::dict m_locals;

  boost::python::object controlMethod(const std::string&, AxialTree&);
#ifndef LPY_NO_PLANTGL_INTERPRETATION
  boost::python::object controlMethod(const std::string&, AxialTree&, const PGL::ScenePtr&);
#endif

  void namespaceInitialisation();

  /** Event when context is made current, release, pushed or restore */
  virtual void currentEvent();
  virtual void doneEvent();
  virtual void pushedEvent(LsysContext * newEvent);
  virtual void restoreEvent(LsysContext * previousEvent);

  /// protected copy constructor.
  LsysContext(const LsysContext& lsys);
  LsysContext& operator=(const LsysContext& lsys);


  /// init options
  void init_options();

  /// attributes for module declaration
  ModuleClassList m_modules;
  ModuleVTableList m_modulesvtables;

  typedef pgl_hash_map_string<ModuleClassPtr> AliasSet;
  AliasSet m_aliases;

  /// next iteration control
  eDirection m_direction;
  size_t m_group;
  bool m_frameDisplay;

  /// iterative production
  AxialTree m_nproduction;


  /// selection required property
  bool m_selection_always_required;
  std::string m_selection_message;
  bool m_selection_requested;

  /// Warn if found sharp module
  bool m_warn_with_sharp_module;

  /// animation step property and its mutex
  double m_animation_step;
  QReadWriteLock m_animation_step_mutex;
  /// animation property
  bool m_animation_enabled;

  /// iteration nb property and its mutex
  size_t m_iteration_nb;
  QReadWriteLock m_iteration_nb_lock;

  size_t m_nbargs_of_starteach;
  size_t m_nbargs_of_start;
  size_t m_nbargs_of_endeach;
  size_t m_nbargs_of_end;

  // list of parametric production
  ParametricProductionList m_paramproductions;

  // list of pattern to find
//  PatternStringList m_patternstrings;
  LstringMatcherPtr m_lstringmatcher;


  // For multithreaded appli, allow to set an early_return
  bool m_early_return;
  QReadWriteLock m_early_return_mutex;
};

/*---------------------------------------------------------------------------*/

class LPY_API LocalContext : public LsysContext {
public:
  // LocalContext();
  LocalContext(const boost::python::dict& globals = boost::python::dict());
  LocalContext(const boost::python::dict& locals, const boost::python::dict& globals);
  ~LocalContext();

  virtual PyObject * globals() const ;
  virtual void clearNamespace();

protected:
  boost::python::dict m_globals;
};

/*---------------------------------------------------------------------------*/

class LPY_API GlobalContext : public LsysContext {
public:
  GlobalContext();
  ~GlobalContext();

  virtual PyObject * globals() const ;
  static boost::python::object getFunctionRepr();

protected:

  boost::python::handle<> m_globals;
  static boost::python::object m_reprFunc;

};

/*---------------------------------------------------------------------------*/

inline void LPY_API nproduce(const AxialTree& prod)
{ LsysContext::currentContext()->nproduce(prod); }

inline void LPY_API nproduce(const boost::python::list& prod)
{ LsysContext::currentContext()->nproduce(prod); }

inline void LPY_API nproduce(const std::string& modules)
{ LsysContext::currentContext()->nproduce(AxialTree(modules)); }

inline void LPY_API pproduce(size_t id, const boost::python::list& args)
{ LsysContext::currentContext()->pproduce(id,args); }

inline void LPY_API pproduce(const boost::python::tuple& args)
{ LsysContext::currentContext()->pproduce(args); }

inline void LPY_API useGroup(size_t gid)
{ LsysContext::currentContext()->useGroup(gid); }

inline size_t LPY_API getGroup()
{ return LsysContext::currentContext()->getGroup(); }

inline void LPY_API frameDisplay(bool enabled)
{ LsysContext::currentContext()->frameDisplay(enabled); }

inline bool LPY_API isFrameDisplayed()
{ return LsysContext::currentContext()->isFrameDisplayed(); }

inline void LPY_API setSelectionAlwaysRequired(bool enabled)
{ LsysContext::currentContext()->setSelectionAlwaysRequired(enabled); }

inline void LPY_API requestSelection(const std::string& message)
{ LsysContext::currentContext()->requestSelection(message); }

inline bool LPY_API isSelectionAlwaysRequired()
{ return LsysContext::currentContext()->isSelectionAlwaysRequired(); }

inline size_t LPY_API getIterationNb()
{ return LsysContext::currentContext()->getIterationNb(); }

inline void LPY_API declare(const std::string& modules)
{ LsysContext::currentContext()->declare(modules); }

inline void LPY_API undeclare(const std::string& modules)
{ LsysContext::currentContext()->undeclare(modules); }

inline void LPY_API isDeclared(const std::string& module)
{ LsysContext::currentContext()->isDeclared(module); }

inline bool LPY_API isAnimationEnabled()
{ return LsysContext::currentContext()->isAnimationEnabled(); }

inline void LPY_API Stop()
{ return LsysContext::currentContext()->stop(); }

inline bool LPY_API pInLeftContext(size_t pid, boost::python::dict& res)
{ return LsysContext::currentContext()-> pInLeftContext(pid,res); }

inline bool inLeftContext(const PatternString& pattern, boost::python::dict& res)
{ return LsysContext::currentContext()-> inLeftContext(pattern,res); }

inline bool LPY_API pInRightContext(size_t pid, boost::python::dict& res)
{ return LsysContext::currentContext()-> pInRightContext(pid,res); }

inline bool inRightContext(const PatternString& pattern, boost::python::dict& res)
{ return LsysContext::currentContext()-> inRightContext(pattern,res); }

/*---------------------------------------------------------------------------*/

struct ContextMaintainer {
    bool is_set;
    LsysContext * context;

    ContextMaintainer(LsysContext * _context) : 
        is_set(!_context->isCurrent()), context(_context)
    { 
		if (is_set) context->makeCurrent(); 
    }

    ~ContextMaintainer() { if (is_set) context->done();  }
};

/*---------------------------------------------------------------------------*/

LPY_END_NAMESPACE
