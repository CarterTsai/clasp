#define	DEBUG_LEVEL_FULL

#include "core/common.h"
#include "core/environment.h"
#include "core/designators.h"
#include "symbolTable.h"
#include "core/lispString.h"
#include "core/character.h"
#include "core/str.h"
#include "core/instance.h"
#include "core/wrappers.h"
namespace core
{

    static bool member_charbag(brclChar c, Sequence_sp char_bag)
    {
	if ( List_sp lcur = char_bag.asOrNull<List_O>() ) {
	    for ( ; lcur.notnilp(); lcur = cCdr(lcur) ) {
		if ( oCar(lcur)->eql(Character_O::create(c))) return true;
	    }
	} else if ( Vector_sp vcur = char_bag.asOrNull<Vector_O>() ) {
	    for ( size_t i=0, iEnd(vcur->length()); i<iEnd; ++i ) {
		if ( vcur->elt(i)->eql(Character_O::create(c)) ) return true;
	    }
	}
	return false;
    }

    static Str_sp string_trim0(bool left_trim, bool right_trim, Sequence_sp char_bag, T_sp tstrng)
    {
	int i, j;
	Str_sp strng = coerce::stringDesignator(tstrng);
	i = 0;
	j = af_length(strng);
	if (left_trim)
	{
	    for (;  i < j;  i++) {
		int c = strng->schar(i);
		if (!member_charbag(c, char_bag))
		    break;
	    }
	}
	if (right_trim) {
	    for (; j > i; j--) {
		int c = strng->schar(j-1);
		if (!member_charbag(c, char_bag)) {
		    break;
		}
	    }
	}
	return Str_O::create(strng->substr(i,(j-i)));
    }


    
    
#define ARGS_af_string_trim "(charbag str)"
#define DECL_af_string_trim ""
#define DOCS_af_string_trim "string_trim"
    Str_sp af_string_trim(Sequence_sp charbag, T_sp str)
    {_G();
	return string_trim0(true,true,charbag,str);
    };

#define ARGS_af_string_left_trim "(charbag str)"
#define DECL_af_string_left_trim ""
#define DOCS_af_string_left_trim "string_left_trim"
    Str_sp af_string_left_trim(Sequence_sp charbag, T_sp str)
    {_G();
	return string_trim0(true,false,charbag,str);
    };

#define ARGS_af_string_right_trim "(charbag str)"
#define DECL_af_string_right_trim ""
#define DOCS_af_string_right_trim "string_right_trim"
    Str_sp af_string_right_trim(Sequence_sp charbag, T_sp str)
    {_G();
	return string_trim0(false,true,charbag,str);
    };


#define DOCS_af_string_upcase "string_upcase"
#define LOCK_af_string_upcase 1
#define ARGS_af_string_upcase "(arg)"
#define DECL_af_string_upcase ""    
    Str_sp af_string_upcase(T_sp arg)
    {_G();
	Str_sp str = coerce::stringDesignator(arg);
	Str_sp result = Str_O::create(str->get());
	for ( Str_O::iterator it = result->begin(); it!=result->end(); ++it ) {
            *it = toupper(*it);
	}
	return(result);
    };


    
    
#define ARGS_af_char "(str idx)"
#define DECL_af_char ""
#define DOCS_af_char "char"
    brclChar af_char(T_sp ostr, int idx)
    {_G();
	/* Return the character at idx - ignore fill pointers */
#ifdef UNICODE
	IMPLEMENT_MEF(BF("Handle UNICODE"));
#endif
	if ( Str_sp str = ostr.asOrNull<Str_O>() )
	{
	    if ( idx >= str->size() )
	    {
		TYPE_ERROR_INDEX(str,idx);
	    }
	    return (*str)[idx];
	}
	WRONG_TYPE_NTH_ARG(1,ostr,cl::_sym_string);
	THROW_HARD_ERROR(BF("Should never reach here"));
    };





#define DOCS_af_string_downcase "string_downcase"
#define LOCK_af_string_downcase 1
#define ARGS_af_string_downcase "(arg)"
#define DECL_af_string_downcase ""    
    Str_sp af_string_downcase(T_sp arg)
    {_G();
	Str_sp str = coerce::stringDesignator(arg);
	Str_sp result = Str_O::create(str->get());
	for ( Str_O::iterator it = result->begin(); it!=result->end(); ++it ) {
            *it = tolower(*it);
	}
	return(result);
    };



#define DOCS_af_nstring_upcase "string_upcase"
#define LOCK_af_nstring_upcase 1
#define ARGS_af_nstring_upcase "(arg)"
#define DECL_af_nstring_upcase ""    
    Str_sp af_nstring_upcase(T_sp arg)
    {_G();
	Str_sp result = coerce::stringDesignator(arg);
	for ( Str_O::iterator it = result->begin(); it!=result->end(); ++it ) {
            *it = toupper(*it);
	}
	return(result);
    };






#define DOCS_af_nstring_downcase "string_downcase"
#define LOCK_af_nstring_downcase 1
#define ARGS_af_nstring_downcase "(arg)"
#define DECL_af_nstring_downcase ""    
    Str_sp af_nstring_downcase(T_sp arg)
    {_G();
	Str_sp result = coerce::stringDesignator(arg);
	for ( Str_O::iterator it = result->begin(); it!=result->end(); ++it ) {
            *it = tolower(*it);
	}
	return(result);
    };





#define DOCS_af_string "string"
#define LOCK_af_string 1
#define ARGS_af_string "(arg)"
#define DECL_af_string ""    
    Str_sp af_string(T_sp arg)
    {_G();
	Str_sp result = coerce::stringDesignator(arg);
	return(result);
    };



