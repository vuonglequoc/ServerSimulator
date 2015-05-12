//* Lua++: Lua API for C++
// These classes implement an API for accessing Lua from C++ code.
// It is based on the Lua 3.0a API for C programs.
// Developed by Waldemar Celes.
// May 1997 - version 1.1 (beta)


#ifndef lua_luaplusplus_h
#define lua_luaplusplus_h

extern "C"
{
#include "lua.h"
}


class Lua;
class LuaValue;
class LuaObject;


////////////////////////////////////////////////////////////////////////////
//* Lua value
// Represents a value that can be passed to Lua.
// When calling Lua functions or indexing Lua tables,
// objects of this class are expected. 
// However, they are implicitly constructed from C++ primitive types.
// Thus, in general,
// there is no need to directly create objects of this class.
class LuaValue
{
	 		//* Copying from LuaObject
			// To convert a LuaValue from a LuaObject 
			// is _not_ allowed.
 LuaValue (const LuaObject&);  // not implemented

			//* Assigning a LuaObject
			// Assigning a LuaObject is _not_ allowed.
 LuaValue& operator= (const LuaObject&); // not implemented
 

protected:
 enum {
  LUA_PP_USERDATA=0,
  LUA_PP_NIL,
  LUA_PP_NUMBER,
  LUA_PP_STRING,
  LUA_PP_CFUNCTION,
  LUA_PP_REFERENCE
 };

 int d_type;
 union {
  float         d_num;
  char*         d_str;
  const char*   d_cstr;
  lua_CFunction d_fnc;
  void*         d_ptr;
  const void*   d_cptr;
  lua_Object    d_lo;	// used by derived class
  int           d_ref;	// used by derived class
 };

			// Protected constructor to create references
 LuaValue (int ref, int mark) : d_type(mark), d_ref(ref) {}
public:
 //* CREATORS
 // The following constructors are available,
 // allowing automatic convertion from 
 // C++ primitive types.

			//* Create a 'nil' object
 LuaValue (void)
					: d_type(LUA_PP_NIL) {}
			//* Create a 'number' from an 'int'
 LuaValue (int v) 			
					: d_type(LUA_PP_NUMBER),   d_num(v)  {}
			//* Create a 'number' from a 'float'
 LuaValue (float v) 			
					: d_type(LUA_PP_NUMBER),   d_num(v)  {}
			//* Create a 'number' from a 'double'
 LuaValue (double v) 			
					: d_type(LUA_PP_NUMBER),   d_num(v)  {}
			//* Create a 'string' from a 'char*'
 LuaValue (char* v)		        
					: d_type(LUA_PP_STRING),   d_str(v)  {}
			//* Create a 'string' from a 'const char*'
 LuaValue (const char* v)		
					: d_type(LUA_PP_STRING),   d_cstr(v) {}
			//* Create a 'cfunction' from a 'lua_CFunction}
 LuaValue (lua_CFunction v)		
					: d_type(LUA_PP_CFUNCTION),d_fnc(v)  {}
			//* Create a 'userdata' from a 'void*'
 LuaValue (void* v, int tag=LUA_PP_USERDATA)	
					: d_type(tag),            d_ptr(v)  {}
			//* Create a 'userdata' from a 'const void*'
 LuaValue (const void* v, int tag=LUA_PP_USERDATA) 
					: d_type(tag),            d_cptr(v) {}
			//* Copy and assignment
 LuaValue (const LuaValue& v)  { *this = v; }
 LuaValue& operator= (const LuaValue& v) 
 {
  d_type = v.d_type;
  d_num = v.d_num;
  return *this;
 }
  



			// Destructor
 virtual ~LuaValue (void) {}

 //* MANIPULATORS

			//* Push 
			// Pushs the value onto Lua stack.
 virtual void push (void) const
 {
  switch (d_type)
  {
   case LUA_PP_NIL:	  lua_pushnil(); 		       break;
   case LUA_PP_NUMBER:	  lua_pushnumber(d_num); 	       break;
   case LUA_PP_STRING:    lua_pushstring((char*)d_str);	       break;
   case LUA_PP_CFUNCTION: lua_pushcfunction(d_fnc); 	       break;
   default:               lua_pushusertag((void*)d_ptr,d_type); break;
  }
 }

