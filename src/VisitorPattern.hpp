#ifndef VISITORPATTERN_HPP
#define VISITORPATTERN_HPP

class OOCP::Library;
class OOCP::PageSettings;
class OOCP::Point;
class OOCP::PrimArc;
class OOCP::PrimBezier;
class OOCP::PrimBitmap;
class OOCP::PrimCommentText;
class OOCP::PrimEllipse;
class OOCP::PrimLine;
class OOCP::PrimPolygon;
class OOCP::PrimPolyline;
class OOCP::PrimRect;
class OOCP::PrimSymbolVector;
class OOCP::StreamAdminData;
class OOCP::StreamBOMDataStream;
class OOCP::StreamCache;
class OOCP::StreamDirectoryStruct;
class OOCP::StreamDsnStream;
class OOCP::StreamDTypeD;
class OOCP::StreamERC;
class OOCP::StreamHierarchy;
class OOCP::StreamHSObjects;
class OOCP::StreamLibrary;
class OOCP::StreamNetBundleMapData;
class OOCP::StreamPackage;
class OOCP::StreamPage;
class OOCP::StreamSchematic;
class OOCP::StreamSymbol;
class OOCP::StructAlias;
class OOCP::StructBookMarkSymbolInst;
class OOCP::StructBusEntry;
class OOCP::StructDevice;
class OOCP::StructERCObject;
class OOCP::StructERCSymbol;
class OOCP::StructGeneralProperties;
class OOCP::StructGlobal;
class OOCP::StructGlobalSymbol;
class OOCP::StructGraphicArcInst;
class OOCP::StructGraphicBezierInst;
class OOCP::StructGraphicBitMapInst;
class OOCP::StructGraphicBoxInst;
class OOCP::StructGraphicCommentTextInst;
class OOCP::StructGraphicEllipseInst;
class OOCP::StructGraphicLineInst;
class OOCP::StructGraphicOleEmbedInst;
class OOCP::StructGraphicPolygonInst;
class OOCP::StructGraphicPolylineInst;
class OOCP::StructLibraryPart;
class OOCP::StructNetDbIdMapping;
class OOCP::StructOffPageConnector;
class OOCP::StructOffPageSymbol;
class OOCP::StructPackage;
class OOCP::StructPartCell;
class OOCP::StructPinShapeSymbol;
class OOCP::StructPlacedInstance;
class OOCP::StructPort;
class OOCP::StructPortSymbol;
class OOCP::StructSthInHierarchy1;
class OOCP::StructSthInHierarchy2;
class OOCP::StructSthInHierarchy3;
class OOCP::StructSthInPages0;
class OOCP::StructSymbol;
class OOCP::StructSymbolBBox;
class OOCP::StructSymbolDisplayProp;
class OOCP::StructSymbolPin;
class OOCP::StructSymbolPinBus;
class OOCP::StructSymbolPinScalar;
class OOCP::StructT0x10;
class OOCP::StructT0x34;
class OOCP::StructT0x35;
class OOCP::StructT0x45;
class OOCP::StructT0x5b;
class OOCP::StructTitleBlock;
class OOCP::StructTitleBlockSymbol;
class OOCP::StructWire;
class OOCP::StructWireBus;
class OOCP::StructWireScalar;
class OOCP::tagLOGFONTA;

namespace OOCP
{
class Visitor
{
public:
    virtual ~Visitor()
    {
    }

