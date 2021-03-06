// SWIG typemaps for std::string
// Roy M. LeCates
// October 23, 2002
//
// Perl implementation

// ------------------------------------------------------------------------
// std::string is typemapped by value
// This can prevent exporting methods which return a string
// in order for the user to modify it.
// However, I think I'll wait until someone asks for it...
// ------------------------------------------------------------------------

// ========== ECMD ADDITION ==========
// This is a copy/paste of the default swig std_string typemap, except I had to remove
// the using namespace stuff and put std:: in front of every string line.
// This had to be done because swig wasn't applying string typemaps to structure members
// Jason Albert - 3/14/05

%include exception.i

%{
#include <string>
%}

/* Overloading check */
%typemap(typecheck) std::string = char *;
%typemap(typecheck) std::string & = char *;
%typemap(typecheck) std::string * = char *;

%typemap(in) std::string {
  STRLEN len;
  const char *ptr = SvPV($input, len);
  if (!ptr) {
    SWIG_croak("Undefined variable in argument $argnum of $symname.");
  } else {
    $1 = std::string(ptr, len);
  }
}

%typemap(in) std::string *REFERENCE(std::string temp), std::string &REFERENCE(std::string temp) {
  STRLEN len;
  const char *ptr = SvPV($input, len);
  if (!ptr) {
    SWIG_croak("Undefined variable in argument $argnum of $symname.");
  } else {
    temp.assign(ptr, len);
    $1 = &temp;
  }
}

%typemap(out) std::string {
  if (argvi >= items) EXTEND(sp, 1);	// bump stack ptr, if needed
  sv_setpvn($result = sv_newmortal(), $1.data(), $1.size());
  ++argvi;
}

%typemap(out) std::string *OUTPUT, std::string &OUTPUT {
  if (argvi >= items) EXTEND(sp, 1);	// bump stack ptr, if needed
  sv_setpvn($result = sv_newmortal(), $1->data(), $1->size());
  ++argvi;
}

%typemap(argout) std::string {
  if (argvi >= items) EXTEND(sp, 1);	// bump stack ptr, if needed
  sv_setpvn($arg, $1.data(), $1.size());
}

%typemap(argout) std::string *REFERENCE, std::string &REFERENCE {
  if (argvi >= items) EXTEND(sp, 1);	// bump stack ptr, if needed
  sv_setpvn($arg, $1->data(), $1->size());
}

%typemap(throws) std::string *, std::string & {
  SWIG_croak($1->c_str());
}

%typemap(throws) std::string {
  SWIG_croak($1.c_str());
}