			//* Store 
			// Stores the value in a given Lua global variable.
 void store (char* varname)
 {
  push();
  lua_storeglobal(varname);
 }

 //* ACCESSORS

 			//* Query if 'nil'
 virtual int isNil      (void) const 
					{ return type()==LUA_PP_NIL; }
 			//* Query if 'number'
 virtual int isNumber   (void) const 
					{ return type()==LUA_PP_NUMBER; }
 			//* Query if 'string'
 virtual int isString   (void) const 
					{ return type()==LUA_PP_STRING; }
 			//* Query if 'table'
 virtual int isTable    (void) const 
					{ return 0; } // never happens
 			//* Query if 'function'
 virtual int isFunction (void) const 
					{ return type()==LUA_PP_CFUNCTION; }
 			//* Query if 'userdata'
 virtual int isUserData (void) const 
					{ return type()>=LUA_PP_USERDATA; }
 			//* Return the corresponding Lua type
 virtual int type       (void) const 
					{ return d_type; }

 //* CONVERSORS
 // Allow conversion to C++ primitive types.

			//* Convert to 'float'
 virtual operator float() const
 { return d_type==LUA_PP_NUMBER ? d_num : 0; }
			//* Convert to {char*}
 virtual operator char*() const
 { return d_type==LUA_PP_STRING ? (char*)d_str : 0; }
			//* Convert to {const char*}
 virtual operator const char*() const
 { return d_type==LUA_PP_STRING ? d_str : 0; }
			//* Convert to {void*}
 virtual operator void*() const
 { return d_type>=LUA_PP_USERDATA ? (void*)d_ptr : 0; }
			//* Convert to {const void*}
 virtual operator const void*() const
 { return d_type>=LUA_PP_USERDATA ? d_ptr : 0; }

};

////////////////////////////////////////////////////////////////////////////
//* Lua object
// Represents a general Lua object, that is, an object stored at Lua stack.
class LuaObject : public LuaValue
{

 lua_Object d_table;	// used for subsructor to create subscript

			// Private constructor to create subscripts
 LuaObject (lua_Object lo, const LuaValue& v) 
	: LuaValue(v), d_table(lo) {}

protected:

 friend class Lua;
			// Protected constructor to create objects from 
			// the stack values
 LuaObject (lua_Object lo) 
        : d_table(LUA_NOOBJECT) { d_lo = lo; }
			// Protected constructor to create references
 LuaObject (int ref, int mark) 
	: LuaValue(ref,mark), d_table(LUA_NOOBJECT) {} 

public:
 //* CREATORS

			//* Create a 'nil' object (default)
 			// After creating a 'nil' object,
			// we use the Lua class methods to
			// access values from the Lua stack.
 LuaObject (void) 
	: LuaValue(-1,LUA_PP_REFERENCE), d_table(LUA_NOOBJECT) {}
 LuaObject (const LuaObject& org) { *this = org; }

 virtual ~LuaObject (void) {}

 LuaObject& operator= (const LuaObject& value)
 {
  d_table = value.d_table;
  LuaValue& base1 = *this;
  const LuaValue& base2 = value;
  base1 = base2;
  return *this;
 }

 LuaObject& operator= (const LuaValue& value)
 {
  if (d_table != LUA_NOOBJECT)
  {
   lua_beginblock();
   lua_pushobject(d_table); 	// push table
   LuaValue::push();		// push index
   value.push();		// push value
   lua_storesubscript();
   lua_endblock();
  }
  else
  {
   lua_error("lua++: cannot assign a LuaValue to a LuaObject.");
  }
  return *this;
 }

