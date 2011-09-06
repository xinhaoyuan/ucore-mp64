/* ezxml.h
 *
 * Copyright 2004-2006 Aaron Voisine <aaron@voisine.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* 
 * Modified for Palacios by Jack Lange <jarusl@cs.northwestern.edu> 
 */

#ifndef __VMM_XML_H
#define __VMM_XML_H

#include <palacios/vmm.h>
#include <palacios/vmm_types.h>


struct v3_xml {
    char *name;      // tag name
    char **attr;     // tag attributes { name, value, name, value, ... NULL }
    char *txt;       // tag character content, empty string if none
    size_t off;      // tag offset from start of parent tag character content
    struct v3_xml * next;    // next tag with same name in this section at this depth
    struct v3_xml * sibling; // next tag with different name in same section and depth
    struct v3_xml * ordered; // next tag, same section and depth, in original order
    struct v3_xml * child;   // head of sub tag list, NULL if none
    struct v3_xml * parent;  // parent tag, NULL if current tag is root tag
    short flags;     // additional information
};

// Given a string of xml data and its length, parses it and creates an v3_xml
// structure. For efficiency, modifies the data by adding null terminators
// and decoding ampersand sequences. If you don't want this, copy the data and
// pass in the copy. Returns NULL on failure.
struct v3_xml * v3_xml_parse(char * buf);


// returns the name of the given tag
#define v3_xml_name(xml) ((xml) ? xml->name : NULL)

// returns the given tag's character content or empty string if none
#define v3_xml_txt(xml) ((xml) ? xml->txt : "")


// returns the first child tag (one level deeper) with the given name or NULL
// if not found
struct v3_xml * v3_xml_child(struct v3_xml * xml, const char * name);

// returns the next tag of the same name in the same section and depth or NULL
// if not found
#define v3_xml_next(xml) ((xml) ? xml->next : NULL)

// Returns the Nth tag with the same name in the same section at the same depth
// or NULL if not found. An index of 0 returns the tag given.
struct v3_xml * v3_xml_idx(struct v3_xml * xml, int idx);

// returns the value of the requested tag attribute, or NULL if not found
const char *v3_xml_attr(struct v3_xml * xml, const char * attr);


// Traverses the v3_xml sturcture to retrieve a specific subtag. Takes a
// variable length list of tag names and indexes. The argument list must be
// terminated by either an index of -1 or an empty string tag name. Example: 
// title = v3_xml_get(library, "shelf", 0, "book", 2, "title", -1);
// This retrieves the title of the 3rd book on the 1st shelf of library.
// Returns NULL if not found.
struct v3_xml * v3_xml_get(struct v3_xml * xml, ...);


// frees the memory allocated for an v3_xml structure
void v3_xml_free(struct v3_xml * xml);
    
#endif // __VMM_XML_H
