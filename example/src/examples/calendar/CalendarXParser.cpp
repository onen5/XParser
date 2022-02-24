/**
 * @class CalendarXParser
 *
 * @brief This class is used to parse the Calendar XML
 *
 * @section DESCRIPTION
 *
 */

// system includes
#include <memory>
#include <string>

// base object includes
#include "examples/calendar/CalendarXParser.h" 

// project sub directory includes - alphabetical order
#include "examples/calendar/Weekday.h"
#include "examples/calendar/WorkingTime.h"

// project includes - alphabetical order


using namespace std;

namespace examples { namespace calendar {

/*
 * Public Methods
 */

CalendarXParser::CalendarXParser() : 
    XmlParser("") 
{
}

CalendarXParser::CalendarXParser(const string& fileName) :
    XmlParser(fileName),
    mCalPtr(nullptr),
    mObjStack()
{
    setDomain("CalendarXParser");

    addStartTagHandler( "calendar", [this] ()
        {
            if( mCalPtr == nullptr ) {
                XML_ERROR("No Calendar object was provided.");
            }

            mObjStack.push_back(mCalPtr);
        }
    );

    addEndTagHandler( "calendar", [this] ()
        {
            mObjStack.pop_back();

            if( !mObjStack.empty() ) {
                XML_ERROR("Calendar was not fully processed.");
            }
        }
    );

    addEndTagHandler( "name", [this] ()
        {
            dynamic_cast<Calendar*>(mObjStack.back())->setName( *getContent() );
        }
    );

    addEndTagHandler( "description", [this] ()
        {
            dynamic_cast<Calendar*>(mObjStack.back())->setDescription( *getContent() );
        }
    );

    addStartTagHandler( "weekday", [this] ()
        {
            cout << "processing push weekday" << endl;

            if(inUserHandler()) {
                auto weekday = make_shared<Weekday>();
                mObjStack.push_back(weekday.get());
                setUserHandlerObject( move(weekday) );
            }
            else {
                mObjStack.push_back( mCalPtr->addWeekday() );
            }
        }
    );

    addEndTagHandler( "weekday", [this] ()
        {
            cout << "processing pop weekday" << endl;
            mObjStack.pop_back();
        }
    );

    addEndTagHandler( "daytype", [this] ()
        {
            cout << "processing daytype" << endl;
            dynamic_cast<Weekday*>(mObjStack.back())->setDayType( getUint16(getContent()) );
        }
    );

    addStartTagHandler( "working-time", [this] ()
        {
            cout << "processing push working-time" << endl;
            auto wtptr = dynamic_cast<Weekday*>(mObjStack.back())->addWorkingTime( );
            mObjStack.push_back( wtptr );
        }
    );

    addEndTagHandler( "working-time", [this] ()
        {
            cout << "processing pop working-time" << endl;
            mObjStack.pop_back();
        }
    );

    addEndTagHandler( "from-time", [this] ()
        {
            cout << "processing from-time" << endl;
            dynamic_cast<WorkingTime*>(mObjStack.back())->setFromTime( getUint16(getContent()) );
        }
    );

    addEndTagHandler( "to-time", [this] ()
        {
            cout << "processing to-time" << endl;
            dynamic_cast<WorkingTime*>(mObjStack.back())->setToTime( getUint16(getContent()) );
        }
    );

    // addStartTagHandler( "name", [this] ()
    //      {
    //         // if(inUserHandler()) {
    //         //     auto step = make_shared<Step>();
    //         //     mObjStack.push_back( step.get() );
    //         //     setUserHandlerObject( move( step ) );
    //         // }
    //         // else {
    //         //     auto step = addStep( mTracePtr );
    //         //     mObjStack.push_back(step);
    //         // }
    //      }
    // );

    // addEndTagHandler( "description", [this] ()
    //      {
    //         // mObjStack.pop_back();
    //      }
    // );

    // addEndTagHandler( "description", [this] ()
    //      {
    //         // dynamic_cast<AbstractSequence*>(mObjStack.back())->getCore()->setId( getUint16( getContent() ) );
    //      }
    // );

    // addStartTagHandler( "Core", [this] ()
    //      {
    //         mObjStack.push_back( dynamic_cast<AbstractSequence*>(mObjStack.back())->getCore() );
    //      }
    // );

    // addEndTagHandler( "Core", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addEndTagHandler( "Id", [this] ()
    //      {
    //         dynamic_cast<Core*>(mObjStack.back())->setId( getUint16( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "AARCH64", [this] ()
    //      {
    //         dynamic_cast<Core*>(mObjStack.back())->isAArch64 = true;
    //      }
    // );

    // addEndTagHandler( "AARCH32", [this] ()
    //      {
    //          dynamic_cast<Core*>(mObjStack.back())->isAArch64 = false;
    //      }
    // );

    // addEndTagHandler( "MODE", [this] ()
    //      {
    //         static unordered_map<string, CpuMode> lookup = {
    //             {"USR" , CpuMode::USR},
    //             {"FIQ" , CpuMode::FIQ},
    //             {"IRQ" , CpuMode::IRQ},
    //             {"SVC" , CpuMode::SVC},
    //             {"ABT" , CpuMode::ABT},
    //             {"UND" , CpuMode::UND},
    //             {"HYP" , CpuMode::HYP},
    //             {"MON" , CpuMode::MON},
    //             {"SYS" , CpuMode::SYS},
    //             {"EL0t", CpuMode::EL0t},
    //             {"EL1t", CpuMode::EL1t},
    //             {"EL1h", CpuMode::EL1h},
    //             {"EL2t", CpuMode::EL2t},
    //             {"EL2h", CpuMode::EL2h},
    //             {"EL3t", CpuMode::EL3t},
    //             {"EL3h", CpuMode::EL3h},
    //             {"InvalMode", CpuMode::InvalMode},
    //         };

    //         const auto& entry = lookup.find( *getContent() );
    //         if(entry == lookup.end()) {
    //             XML_ERROR("Unknown Mode type: [" << *getContent() << "]");
    //         }

    //         dynamic_cast<Core*>(mObjStack.back())->setMode( entry->second );
    //      }
    // );

    // addEndTagHandler( "QAD1", [/*this*/] ()
    //      {
    //         // added for completeness.  Has no current value.
    //      }
    // );

    // addEndTagHandler( "NS", [this] ()
    //      {
    //         dynamic_cast<Core*>(mObjStack.back())->isSecure = false;
    //      }
    // );

    // addEndTagHandler( "S", [this] ()
    //      {
    //         dynamic_cast<Core*>(mObjStack.back())->isSecure = true;
    //      }
    // );

    // addEndTagHandler( "Seq", [this] ()
    //      {
    //         dynamic_cast<AbstractSequence*>(mObjStack.back())->setSequenceId( getUint64( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "LoadRegs", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::LoadRegs );
    //      }
    // );

    // addEndTagHandler( "Rabble", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::Rabble );
    //      }
    // );

    // addEndTagHandler( "RabbleTranslate", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::RabbleTranslate );
    //      }
    // );

    // addEndTagHandler( "Reset", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::Reset );
    //      }
    // );

    // addEndTagHandler( "CheckInterrupt", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::CheckInterrupt );
    //      }
    // );

    // addEndTagHandler( "CheckWait", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::CheckWait );
    //      }
    // );

    // addEndTagHandler( "CheckVectorCatch", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::CheckVectorCatch );
    //      }
    // );

    // addEndTagHandler( "SEVBroadcast", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::SEVBroadcast );
    //      }
    // );

    // addEndTagHandler( "ExclMonUpdate", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::ExclMonUpdate );
    //      }
    // );

    // addEndTagHandler( "HaltedInDebugState", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::HaltedInDebugState );
    //      }
    // );

    // addEndTagHandler( "ExitDebugState", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::ExitDebugState );
    //      }
    // );

    // addEndTagHandler( "Uncategorized", [this] ()
    //      {
    //         dynamic_cast<Event*>(mObjStack.back())->setEventType( EventType::Uncategorized );
    //      }
    // );

    // addStartTagHandler( "Event", [this] ()
    //      {
    //         mObjStack.push_back( addEvent( mTracePtr ) );
    //      }
    // );

    // addEndTagHandler( "Event", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addStartTagHandler( "Instr", [this] ()
    //      {
    //         mObjStack.push_back( dynamic_cast<Step*>(mObjStack.back())->getInstruction() );
    //      }
    // );

    // addEndTagHandler( "Instr", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addEndTagHandler( "IN", [this] ()
    //      {
    //         dynamic_cast<Instruction*>(mObjStack.back())->setNumber( getUint32( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "VA", [this] ()
    //      {
    //         dynamic_cast<VaProviderInterface*>(mObjStack.back())->setVa( getUint64Hex( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "PA", [this] ()
    //      {
    //         resolveHtwEntry();

    //         auto pac = dynamic_cast<PaProviderInterface*>(mObjStack.back());

    //         // ns and qad are mutually exclusive.  We don't sanitize or check the data here, and rely on quality generation.
    //         pac->setPa( getUint64Hex( getContent() ), !getBoolAttrib("NS"), getBoolAttrib("QAD1") );
    //      }
    // );

    // addEndTagHandler( "Uninitialized", [this] ()
    //      {
    //         dynamic_cast<InitializableInterface*>(mObjStack.back())->setIsInitialized( false );
    //      }
    // );

    // addEndTagHandler( "Enc", [this] ()
    //      {
    //         dynamic_cast<Instruction*>(mObjStack.back())->setEncoding( getUint32Hex( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "DisarmID", [this] ()
    //      {
    //         dynamic_cast<Instruction*>(mObjStack.back())->setDisarmId( *getContent() );
    //      }
    // );

    // addEndTagHandler( "DisAsm", [this] ()
    //      {
    //         dynamic_cast<Instruction*>(mObjStack.back())->setDisasm( *getContent() );
    //      }
    // );

    // addEndTagHandler( "Note", [this] ()
    //      {
    //         static const unordered_map<string, InstrExplainType> lookup = {
    //             {"CC Fail", InstrExplainType::CC_Fail},
    //             {"FAULTED", InstrExplainType::FAULTED},
    //             {"NOP"    , InstrExplainType::NOP},
    //             {"Translation Fault", InstrExplainType::Translation_Fault},
    //         };

    //         const auto& entry = lookup.find( *getContent() );
    //         if(entry == lookup.end()) {
    //             XML_ERROR("Unknown Explain Type: " << *getContent());
    //         }

    //         dynamic_cast<Step*>(mObjStack.back())->setNote( entry->second );
    //      }
    // );

    // addStartTagHandler( "R.Reg", [this] ()
    //      {
    //         auto reg = addRegister( dynamic_cast<AbstractSequence*>(mObjStack.back()) );
    //         reg->setRnotW( true );

    //         mObjStack.push_back(reg);
    //      }
    // );

    // addEndTagHandler( "R.Reg", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addStartTagHandler( "W.Reg", [this] ()
    //      {
    //         auto reg = addRegister( dynamic_cast<AbstractSequence*>(mObjStack.back()) );
    //         reg->setRnotW( false );

    //         mObjStack.push_back(reg);
    //      }
    // );

    // addEndTagHandler( "W.Reg", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addEndTagHandler( "Name", [this] ()
    //      {
    //         auto reg = dynamic_cast<Register*>(mObjStack.back());
    //         reg->setName( *getContent() );

    //         if( (reg->getName()[0] == 'V') && ( isdigit(reg->getName()[1])) ) {
    //             reg->setIs128Bit( true );
    //         }

    //      }
    // );

    // addEndTagHandler( "Value", [this] ()
    //      {
    //         auto vprov = dynamic_cast<ValueProviderInterface*>(mObjStack.back());

    //         uint64_t value;
    //         if(vprov->is128Bit()) {
    //             uint64_t value2;
    //             getUint128Hex(getContent(), value, value2);
    //             vprov->setValue2( value2 );
    //         }
    //         else {
    //             value = getUint64Hex(getContent());
    //         }
    //         vprov->setValue( value );
    //      }
    // );

    // addEndTagHandler( "Mask", [this] ()
    //      {
    //         dynamic_cast<Register*>(mObjStack.back())->setMask( getUint64Hex( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "Volatile", [this] ()
    //      {
    //         auto attribs = getAttributes();

    //         const auto& maskEntry = attribs->find("Mask");
    //         if( maskEntry != attribs->end() ) {
    //             dynamic_cast<Register*>(mObjStack.back())->setVolatileMask( getUint64Hex( maskEntry->second ) );
    //         }
    //      }
    // );

    // addEndTagHandler( "Unmodeled", [this] ()
    //      {
    //         auto attribs = getAttributes();

    //         const auto& maskEntry = attribs->find("Mask");
    //         if( maskEntry != attribs->end() ) {
    //             dynamic_cast<UnmodeledProviderInterface*>(mObjStack.back())->setUnmodeledMask( getUint64Hex( maskEntry->second ) );
    //         }
    //      }
    // );

    // addStartTagHandler( "R.Mem", [this] ()
    //      {
    //         auto mem = new Memory(true, getBoolAttrib("PTEWriteBack"));
    //         mObjStack.push_back(mem);
    //      }
    // );

    // addEndTagHandler( "R.Mem", [this] ()
    //      {
    //         auto mem = dynamic_cast<Memory*>(mObjStack.back());
    //         mObjStack.pop_back();

    //         dynamic_cast<Step*>(mObjStack.back())->getMemoryAccess()->emplace_back(*mem);

    //         delete mem;
    //      }
    // );

    // addStartTagHandler( "W.Mem", [this] ()
    //      {
    //         auto mem = new Memory(false, getBoolAttrib("PTEWriteBack"));
    //         mObjStack.push_back(mem);
    //      }
    // );

    // addEndTagHandler( "W.Mem", [this] ()
    //      {
    //         auto mem = dynamic_cast<Memory*>(mObjStack.back());
    //         mObjStack.pop_back();

    //         dynamic_cast<Step*>(mObjStack.back())->getMemoryAccess()->emplace_back(*mem);

    //         delete mem;
    //      }
    // );

    // addEndTagHandler( "Size", [this] ()
    //      {
    //         dynamic_cast<SizeProviderInterface*>(mObjStack.back())->setSize( getUint64( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "Piece", [this] ()
    //      {
    //         dynamic_cast<Memory*>(mObjStack.back())->setPiece( getFloatAttrib( "Num" ) );
    //      }
    // );

    // addEndTagHandler( "BigEndian", [this] ()
    //      {
    //         dynamic_cast<BigEndianInterface*>(mObjStack.back())->setIsBigEndian( true );
    //      }
    // );

    // addStartTagHandler( "HTW", [this] ()
    //      {
    //         auto htw = addHtw( dynamic_cast<AbstractSequence*>(mObjStack.back()) );

    //         htw->isDSide = getBoolAttrib( "DSide" );
    //         htw->setAccessType( getStringAttrib( "AccessType" ) );
    //         htw->setEl( static_cast<ExceptionLevel>( getUint16Attrib( "EL" ) ) );

    //         mObjStack.push_back(htw);
    //      }
    // );
    // addEndTagHandler( "HTW", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addStartTagHandler( "Translation", [this] ()
    //      {
    //         resolveHtwEntry();

    //         auto stage = getStringAttrib( "Stage" );
    //         const auto& entry = mStageLookup.find( stage );

    //         if(entry == mStageLookup.end()) {
    //             XML_ERROR("Unknown Stage type: " << stage);
    //             return;
    //         }

    //         auto htw = dynamic_cast<Htw*>(mObjStack.back());
    //         htw->setHasTranslation(true);

    //         auto trans = addTranslation( htw );
    //         trans->setStage( entry->second );

    //         mObjStack.push_back( trans );
    //      }
    // );

    // addEndTagHandler( "Translation", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );


    // addEndTagHandler( "OuterCacheability", [this] ()
    //      {
    //         auto trans = dynamic_cast<Translation*>(mObjStack.back());

    //         trans->getOuterCacheability()->isICacheEnabled  = getBoolAttrib( "ICacheEnabled" );
    //         trans->getOuterCacheability()->isDUCacheEnabled = getBoolAttrib( "DUCacheEnabled" );
    //         trans->getOuterCacheability()->isWriteAlloc     = getBoolAttrib( "WriteAlloc" );
    //         trans->getOuterCacheability()->isReadAlloc      = getBoolAttrib( "ReadAlloc" );
    //         trans->getOuterCacheability()->isWriteBack      = getBoolAttrib( "WriteBack" );
    //         trans->getOuterCacheability()->isNonTransient   = getBoolAttrib( "NonTransient" );

    //      }
    // );

    // addEndTagHandler( "InnerCacheability", [this] ()
    //      {
    //         auto trans = dynamic_cast<Translation*>(mObjStack.back());

    //         trans->getInnerCacheability()->isICacheEnabled  = getBoolAttrib( "ICacheEnabled" );
    //         trans->getInnerCacheability()->isDUCacheEnabled = getBoolAttrib( "DUCacheEnabled" );
    //         trans->getInnerCacheability()->isWriteAlloc     = getBoolAttrib( "WriteAlloc" );
    //         trans->getInnerCacheability()->isReadAlloc      = getBoolAttrib( "ReadAlloc" );
    //         trans->getInnerCacheability()->isWriteBack      = getBoolAttrib( "WriteBack" );
    //         trans->getInnerCacheability()->isNonTransient   = getBoolAttrib( "NonTransient" );

    //      }
    // );

    // addEndTagHandler( "Shareability", [this] ()
    //      {
    //         auto trans = dynamic_cast<Translation*>(mObjStack.back());

    //         trans->getShareability()->isInner = getBoolAttrib( "Inner" );
    //         trans->getShareability()->isOuter = getBoolAttrib( "Outer" );

    //      }
    // );

    // addEndTagHandler( "Permissions", [this] ()
    //      {
    //         auto trans = dynamic_cast<Translation*>(mObjStack.back());

    //         trans->getPermission()->isPR = getBoolAttrib( "PR" );
    //         trans->getPermission()->isPW = getBoolAttrib( "PW" );
    //         trans->getPermission()->isPX = getBoolAttrib( "PX" );
    //         trans->getPermission()->isUR = getBoolAttrib( "UR" );
    //         trans->getPermission()->isUW = getBoolAttrib( "UW" );
    //         trans->getPermission()->isUX = getBoolAttrib( "UX" );

    //     }
    // );

    // addEndTagHandler( "MemoryType", [this] ()
    //      {
    //         dynamic_cast<Translation*>(mObjStack.back())->setMemoryType( *getContent() );
    //      }
    // );

    // addEndTagHandler( "InAddr", [this] ()
    //      {
    //         dynamic_cast<Page*>(mObjStack.back())->setInAddr( getUint64Hex( getContent() ), !getBoolAttrib( "NS" ) );
    //      }
    // );

    // addEndTagHandler( "OutAddr", [this] ()
    //      {
    //         dynamic_cast<Page*>(mObjStack.back())->setOutAddr( getUint64Hex( getContent() ), !getBoolAttrib( "NS" ) );
    //      }
    // );

    // addEndTagHandler( "Page", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );
    // addStartTagHandler( "Page", [this] ()
    //      {
    //         auto page = dynamic_cast<Translation*>(mObjStack.back())->getPage();

    //         page->setSizeBytes( getUint64Attrib( "SizeBytes" ) );
    //         page->setSizeBits( getUint32Attrib( "SizeBits" ) );

    //         page->isGlobal = getBoolAttrib( "IsGlobal" );

    //         mObjStack.push_back(page);
    //      }
    // );

    // addEndTagHandler( "Fault", [this] ()
    //      {
    //         resolveHtwEntry();

    //         auto htw = dynamic_cast<Htw*>(mObjStack.back());

    //         auto stage = getStringAttrib( "Stage" );
    //         const auto& entry = mStageLookup.find( stage );

    //         if(entry == mStageLookup.end()) {
    //             XML_ERROR("Unknown Stage Type: " << stage);
    //         }

    //         auto fault = htw->getFault();
    //         fault->setStage( entry->second );
    //         fault->setLevel( getUint32Attrib( "Level" ) );
    //         fault->setMessage( *getContent() );

    //         htw->setHasFault( true );
    //      }
    // );

    // addEndTagHandler( "AlignSize", [this] ()
    //      {
    //         dynamic_cast<Htw*>(mObjStack.back())->setAlignSize( getUint16( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "IPA", [this] ()
    //      {
    //         resolveHtwEntry();

    //         bInHtwEntry = true;

    //         auto htwEntry = addHtwEntry( dynamic_cast<Htw*>(mObjStack.back()) );
    //         htwEntry->setIpa( getUint64Hex( getContent() ), !getBoolAttrib( "NS" ) );

    //         mObjStack.push_back(htwEntry);

    //      }
    // );

    // addStartTagHandler( "PTE", [this] ()
    //      {
    //         HtwEntry* htwEntry;
    //         if(bInHtwEntry) { // if were are in one that means it was the previous one
    //             htwEntry = dynamic_cast<HtwEntry*>(mObjStack.back());
    //         }
    //         else {
    //             bInHtwEntry = true;
    //             htwEntry = addHtwEntry( dynamic_cast<Htw*>(mObjStack.back()) );
    //             mObjStack.push_back(htwEntry);
    //         }

    //         mObjStack.push_back( addPte( htwEntry ) );
    //      }
    // );

    // addEndTagHandler( "PTE", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addEndTagHandler( "Stage", [this] ()
    //      {
    //         dynamic_cast<Pte*>(mObjStack.back())->setStage( getUint16( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "Level", [this] ()
    //      {
    //         dynamic_cast<Pte*>(mObjStack.back())->setLevel( getUint16( getContent() ) );
    //      }
    // );

    // addStartTagHandler( "Exception", [this] ()
    //      {
    //         auto abSeq = dynamic_cast<AbstractSequence*>(mObjStack.back());

    //         abSeq->setHasException(true);
    //         mObjStack.push_back( abSeq->getException() );
    //      }
    // );

    // addEndTagHandler( "Exception", [this] ()
    //      {
    //         mObjStack.pop_back();
    //      }
    // );

    // addEndTagHandler( "Class", [this] ()
    //      {
    //         dynamic_cast<Exception*>(mObjStack.back())->setClass( getUint64Hex( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "SyndromeOrFsr", [this] ()
    //      {
    //         dynamic_cast<Exception*>(mObjStack.back())->setSyndrome( getUint64Hex( getContent() ) );
    //      }
    // );

    // addEndTagHandler( "Reason", [this] ()
    //      {
    //         dynamic_cast<ReasonsProviderInterface*>(mObjStack.back())->getReasons()->push_back( *getContent() );
    //      }
    // );

    // addEndTagHandler( "UNPRED", [this] ()
    //      {
    //         auto unpred = addUnpred( dynamic_cast<AbstractSequence*>(mObjStack.back()) );

    //         auto attribs = getAttributes();

    //         const auto& rblIgnoreEntry = attribs->find( "RabbleIgnore" );
    //         if( rblIgnoreEntry == attribs->end() ) {
    //             unpred->setRabbleIgnore( false );
    //         }
    //         else
    //         {
    //             unpred->setRabbleIgnore( getBoolAttrib( "RabbleIgnore" ) );
    //         }

    //         unpred->setReason( *getContent() );
    //      }
    // );

}

void CalendarXParser::setCalendar(Calendar* cal) {
    mCalPtr = cal;
}

} }