		// Get the corresponding Lua object
 virtual lua_Object getobject (void) const
 {
  if (d_table == LUA_NOOBJECT) 
  {
   if (d_type == LUA_PP_REFERENCE) return lua_getref(d_ref);
   else                      return d_lo;
  }
  else
  {
   lua_pushobject(d_table);
   LuaValue::push();
   return  lua_getsubscript();
  }
 }

                // Base virtual functions
 void push (void) const 
 { 
  if (d_table == LUA_NOOBJECT)
  {
   if (d_type == LUA_PP_REFERENCE) lua_pushref(d_ref);
   else lua_pushobject(d_lo); 
  }
  else
  {
   lua_beginblock();
   lua_pushobject(d_table);
   LuaValue::push();
   lua_Object lo = lua_getsubscript();
   lua_endblock();
   lua_pushobject(lo);
  }
 }

 int isNil        (void) const 
 { 
  lua_beginblock();
  int result = lua_isnil(getobject()); 
  lua_endblock();
  return result;
 }
 int isNumber     (void) const 
 { 
  lua_beginblock();
  int result = lua_isnumber(getobject()); 
  lua_endblock();
  return result;
 }
 int isString     (void) const 
 { 
  lua_beginblock();
  int result = lua_isstring(getobject()); 
  lua_endblock();
  return result;
 }
 int isFunction   (void) const 
 { 
  lua_beginblock();
  int result = lua_isfunction(getobject()); 
  lua_endblock();
  return result;
 }
 int isUserData   (void) const 
 { 
  lua_beginblock();
  int result = lua_isuserdata(getobject()); 
  lua_endblock();
  return result;
 }
 int isTable      (void) const 
 { 
  lua_beginblock();
  int result = lua_istable(getobject()); 
  lua_endblock();
  return result;
 }
 int type         (void) const 
 { 
  lua_beginblock();
  int result = lua_type(getobject()); 
  lua_endblock();
  return result;
 }
			// Convert lua value to ordinary C types
 operator float() const
 { 
  lua_beginblock();
  lua_Object lo = getobject();
  float res = lua_getnumber(lo);
  lua_endblock();
  return res;
 }
 operator char*() const
 {
  lua_beginblock();
  lua_Object lo = getobject();
  char* res = lua_getstring(lo);
  lua_endblock();
  return res;
 }
 operator const char*() const
 { 
  lua_beginblock();
  lua_Object lo = getobject();
  const char* res = lua_getstring(lo);
  lua_endblock();
  return res;
 }
 operator void*() const
 {
  lua_beginblock();
  lua_Object lo = getobject();
  void* res = lua_getuserdata(lo);
  lua_endblock();
  return res;
 }
 operator const void*() const
 { 
  lua_beginblock();
  lua_Object lo = getobject();
  const void* res = lua_getuserdata(lo);
  lua_endblock();
  return res;
 }

 //* TABLES

			//* Subscription
			// If the object is a Lua table,
			// the {operator[]} can be used to indexed the Lua
			// corresponding value. 
 LuaObject operator[] (const LuaValue& index)
 {
  return LuaObject(getobject(),index);
 }
 LuaObject operator[] (int index)
 {
  return LuaObject(getobject(),LuaValue(index));
 }

 //* FUNCTIONS
 // If the object is a Lua function,
 // the operator () can be used to call it.