    virtual void visit(const Library& aObj)                      = 0;
    virtual void visit(const PageSettings& aObj)                 = 0;
    virtual void visit(const Point& aObj)                        = 0;
    virtual void visit(const PrimArc& aObj)                      = 0;
    virtual void visit(const PrimBezier& aObj)                   = 0;
    virtual void visit(const PrimBitmap& aObj)                   = 0;
    virtual void visit(const PrimCommentText& aObj)              = 0;
    virtual void visit(const PrimEllipse& aObj)                  = 0;
    virtual void visit(const PrimLine& aObj)                     = 0;
    virtual void visit(const PrimPolygon& aObj)                  = 0;
    virtual void visit(const PrimPolyline& aObj)                 = 0;
    virtual void visit(const PrimRect& aObj)                     = 0;
    virtual void visit(const PrimSymbolVector& aObj)             = 0;
    virtual void visit(const StreamAdminData& aObj)              = 0;
    virtual void visit(const StreamBOMDataStream& aObj)          = 0;
    virtual void visit(const StreamCache& aObj)                  = 0;
    virtual void visit(const StreamDirectoryStruct& aObj)        = 0;
    virtual void visit(const StreamDsnStream& aObj)              = 0;
    virtual void visit(const StreamDTypeD& aObj)                 = 0;
    virtual void visit(const StreamERC& aObj)                    = 0;
    virtual void visit(const StreamHierarchy& aObj)              = 0;
    virtual void visit(const StreamHSObjects& aObj)              = 0;
    virtual void visit(const StreamLibrary& aObj)                = 0;
    virtual void visit(const StreamNetBundleMapData& aObj)       = 0;
    virtual void visit(const StreamPackage& aObj)                = 0;
    virtual void visit(const StreamPage& aObj)                   = 0;
    virtual void visit(const StreamSchematic& aObj)              = 0;
    virtual void visit(const StreamSymbol& aObj)                 = 0;
    virtual void visit(const StructAlias& aObj)                  = 0;
    virtual void visit(const StructBookMarkSymbolInst& aObj)     = 0;
    virtual void visit(const StructBusEntry& aObj)               = 0;
    virtual void visit(const StructDevice& aObj)                 = 0;
    virtual void visit(const StructERCObject& aObj)              = 0;
    virtual void visit(const StructERCSymbol& aObj)              = 0;
    virtual void visit(const StructGeneralProperties& aObj)      = 0;
    virtual void visit(const StructGlobal& aObj)                 = 0;
    virtual void visit(const StructGlobalSymbol& aObj)           = 0;
    virtual void visit(const StructGraphicArcInst& aObj)         = 0;
    virtual void visit(const StructGraphicBezierInst& aObj)      = 0;
    virtual void visit(const StructGraphicBitMapInst& aObj)      = 0;
    virtual void visit(const StructGraphicBoxInst& aObj)         = 0;
    virtual void visit(const StructGraphicCommentTextInst& aObj) = 0;
    virtual void visit(const StructGraphicEllipseInst& aObj)     = 0;
    virtual void visit(const StructGraphicLineInst& aObj)        = 0;
    virtual void visit(const StructGraphicOleEmbedInst& aObj)    = 0;
    virtual void visit(const StructGraphicPolygonInst& aObj)     = 0;
    virtual void visit(const StructGraphicPolylineInst& aObj)    = 0;
    virtual void visit(const StructLibraryPart& aObj)            = 0;
    virtual void visit(const StructNetDbIdMapping& aObj)         = 0;
    virtual void visit(const StructOffPageConnector& aObj)       = 0;
    virtual void visit(const StructOffPageSymbol& aObj)          = 0;
    virtual void visit(const StructPackage& aObj)                = 0;
    virtual void visit(const StructPartCell& aObj)               = 0;
    virtual void visit(const StructPinShapeSymbol& aObj)         = 0;
    virtual void visit(const StructPlacedInstance& aObj)         = 0;
    virtual void visit(const StructPort& aObj)                   = 0;
    virtual void visit(const StructPortSymbol& aObj)             = 0;
    virtual void visit(const StructSthInHierarchy1& aObj)        = 0;
    virtual void visit(const StructSthInHierarchy2& aObj)        = 0;
    virtual void visit(const StructSthInHierarchy3& aObj)        = 0;
    virtual void visit(const StructSthInPages0& aObj)            = 0;
    virtual void visit(const StructSymbol& aObj)                 = 0;
    virtual void visit(const StructSymbolBBox& aObj)             = 0;
    virtual void visit(const StructSymbolDisplayProp& aObj)      = 0;
    virtual void visit(const StructSymbolPin& aObj)              = 0;
    virtual void visit(const StructSymbolPinBus& aObj)           = 0;
    virtual void visit(const StructSymbolPinScalar& aObj)        = 0;
    virtual void visit(const StructT0x10& aObj)                  = 0;
    virtual void visit(const StructT0x34& aObj)                  = 0;
    virtual void visit(const StructT0x35& aObj)                  = 0;
    virtual void visit(const StructT0x45& aObj)                  = 0;
    virtual void visit(const StructT0x5b& aObj)                  = 0;
    virtual void visit(const StructTitleBlock& aObj)             = 0;
    virtual void visit(const StructTitleBlockSymbol& aObj)       = 0;
    virtual void visit(const StructWire& aObj)                   = 0;
    virtual void visit(const StructWireBus& aObj)                = 0;
    virtual void visit(const StructWireScalar& aObj)             = 0;
    virtual void visit(const tagLOGFONTA& aObj)                  = 0;
};

class Component
{
public:
    virtual ~Component()
    {
    }

    virtual void accept(Visitor& aVisitor) const = 0;
};
} // namespace OOCP
#endif // VISITORPATTERN_HPP