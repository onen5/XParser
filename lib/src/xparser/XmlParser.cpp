/**
 * @class XmlParser
 *
 * @brief This class is used as a base class for XML parser
 *
 * @section DESCRIPTION
 *  Base class for XML parsers
 *
 */

#include "xparser/XmlParser.h"

#include <cstring>

#include "libxml/parser.h"
#include "libxml/parserInternals.h"

using namespace std;

namespace xparser {

/*
 * Public Methods
 */
unordered_map< string, unordered_map< string, function< void() > > > XmlParser::endHandlerMap;
unordered_map< string, unordered_map< string, function< void() > > > XmlParser::startHandlerMap;
unordered_map< string, unordered_map< string, function< void( void* ) > > > XmlParser::userHandlerMap;

XmlParser::XmlParser( const string& filename ) :
    bUseDom( false ),
    mCurrCharacters( "" ),
    mCurrAttribs(),
    mFilename( filename ),
    mDomain( "XmlParser" ),
    mUserHandlerObject( nullptr ),
    bInUserHandler( false )
{
}

XmlParser::XmlParser() :
    XmlParser( string( "" ) )
{
}

XmlParser::~XmlParser() { }

/**
 * ConvertInput: *** taken from xmlwriter.c example
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or nullptr in case of error.
 */
xmlChar* XmlParser::convertInput( const char* in, const char* encoding )
{
    xmlChar* out;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if ( in == 0 ) {
        return 0;
    }

    handler = xmlFindCharEncodingHandler( encoding );

    if ( !handler ) {
        printf( "XmlParser::convertInput: no encoding handler found for '%s'\n", encoding ? encoding : "" );
        return 0;
    }

    size     = (int)strlen( in ) + 1;
    out_size = size * 2 - 1;
    out      = (uint8_t*)xmlMalloc( (size_t)out_size );

    if ( out != 0 ) {
        temp    = size - 1;
        int ret = handler->input( out, &out_size, (const xmlChar*)in, &temp );
        if ( ( ret < 0 ) || ( temp - size + 1 ) ) {
            if ( ret < 0 ) {
                printf( "XmlParser::convertInput: conversion wasn't successful.\n" );
            }
            else {
                printf( "XmlParser::convertInput: conversion wasn't successful. converted: %i octets.\n", temp );
            }

            xmlFree( out );
            out = 0;
        }
        else {
            out             = (uint8_t*)xmlRealloc( out, out_size + 1 );
            out[ out_size ] = 0; /*null terminating out */
        }
    }
    else {
        printf( "XmlParser::convertInput: no mem\n" );
    }

    return out;
}

const string& XmlParser::getFileName() const
{
    return mFilename;
}

unique_ptr< string > XmlParser::getStringPtr( const xmlChar* str ) const
{
    if ( str != nullptr ) {
        return make_unique< string >( (char*)str );
    }

    return make_unique< string >( "" );
}

const string& XmlParser::getStringAttrib( const string& attrib ) const
{
    return *( mCurrAttribs[ attrib ] );
}

const string& XmlParser::getStringAttrib( const string& attrib, const string& defaultValue ) const
{
    auto entry = mCurrAttribs.find( attrib );
    if ( entry == mCurrAttribs.end() ) {
        return defaultValue;
    }
    return *( entry->second );
}

string XmlParser::getString( const xmlChar* str ) const
{
    return *( getStringPtr( str ) );
}

string XmlParser::getString( const xmlChar* str, int len ) const
{
    if ( str != nullptr ) {
        return string( (char*)str, len );
    }

    return string( "" );
}

bool XmlParser::getBoolAttrib( const string& attrib ) const
{
    return getBool( mCurrAttribs[ attrib ] );
}

bool XmlParser::getBoolAttrib( const string& attrib, bool defaultValue ) const
{
    auto entry = mCurrAttribs.find( attrib );
    if ( entry == mCurrAttribs.end() ) {
        return defaultValue;
    }
    return getBool( entry->second );
}

bool XmlParser::getBool( const xmlChar* bVal ) const
{
    return getBool( getStringPtr( bVal ).get() );
}

bool XmlParser::getBool( const unique_ptr< string >& str ) const
{
    return getBool( str.get() );
}

bool XmlParser::getBool( const string* str ) const
{
    if ( str == nullptr ) {
        return false;
    }
    return ( ( *str == "true" ) || ( *str == "1" ) );  // valid xsd:boolean
}

float XmlParser::getFloatAttrib( const string& attrib ) const
{
    return getFloat( mCurrAttribs[ attrib ].get() );
}

float XmlParser::getFloatAttrib( const string& attrib, float defaultValue ) const
{
    auto entry = mCurrAttribs.find( attrib );
    if ( entry == mCurrAttribs.end() ) {
        return defaultValue;
    }
    return getFloat( entry->second );
}

float XmlParser::getFloat( const xmlChar* number ) const
{
    return getFloat( getStringPtr( number ).get() );
}

float XmlParser::getFloat( const unique_ptr< string >& str ) const
{
    return getFloat( str.get() );
}

float XmlParser::getFloat( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    string::size_type sz;
    return stof( *str, &sz );
}

uint16_t XmlParser::getUint16Attrib( const string& attrib ) const
{
    return getUint16( mCurrAttribs[ attrib ].get() );
}

uint16_t XmlParser::getUint16( const xmlChar* number ) const
{
    return getUint16( getStringPtr( number ).get() );
}

uint16_t XmlParser::getUint16( const unique_ptr< string >& str ) const
{
    return getUint16( str.get() );
}

uint16_t XmlParser::getUint16( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    return static_cast< uint16_t >( strtol( str->c_str(), (char**)0, 0 ) );
}

uint32_t XmlParser::getUint32Attrib( const string& attrib ) const
{
    return getUint32( mCurrAttribs[ attrib ].get() );
}

uint32_t XmlParser::getUint32( const xmlChar* number ) const
{
    return getUint32( getStringPtr( number ).get() );
}

uint32_t XmlParser::getUint32( const unique_ptr< string >& str ) const
{
    return getUint32( str.get() );
}

uint32_t XmlParser::getUint32( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    return static_cast< uint32_t >( strtol( str->c_str(), (char**)0, 0 ) );
}

uint64_t XmlParser::getUint64Attrib( const string& attrib ) const
{
    return getUint64( mCurrAttribs[ attrib ].get() );
}

uint64_t XmlParser::getUint64( const xmlChar* number ) const
{
    return getUint64( getStringPtr( number ).get() );
}

uint64_t XmlParser::getUint64( const unique_ptr< string >& str ) const
{
    return getUint64( str.get() );
}

uint64_t XmlParser::getUint64( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    return strtoull( str->c_str(), (char**)0, 0 );
}

uint16_t XmlParser::getUint16HexAttrib( const string& attrib ) const
{
    return getUint16Hex( mCurrAttribs[ attrib ].get() );
}

uint16_t XmlParser::getUint16Hex( const xmlChar* number ) const
{
    return getUint16Hex( getStringPtr( number ).get() );
}

uint16_t XmlParser::getUint16Hex( const unique_ptr< string >& str ) const
{
    return getUint16Hex( str.get() );
}

uint16_t XmlParser::getUint16Hex( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    return static_cast< uint16_t >( strtol( str->c_str(), (char**)0, 16 ) );
}

uint32_t XmlParser::getUint32HexAttrib( const string& attrib ) const
{
    return getUint32Hex( mCurrAttribs[ attrib ].get() );
}

uint32_t XmlParser::getUint32Hex( const xmlChar* number ) const
{
    return getUint32Hex( getStringPtr( number ).get() );
}

uint32_t XmlParser::getUint32Hex( const unique_ptr< string >& str ) const
{
    return getUint32Hex( str.get() );
}

uint32_t XmlParser::getUint32Hex( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    return static_cast< uint32_t >( strtol( str->c_str(), (char**)0, 16 ) );
}

uint64_t XmlParser::getUint64HexAttrib( const string& attrib ) const
{
    return getUint64Hex( mCurrAttribs[ attrib ].get() );
}

uint64_t XmlParser::getUint64Hex( const xmlChar* number ) const
{
    return getUint64Hex( getStringPtr( number ).get() );
}

uint64_t XmlParser::getUint64Hex( const unique_ptr< string >& str ) const
{
    return getUint64Hex( str.get() );
}

uint64_t XmlParser::getUint64Hex( const string* str ) const
{
    if ( str == nullptr ) {
        return 0;
    }
    return strtoull( str->c_str(), (char**)0, 16 );
}

void XmlParser::getUint128HexAttrib( const string& attrib, uint64_t& val1, uint64_t& val2 )
{
    getUint128Hex( mCurrAttribs[ attrib ].get(), val1, val2 );
    return;
}

void XmlParser::getUint128Hex( const xmlChar* number, uint64_t& val1, uint64_t& val2 )
{
    getUint128Hex( getStringPtr( number ).get(), val1, val2 );
    return;
}

void XmlParser::getUint128Hex( const unique_ptr< string >& str, uint64_t& val1, uint64_t& val2 )
{
    getUint128Hex( str.get(), val1, val2 );
    return;
}

void XmlParser::getUint128Hex( const string* str, uint64_t& val1, uint64_t& val2 )
{
    if ( str == nullptr ) {
        val1 = 0;
        val2 = 0;
        return;
    }
    val1 = strtoull( str->substr( 16 ).c_str(), (char**)0, 16 );
    val2 = strtoull( str->substr( 0, 16 ).c_str(), (char**)0, 16 );
    return;
}

const string* XmlParser::getContent( xmlNode* nodePtr ) const
{
    mCurrCharacters = *( getStringPtr( nodePtr->children->content ) );
    return &mCurrCharacters;
}

bool XmlParser::parse()
{
    if ( bUseDom ) {
        return parseXmlDoc( xmlReadFile( getFileName().c_str(), nullptr, 0 ) );
    }

    xmlParserCtxtPtr ctxt = nullptr;

    // create the context
    if ( ( ctxt = xmlCreateFileParserCtxt( getFileName().c_str() ) ) == nullptr ) {
        xmlSetStructuredErrorFunc( nullptr, nullptr );

        XML_ERROR( "Problem parsing file:" << getFileName() );
    }

    // register sax handler with the context
    xmlSAXHandler saxh;
    make_sax_handler( &saxh );
    ctxt->sax      = &saxh;
    ctxt->userData = this;

    // parse the doc
    xmlParseDocument( ctxt );

    // well-formed document?
    if ( !ctxt->wellFormed ) {
        xmlFreeParserCtxt( ctxt );
        XML_ERROR( "XML Document is not well formed: " << getFileName() );
    }

    ctxt->sax      = nullptr;
    ctxt->userData = nullptr;

    xmlFreeParserCtxt( ctxt );

    return true;
}

bool XmlParser::parseXmlDoc( xmlDoc* doc )
{
    if ( doc == nullptr ) {
        XML_ERROR( "doc is null in getRootXmlNode so could not parse file " + mFilename );
    }
    else {
        processElement( xmlDocGetRootElement( doc ) );
        xmlFreeDoc( doc );
    }

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
    return true;
}

bool XmlParser::parseXmlStr( char* xStr, long fsize )
{
    xmlSAXHandler saxHandler;
    make_sax_handler( &saxHandler );

    int result = xmlSAXUserParseMemory( &saxHandler, this, xStr, fsize );
    if ( result != 0 ) {
        XML_ERROR( "Problem parsing file:" << getFileName() );
    }
    xmlCleanupParser();
    return true;
}

bool XmlParser::parseXmlStr( xmlChar* xStr )
{
    if ( bUseDom ) {
        return parseXmlDoc( xmlParseDoc( xStr ) );
    }

    return parseXmlStr( (char*)xStr, strlen( (char*)xStr ) );
}

/* Recursive function that processes the elements In Order traversal */
void XmlParser::processElement( xmlNode* nodePtr )
{
    for ( auto currNodePtr = nodePtr; currNodePtr; currNodePtr = currNodePtr->next ) {
        if ( currNodePtr->type == XML_ELEMENT_NODE ) {
            processNode( currNodePtr );
        }
    }
}

const xParserAttribMap* XmlParser::getAttributes( xmlNode* nodePtr ) const
{
    for ( auto attrPtr = nodePtr->properties; attrPtr; attrPtr = attrPtr->next ) {
        string name( (char*)attrPtr->name );
        mCurrAttribs.insert( make_pair( name, getStringPtr( attrPtr->children->content ) ) );
    }

    return &mCurrAttribs;
}

void XmlParser::setDomain( const string& domain )
{
    mDomain = domain;
}

void XmlParser::processNode( xmlNode* nodePtr )
{
    auto name = getStringPtr( nodePtr->name );

    auto handlerFunction = handlerMap.find( *name );

    if ( handlerFunction != handlerMap.end() ) {
        handlerFunction->second( nodePtr );
    }
    else {
        XML_ERROR( mDomain << ": Unknown element <" << *name << ">" );
    }
}

void XmlParser::addTagHandler( const string& tagName, const function< void( xmlNode* ) >& func )
{
    handlerMap[ tagName ] = func;
}

const function< void( xmlNode* ) >& XmlParser::getTagHandler( const string& tagName ) const
{
    return handlerMap.find( tagName )->second;
}

/*
 * SAXY Code
 */

const xParserAttribMap* XmlParser::getAttributes() const
{
    return &mCurrAttribs;
}

const string* XmlParser::getContent() const
{
    return &mCurrCharacters;
}

void XmlParser::addEndTagHandler( const string& tagName, const function< void() >& func )
{
    XmlParser::endHandlerMap[ mDomain ][ tagName ] = func;
}

bool XmlParser::getEndTagHandler( const string& tagName, function< void() >& func ) const
{
    const auto& entry = XmlParser::endHandlerMap.find( mDomain );

    if ( entry != XmlParser::endHandlerMap.end() ) {
        auto domainEntry = entry->second.find( tagName );
        if ( domainEntry != entry->second.end() ) {
            func = domainEntry->second;
            return true;
        }
    }
    return false;
}

void XmlParser::addStartTagHandler( const string& tagName, const function< void() >& func )
{
    XmlParser::startHandlerMap[ mDomain ][ tagName ] = func;
}

bool XmlParser::getStartTagHandler( const string& tagName, function< void() >& func ) const
{
    const auto& entry = XmlParser::startHandlerMap.find( mDomain );

    if ( entry != XmlParser::startHandlerMap.end() ) {
        const auto& domainEntry = entry->second.find( tagName );
        if ( domainEntry != entry->second.end() ) {
            func = domainEntry->second;
            return true;
        }
    }
    return false;
}

void XmlParser::addUserTagHandler( const string& tagName, const function< void( void* ) >& func )
{
    XmlParser::userHandlerMap[ mDomain ][ tagName ] = func;
}

bool XmlParser::getUserTagHandler( const string& tagName, function< void( void* ) >& func ) const
{
    const auto& entry = XmlParser::userHandlerMap.find( mDomain );

    if ( entry != XmlParser::userHandlerMap.end() ) {
        auto domainEntry = entry->second.find( tagName );
        if ( domainEntry != entry->second.end() ) {
            func = domainEntry->second;
            return true;
        }
    }
    return false;
}

bool XmlParser::hasUserTagHandler( const string& tagName ) const
{
    const auto& entry = XmlParser::userHandlerMap.find( mDomain );
    if ( entry != XmlParser::userHandlerMap.end() ) {
        auto domainEntry = entry->second.find( tagName );
        if ( domainEntry != entry->second.end() ) {
            return true;
        }
    }
    return false;
}

bool XmlParser::inUserHandler() const
{
    return bInUserHandler;
}

void XmlParser::setUserHandlerObject( std::shared_ptr< void >&& obj )
{
    mUserHandlerObject = move( obj );
}

void XmlParser::make_sax_handler( xmlSAXHandler* saxHandler )
{
    memset( saxHandler, 0, sizeof( xmlSAXHandler ) );

    saxHandler->initialized  = XML_SAX2_MAGIC;
    saxHandler->startElement = &OnStartElement;
    saxHandler->endElement   = &OnEndElement;
    saxHandler->characters   = &OnCharacters;

    saxHandler->error   = &error;
    saxHandler->warning = &warning;
}

void XmlParser::OnCharacters( void* ctx, const xmlChar* ch, int len )
{
    if ( len > 0 ) {
        XmlParser* xp       = static_cast< XmlParser* >( ctx );
        xp->mCurrCharacters = string( (char*)ch, len );
    }
}

void XmlParser::OnEndElement( void* ctx, const xmlChar* name )
{
    auto xp = static_cast< XmlParser* >( ctx );

    // Are we in a user handler? does one exist?
    if ( !xp->bInUserHandler && !xp->userHandlerMap.empty() ) {
        return;
    }

    string tagName( (char*)name );

    function< void() > handler;
    if ( xp->getEndTagHandler( tagName, handler ) ) {
        handler();
    }

    // if we are in a user handler is it time to turn off the flag?
    if ( xp->bInUserHandler ) {
        function< void( void* ) > handler;
        if ( xp->getUserTagHandler( tagName, handler ) ) {
            handler( xp->mUserHandlerObject.get() );
            xp->bInUserHandler     = false;
            xp->mUserHandlerObject = nullptr;
        }
    }

    xp->mCurrAttribs.clear();
    xp->mCurrCharacters = "";
}

void XmlParser::OnStartElement( void* ctx, const xmlChar* name, const xmlChar** attributes )
{
    auto xp = static_cast< XmlParser* >( ctx );
    string tagName( (char*)name );

    // If we have a handler map and we aren't in it then skip it
    if ( !xp->bInUserHandler && !xp->userHandlerMap.empty() ) {
        if ( xp->hasUserTagHandler( tagName ) ) {
            xp->bInUserHandler = true;
        }
        else {
            return;
        }
    }

    xp->mCurrAttribs.clear();
    xp->mCurrCharacters = "";

    if ( attributes != nullptr ) {
        uint16_t index = 0;
        while ( attributes[ index ] != nullptr ) {
            xp->mCurrAttribs.insert( make_pair( string( (char*)attributes[ index ] ),
                                                make_unique< string >( (char*)attributes[ index + 1 ] ) ) );
            index += 2;
        }
    }

    function< void() > handler;
    if ( xp->getStartTagHandler( tagName, handler ) ) {
        handler();
    }
}

/** Display and format an error messages, callback.
   ctx:  an XML parser context
   msg:  the message to display/transmit
   ...:  extra parameters for the message display
   */
void XmlParser::error( void* /*ctx*/, const char* msg, ... )
{
    va_list args;
    va_start( args, msg );
    vprintf( msg, args );
    va_end( args );
}

/** Display and format a warning messages, callback.
   ctx:  an XML parser context
   msg:  the message to display/transmit
   ...:  extra parameters for the message display
   */
void XmlParser::warning( void* /*ctx*/, const char* msg, ... )
{
    va_list args;
    va_start( args, msg );
    vprintf( msg, args );
    va_end( args );
}

}  // namespace xparser
