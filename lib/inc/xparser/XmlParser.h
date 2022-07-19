/*
 */

#ifndef __XPARSER_XMLPARSER_H__
#define __XPARSER_XMLPARSER_H__

// system includes
#include <libxml/tree.h>

#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

// base object includes

// project sub directory includes - alphabetical order
#include "xparser/ParserException.h"

// project includes - alphabetical order

#define XML_ERROR( MESSAGE ) \
    stringstream xml_ss;     \
    xml_ss << MESSAGE;       \
    throw xparser::ParserException( xml_ss.str() );

typedef std::unordered_map< std::string, std::unique_ptr< std::string > > xParserAttribMap;

namespace xparser {

class XmlParser {
  public:
    XmlParser();
    XmlParser( const std::string& );
    virtual ~XmlParser();

    static xmlChar* convertInput( const char*, const char* );

    std::string getString( const xmlChar* ) const;
    std::string getString( const xmlChar*, int len ) const;

    bool getBool( const xmlChar* ) const;
    float getFloat( const xmlChar* ) const;

    uint16_t getUint16( const xmlChar* ) const;
    uint32_t getUint32( const xmlChar* ) const;
    uint64_t getUint64( const xmlChar* ) const;

    uint16_t getUint16Hex( const xmlChar* ) const;
    uint32_t getUint32Hex( const xmlChar* ) const;
    uint64_t getUint64Hex( const xmlChar* ) const;

    void getUint128Hex( const xmlChar* number, uint64_t& val1, uint64_t& val2 );

    bool getBool( const std::string* str ) const;
    float getFloat( const std::string* str ) const;

    uint16_t getUint16( const std::string* str ) const;
    uint32_t getUint32( const std::string* str ) const;
    uint64_t getUint64( const std::string* str ) const;

    uint16_t getUint16Hex( const std::string* str ) const;
    uint32_t getUint32Hex( const std::string* str ) const;
    uint64_t getUint64Hex( const std::string* str ) const;

    void getUint128Hex( const std::string* number, uint64_t& val1, uint64_t& val2 );

    bool getBool( const std::unique_ptr< std::string >& str ) const;
    float getFloat( const std::unique_ptr< std::string >& str ) const;

    uint16_t getUint16( const std::unique_ptr< std::string >& str ) const;
    uint32_t getUint32( const std::unique_ptr< std::string >& str ) const;
    uint64_t getUint64( const std::unique_ptr< std::string >& str ) const;

    uint16_t getUint16Hex( const std::unique_ptr< std::string >& str ) const;
    uint32_t getUint32Hex( const std::unique_ptr< std::string >& str ) const;
    uint64_t getUint64Hex( const std::unique_ptr< std::string >& str ) const;

    void getUint128Hex( const std::unique_ptr< std::string >& number, uint64_t& val1, uint64_t& val2 );

    // these do the attrib look up for you.
    const std::string& getStringAttrib( const std::string& attrib ) const;
    const std::string& getStringAttrib( const std::string& attrib, const std::string& defaultValue ) const;

    bool getBoolAttrib( const std::string& attrib ) const;
    bool getBoolAttrib( const std::string& attrib, bool defaultValue ) const;
    float getFloatAttrib( const std::string& attrib ) const;
    float getFloatAttrib( const std::string& attrib, float defaultValue ) const;

    uint16_t getUint16Attrib( const std::string& attrib ) const;
    uint16_t getUint16Attrib( const std::string& attrib, uint16_t defaultValue ) const;
    uint32_t getUint32Attrib( const std::string& attrib ) const;
    uint32_t getUint32Attrib( const std::string& attrib, uint32_t defaultValue ) const;
    uint64_t getUint64Attrib( const std::string& attrib ) const;
    uint64_t getUint64Attrib( const std::string& attrib, uint64_t defaultValue ) const;

    uint16_t getUint16HexAttrib( const std::string& attrib ) const;
    uint16_t getUint16HexAttrib( const std::string& attrib, uint16_t defaultValue ) const;
    uint32_t getUint32HexAttrib( const std::string& attrib ) const;
    uint32_t getUint32HexAttrib( const std::string& attrib, uint32_t defaultValue ) const;
    uint64_t getUint64HexAttrib( const std::string& attrib ) const;
    uint64_t getUint64HexAttrib( const std::string& attrib, uint64_t defaultValue ) const;

    void getUint128HexAttrib( const std::string& attrib, uint64_t& val1, uint64_t& val2 );

    bool parse();
    bool parseXmlDoc( xmlDoc* );
    bool parseXmlStr( xmlChar* );
    bool parseXmlStr( char*, long );

    const std::string& getFileName() const;

    /***** DOM processing *****/
    virtual void processElement( xmlNode* );
    virtual void processNode( xmlNode* );
    const xParserAttribMap* getAttributes( xmlNode* ) const;
    const std::string* getContent( xmlNode* ) const;

    void addTagHandler( const std::string& tagName, const std::function< void( xmlNode* ) >& func );
    const std::function< void( xmlNode* ) >& getTagHandler( const std::string& tagName ) const;

    /***** SAX processing *****/
    const xParserAttribMap* getAttributes() const;
    const std::string* getContent() const;

    void addEndTagHandler( const std::string& tagName, const std::function< void() >& func );
    bool getEndTagHandler( const std::string& tagName, std::function< void() >& func ) const;

    void addStartTagHandler( const std::string& tagName, const std::function< void() >& func );
    bool getStartTagHandler( const std::string& tagName, std::function< void() >& func ) const;

    // customized handlers for users
    void addUserTagHandler( const std::string& tagName, const std::function< void( void* ) >& func );
    bool getUserTagHandler( const std::string& tagName, std::function< void( void* ) >& func ) const;
    bool hasUserTagHandler( const std::string& tagName ) const;

  protected:
    bool bUseDom;
    mutable std::string mCurrCharacters;
    mutable xParserAttribMap mCurrAttribs;

    void setDomain( const std::string& domain );

    /***** SAX processing *****/
    bool inUserHandler() const;
    void setUserHandlerObject( std::shared_ptr< void >&& obj );

    static void error( void* ctx, const char* msg, ... );
    static void warning( void* ctx, const char* msg, ... );
    static void OnCharacters( void* ctx, const xmlChar* ch, int len );
    static void OnEndElement( void* ctx, const xmlChar* name );
    static void OnStartElement( void* ctx, const xmlChar* name, const xmlChar** attributes );

  private:
    std::string mFilename;

    /***** DOM processing *****/
    std::unique_ptr< std::string > getStringPtr( const xmlChar* ) const;
    std::unordered_map< std::string, std::function< void( xmlNode* ) > > handlerMap;

    /***** SAX processing *****/
    std::string mDomain;  // domain of the parser (ie CalendarXParser)
    std::shared_ptr< void > mUserHandlerObject;
    bool bInUserHandler;

    // Domain->Tag->Function
    static std::unordered_map< std::string, std::unordered_map< std::string, std::function< void() > > > endHandlerMap;
    static std::unordered_map< std::string, std::unordered_map< std::string, std::function< void() > > > startHandlerMap;

    static std::unordered_map< std::string, std::unordered_map< std::string, std::function< void( void* ) > > > userHandlerMap;

    void make_sax_handler( xmlSAXHandler* saxHandler );
};

}  // namespace xparser

#endif /* __XPARSER_XMLPARSER_H__ */
