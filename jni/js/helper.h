#ifndef ROBOTO_JS_V8HELPER_H
#define ROBOTO_JS_V8HELPER_H

#define V8_CREATE_CLASS(name) static v8::Handle<v8::Value> v8New##name( const v8::Arguments& args ){ \
    v8::HandleScope handlescope; \
    v8::Handle<v8::Object> that = args.This(); \
    name* internalobj = new name( that ); \
    that->SetInternalField( 0, v8::External::New( internalobj ) ); \
    return args.This(); \
  } \
  v8::Handle<v8::FunctionTemplate> name::getTemplate(){ \
    v8::HandleScope scope; \
    v8::Handle<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New(v8New##name); \
    tmpl->SetClassName( v8::String::New(#name) ); \
    tmpl->InstanceTemplate()->SetInternalFieldCount(1); \
    return scope.Close( tmpl ); \
  }
  
#define V8_CREATE_INHERITED_CLASS(name, parent) static v8::Handle<v8::Value> v8New##name( const v8::Arguments& args ){ \
    v8::HandleScope handlescope; \
    v8::Handle<v8::Object> that = args.This(); \
    name* internalobj = new name( that ); \
    that->SetInternalField( 0, v8::External::New( internalobj ) ); \
    return args.This(); \
  } \
  v8::Handle<v8::FunctionTemplate> name::getTemplate(){ \
    v8::HandleScope scope; \
    v8::Handle<v8::FunctionTemplate> tmpl = v8::FunctionTemplate::New(v8New##name); \
    tmpl->SetClassName( v8::String::New(#name) ); \
    tmpl->InstanceTemplate()->SetInternalFieldCount(1); \
    tmpl->Inherit( parent ); \
    return scope.Close( tmpl ); \
  }

#define V8_ADD_FUNCTION(obj, name) obj->Set( v8::String::New(#name), v8::FunctionTemplate::New( v8##name )->GetFunction() );

#endif