    bool brcl_memberChar(brclChar c, T_sp charBag )
    {_G();
	if ( Str_sp scharBag = charBag.asOrNull<Str_O>() )
	{
	    for ( size_t i=0, iEnd(scharBag->size()); i<iEnd; ++i ) {
		if ( (*scharBag)[i] == c ) return true;
	    }
	    return false;
	}
	SIMPLE_ERROR(BF("Handle brcl_memberChar for %s") % _rep_(charBag));
    }



// ----------------------------------------------------------------------
//

    EXPOSE_CLASS(core,String_O);

    void String_O::exposeCando(::core::Lisp_sp lisp)
    {_G();
	::core::class_<String_O>()
//	.initArgs("(self)")
	      ;
	SYMBOL_EXPORT_SC_(ClPkg,string);
	Defun(string);
	SYMBOL_EXPORT_SC_(ClPkg,string_upcase);
	Defun(string_upcase);
	SYMBOL_EXPORT_SC_(ClPkg,string_downcase);
	Defun(string_downcase);
	SYMBOL_EXPORT_SC_(ClPkg,nstring_upcase);
	Defun(nstring_upcase);
	SYMBOL_EXPORT_SC_(ClPkg,nstring_downcase);
	Defun(nstring_downcase);
	SYMBOL_EXPORT_SC_(ClPkg,string_trim);
	Defun(string_trim);
	SYMBOL_EXPORT_SC_(ClPkg,string_left_trim);
	Defun(string_left_trim);
	SYMBOL_EXPORT_SC_(ClPkg,string_right_trim);
	Defun(string_right_trim);
	SYMBOL_EXPORT_SC_(ClPkg,char);
	Defun(char);
    }

    void String_O::exposePython(::core::Lisp_sp lisp)
    {
#ifdef USEBOOSTPYTHON
	PYTHON_CLASS(Pkg(),String,"","",_LISP)
//	.initArgs("(self)")
	    ;
#endif
    }


    T_sp String_O::__init__(::core::Function_sp exec, ::core::Cons_sp args, ::core::Environment_sp env, ::core::Lisp_sp lisp)
    {_G();
//      this->Base::__init__(exec,args,env,lisp);
//      arg = translate::from_object<XXXX>::convert(env->lookup(this->Package(),"YYY"));
	return _Nil<T_O>();
    }

#if defined(XML_ARCHIVE)
    void String_O::archiveBase(::core::ArchiveP node)
    {
        this->Base::archiveBase(node);
	// Archive other instance variables here
    }
#endif // defined(XML_ARCHIVE)


    void String_O::initialize()
    {_OF();
        this->Base::initialize();
    }

    

}; /* core */
