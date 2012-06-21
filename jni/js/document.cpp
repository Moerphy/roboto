#include "document.h"

#include "helper.h"
#include "../roboto/log.h"

/*
partial interface Document {
  // resource metadata management
  [PutForwards=href] readonly attribute Location? location;
  readonly attribute DOMString URL;
           attribute DOMString domain;
  readonly attribute DOMString referrer;
           attribute DOMString cookie;
  readonly attribute DOMString lastModified;
  readonly attribute DOMString readyState;

  // DOM tree accessors
  getter object (DOMString name);
           attribute DOMString title;
           attribute DOMString dir;
           attribute HTMLElement? body;
  readonly attribute HTMLHeadElement? head;
  readonly attribute HTMLCollection images;
  readonly attribute HTMLCollection embeds;
  readonly attribute HTMLCollection plugins;
  readonly attribute HTMLCollection links;
  readonly attribute HTMLCollection forms;
  readonly attribute HTMLCollection scripts;
  NodeList getElementsByName(DOMString elementName);
  NodeList getItems(optional DOMString typeNames); // microdata 
  readonly attribute DOMElementMap cssElementMap;

  // dynamic markup insertion
  Document open(optional DOMString type, optional DOMString replace);
  WindowProxy open(DOMString url, DOMString name, DOMString features, optional boolean replace);
  void close();
  void write(DOMString... text);
  void writeln(DOMString... text);

  // user interaction
  readonly attribute WindowProxy? defaultView;
  readonly attribute Element? activeElement;
  boolean hasFocus();
           attribute DOMString designMode;
  boolean execCommand(DOMString commandId);
  boolean execCommand(DOMString commandId, boolean showUI);
  boolean execCommand(DOMString commandId, boolean showUI, DOMString value);
  boolean queryCommandEnabled(DOMString commandId);
  boolean queryCommandIndeterm(DOMString commandId);
  boolean queryCommandState(DOMString commandId);
  boolean queryCommandSupported(DOMString commandId);
  DOMString queryCommandValue(DOMString commandId);
  readonly attribute HTMLCollection commands;

  // event handler IDL attributes
  [TreatNonCallableAsNull] attribute Function? onabort;
  [TreatNonCallableAsNull] attribute Function? onblur;
  [TreatNonCallableAsNull] attribute Function? oncancel;
  [TreatNonCallableAsNull] attribute Function? oncanplay;
  [TreatNonCallableAsNull] attribute Function? oncanplaythrough;
  [TreatNonCallableAsNull] attribute Function? onchange;
  [TreatNonCallableAsNull] attribute Function? onclick;
  [TreatNonCallableAsNull] attribute Function? onclose;
  [TreatNonCallableAsNull] attribute Function? oncontextmenu;
  [TreatNonCallableAsNull] attribute Function? oncuechange;
  [TreatNonCallableAsNull] attribute Function? ondblclick;
  [TreatNonCallableAsNull] attribute Function? ondrag;
  [TreatNonCallableAsNull] attribute Function? ondragend;
  [TreatNonCallableAsNull] attribute Function? ondragenter;
  [TreatNonCallableAsNull] attribute Function? ondragleave;
  [TreatNonCallableAsNull] attribute Function? ondragover;
  [TreatNonCallableAsNull] attribute Function? ondragstart;
  [TreatNonCallableAsNull] attribute Function? ondrop;
  [TreatNonCallableAsNull] attribute Function? ondurationchange;
  [TreatNonCallableAsNull] attribute Function? onemptied;
  [TreatNonCallableAsNull] attribute Function? onended;
  [TreatNonCallableAsNull] attribute Function? onerror;
  [TreatNonCallableAsNull] attribute Function? onfocus;
  [TreatNonCallableAsNull] attribute Function? oninput;
  [TreatNonCallableAsNull] attribute Function? oninvalid;
  [TreatNonCallableAsNull] attribute Function? onkeydown;
  [TreatNonCallableAsNull] attribute Function? onkeypress;
  [TreatNonCallableAsNull] attribute Function? onkeyup;
  [TreatNonCallableAsNull] attribute Function? onload;
  [TreatNonCallableAsNull] attribute Function? onloadeddata;
  [TreatNonCallableAsNull] attribute Function? onloadedmetadata;
  [TreatNonCallableAsNull] attribute Function? onloadstart;
  [TreatNonCallableAsNull] attribute Function? onmousedown;
  [TreatNonCallableAsNull] attribute Function? onmousemove;
  [TreatNonCallableAsNull] attribute Function? onmouseout;
  [TreatNonCallableAsNull] attribute Function? onmouseover;
  [TreatNonCallableAsNull] attribute Function? onmouseup;
  [TreatNonCallableAsNull] attribute Function? onmousewheel;
  [TreatNonCallableAsNull] attribute Function? onpause;
  [TreatNonCallableAsNull] attribute Function? onplay;
  [TreatNonCallableAsNull] attribute Function? onplaying;
  [TreatNonCallableAsNull] attribute Function? onprogress;
  [TreatNonCallableAsNull] attribute Function? onratechange;
  [TreatNonCallableAsNull] attribute Function? onreset;
  [TreatNonCallableAsNull] attribute Function? onscroll;
  [TreatNonCallableAsNull] attribute Function? onseeked;
  [TreatNonCallableAsNull] attribute Function? onseeking;
  [TreatNonCallableAsNull] attribute Function? onselect;
  [TreatNonCallableAsNull] attribute Function? onshow;
  [TreatNonCallableAsNull] attribute Function? onstalled;
  [TreatNonCallableAsNull] attribute Function? onsubmit;
  [TreatNonCallableAsNull] attribute Function? onsuspend;
  [TreatNonCallableAsNull] attribute Function? ontimeupdate;
  [TreatNonCallableAsNull] attribute Function? onvolumechange;
  [TreatNonCallableAsNull] attribute Function? onwaiting;

  // special event handler IDL attributes that only apply to Document objects
  [TreatNonCallableAsNull,LenientThis] attribute Function? onreadystatechange;
};
*/

namespace roboto{
namespace js {

  Document::Document(v8::Handle<v8::Object>& that) : EventTarget(that), DocumentEvent(that) {
    
  }


  V8_CREATE_CLASS(Document)
  
}}