			//* Call function with no argument
 int operator() (void) const
 {
  lua_Object lo = getobject();
  return lua_callfunction(lo);
 }
                        //* Call function with 1 argument
 int operator() (const LuaValue& arg1)
 {
  lua_Object lo = getobject();
  arg1.push();
  return lua_callfunction(lo);
 }
                        //* Call function with 2 arguments
 int operator() (const LuaValue& arg1, const LuaValue& arg2)
 {
  lua_Object lo = getobject();
  arg1.push(); arg2.push();
  return lua_callfunction(lo);
 }
                        //* Call function with 3 arguments
 int operator() (const LuaValue& arg1, 
                 const LuaValue& arg2, 
                 const LuaValue& arg3)
 {
  lua_Object lo = getobject();
  arg1.push(); arg2.push(); arg3.push();
  return lua_callfunction(lo);
 }
                        //* Call function with 4 arguments
 int operator() (const LuaValue& arg1,
                 const LuaValue& arg2,
                 const LuaValue& arg3,
                 const LuaValue& arg4)
 {
  lua_Object lo = getobject();
  arg1.push(); arg2.push(); arg3.push(); arg4.push();
  return lua_callfunction(lo);
 }
                        //* Call function with 5 arguments
 int operator() (const LuaValue& arg1,
                 const LuaValue& arg2,
                 const LuaValue& arg3,
                 const LuaValue& arg4,
                 const LuaValue& arg5)
 {
  lua_Object lo = getobject();
  arg1.push(); arg2.push(); arg3.push(); arg4.push(); arg5.push();
  return lua_callfunction(lo);
 }
			// Do you want more?

};

////////////////////////////////////////////////////////////////////////////
//* Lua table
// Allows a Lua table to be explicitly created from C++ code.
class LuaTable : public LuaObject
{
public:
			//* Creator
			// An initialy empty Lua table is created.
 LuaTable (void) : LuaObject(lua_createtable()) {}

 ~LuaTable (void) {}
  
};

////////////////////////////////////////////////////////////////////////////
//* Lua reference
// If the user needs to store references to LuaObject,
// this reference must be explicitly created, creating objects of
// this class.
class LuaReference : public LuaObject
{
public:
 //* CREATORS

			//* Create a reference to a 'nil' object (default)
 LuaReference (void) 
	: LuaObject() {}
			//* Create a reference to the given Lua object
			// An optinal parameter specifies if the
			// reference is not to be garbage collected by Lua.
			// The default, {lock=0}, means the reference becomes
			// invalid if Lua collects the referenced object.
 LuaReference (const LuaObject& obj, int lock=0)
	:  LuaObject()
 {
  ref(obj,lock);
 }
 
			//* Replace the reference
			// This method allows the user to replace the
			// referenced object; it is useful when the object
			// is created with the default constructor.
			// _Note_: if there is already a reference to a
			// non-'nil' object, this reference, if not used
			// by other LuaReference objets, should be
			// eliminated (using the 'unref' method) before
			// calling this method.
 void ref (const LuaObject& obj, int lock=0)
 { 
  lua_beginblock();
  obj.push();
  d_ref = lua_ref(lock); 
  lua_endblock();
 }

			//* Unreference an object
 void unref (void)
 {
  if (d_type == LUA_PP_REFERENCE) 
   lua_unref(d_ref);
  d_ref = -1;
 }
};


////////////////////////////////////////////////////////////////////////////
//* Lua class
// Binds global functions of Lua API. 
// Refer to Lua manual for details.
class Lua
{
public:

			//* Execute a Lua chunk stored in a file
 static int dofile (char *filename)          
 { return lua_dofile(filename); }
			//* Execute a Lua chunk stored in a string
 static int dostring (char *string)          
 { return lua_dostring(string); }
			//* Generate a Lua error, with the given message
 static void error (char* message)           
 { lua_error(message); }
			//* Get a parameter from Lua stack
 static LuaObject getparam (int number)  
 { return LuaObject(lua_getparam(number)); }
			//* Get a result from Lua stack
 static LuaObject getresult (int number)  
 { return LuaObject(lua_getresult(number)); }
			//* Get a Lua global variable
 static LuaObject getglobal (char* var)  
 { return LuaObject(lua_getglobal(var)); }
			//* Set Lua fallback
 static LuaObject setfallback (char* name, lua_CFunction fallback)
 { return LuaObject(lua_setfallback(name,fallback)); }
			//* Register a C++ function to be called from Lua
 static void record (char* name, lua_CFunction func)
 {
  lua_beginblock();
  lua_register(name,func);
  lua_endblock();
 }
			//* Create a LuaObject from a lua_Object
 static LuaObject createObject (lua_Object lo)
 { return LuaObject(lo); }

};

#endif
